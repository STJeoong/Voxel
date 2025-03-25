#pragma once
#include <vector>

struct EngineConfig;
class Object;

class GameEngine
{
public:
	static void init();
	static void run();
	static void terminate();
	static const EngineConfig& config();
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