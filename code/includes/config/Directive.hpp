#pragma once

#include <string>
#include <vector>

class Directive
{
	public:
		Directive();
		Directive(std::string name);
		Directive(std::string name, std::string param1);
		Directive(std::string name, std::string param1, std::string param2);
		// Directive(const Directive &other); 				// why is this needed again?
		// Directive &operator=(const Directive &other);	// why is this needed again?
		// ~Directive();

		bool                     isInitialized();
		std::vector<Directive>   getChildren();
		std::vector<std::string> getParameters();
		std::string              getName();
		void                     setDirectiveName(std::string name);
		std::string              addParam(std::string param);
		Directive                addChild(Directive newChild);

	private:
		bool                     _initialized;
		std::string              _directiveName;
		std::vector<std::string> _parameters;
		std::vector<Directive>   _children;
};
