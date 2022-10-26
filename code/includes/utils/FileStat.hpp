#pragma once

#include <iostream>
#include <string>
#include <sys/stat.h>

class FileStat {
	public:
		FileStat();
		FileStat(std::string fullpath);
		FileStat(std::string path, std::string file);

		bool               isDir() const;
		bool               isReg() const;
		bool               isWriteable() const;
		bool               isExecutable() const;
		bool               isReadable() const;

		const std::string &getPath() const;
		const std::string &getFilename() const;
		const std::string &getExtension() const;
		const std::string &getFull() const;

	private:
		struct stat _stat;
		std::string _path;
		std::string _filename;
		std::string _extension;
		std::string _fullpath;
		bool        _isOk;
		void        setFilenameAndPathFromFullpath(void);
};
