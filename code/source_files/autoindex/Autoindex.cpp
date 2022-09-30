#include "autoindex/Autoindex.hpp"

Autoindex::Autoindex(const std::string &root) : _root(root) {
	this->_template = std::string(AUTOINDEX_TEMPLATE);
	template_replace("${ROOT}", _root);
	template_replace("${TITLE}", _root);

	DIR           *dir_stream = opendir(this->_root.c_str());
	struct dirent *dir;

	if (dir_stream == NULL) {
		throw std::runtime_error("Unable to open directory");
	}

	std::string list;
	std::string currentString(".");
	while (true) {
		dir = readdir(dir_stream);
		if (dir == nullptr) {
			break;
		}
		if (std::string(dir->d_name) == currentString) {
			continue;
		}
		list += createListItem(dir->d_name, dir->d_type == DT_DIR);
	}
	template_replace("${LIST}", list);
	std::cout << this->_template << std::endl;
}

std::string Autoindex::createListItem(std::string name, bool isdir) {
	if (isdir) {
		if (name.at(0) == '.') {
			return (LIST_START + A_HREF_OPEN + name + A_HREF_CLOSE + name + LIST_END);
		}
		return (LIST_START + A_HREF_OPEN + "/" + name + A_HREF_CLOSE + name + LIST_END);
	}
	return ("\t\t<li><a href=\"" + name + "\">" + name + (append_slash ? "/" : "") +
			"</a></li>\n");
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
