#pragma once
#include <string>
#include <nlohmann/json.hpp>
struct EngineConfig
{
	std::string program;
	int width;
	int height;
	bool fullScreen;
	bool vsync;

	// camera
	float fov;
	float screenNear;
	float screenFar;
	float mouseSensitivity;
	float camSpeed;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EngineConfig, program, width, height, fullScreen, vsync, fov, screenNear, screenFar, mouseSensitivity, camSpeed)