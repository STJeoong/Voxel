#include "GameEngine.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <format>
#include "Object.h"
#include "Structs.h"
#include "Input.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include "Shader.h"
#include <iostream>

using json = nlohmann::json;

static EngineConfig s_config = {};
static GLFWwindow* s_window = nullptr;
static std::vector<Object*> s_gameObjects;
static Shader* s_meshShader = nullptr;


#pragma region public static
void GameEngine::init()
{
	std::ifstream fstream("C:/Users/taejeong/source/repos/GameEngine/GameEngine/config.json");
	json j = json::parse(fstream);
	j.get_to(s_config);
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	s_window = glfwCreateWindow(s_config.width, s_config.height, s_config.program.c_str(), nullptr, nullptr);
	if (s_window == nullptr)
		throw std::runtime_error("Failed to create GLFW window");
	glfwMakeContextCurrent(s_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw std::runtime_error("Failed to initialize GLAD");
	glViewport(0, 0, s_config.width, s_config.height);

	// set callback
	glfwSetInputMode(s_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(s_window, [](GLFWwindow* window, int width, int height)
		{ glViewport(0, 0, width, height); s_config.width = width; s_config.height = height; });

	Input::init();
	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	s_meshShader = new Shader("./Shader/Mesh.vs", "./Shader/Mesh.fs");
	s_meshShader->setInt("texture0", 0);
	s_meshShader->setInt("texture1", 1);
	s_meshShader->setInt("texture2", 2);
	s_meshShader->setInt("texture3", 3);
	s_meshShader->setActive();
	GameEngine::loadData();
}
void GameEngine::run()
{
	double last = 0.0;
	double current = glfwGetTime();
	int i = 0;
	while (!glfwWindowShouldClose(s_window))
	{
		++i;
		if (i == 20)
		{
			i = 0;
			std::cout << std::format("{} fps\n", 1 / (current - last));
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Input::update();
		if (Input::getKey(GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(s_window, true);
		for (Object* obj : s_gameObjects)
			if (obj->active())
				obj->update(current - last);
		glfwSwapBuffers(s_window);
		glfwPollEvents();
		last = current;
		current = glfwGetTime();
	}
}
void GameEngine::terminate()
{
	GameEngine::unloadData();
	delete s_meshShader;
	glfwTerminate();
}
const EngineConfig& GameEngine::config() { return s_config; }
GLFWwindow* GameEngine::window() { return s_window; }
Shader* GameEngine::shader() { return s_meshShader; }
Object* GameEngine::instantiate()
{
	Object* obj = new Object();
	s_gameObjects.push_back(obj);
	return obj;
}
void GameEngine::destroy(Object* obj)
{
	auto it = std::find(s_gameObjects.begin(), s_gameObjects.end(), obj);
	if (it == s_gameObjects.end())
		return;
	s_gameObjects.erase(it);
	delete obj;
}
#pragma endregion



#pragma region private static
#include "Camera.h"
#include "CameraController.h"
#include "Mesh.h"
#include "PerlinNoise.h"

int voxels[64][64][64] = {};
void GameEngine::loadData()
{
	Object* cam = GameEngine::instantiate();
	cam->addComponent<Camera>();
	cam->addComponent<CameraController>();


	siv::PerlinNoise perlin(1234);
	for (int i = 0; i < 32; ++i)
		for (int j = 0; j < 32; ++j)
		{
			double noise = perlin.octave2D_01(i * 0.01, j * 0.01, 4);
			int height = static_cast<int>(noise * 32);
			for (int k = 0; k <= height; ++k)
			{
				voxels[i][k][j] = 1;
				Object* cube = GameEngine::instantiate();
				cube->transform.pos = { (float)i,(float)k,(float)j };
				cube->mesh(Mesh::get("./Resources/Cube.gpmesh"));
			}
		}
}
void GameEngine::unloadData()
{
	for (Object* obj : s_gameObjects)
		delete obj;
}
#pragma endregion