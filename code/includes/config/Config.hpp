#pragma once

#include "request/Request.hpp"
#include "server/Location.hpp"
#include "utils/Utils.hpp"
#include <algorithm>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <stdint.h>
#include <vector>

class Request;

class Config {
	public:
		Config();
		~Config();

		Location            *findLocation(const Request &request);

		friend std::ostream &operator<<(std::ostream &os, const Config &config);
		friend class ConfigParser;

		// getters
		std::string const              &getRoot() const;
		std::string const              &getRoot(Location *const primary) const;

		std::vector<std::string> const &getIndex() const;
		std::vector<std::string> const &getIndex(Location *const primary) const;

		std::vector<std::string> const &getAllow() const;
		std::vector<std::string> const &getAllow(Location *const primary) const;

		int64_t                         getMaxBodySize() const;
		int64_t                         getMaxBodySize(Location *const primary) const;

		bool                            getAllowUpload() const;
		bool                            getAllowUpload(Location *const primary) const;

		std::string const              &getAutoIndex() const;
		std::string const              &getAutoIndex(Location *const primary) const;

		bool                            containsServerName(std::string to_search);
		std::vector<Location> const    &getLocations();

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
		std::string                        _autoIndex;
		std::vector<Location>              _locations;

	private:
};
