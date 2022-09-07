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


// Param(const Param &other) {
// }

// Param &operator=(const Param &other) {
// }

// Param::~Param() {
// }
