#pragma once
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
class GameEngine
{
public:
	static void run();
private:
	static GLFWwindow* s_window;
	static VkInstance s_vkInstance;
};