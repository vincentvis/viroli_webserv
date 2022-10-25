#include "utils/FileStat.hpp"

FileStat::FileStat() {
	_isOk = false;
}

FileStat::FileStat(std::string fullpath) : _fullpath(fullpath) {
	_isOk        = true;
	int stat_ret = stat(_fullpath.c_str(), &_stat);
	if (stat_ret == -1) {
		_isOk = false;
	}

	// split if it is a file
	if (isReg()) {
		std::string::size_type last_slash = fullpath.find_last_of("/");
		if (last_slash == std::string::npos) {
			_filename = fullpath;
			_path     = "./";
		} else {
			_filename = fullpath.substr(last_slash + 1);
			_path     = fullpath.substr(0, last_slash + 1);
		}
		_fullpath                      = _path + _filename;
		std::string::size_type dot_pos = _filename.find_last_of(".");
		if (dot_pos != std::string::npos) {
			_extension = _filename.substr(dot_pos + 1);
		}
	}
	if (isDir()) {
		_path = fullpath;
		if (*_path.rbegin() != '/') {
			_path.append("/");
		}
	}
}

FileStat::FileStat(std::string path, std::string filename) :
	_path(path), _filename(filename) {
	while (_filename.at(0) == '/') {
		_filename = _filename.substr(1);
	}
	if (*_path.rbegin() != '/') {
		_path.append("/");
	}
	_fullpath    = _path + _filename;


	_isOk        = true;
	int stat_ret = stat(_fullpath.c_str(), &_stat);
	if (stat_ret == -1) {
		_isOk = false;
	}

	if (isReg()) {
		std::string::size_type dot_pos = _filename.find_last_of(".");
		if (dot_pos != std::string::npos) {
			_extension = _filename.substr(dot_pos + 1);
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
