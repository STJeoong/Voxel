#include "GameEngine.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "Graphics.h"
#include "Window.h"

using json = nlohmann::json;

#pragma region public static
void GameEngine::init()
{
	std::ifstream fstream("C:/Users/taejeong/source/repos/GameEngine/GameEngine/config.json");
	json j = json::parse(fstream);
	j.get_to(s_config);

	if (s_config.fullScreen)
	{
		s_config.width = GetSystemMetrics(SM_CXSCREEN);
		s_config.height = GetSystemMetrics(SM_CYSCREEN);
	}
	
	s_window = new Window();
	s_graphics = new Graphics(reinterpret_cast<Window*>(s_window));
}
void GameEngine::run()
{
	while (!s_window->isTerminated())
	{
		s_window->dispatchInput();
		s_graphics->update();
	}
}
void GameEngine::terminate()
{	
	delete s_window;
	delete s_graphics;
}
#pragma endregion

#pragma region private static
#pragma endregion



S_EngineConfig GameEngine::s_config = {};
Window* GameEngine::s_window = nullptr;
Graphics* GameEngine::s_graphics = nullptr;