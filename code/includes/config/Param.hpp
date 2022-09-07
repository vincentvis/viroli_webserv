#pragma once

#include <map>
#include <string>
#include <vector>

class Param {
	protected:
		std::string                                _name;
		std::vector<std::string>                   _values;
		std::map<std::string, std::vector<Param> > _children;

	public:
		Param();
		Param(std::string name);
		// Param(const Param &Param);
		// Param &operator=(const Param &Param);
		// ~Param();

		std::string getName();
		void        setName(std::string name);
};
