#pragma once

#include "utils/Defines.hpp"
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

	private:
		std::map<std::string, std::string> _executables;
		Executables() {
			_executables["py"] = "python";
			_executables["sh"] = "bash";
		}
		Executables(Executables const &);
		void operator=(Executables const &);
};
