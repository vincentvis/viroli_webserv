#pragma once

#include <string>

class Location {
	public:
		Location();
		~Location();

		std::string getMatch() const;
		void        setMatch(std::string value);

		bool        getExactMatch() const;
		void        setExactMatch(bool value);

		std::string getRoot() const;
		void        setRoot(std::string value);

		std::string getAllow() const;
		void        setAllow(std::string value);

		std::string getIndex() const;
		void        setIndex(std::string value);

		bool        getAutoIndex() const;
		void        setAutoIndex(bool value);


	private:
		// disable copying
		Location(const Location &other);
		Location   &operator=(const Location &other);

		std::string _match;
		bool        _exactMatch;
		std::string _root;
		std::string _allow;
		std::string _index;
		bool        _autoIndex;
};
