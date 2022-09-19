#pragma once

#include "server/Location.hpp"
#include "utils/Utils.hpp"
#include <iostream>
#include <map>
#include <stdint.h>
#include <vector>

class Config {
	public:
		Config();
		~Config();

		friend std::ostream &operator<<(std::ostream &os, const Config &config);
		friend class ConfigParser;

	protected:
		int32_t                            _priority;
		std::vector<uint16_t>              _ports;
		std::vector<std::string>           _ips;
		std::vector<std::string>           _serverNames;
		std::string                        _root;
		std::vector<std::string>           _allow;
		std::vector<std::string>           _index;
		std::map<std::string, std::string> _errorPages;
		int64_t                            _maxBodySize;
		bool                               _allowUpload;
		bool                               _autoIndex;
		int                                _bufferSize;
		std::vector<Location>              _locations;

	private:
};
