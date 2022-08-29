#include "config/Directive.hpp"

Directive::Directive()
{
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

std::vector<DirectiveParam> Directive::getParameters()
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

void Directive::setDirectiveName(std::string name)
{
	_directiveName = name;
}
