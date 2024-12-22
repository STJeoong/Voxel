#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
class GameEngine
{
public:
	void run();
private:
	void initWindow();
	void initVulkan();
	void createInstance();
	void pickPhysicalDevice();
	void createLogicalDevice();
	VkQueueFamilyProperties getQueueFamilyProperties(VkQueueFlags flags);
	bool isDeviceSuitable(VkPhysicalDevice device);
	void createSurface();
	void createSwapChain();
	void mainLoop();
	void cleanUp();

	static const uint32_t WIDTH = 800;
	static const uint32_t HEIGHT = 600;
	GLFWwindow* _window;
	VkInstance _vkInstance;
	VkPhysicalDevice _physicalDevice;
	VkDevice _device; // logical device
	VkQueue _graphicsQueue;
	VkSurfaceKHR _surface;
	VkSwapchainKHR _swapChain;
	VkFormat _swapChainImageFormat;
	VkExtent2D _swapChainExtent;

};