#pragma once
#include <string>
#include "S_EngineConfig.h"

class Window;
class Graphics;
class GameEngine
{
public:
	static void init();
	static void run();
	static void terminate();
	static const S_EngineConfig& config() { return s_config; }
private:

	static S_EngineConfig s_config;
	static Window* s_window;
	static Graphics* s_graphics;

public:
	GameEngine() = delete;
	GameEngine(const GameEngine& other) = delete;
	~GameEngine() = delete;
};