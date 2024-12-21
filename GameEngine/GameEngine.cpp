#include "GameEngine.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <GLFW/glfw3.h>

#pragma region static public
void GameEngine::run()
{
	if (!glfwInit())
		throw std::runtime_error("Failed to initialize GLFW");

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	s_window = glfwCreateWindow(800, 600, "My Game Engine", nullptr, nullptr);
	if (!s_window)
		throw std::runtime_error("Failed to create GLFW window");

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "My Game Engine";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Custom Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;
	createInfo.enabledLayerCount = 0;

	if (vkCreateInstance(&createInfo, nullptr, &s_vkInstance) != VK_SUCCESS)
		throw std::runtime_error("Failed to create Vulkan instance");


	while (!glfwWindowShouldClose(s_window))
	{
		glfwPollEvents();
	}

	vkDestroyInstance(s_vkInstance, nullptr);
	glfwDestroyWindow(s_window);
	glfwTerminate();
}
#pragma endregion

#pragma region static private
#pragma endregion


GLFWwindow* GameEngine::s_window = nullptr;
VkInstance GameEngine::s_vkInstance = {};