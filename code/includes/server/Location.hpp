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
		std::string const                 &getMatch() const;
		std::string const                 &getRoot() const;
		std::vector<std::string> const    &getAllow() const;
		std::vector<std::string> const    &getIndex() const;
		std::map<std::string, std::string> getErrorPages() const;
		std::string const                 &getAutoIndex() const;
		int64_t                            getMaxBodySize() const;

		int                                getSortWeight() const;

		bool                               getShouldRedirect() const;
		std::string const                 &getRedirect() const;
		std::string const                 &getRedirectType() const;

	private:
		std::string                        _match;
		std::string                        _root;
		std::vector<std::string>           _allow;
		std::vector<std::string>           _index;
		std::map<std::string, std::string> _errorPages;
		std::string                        _autoIndex;
		int64_t                            _maxBodySize;
		std::string                        _redirect;
		std::string                        _redirectType;
		bool                               _shouldRedirect;
		int                                _sortWeight;
};
