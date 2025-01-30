#include "GameEngine.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>

#pragma region debug message
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		std::cerr << "Validation layer: " << pCallbackData->pMessage << '\n';

	return VK_FALSE;
}
static VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr)
		return func(instance, pCreateInfo, pAllocator, pCallback);
	else
		return VK_ERROR_EXTENSION_NOT_PRESENT;
}
static void destroyDebugUtilsMessengerEXT(VkInstance instance, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT callback) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr)
		func(instance, callback, pAllocator);
}
#pragma endregion


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
#ifdef _DEBUG
	this->setupDebugMessenger();
#endif
	this->pickPhysicalDevice();
	this->createLogicalDevice();
	this->createSurface();
	this->createSwapChain();
}
void GameEngine::createInstance()
{
#ifdef _DEBUG
	if (!this->checkValidationLayerSupport())
		throw std::runtime_error("Couldn't find validation layer");
#endif

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
#ifdef _DEBUG
	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	const char* layers[] = { "VK_LAYER_KHRONOS_validation" };
	createInfo.enabledLayerCount = 1;
	createInfo.ppEnabledLayerNames = layers;

	VkDebugUtilsMessengerCreateInfoEXT instanceDebugMessenger = {};
	instanceDebugMessenger.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	instanceDebugMessenger.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
	instanceDebugMessenger.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	instanceDebugMessenger.pfnUserCallback = debugCallback;
	instanceDebugMessenger.pUserData = nullptr;
	createInfo.pNext = &instanceDebugMessenger;
#else
	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;
#endif

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
	// TODO : 성능 좋은 GPU 선택하기 (rating)
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
	auto [queueFamily, idx] = this->getQueueFamilyProperties(VK_QUEUE_GRAPHICS_BIT);
	VkDeviceQueueCreateInfo	queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = idx;
	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;

	// 물리 장치에서 논리적 장치를 생성
	if (vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device) != VK_SUCCESS)
		throw std::runtime_error("Failed to create logical device");

	vkGetDeviceQueue(_device, idx, 0, &_graphicsQueue);
}
std::pair<VkQueueFamilyProperties, int> GameEngine::getQueueFamilyProperties(VkQueueFlags flags)
{
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyCount, queueFamilies.data());

	int idx = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if ((queueFamily.queueFlags & flags) == flags)
			return { queueFamily, idx };
		++idx;
	}

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
void GameEngine::setupDebugMessenger()
{
	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData = nullptr;
	if (createDebugUtilsMessengerEXT(_vkInstance, &createInfo, nullptr, &_debugMessenger) != VK_SUCCESS)
		throw std::runtime_error("Could not create debug messenger");
}
bool GameEngine::checkValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> layers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

	bool found = std::find_if(layers.begin(), layers.end(), [&](const VkLayerProperties& props) {
		return strcmp(props.layerName, "VK_LAYER_KHRONOS_validation") == 0;
		}) != layers.end();

		if (!found)
			return false;
		return true;
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
#ifdef _DEBUG
	destroyDebugUtilsMessengerEXT(_vkInstance, nullptr, _debugMessenger);
#endif
	vkDestroySwapchainKHR(_device, _swapChain, nullptr);
	vkDestroyDevice(_device, nullptr);
	vkDestroySurfaceKHR(_vkInstance, _surface, nullptr);
	vkDestroyInstance(_vkInstance, nullptr);
	glfwDestroyWindow(_window);
	glfwTerminate();
}
#pragma endregion