#pragma once

#include <string>
#include <sys/stat.h>

class Files {
	public:
		Files(std::string fullpath);
		Files(std::string path, std::string file);

	private:
		Files();
		struct stat _stat;
		std::string _path;
		std::string _filename;
		bool        _isOk;
		bool        _isDir;
		bool        _isFile;
};
