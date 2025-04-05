#pragma once
struct GLFWwindow;
class Input
{
	friend class GameEngine;
public:
	static bool getKey(int key);
	static bool getKeyDown(int key);
	static bool getKeyUp(int key);
	static float getMouseAxisX();
	static float getMouseAxisY();
private:
	static void init();
	static void update();
};