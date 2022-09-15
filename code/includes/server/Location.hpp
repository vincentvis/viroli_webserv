#pragma once

#include <map>
#include <string>
#include <vector>

class Location {
	public:
		Location();
		~Location();
		friend class ConfigParser;

		std::string                         getMatch() const;
		void                                setMatch(std::string value);

		bool                                getExactMatch() const;
		void                                setExactMatch(bool value);

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


	private:
		std::string                        _match;
		bool                               _exactMatch;
		std::string                        _root;
		std::vector<std::string>           _allow;
		std::vector<std::string>           _index;
		std::map<std::string, std::string> _errorPages;
		bool                               _autoIndex;
		int                                _sortWeight;
};
