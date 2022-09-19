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

		const Location       findLocation(struct tmp_request &request) const;

		friend std::ostream &operator<<(std::ostream &os, const Config &config);
		friend class ConfigParser;

		// getters
		std::string              getRoot() const;
		std::vector<std::string> getAllow() const;
		int64_t                  getMaxBodySize() const;
		bool                     getAllowUpload() const;
		bool                     getAutoIndex() const;
		bool                     containsServerName(std::string to_search);

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
		std::vector<Location>              _locations;

	private:
};
