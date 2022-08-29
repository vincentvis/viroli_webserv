#pragma once

#include "config/DirectiveParam.hpp"
#include <vector>

class Directive
{
	public:
		Directive();
		// Directive(const Directive &other); 				// why is this needed again?
		// Directive &operator=(const Directive &other);	// why is this needed again?
		// ~Directive();

		std::vector<Directive>      getChildren();
		std::vector<DirectiveParam> getParameters();
		std::string                 getName();
		void                        setDirectiveName(std::string name);
		Directive                   addChild(Directive newChild);

	private:
		std::string                 _directiveName;
		std::vector<DirectiveParam> _parameters;
		std::vector<Directive>      _children;
};
