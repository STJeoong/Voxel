#include "GameEngine.h"
#include <exception>
#include <iostream>

int main()
{
	try
	{
		GameEngine::run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Fatal exception happened: " << e.what() << '\n';
	}
	return 0;
}