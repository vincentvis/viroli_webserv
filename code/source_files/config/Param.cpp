#include "config/Param.hpp"

Param::Param() {
}

Param::Param(std::string name) {
	_name = name;
}

std::string Param::getName() {
	return (_name);
}

void Param::setName(std::string name) {
	_name = name;
}

void Param::addValue(std::string value) {
	_values.push_back(value);
}

std::vector<std::string>::size_type Param::getNumValues() {
	return (_values.size());
}

void Param::setChildren(std::map<std::string, std::vector<Param> > children) {
	_children = children;
}

std::ostream &operator<<(std::ostream &os, const Param &param) {
	os << "{" << std::endl << "\tname: \"" << param._name << "\"" << std::endl;
	os << "\tvalues: [";
	for (std::size_t i = 0; i < param._values.size(); i++) {
		os << "'" << param._values[i] << "'";
		if (i != param._values.size() - 1)
			os << ", ";
	}
	os << "]" << std::endl;
	if (param._children.size() > 0) {
		std::cout << "children [" << std::endl;
		param.printMap(param._children);
		std::cout << "]," << std::endl;
	}

	return os;
}

void Param::printVectorOfMaps(
	std::vector<std::map<std::string, std::vector<Param> > > maps
) {
	setDepth(0);
	std::vector<std::map<std::string, std::vector<Param> > >::iterator elem =
		maps.begin();
	std::vector<std::map<std::string, std::vector<Param> > >::iterator end = maps.end();

	while (elem != end) {
		std::cout << "server [" << std::endl;
		printMap(*elem);
		std::cout << "]," << std::endl;
		elem++;
	}
}

void Param::printMap(std::map<std::string, std::vector<Param> > currentMap) const {
	std::map<std::string, std::vector<Param> >::iterator elem = currentMap.begin();
	std::map<std::string, std::vector<Param> >::iterator end  = currentMap.end();

	while (elem != end) {
		std::vector<Param>::iterator directive    = elem->second.begin();
		std::vector<Param>::iterator directiveEnd = elem->second.end();
		while (directive != directiveEnd) {
			std::cout << *directive;
			directive++;
		}
		elem++;
	}
}
