#pragma once
class Input
{
	friend class GameEngine;
public:
	static bool getKey(int key);
	static bool getKeyDown(int key);
	static bool getKeyUp(int key);
private:
	static void init();
	static void update();
};