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


// Directive::~Directive()
// {
// }

// Directive(const Directive &other)
// {
// }

// Directive &operator=(const Directive &other)
// {
// }

bool Directive::isInitialized()
{
	return _initialized;
}

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

Directive Directive::addChild(Directive newDirective)
{
	_children.push_back(newDirective);
	return (_children.back());
}

DirectiveParam Directive::addParam(std::string newParam)
{
	if (newParam == "false" || newParam == "true")
	{
		if (newParam == "true")
		{
			_parameters.push_back(DirectiveParam(false));
		}
		else
		{
			_parameters.push_back(DirectiveParam(true));
		}
		return (_parameters.back());
	}
	try
	{
		std::size_t pos = 0;
		int         num = std::stoi(newParam, &pos);
		if (pos != newParam.length())
		{
			_parameters.push_back(DirectiveParam(newParam));
			return (_parameters.back());
		}
		_parameters.push_back(DirectiveParam(num));
		return (_parameters.back());
	}
	catch (const std::exception &e)
	{
		// something went wrong in stoi.. so I guess it should just be a string?
	}
	_parameters.push_back(DirectiveParam(newParam));
	return (_parameters.back());
}

void Directive::setDirectiveName(std::string name)
{
	_directiveName = name;
	_initialized   = true;
}

void Directive::printDirectiveInfo(int depth)
{
	std::cout << std::setw(depth * 4) << ""
		  << "name: " << this->_directiveName << std::endl;
	if (this->_parameters.size() > 0)
	{
		std::cout << std::setw(depth * 4) << ""
			  << "params: {" << std::endl;
		for (std::size_t i = 0; i < this->_parameters.size(); i++)
		{
			std::cout << std::setw(depth * 4 + 2) << "" << i << ": ";
			this->_parameters[i].printParam();
			std::cout << std::endl;
		}
		std::cout << std::setw(depth * 4) << ""
			  << "}" << std::endl;
		;
	}
	if (this->_children.size() > 0)
	{
		std::cout << "children: {" << std::endl;
		for (std::size_t i = 0; i < this->_children.size(); i++)
		{
			this->_children[i].printDirectiveInfo(depth + 1);
		}
		std::cout << std::setw(depth * 4) << ""
			  << "}" << std::endl;
	}
}
