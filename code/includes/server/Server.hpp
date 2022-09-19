#pragma once

#include "config/Config.hpp"
#include "utils/Utils.hpp"
#include <iostream>
#include <stdint.h>
#include <vector>

// temporary object, should be replace by actuall Request class
struct tmp_request {
		std::string host;
		std::string path;
};

class Server {
	public:
		Server();
		~Server();

		const Config         findConfig(struct tmp_request &request) const;

		friend std::ostream &operator<<(std::ostream &os, const Server &server);
		friend class ConfigParser;

		// getters
		uint16_t getPort() const;

	protected:
		uint16_t              _port;
		std::vector<Config *> _configs;
};
