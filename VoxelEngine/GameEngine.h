#pragma once
#include <vector>

struct GLFWwindow;
struct EngineConfig;
class Shader;

class GameEngine
{
public:
	static void init();
	static void run();
	static void terminate();
	static const EngineConfig& config();
	static GLFWwindow* window();
	static Shader* shader();


public:
	GameEngine() = delete;
	GameEngine(const GameEngine& other) = delete;
	~GameEngine() = delete;
};