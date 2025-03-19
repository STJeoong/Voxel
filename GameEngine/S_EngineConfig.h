#pragma once
#include <string>
#include <nlohmann/json.hpp>
struct S_EngineConfig
{
	std::string program;
	int width;
	int height;
	bool fullScreen;
	bool vsync;
	float screenDepth;
	float screenNear;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_EngineConfig, program, width, height, fullScreen, vsync, screenDepth, screenNear)