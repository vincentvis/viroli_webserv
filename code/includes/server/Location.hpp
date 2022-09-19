#pragma once

#include "utils/Utils.hpp"
#include <iostream>
#include <map>
#include <string>
#include <vector>

class Location {
	public:
		Location();
		~Location();

		friend class ConfigParser;
		friend std::ostream &operator<<(std::ostream &os, const Location &location);

		// getters
		std::string                        getMatch() const;
		std::string                        getRoot() const;
		std::vector<std::string>           getAllow() const;
		std::vector<std::string>           getIndex() const;
		std::map<std::string, std::string> getErrorPages() const;
		bool                               getAutoIndex() const;

		int                                getSortWeight() const;

	private:
		std::string                        _match;
		std::string                        _root;
		std::vector<std::string>           _allow;
		std::vector<std::string>           _index;
		std::map<std::string, std::string> _errorPages;
		bool                               _autoIndex;
		int64_t                            _maxBodySize;
		std::string                        _redirect;
		std::string                        _redirectType;
		bool                               _shouldRedirect;
		int                                _sortWeight;
};
