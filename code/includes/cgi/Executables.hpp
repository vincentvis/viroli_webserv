#pragma once

#include "utils/Defines.hpp"
#include "utils/Exception.hpp"
#include "utils/FileStat.hpp"
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
			if (it != getInstance()._end) {
				return (it->second);
			}
			return (Utils::default_executable);
		}

		static bool isCgiRequest(const std::string &uri, const std::string &root) {
			FileStat file(root, uri);

			if (file.isReg() == false) {
				return (false);
			}
			std::map<std::string, std::string>::iterator it =
				getInstance()._executables.find(file.getExtension());
			if (it == getInstance()._end) {
				return (false);
			}
			return (true);
		}

		static bool isCgiRequest(FileStat file) {
			if (file.isReg() == false) {
				return (false);
			}
			std::map<std::string, std::string>::iterator it =
				getInstance()._executables.find(file.getExtension());
			if (it == getInstance()._end) {
				return (false);
			}
			return (true);
		}

		static std::string findExecutableInPath(std::string &executable_name) {
			char *envpath = getenv("PATH");
			if (envpath == NULL) {
				throw Utils::ErrorPageException("502");
			}
			std::string            pathvar = std::string(envpath);
			std::string::size_type offset  = 0;
			std::string::size_type pos     = pathvar.find(":", offset);
			while (pos != std::string::npos) {
				std::string path = pathvar.substr(offset, pos - offset);
				FileStat    executable(path, executable_name);
				if (executable.isExecutable()) {
					return (executable.getFull());
				}
				offset = pos + 1;
				pos    = pathvar.find(":", offset);
			}
			throw Utils::ErrorPageException("502");
			return ("");
		}

	private:
		std::map<std::string, std::string>           _executables;
		std::map<std::string, std::string>::iterator _end;
		Executables() {
			_executables["py"] = "python3";
			_executables["sh"] = "bash";
			_end               = _executables.end();
		}
		Executables(Executables const &);
		void operator=(Executables const &);
};
