
#include "config/ConfigParser.hpp"
#include "config/Directive.hpp"
#include <iostream>
#include <string>

int main(int argc, char const *argv[])
{
	try
	{
		ConfigParser config(argc, argv);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what();
		return (1);
	}
	std::cout << "Ran " << argv[0] << " with argc: " << argc << std::endl;

	return 0;
}
