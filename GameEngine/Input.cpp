#include "Input.h"
#include "GameEngine.h"
#include <GLFW/glfw3.h>

static GLFWwindow* s_window = nullptr;
static bool s_key[GLFW_KEY_LAST + 1] = {};
static bool s_keyDown[GLFW_KEY_LAST + 1] = {};
static bool s_keyUp[GLFW_KEY_LAST + 1] = {};

#pragma region public
bool Input::getKey(int key) { return s_key[key]; }
bool Input::getKeyDown(int key) { return s_keyDown[key]; }
bool Input::getKeyUp(int key) { return s_keyUp[key]; }
#pragma endregion




#pragma region private
void Input::init()
{
    s_window = GameEngine::window();
}
void Input::update()
{
    for (int i = GLFW_KEY_SPACE; i <= GLFW_KEY_LAST; ++i)
    {
        if (glfwGetKey(s_window, i) == GLFW_PRESS)
        {
            if (s_key[i] == false)
                s_keyDown[i] = true;
            else
                s_keyDown[i] = false;
            s_key[i] = true;
            s_keyUp[i] = false;
        }
        else
        {
            if (s_key[i] == true)
                s_keyUp[i] = true;
            else
                s_keyUp[i] = false;
            s_key[i] = false;
            s_keyDown[i] = false;
        }
    }
}
#pragma endregion