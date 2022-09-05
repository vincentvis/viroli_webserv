
#include "config/ConfigParser.hpp"
#include "config/Directive.hpp"
#include <iostream>
#include <string>

int main(int argc, char const *argv[])
{
	ConfigParser           config;
	std::vector<Directive> parseResult;

	try
	{
		config.parseFromArgs(argc, argv);
	}
	catch (const std::exception &e)
	{
		std::cerr << "exception: \n" << e.what() << std::endl;
		return (1);
	}

	config.printDirectiveInfo();

	// std::cout << "Ran " << argv[0] << " with argc: " << argc << std::endl;

	return 0;
}
