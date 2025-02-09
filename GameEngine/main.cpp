#include <windows.h>
#include <exception>
#include <iostream>
#include "GameEngine.h"

#pragma comment(lib, "d3d11.lib")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    try
    {
        GameEngine::init();
        GameEngine::run();
        GameEngine::terminate();
    }
    catch (const std::exception& e)
    {
        std::cout << "Fatal error : " << e.what() << '\n';
    }
    return 0;
}
