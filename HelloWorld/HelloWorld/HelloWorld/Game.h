#pragma once
#include <wrl.h>
#include <windows.graphics.directx.h>

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace Platform;

class CGame
{
public:
	ComPtr<ID3D11Device1> Device;					// The device interface (A virtual reprensentation of the graphics card)
	ComPtr<ID3D11DeviceContext1> DeviceContext;		// The "control panel" for the GPU
	ComPtr<IDXGISwapChain1> SwapChain;				// Swap Chain interface
	ComPtr<ID3D11RenderTargetView> RenderTarget;
	ComPtr<ID3D11Buffer> VertexBuffer;
	ComPtr<ID3D11VertexShader> VertexShader;
	ComPtr<ID3D11PixelShader> PixelShader;
	ComPtr<ID3D11InputLayout> InputLayout;

	struct VERTEX
	{
		float X, Y, Z;	// vertex position
		float R, G, B;	// color data
	};

	void Initialize(); // Initialization code
	void InitGraphics();
	void Update(); // Code that manipulates the game
	void Render(); // Draw graphics
	void InitPipeline();


	// To Create a triangle
	// 1. Creating 3 vertices to make a triangle 
	// 2. Storing the vertices in vRam
	// 3. Inform the GPU how to read the vertices
	// 4. Inform the GPU how to translate the vertices into a flat image
	// 5. Inform the GPU where on the back buffer should the image appear
	// 6. Render the triangle
};