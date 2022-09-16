#pragma once

#include "config/DirectiveParam.hpp"

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

class Directive {
	public:
		Directive();
		Directive(std::string name);
		Directive(std::string name, std::string param1);
		Directive(std::string name, std::string param1, std::string param2);

		bool                        isInitialized();
		std::vector<Directive>      getChildren() const;
		std::vector<Directive>     *getChildVector();
		std::vector<DirectiveParam> getParameters();
		std::string                 getName() const;
		void                        setDirectiveName(std::string name);
		DirectiveParam              addParam(std::string newParam);
		Directive                   addChild(Directive newChild);

		void                        printDirectiveInfo(int depth = 0);

	private:
		bool                        _initialized;
		std::string                 _directiveName;
		std::vector<DirectiveParam> _parameters;
		std::vector<std::string>    _stringParams;
		std::vector<Directive>      _children;
};