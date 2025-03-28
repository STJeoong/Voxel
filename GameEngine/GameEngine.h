﻿#pragma once
#include <vector>

struct GLFWwindow;
struct EngineConfig;
class Object;
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
	static Object* instantiate();
	static void destroy(Object* obj);
private:
	static void loadData();
	static void unloadData();


public:
	GameEngine() = delete;
	GameEngine(const GameEngine& other) = delete;
	~GameEngine() = delete;
};