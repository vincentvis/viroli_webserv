#include "config/Directive.hpp"

Directive::Directive()
{
	_initialized = false;
}

Directive::Directive(std::string name)
{
	_directiveName = name;
	_initialized   = true;
}

Directive::Directive(std::string name, std::string param1)
{
	_directiveName = name;
	_parameters.push_back(DirectiveParam(param1));
	_initialized = true;
}

Directive::Directive(std::string name, std::string param1, std::string param2)
{
	_directiveName = name;
	_parameters.push_back(DirectiveParam(param1));
	_parameters.push_back(DirectiveParam(param2));
	_initialized = true;
}

bool Directive::isInitialized()
{
	return _initialized;
}

std::vector<Directive> Directive::getChildren()
{
	return _children;
}

std::vector<Directive> *Directive::getChildVector()
{
	return &_children;
}

std::vector<DirectiveParam> Directive::getParameters()
{
	return _parameters;
}

std::string Directive::getName()
{
	return _directiveName;
}

Directive Directive::addChild(Directive newDirective)
{
	_children.push_back(newDirective);
	return (_children.back());
}

DirectiveParam Directive::addParam(std::string newParam)
{
	_parameters.push_back(DirectiveParam(newParam));
	return (_parameters.back());
}

void Directive::setDirectiveName(std::string name)
{
	_directiveName = name;
	_initialized   = true;
}

void Directive::addChildrenToVector(std::vector<Directive> children)
{
	for (std::vector<Directive>::size_type i = 0; i < children.size(); ++i)
	{
		_children.push_back(children[i]);
	}
}


void Directive::printDirectiveInfo(int depth)
{
	std::cout << std::setw(depth * 4) << ""
		  << "name: " << _directiveName << std::endl;
	if (_parameters.size() > 0)
	{
		std::cout << std::setw(depth * 4) << ""
			  << "params:" << std::endl
			  << std::setw(depth * 4) << ""
			  << "{" << std::endl;
		for (std::size_t i = 0; i < _parameters.size(); i++)
		{
			std::cout << std::setw((depth + 1) * 4) << "" << i << ": ";
			_parameters[i].printParam();
			std::cout << std::endl;
		}
		std::cout << std::setw(depth * 4) << ""
			  << "}," << std::endl;
		;
	}
	if (_children.size() > 0)
	{
		std::cout << std::setw(depth * 4) << ""
			  << "children:" << std::endl;
		for (std::size_t i = 0; i < _children.size(); i++)
		{
			std::cout << std::setw(depth * 4) << ""
				  << "{" << std::endl;
			_children[i].printDirectiveInfo(depth + 1);
			std::cout << std::setw(depth * 4) << ""
				  << "}," << std::endl;
		}
	}
}
