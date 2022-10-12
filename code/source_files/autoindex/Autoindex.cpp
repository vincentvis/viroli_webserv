#include "autoindex/Autoindex.hpp"

Autoindex::Autoindex(const std::string &root) : _root(root) {
	DIR *dir_stream = opendir(this->_root.c_str());

	if (dir_stream == NULL) {
		throw Utils::AutoindexException("Unable to open directory");
	}

	struct dirent **namelist = NULL;
	int             n        = scandir(_root.c_str(), &namelist, NULL, alphasort);
	if (n < 0) {
		throw Utils::AutoindexException("Failed to create directory list");
	}

	int         i = 0;
	std::string currentDir(".");
	std::string slash("/");
	std::string list;

	while (i < n) {
		if (namelist[i]->d_name != currentDir) {
			std::string dir_str  = this->_root;
			std::string dir_name = slash + namelist[i]->d_name + slash;
			dir_str += dir_name;
			errno       = 0;
			DIR *is_dir = opendir(dir_str.c_str());
			if (is_dir) {
				closedir(is_dir);
			}

			if (errno == ENOTDIR) {
				list += AUTOINDEX_LI(namelist[i]->d_name);
			} else {
				list += AUTOINDEX_LI_DIR(namelist[i]->d_name);
			}
		}
		free(namelist[i]);
		i++;
	}
	free(namelist);
	this->_template = std::string(AUTOINDEX_TEMPLATE);
	template_replace("${ROOT}", _root);
	template_replace("${TITLE}", _root);
	template_replace("${LIST}", list);
	closedir(dir_stream);
}

Autoindex::~Autoindex() {
}

std::string Autoindex::getHtml() {
	return _template;
}

bool Autoindex::template_replace(std::string match, const std::string &value) {
	std::string::size_type pos = this->_template.find(match);
	if (pos == std::string::npos) {
		return (false);
	}
	this->_template.replace(pos, match.length(), value);
	return (true);
}
