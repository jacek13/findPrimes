#include <exception>

#include"client.h"

int main(int argc, char ** args)
{
	try
	{
		Client app(args, argc);
		app.init();
		app.loop();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}