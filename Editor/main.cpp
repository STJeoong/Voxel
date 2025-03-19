#include <GameEngine.h>
#include <iostream>
#include <fmt/core.h>

int main(int argc, char** argv)
{
	try
	{
		GameEngine::init();
		GameEngine::run();
		GameEngine::terminate();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}

	
	return 0;
}