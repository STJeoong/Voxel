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
	float screenDepth;
	float screenNear;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EngineConfig, program, width, height, fullScreen, vsync, screenDepth, screenNear)