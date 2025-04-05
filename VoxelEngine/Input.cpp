#include "Input.h"
#include "GameEngine.h"
#include "Structs.h"
#include <GLFW/glfw3.h>

static GLFWwindow* s_window = nullptr;
static float s_lastX = 0.0f;
static float s_lastY = 0.0f;
static float s_offsetX = 0.0f;
static float s_offsetY = 0.0f;

static bool s_key[GLFW_KEY_LAST + 1] = {};
static bool s_keyDown[GLFW_KEY_LAST + 1] = {};
static bool s_keyUp[GLFW_KEY_LAST + 1] = {};

#pragma region public
bool Input::getKey(int key) { return s_key[key]; }
bool Input::getKeyDown(int key) { return s_keyDown[key]; }
bool Input::getKeyUp(int key) { return s_keyUp[key]; }
float Input::getMouseAxisX() { return s_offsetX; }
float Input::getMouseAxisY() { return s_offsetY; }
#pragma endregion




#pragma region private
void Input::init()
{
    s_window = GameEngine::window();
    double x, y;
    glfwGetCursorPos(s_window, &x, &y);
    s_lastX = static_cast<float>(x);
    s_lastY = static_cast<float>(y);
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

    double currentX, currentY;
    glfwGetCursorPos(s_window, &currentX, &currentY);
    s_offsetX = static_cast<float>(currentX) - s_lastX;
    s_offsetY = s_lastY - static_cast<float>(currentY);
    s_lastX = static_cast<float>(currentX);
    s_lastY = static_cast<float>(currentY);
}
#pragma endregion