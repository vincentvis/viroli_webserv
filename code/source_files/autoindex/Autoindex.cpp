#include "autoindex/Autoindex.hpp"

Autoindex::Autoindex(const std::string &root) : _root(root) {
	this->_template = std::string(AUTOINDEX_TEMPLATE);
	template_replace("${ROOT}", _root);
	template_replace("${TITLE}", _root);

	DIR           *dir_stream = opendir(this->_root);
	struct dirent *dir;

	if (dir_stream == NULL) {
	}
}

Autoindex::~Autoindex() {
}

bool Autoindex::template_replace(std::string match, const std::string &value) {
	std::string::size_type pos = this->_template.find(match);
	if (pos == std::string::npos) {
		return (false);
	}
	this->_template.replace(pos, match.length(), value);
	return (true);
}
