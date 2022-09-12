#pragma once

#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class Param {
	private:
		static int _depth;

	protected:
		std::string                                _name;
		std::vector<std::string>                   _values;
		std::map<std::string, std::vector<Param> > _children;

		//
		static int getDepth() {
			return _depth;
		};
		static int incrementDepth() {
			_depth++;
			return _depth;
		};
		static int decrementDepth() {
			_depth--;
			return _depth;
		};
		static int setDepth(int newDepth) {
			Param::_depth = newDepth;
			return _depth;
		};


	public:
		Param();
		// Param(const Param &Param);
		// Param &operator=(const Param &Param);
		// ~Param();
		Param(std::string name);

		std::string getName();
		void        setName(std::string name);
		void        addValue(std::string value);
		void        setChildren(std::map<std::string, std::vector<Param> > children);
		std::vector<std::string>::size_type getNumValues();

		friend std::ostream &operator<<(std::ostream &os, const Param &param);
		void
		printVectorOfMaps(std::vector<std::map<std::string, std::vector<Param> > > map);
		void printMap(std::map<std::string, std::vector<Param> > currentMap) const;
};
