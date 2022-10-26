#include "utils/FileStat.hpp"

FileStat::FileStat() {
	_isOk = false;
}

FileStat::FileStat(std::string fullpath) : _fullpath(fullpath) {
	_isOk = true;

	// what if fullpath is a file or a directory? should we check for slash at the end..
	int stat_ret = stat(_fullpath.c_str(), &_stat);
	if (stat_ret == -1) {
		_isOk = false;
	}
	setFilenameAndPathFromFullpath();
}

FileStat::FileStat(std::string path, std::string filename) {
	while (filename.empty() == false && filename.at(0) == '/') {
		filename = filename.substr(1);
	}
	if (path.empty() == false && *path.rbegin() != '/') {
		path.append("/");
	}
	_fullpath = path + filename;

	// this is important to have here since it gets messed up if filename is empty or
	// if the file does not exist
	if (*_fullpath.rbegin() == '/') {
		_path     = _fullpath;
		_filename = "";
	} else {
		std::string::size_type last_slash = _fullpath.find_last_of("/");
		_path                             = _fullpath.substr(0, last_slash + 1);
		_filename                         = _fullpath.substr(last_slash + 1);
	}

	_isOk        = true;
	int stat_ret = stat(_fullpath.c_str(), &_stat);
	if (stat_ret == -1) {
		_isOk = false;
	}
	// setFilenameAndPathFromFullpath();
}

void FileStat::setFilenameAndPathFromFullpath(void) {
	// split if it is a file
	if (isReg()) {
		std::string::size_type last_slash = _fullpath.find_last_of("/");
		if (last_slash == std::string::npos) {
			_filename = _fullpath;
			_path     = "./";
		} else {
			_filename = _fullpath.substr(last_slash + 1);
			_path     = _fullpath.substr(0, last_slash + 1);
		}
		_fullpath                      = _path + _filename;
		std::string::size_type dot_pos = _filename.find_last_of(".");
		if (dot_pos != std::string::npos) {
			_extension = _filename.substr(dot_pos + 1);
		}
	}
	if (isDir()) {
		_path = _fullpath;
		if (*_path.rbegin() != '/') {
			_path.append("/");
		}
	}
}

bool FileStat::isDir() const {
	return (_isOk && S_ISDIR(_stat.st_mode));
}

bool FileStat::isReg() const {
	return (_isOk && S_ISREG(_stat.st_mode));
}

bool FileStat::isWriteable() const {
	return (_isOk && (_stat.st_mode & S_IWUSR));
}

bool FileStat::isExecutable() const {
	return (_isOk && (_stat.st_mode & S_IXUSR));
}

bool FileStat::isReadable() const {
	return (_isOk && (_stat.st_mode & S_IRUSR));
}

const std::string &FileStat::getPath() const {
	return (_path);
}

const std::string &FileStat::getFilename() const {
	return (_filename);
}

const std::string &FileStat::getFull() const {
	return (_fullpath);
}

const std::string &FileStat::getExtension() const {
	return (_extension);
}
