#pragma once

#include "utils/Defines.hpp"
#include "utils/Utils.hpp"
#include <map>
#include <string>

class Executables {
	public:
		static Executables &getInstance() {
			static Executables instance;
			return instance;
		}
		static const std::string &getExecutable(const std::string &ext) {
			std::map<std::string, std::string>::iterator it =
				getInstance()._executables.find(ext);
			if (it != getInstance()._executables.end()) {
				return (it->second);
			}
			return (Utils::default_executable);
		}

		static bool isCgiRequest(const std::string &uri) {
			std::string::size_type dot_pos = uri.find_last_of(".");
			if (dot_pos == std::string::npos || dot_pos >= uri.length()) {
				return (false);
			}
			std::string                                  ext = uri.substr(dot_pos + 1);
			std::map<std::string, std::string>::iterator it =
				getInstance()._executables.find(ext);
			if (it == getInstance()._executables.end()) {
				return (false);
			}
			return (true);
		}

	private:
		std::map<std::string, std::string>           _executables;
		std::map<std::string, std::string>::iterator _end;
		Executables() {
			_executables["py"] = "python";
			_executables["sh"] = "bash";
			_end               = _executables.end();
		}
		Executables(Executables const &);
		void operator=(Executables const &);
};
