#include "Input.h"

#pragma region public static
void Input::keyDown(unsigned int keyCode) { _keys[keyCode] = true; }
void Input::keyUp(unsigned int keyCode) { _keys[keyCode] = false; }
bool Input::isKeyDown(unsigned int keyCode) { return keyCode >= 0 && keyCode < 256 && _keys[keyCode]; }
#pragma endregion

#pragma region private static
#pragma endregion

bool Input::_keys[256] = {};