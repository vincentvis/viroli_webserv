#include "server/Location.hpp"

Location::Location() {
	_match          = "";
	_root           = "";
	_autoIndex      = false;
	_sortWeight     = 0;
	_shouldRedirect = false;
	_redirect       = "";
	_redirectType   = "";
	_maxBodySize    = -1;
}

Location::~Location() {
}

// getters
std::string Location::getMatch() const {
	return _match;
}

std::string Location::getRoot() const {
	return _root;
}

std::vector<std::string> Location::getAllow() const {
	return _allow;
}

std::vector<std::string> Location::getIndex() const {
	return _index;
}

std::map<std::string, std::string> Location::getErrorPages() const {
	return _errorPages;
}

bool Location::getAutoIndex() const {
	return _autoIndex;
}

int Location::getSortWeight() const {
	return _sortWeight;
}

// stream output
std::ostream &operator<<(std::ostream &os, const Location &location) {
#define PRINT_ALIGN "    " << std::setw(15) << std::left

	os << "{" << std::endl;

	os << PRINT_ALIGN << "Match"
	   << "   : " << location._match << std::endl;

	os << PRINT_ALIGN << "Root"
	   << "   : " << location._root << std::endl;

	os << PRINT_ALIGN << "Allow"
	   << "(" << location._allow.size() << "): [";
	Utils::print_vector<std::string>(location._allow, "\"", "\"", ", ");
	os << "]" << std::endl;

	os << PRINT_ALIGN << "Index"
	   << "(" << location._index.size() << "): [";
	Utils::print_vector<std::string>(location._index, "\"", "\"", ", ");
	os << "]" << std::endl;

	os << PRINT_ALIGN << "Error pages"
	   << "(" << location._errorPages.size() << "): [";
	Utils::print_map<std::string, std::string>(
		location._errorPages, "\n\t\t\t  {", ": ", "}", ", "
	);
	if (location._errorPages.size()) {
		os << std::endl << "\t\t\t]" << std::endl;
	} else {
		os << "]" << std::endl;
	}

	os << PRINT_ALIGN << "Autoindex"
	   << "   : " << std::boolalpha << location._autoIndex << std::endl;

	os << PRINT_ALIGN << "max body size"
	   << "   : " << location._maxBodySize << std::endl;

	os << PRINT_ALIGN << "Redirect"
	   << "   : " << location._redirect << std::endl;

	os << PRINT_ALIGN << "Redirect type"
	   << "   : " << location._redirectType << std::endl;

	os << "},";

#undef PRINT_ALIGN

	return os;
}
