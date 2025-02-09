#include "Window.h"
#include "Input.h"
#include "GameEngine.h"


#pragma region public
LRESULT Window::messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
	case WM_CLOSE: PostQuitMessage(0); return 0;
	case WM_KEYDOWN: Input::keyDown((unsigned int)wParam); return 0;
	case WM_KEYUP: Input::keyUp((unsigned int)wParam); return 0;
	default: return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}
Window::Window()
{
	const S_EngineConfig& config = GameEngine::config();
	DEVMODE dmScreenSettings = {};
	int posX, posY;

	_programName.assign(config.program.begin(), config.program.end());
	_wc.lpfnWndProc = Window::messageHandler;
	_wc.hInstance = GetModuleHandle(NULL);
	_wc.lpszClassName = _programName.c_str();
	_wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	_wc.cbClsExtra = 0;
	_wc.cbWndExtra = 0;
	_wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	_wc.hIconSm = _wc.hIcon;
	_wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	_wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	_wc.lpszMenuName = NULL;
	_wc.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&_wc);

	if (config.fullScreen)
	{
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)config.width;
		dmScreenSettings.dmPelsHeight = (unsigned long)config.height;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		posX = (GetSystemMetrics(SM_CXSCREEN) - config.width) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - config.height) / 2;
	}

	_hwnd = CreateWindowEx(WS_EX_APPWINDOW, _programName.c_str(), _programName.c_str(),
		WS_OVERLAPPEDWINDOW,
		//WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, config.width, config.height, NULL, NULL, GetModuleHandle(NULL), NULL);
	ShowWindow(_hwnd, SW_SHOW);
	SetForegroundWindow(_hwnd);
	SetFocus(_hwnd);
}
Window::~Window()
{
	if (GameEngine::config().fullScreen)
		ChangeDisplaySettings(NULL, 0);
	DestroyWindow(_hwnd);
	UnregisterClass(_programName.c_str(), _wc.hInstance);
}
bool Window::isTerminated() const
{
	return _msg.message == WM_QUIT;
}
void Window::dispatchInput()
{
	if (PeekMessage(&_msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&_msg);
		DispatchMessage(&_msg);
	}
}
#pragma endregion