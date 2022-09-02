#pragma once

#include "config/DirectiveParam.hpp"

#include <iomanip>
#include <iostream>
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

		bool                        isInitialized();
		std::vector<Directive>      getChildren();
		std::vector<Directive>     *getChildVector();
		std::vector<DirectiveParam> getParameters();
		std::string                 getName();
		void                        setDirectiveName(std::string name);
		void                        addChildrenToVector(std::vector<Directive> children);
		DirectiveParam              addParam(std::string newParam);
		Directive                   addChild(Directive newChild);

		void printDirectiveInfo(int depth = 0);

	private:
		bool                        _initialized;
		std::string                 _directiveName;
		std::vector<DirectiveParam> _parameters;
		std::vector<Directive>      _children;
};
