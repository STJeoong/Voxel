#include "GameEngine.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <format>
#include "Object.h"
#include "Shader.h"
#include "VertexArray.h"

using json = nlohmann::json;

S_EngineConfig s_config = {};
GLFWwindow* s_window = nullptr;
Shader* s_shader = nullptr;
VertexArray* s_vertexArr = nullptr;
std::vector<Object*> s_gameObjects;

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
	glfwSetFramebufferSizeCallback(s_window, [](GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); });

	s_shader = new Shader("C:/Users/taejeong/source/repos/GameEngine/GameEngine/Shader/Sprite.vs",
		"C:/Users/taejeong/source/repos/GameEngine/GameEngine/Shader/Sprite.fs");
	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};
	unsigned int idxs[] = { 0,1,2 };
	s_vertexArr = new VertexArray(vertices, 3, idxs, 3);

	GameEngine::loadData();
}
void GameEngine::run()
{
	while (!glfwWindowShouldClose(s_window))
	{
		// input
		if (glfwGetKey(s_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(s_window, true);

		// rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		s_shader->setActive();
		s_vertexArr->setActive();
		glDrawArrays(GL_TRIANGLES, 0, 3);


		// check and call events and swap the buffers
		glfwSwapBuffers(s_window);
		glfwPollEvents();
	}
}
void GameEngine::terminate()
{	
	GameEngine::unloadData();
	glfwTerminate();
	delete s_shader;
}
const S_EngineConfig& GameEngine::config() { return s_config; }
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
void GameEngine::processInput()
{
}
void GameEngine::update()
{
	for (Object* obj : s_gameObjects)
		if (obj->active())
			obj->update(0.05f);
}
void GameEngine::loadData()
{
}
void GameEngine::unloadData()
{
	for (Object* obj : s_gameObjects)
		delete obj;
}
#pragma endregion