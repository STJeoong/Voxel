#include "GameEngine.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>

#pragma region static public
void GameEngine::run()
{
	this->initWindow();
	this->initVulkan();
	this->mainLoop();
	this->cleanUp();
}
#pragma endregion

#pragma region static private
void GameEngine::initWindow()
{
	if (!glfwInit())
		throw std::runtime_error("Failed to initialize GLFW");

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	_window = glfwCreateWindow(GameEngine::WIDTH, GameEngine::HEIGHT, "My Game Engine", nullptr, nullptr);
	if (!_window)
		throw std::runtime_error("Failed to create GLFW window");
}
void GameEngine::initVulkan()
{
	this->createInstance();
	this->pickPhysicalDevice();
	this->createLogicalDevice();
	this->createSurface();
	this->createSwapChain();
}
void GameEngine::createInstance()
{
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

	if (vkCreateInstance(&createInfo, nullptr, &_vkInstance) != VK_SUCCESS)
		throw std::runtime_error("Failed to create Vulkan instance");
}
void GameEngine::pickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(_vkInstance, &deviceCount, nullptr);
	if (deviceCount == 0)
		throw std::runtime_error("Failed to find GPUs with Vulkan support");

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(_vkInstance, &deviceCount, devices.data());

	// 조건에 맞는 Physical Device 선택 (가장 적합한 디바이스)
	for (const auto& device : devices)
		if (this->isDeviceSuitable(device))
		{
			_physicalDevice = device;
			break;
		}

	if (_physicalDevice == VK_NULL_HANDLE)
		throw std::runtime_error("Failed to find a suitable GPU");
}
void GameEngine::createLogicalDevice()
{
	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	// graphics queue 생성
	VkQueueFamilyProperties queueFamily = this->getQueueFamilyProperties(VK_QUEUE_GRAPHICS_BIT);
	VkDeviceQueueCreateInfo	queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = 0; // 임의의 값 지정
	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;

	// 물리 장치에서 논리적 장치를 생성
	if (vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device) != VK_SUCCESS)
		throw std::runtime_error("Failed to create logical device");

	vkGetDeviceQueue(_device, 0, 0, &_graphicsQueue);
}
VkQueueFamilyProperties GameEngine::getQueueFamilyProperties(VkQueueFlags flags)
{
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyCount, queueFamilies.data());

	for (const auto& queueFamily : queueFamilies)
		if ((queueFamily.queueFlags & flags) == flags)
			return queueFamily;

	throw std::runtime_error("Failed to find a suitable queue family");
}
bool GameEngine::isDeviceSuitable(VkPhysicalDevice device)
{
	// GPU에서 사용할 수 있는 기능 검사 (예: 그래픽 지원 여부)
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	// 예: Vulkan 1.0 이상을 지원하는지 확인
	return deviceProperties.apiVersion >= VK_API_VERSION_1_0;
}
void GameEngine::createSurface()
{
	if (glfwCreateWindowSurface(_vkInstance, _window, nullptr, &_surface) != VK_SUCCESS)
		throw std::runtime_error("Failed to create window surface");
}
void GameEngine::createSwapChain()
{
	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = _surface;

	VkSurfaceCapabilitiesKHR capabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_physicalDevice, _surface, &capabilities);

	_swapChainExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, GameEngine::WIDTH));
	_swapChainExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, GameEngine::HEIGHT));

	createInfo.imageExtent = _swapChainExtent;
	createInfo.imageFormat = VK_FORMAT_B8G8R8A8_UNORM; // 일반적인 화면 출력용 포맷
	createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	createInfo.minImageCount = 2; // 더블 버퍼링
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	if (vkCreateSwapchainKHR(_device, &createInfo, nullptr, &_swapChain) != VK_SUCCESS)
		throw std::runtime_error("Failed to create swapchain");
}
void GameEngine::mainLoop()
{
	while (!glfwWindowShouldClose(_window))
	{
		glfwPollEvents();
	}
}
void GameEngine::cleanUp()
{
	vkDestroySwapchainKHR(_device, _swapChain, nullptr);
	vkDestroyDevice(_device, nullptr);
	vkDestroySurfaceKHR(_vkInstance, _surface, nullptr);
	vkDestroyInstance(_vkInstance, nullptr);
	glfwDestroyWindow(_window);
	glfwTerminate();
}
#pragma endregion