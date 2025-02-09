#pragma once
class Window;
class Input
{
	friend class Window;
public:
	static bool isKeyDown(unsigned int keyCode);
private:
	static void keyDown(unsigned int keyCode);
	static void keyUp(unsigned int keyCode);

	static bool _keys[256];
};