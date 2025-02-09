#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

class Window;
class Graphics
{
public:
	Graphics(Window* window);
	~Graphics();
	void update();
private:

	HWND _hwnd = {};
	int _videoCardMemory = {};
	char _videoCardDescription[128] = {};
	IDXGISwapChain* _swapChain = nullptr;
	ID3D11Device* _device = nullptr;
	ID3D11DeviceContext* _deviceContext = nullptr;
	ID3D11RenderTargetView* _renderTargetView = nullptr;
	ID3D11Texture2D* _depthStencilBuffer = nullptr;
	ID3D11DepthStencilState* _depthStencilState = nullptr;
	ID3D11DepthStencilView* _depthStencilView = nullptr;
	ID3D11RasterizerState* _rasterState = nullptr;
	DirectX::XMMATRIX _projectionMat = {};
	DirectX::XMMATRIX _worldMat = {};
	DirectX::XMMATRIX _orthoMat = {};
	D3D11_VIEWPORT _viewport = {};
};