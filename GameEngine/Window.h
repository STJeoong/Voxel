#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
class Window
{
private:
	static LRESULT CALLBACK messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	Window();
	~Window();
	HWND hwnd() const { return _hwnd; }
	bool isTerminated() const;
	void dispatchInput();
private:

	MSG _msg = {};
	WNDCLASSEX _wc = {};
	HWND _hwnd = {};
	std::wstring _programName;
};