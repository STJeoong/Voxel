#include "GameEngine.h"

#include <Windows.h>
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CLOSE)
		PostQuitMessage(0);
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	// 윈도우 클래스 설정
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"DX11WindowClass";
	RegisterClass(&wc);

	// 윈도우 생성
	HWND hwnd = CreateWindow(L"DX11WindowClass", L"Directx 11 Window", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, nullptr, nullptr, hInstance, nullptr);
	ShowWindow(hwnd, nCmdShow);

	// Directx 11 초기화
	D3D11_CREATE_DEVICE_FLAG deviceFlags = (D3D11_CREATE_DEVICE_FLAG)0;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;
	IDXGISwapChain* swapChain = nullptr;

	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferCount = 1;
	scd.BufferDesc.Width = 1280;
	scd.BufferDesc.Height = 720;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hwnd;
	scd.SampleDesc.Count = 1;
	scd.Windowed = TRUE;

	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlags, nullptr, 0,
		D3D11_SDK_VERSION, &scd, &swapChain, &device, nullptr, &context);

	// 메세지 루프
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// 정리
	swapChain->Release();
	context->Release();
	device->Release();

	/*GameEngine engine;
	try
	{
		engine.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Fatal exception happened: " << e.what() << '\n';
	}*/
	return 0;
}