#include "config/Directive.hpp"

Directive::Directive()
{
}

Directive::Directive(std::string name)
{
	_directiveName = name;
}

Directive::Directive(std::string name, std::string param1)
{
	_directiveName = name;
	_parameters.push_back(param1);
}

Directive::Directive(std::string name, std::string param1, std::string param2)
{
	_directiveName = name;
	_parameters.push_back(param1);
	_parameters.push_back(param2);
}


// Directive::~Directive()
// {
// }

// Directive(const Directive &other)
// {
// }

// Directive &operator=(const Directive &other)
// {
// }

std::vector<Directive> Directive::getChildren()
{
	return _children;
}

std::vector<std::string> Directive::getParameters()
{
	return _parameters;
}

std::string Directive::getName()
{
	return _directiveName;
}

Directive Directive::addChild(Directive newChild)
{
	_children.push_back(newChild);
	return _children.back();
}

std::string Directive::addParam(std::string param)
{
	_parameters.push_back(param);
	return (_parameters.back());
}

void Directive::setDirectiveName(std::string name)
{
	_directiveName = name;
}
