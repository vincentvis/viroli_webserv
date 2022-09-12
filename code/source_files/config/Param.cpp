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
	os << "{" << std::endl << "\tname:" << param._name << std::endl;
	os << "\tvalues: {" << std::endl;
	for (std::size_t i = 0; i < param._values.size(); i++) {
		os << "\t\t" << i << ": " << param._values[i] << std::endl;
	}
	os << "\t}," << std::endl;
	return os;
}
