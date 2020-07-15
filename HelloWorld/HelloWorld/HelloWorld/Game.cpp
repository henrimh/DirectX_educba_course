#include "pch.h"
#include "Game.h"
#include <fstream>

// Loading a file into an Array^
Array<byte>^ LoadShaderFile(std::string File)
{
	Array<byte>^ FileData = nullptr;

	// open the file
	std::ifstream VertexFile(File, std::ios::in | std::ios::binary | std::ios::ate);

	// Check if file open succeeded
	if (VertexFile.is_open())
	{
		// Find the file length
		int length = (int)VertexFile.tellg();

		// Collect the data
		FileData = ref new Array<byte>(length);
		VertexFile.seekg(0, std::ios::beg);
		VertexFile.read(reinterpret_cast<char*>(FileData->Data), length);
		VertexFile.close();

	}

	return FileData;
}

// initializes and prepares Direct3D
void CGame::Initialize()
{
	// Define temp pointer variables to a device and a device context
	ComPtr<ID3D11Device> tempDevice;
	ComPtr<ID3D11DeviceContext> tempDeviceContext;

	// Creating the device and the device context objects
	D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&tempDevice,
		nullptr,
		&tempDeviceContext
	);

	// Convert the pointers from the DX 11 version to the DX 11.2 version
	tempDevice.As(&Device);
	tempDeviceContext.As(&DeviceContext);

	// *****

	// 1. Convert our D3D11Device1 interface to an IDXGIDevice1
	ComPtr<IDXGIDevice1> dxgiDevice;
	Device.As(&dxgiDevice);

	// 2. Utilize the IDXGIDevice1 interface to get access to the adapter.
	ComPtr<IDXGIAdapter> dxgiAdapter;
	dxgiDevice->GetAdapter(&dxgiAdapter);

	// 3. Use the IDXGIAdapter interface to get access to the parent. // To get dxgi factory
	ComPtr<IDXGIFactory2> dxgiFactory;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), &dxgiFactory);

	// *****

	//  Set up the swap chain description struct
	DXGI_SWAP_CHAIN_DESC1 swapChainDescription = { 0 };

	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // How the swap chain should be utilised.
	swapChainDescription.BufferCount = 2;								// A front buffer and a back buffer. //(2 buffers) duh
	swapChainDescription.Format = DXGI_FORMAT_B8G8R8A8_UNORM;			// A common swap chain format. // UNORM = unsigned normalized interger.
	swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;	// The recommended flip mode.
	swapChainDescription.SampleDesc.Count = 1;							// Disables anti-aliasing.

	CoreWindow^ Window = CoreWindow::GetForCurrentThread(); // Obtain a ref pointer to the window

	dxgiFactory->CreateSwapChainForCoreWindow(
		Device.Get(),							// Address of the device
		reinterpret_cast<IUnknown*>(Window),	// Address of the window
		&swapChainDescription,					// Address of the swap chain description
		nullptr,
		&SwapChain
	);


	// Get a direct pointer to the back buffer
	ComPtr<ID3D11Texture2D> backBuffer;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer);

	// create a render target that points to our back buffer
	Device->CreateRenderTargetView(backBuffer.Get(), nullptr, &RenderTarget);

	// Set the viewport 
	D3D11_VIEWPORT viewPort = { 0 };

	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = Window->Bounds.Width;
	viewPort.Height = Window->Bounds.Height;

	DeviceContext->RSSetViewports(1, &viewPort);
	

	InitGraphics();
	InitPipeline();
}

// Performs updates to the game state
void CGame::Update()
{
}

// Renders a single frame of 3D graphics
void CGame::Render()
{
	// Set the new render target as active render target
	DeviceContext->OMSetRenderTargets(1, RenderTarget.GetAddressOf(), nullptr);

	// Clear the Back buffer to some color
	float color[4] = { 0.4f, 0.5f, 0.3f, 1.0f };
	DeviceContext->ClearRenderTargetView(RenderTarget.Get(), color);

	// Setting up the Vertex buffer
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	DeviceContext->IAGetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &stride, &offset);
	 
	// Setting up the primitive topology
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DeviceContext->Draw(3, 0);

	// Switch the back buffer and the front buffer
	SwapChain->Present(1, 0);
}

void CGame::InitGraphics()
{
	VERTEX Vertices[] =
	{
		{0.0f, 0.5f, 0.0f},
		{0.45f, -0.5f, 0.0f},
		{-0.45f, -0.5f, 0.0f }
	};

	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.ByteWidth = sizeof(VERTEX) * ARRAYSIZE(Vertices);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subResourceData = { Vertices, 0, 0 };

	Device->CreateBuffer(&bufferDesc, &subResourceData, &VertexBuffer);
}

// Initializes the GPU settings and prepares it for rendering 
void CGame::InitPipeline()
{
	// Load the shader files
	Array<byte>^ VSFile = LoadShaderFile("VertexShader.cso");
	Array<byte>^ PSFile = LoadShaderFile("PixelShader.cso");
	
	// Creating the shader objects
	Device->CreateVertexShader(VSFile->Data, VSFile->Length, nullptr, &VertexShader);
	Device->CreatePixelShader(PSFile->Data, PSFile->Length, nullptr, &PixelShader);


	// Set the shaders as active shaders.
	DeviceContext->VSSetShader(VertexShader.Get(), nullptr, 0);
	DeviceContext->PSSetShader(PixelShader.Get(), nullptr, 0);

	// initialize input layout 
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	Device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), VSFile->Data, VSFile->Length, &InputLayout);
	DeviceContext->IASetInputLayout(InputLayout.Get());
} 