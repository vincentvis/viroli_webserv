#include "autoindex/Autoindex.hpp"

Autoindex::Autoindex() {
	this->_template = std::string(AUTOINDEX_TEMPLATE);
}

Autoindex::Autoindex(const std::string &root) : _root(root) {
	this->_template = std::string(AUTOINDEX_TEMPLATE);
}

Autoindex::~Autoindex() {
}

void Autoindex::setTitle(std::string &title) {
	std::string::size_type title_pos = this->_template.find("${TITLE}");
	if (title_pos == std::string::npos) {
		return;
	}
	this->_template.replace(title_pos, title_pos + 8, title);
}