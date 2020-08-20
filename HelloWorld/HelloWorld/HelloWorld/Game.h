#pragma once
#include <wrl.h>
#include <windows.graphics.directx.h>

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace Platform;
using namespace DirectX;

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
	ComPtr<ID3D11Buffer> ConstantBuffer;

	struct VERTEX
	{
		float X, Y, Z;	// vertex position
		float R, G, B;	// color data
	};

	struct OFFSET
	{
		float X, Y, Z;
	};

	void Initialize(); // Initialization code
	void InitGraphics();
	void Update(); // Code that manipulates the game
	void Render(); // Draw graphics
	void InitPipeline();

	float time;

};