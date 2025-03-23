#pragma once
#include "S_EngineConfig.h"
#include <vector>

class Object;

class GameEngine
{
public:
	static void init();
	static void run();
	static void terminate();
	static const S_EngineConfig& config();
	static Object* instantiate();
	static void destroy(Object* obj);
private:
	static void processInput();
	static void update();
	static void loadData();
	static void unloadData();


public:
	GameEngine() = delete;
	GameEngine(const GameEngine& other) = delete;
	~GameEngine() = delete;
};