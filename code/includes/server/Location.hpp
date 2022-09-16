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

		//
		std::string                         getMatch() const;
		void                                setMatch(std::string value);

		std::string                         getRoot() const;
		void                                setRoot(std::string value);

		std::vector<std::string>            getAllow() const;
		void                                addAllow(std::string value);

		std::vector<std::string>            getIndex() const;
		std::vector<std::string>           *getIndexVector();
		void                                addIndex(std::string value);

		std::map<std::string, std::string>  getErrorPages() const;
		std::map<std::string, std::string> *getErrorPagesMap();
		void addErrorPages(std::string key, std::string value);

		bool getAutoIndex() const;
		void setAutoIndex(bool value);

		int  getSortWeight() const;


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
		int                                _sortWeight; // << _-- IMPLEMENT SORTING
};
