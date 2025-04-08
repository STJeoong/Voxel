#include "GameEngine.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <format>
#include "Structs.h"
#include "Input.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include "Shader.h"
#include <iostream>
#include "Camera.h"
#include "Chunk.h"

using json = nlohmann::json;

static EngineConfig s_config = {};
static GLFWwindow* s_window = nullptr;
static Shader* s_meshShader = nullptr;


#pragma region public static
void GameEngine::init()
{
	std::ifstream fstream("./config.json");
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

	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	Input::init();
	Camera::init();

	s_meshShader = new Shader("./Shader/Mesh.vs", "./Shader/Mesh.fs");
	s_meshShader->setActive();
	GameEngine::loadData();
}
void GameEngine::run()
{
	float last = 0.0;
	float current = static_cast<float>(glfwGetTime());
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
		else if (Input::getKeyDown(GLFW_KEY_1))
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else if (Input::getKeyDown(GLFW_KEY_2))
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		Camera::update(current - last);
		Chunk::update();
		glfwSwapBuffers(s_window);
		glfwPollEvents();
		last = current;
		current = static_cast<float>(glfwGetTime());
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
#pragma endregion



#pragma region private static
void GameEngine::loadData()
{
}
void GameEngine::unloadData()
{
}
#pragma endregion