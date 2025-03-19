#include "GameEngine.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "Object.h"
#include <SDL2/SDL_image.h>
#include <gl/glew.h>
#include <fmt/core.h>
#include "Shader.h"

using json = nlohmann::json;

S_EngineConfig s_config = {};
SDL_Window* s_window = nullptr;
SDL_GLContext s_context;
Shader* s_shader = nullptr;
bool s_isRunning = true;
Uint32 s_prevTicks = 0;
std::vector<Object*> s_gameObjects;

#pragma region public static
void GameEngine::init()
{
	std::ifstream fstream("C:/Users/taejeong/source/repos/GameEngine/GameEngine/config.json");
	json j = json::parse(fstream);
	j.get_to(s_config);
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		throw std::runtime_error(fmt::format("Unable to init SDL : {}", SDL_GetError()));

	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	s_window = SDL_CreateWindow(s_config.program.c_str(), 100, 100, s_config.width, s_config.height, SDL_WINDOW_OPENGL);
	if (s_window == nullptr)
		throw std::runtime_error(fmt::format("Failed to create window : {}", SDL_GetError()));
	s_context = SDL_GL_CreateContext(s_window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		throw std::runtime_error("Failed to init glew");
	glGetError();

	s_shader = new Shader("C:/Users/taejeong/Source/repos/GameEngine/GameEngine/Shader/Sprite.vert", "C:/Users/taejeong/source/repos/GameEngine/GameEngine/Shader/Sprite.frag");
	GameEngine::loadData();
	s_prevTicks = SDL_GetTicks();
}
void GameEngine::run()
{
	while (s_isRunning)
	{
		GameEngine::processInput();
		glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//SDL_SetRenderDrawColor(s_renderer, 0, 0, 255, 255); // 붓 색깔 고르는 함수인듯
		//SDL_RenderClear(s_renderer); // 그리는 함수인듯? ( 아직 화면에 그려지지 않고 버퍼에 쓰는 작업 )
		GameEngine::update();
		SDL_GL_SwapWindow(s_window); // 전면 버퍼와 후면 버퍼 교환
		//SDL_RenderPresent(s_renderer); // 전면 버퍼와 후면 버퍼를 교환 ( 실제 화면에 그려지는 작업 )
	}
}
void GameEngine::terminate()
{	
	GameEngine::unloadData();
	delete s_shader;
	SDL_GL_DeleteContext(s_context);
	SDL_DestroyWindow(s_window);
	SDL_Quit();
}
const S_EngineConfig& GameEngine::config() { return s_config; }
Object* GameEngine::instantiate()
{
	Object* obj = new Object();
	s_gameObjects.push_back(obj);
	return obj;
}
void GameEngine::destroy(Object* obj)
{
	auto it = std::find(s_gameObjects.begin(), s_gameObjects.end(), obj);
	if (it == s_gameObjects.end())
		return;
	s_gameObjects.erase(it);
	delete obj;
}
SDL_Window* GameEngine::window() { return s_window; }
//SDL_Renderer* GameEngine::renderer() { return s_renderer; }
#pragma endregion

#pragma region private static
void GameEngine::processInput()
{
	SDL_Event evt = {};
	while (SDL_PollEvent(&evt))
	{
		switch (evt.type)
		{
		case SDL_QUIT: s_isRunning = false; break;
		default: break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
		s_isRunning = false;
}
void GameEngine::update()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), s_prevTicks + 16))
		;
	Uint32 curTicks = SDL_GetTicks();
	float dt = (curTicks - s_prevTicks) / 1000.0f;

	if (dt > 0.05f)
		dt = 0.05f;
	for (Object* obj : s_gameObjects)
		if (obj->active())
			obj->update(dt);
	s_prevTicks = curTicks;
}
void GameEngine::loadData()
{
}
void GameEngine::unloadData()
{
	for (Object* obj : s_gameObjects)
		delete obj;
}
#pragma endregion