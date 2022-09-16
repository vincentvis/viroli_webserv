#pragma once

#include "server/Location.hpp"
#include "utils/Utils.hpp"
#include <iostream>
#include <map>
#include <stdint.h>
#include <vector>

class Server {
	private:

	protected:
		std::vector<uint16_t>              _ports;
		std::vector<std::string>           _ips;
		std::vector<std::string>           _serverNames;
		std::string                        _root;
		std::vector<std::string>           _allow;
		std::vector<std::string>           _index;
		int64_t                            _maxBodySize;

		std::string                        _defaultIndexPage;
		std::string                        _defaultErrorPage;
		std::map<std::string, std::string> _errorPages;

		bool                               _allowUpload;
		bool                               _autoIndex;
		int                                _bufferSize;
		std::vector<Location>              _locations; // moet nog aangemaakt worden

	public:
		Server();
		~Server();
		friend std::ostream &operator<<(std::ostream &os, const Server &server);
		friend class ConfigParser;
};
