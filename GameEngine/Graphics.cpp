#include "Graphics.h"
#include "GameEngine.h"
#include "Window.h"

// setting up and drawing 3D graphics
#pragma comment(lib, "d3d11.lib")
// contains tools to interface with the hardware on the computer to obtain information
// about the refresh rate of the monitor, the video card being used, and so forth.
#pragma comment(lib, "dxgi.lib")
// compiling shaders
#pragma comment(lib, "d3dcompiler.lib")

#pragma region public
Graphics::Graphics(Window* window)
{
	_hwnd = window->hwnd();
	const S_EngineConfig& config = GameEngine::config();
	IDXGIFactory* factory = nullptr;
	IDXGIAdapter* adapter = nullptr;
	IDXGIOutput* adapterOutput = nullptr;
	unsigned int numModes, i, numerator, denominator;
	unsigned long long stringLength;
	DXGI_MODE_DESC* displayModeList = nullptr;
	DXGI_ADAPTER_DESC adapterDesc = {};
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	D3D_FEATURE_LEVEL featureLevel = {};
	ID3D11Texture2D* backBuffer = nullptr;
	D3D11_TEXTURE2D_DESC depthBufferDesc = {};
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	D3D11_RASTERIZER_DESC rasterDesc = {};
	float fieldOfView, screenAspect;

	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory)))
		throw std::runtime_error("CreateEXGIFactory Failed");
	if (FAILED(factory->EnumAdapters(0, &adapter)))
		throw std::runtime_error("EnumAdapter Failed");
	if (FAILED(adapter->EnumOutputs(0, &adapterOutput)))
		throw std::runtime_error("EnumOutput Failed");
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL)))
		throw std::runtime_error("GetDisplayModeList Failed");
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (displayModeList == nullptr)
		throw std::runtime_error("Create DXGI_MODE_DESC Failed");
	// Now fill the display mode list structures.
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList)))
		throw std::runtime_error("Create DXGI_MODE_DESC Failed");
	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i < numModes; i++)
		if (displayModeList[i].Width == (unsigned int)config.width)
			if (displayModeList[i].Height == (unsigned int)config.height)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
	// Get the adapter (video card) description.
	if (FAILED(adapter->GetDesc(&adapterDesc)))
		throw std::runtime_error("GetDesc Failed");
	// Store the dedicated video card memory in megabytes.
	_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
	// Convert the name of the video card to a character array and store it.
	error = wcstombs_s(&stringLength, _videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
		throw std::runtime_error("wcstombs_s Failed");
	// Release the display mode list.
	delete[] displayModeList;
	// Release the adapter output.
	adapterOutput->Release();
	// Release the adapter.
	adapter->Release();
	// Release the factory.
	factory->Release();

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;
	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = config.width;
	swapChainDesc.BufferDesc.Height = config.height;
	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// Set the refresh rate of the back buffer.
	if (config.vsync)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = _hwnd;
	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	// Set to full screen or windowed mode.
	if (config.fullScreen)
		swapChainDesc.Windowed = false;
	else
		swapChainDesc.Windowed = true;
	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;
	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;
	// Create the swap chain, Direct3D device, and Direct3D device context.
	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &_swapChain, &_device, NULL, &_deviceContext)))
		throw std::runtime_error("CreateDeviceAndSwapChain Failed");

	// Get the pointer to the back buffer.
	if (FAILED(_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer)))
		throw std::runtime_error("GetBuffer Failed");
	// Create the render target view with the back buffer pointer.
	if (FAILED(_device->CreateRenderTargetView(backBuffer, NULL, &_renderTargetView)))
		throw std::runtime_error("CreateRenderTargetView Faield");
	// Release pointer to the back buffer as we no longer need it.
	backBuffer->Release();

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = config.width;
	depthBufferDesc.Height = config.height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	if (FAILED(_device->CreateTexture2D(&depthBufferDesc, NULL, &_depthStencilBuffer)))
		throw std::runtime_error("CreateTexture2D Failed");

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	if (FAILED(_device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState)))
		throw std::runtime_error("CreateDepthStencilState Failed");
	// Set the depth stencil state.
	_deviceContext->OMSetDepthStencilState(_depthStencilState, 1);

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	if (FAILED(_device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView)))
		throw std::runtime_error("CreateDepthStencilView Failed");

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	_deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);
	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	if (FAILED(_device->CreateRasterizerState(&rasterDesc, &_rasterState)))
		throw std::runtime_error("CreateRasterizerState Failed");

	// Now set the rasterizer state.
	_deviceContext->RSSetState(_rasterState);
	// Setup the viewport for rendering.
	_viewport.Width = (float)config.width;
	_viewport.Height = (float)config.height;
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;
	_viewport.TopLeftX = 0.0f;
	_viewport.TopLeftY = 0.0f;

	// Create the viewport.
	_deviceContext->RSSetViewports(1, &_viewport);

	// Setup the projection matrix.
	fieldOfView = 3.141592654f / 4.0f;
	screenAspect = (float)config.width / (float)config.height;

	// Create the projection matrix for 3D rendering.
	_projectionMat = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, config.screenNear, config.screenDepth);
	// Initialize the world matrix to the identity matrix.
	_worldMat = DirectX::XMMatrixIdentity();
	// Create an orthographic projection matrix for 2D rendering.
	_orthoMat = DirectX::XMMatrixOrthographicLH((float)config.width, (float)config.height, config.screenNear, config.screenDepth);
}
Graphics::~Graphics()
{
}
void Graphics::update()
{
}
#pragma endregion

#pragma region private
#pragma endregion