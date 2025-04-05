#include "GameEngine.h"
#include <iostream>

int main(int argc, char** argv)
{
	try
	{
		GameEngine::init();
		GameEngine::run();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
	GameEngine::terminate();

	return 0;
}