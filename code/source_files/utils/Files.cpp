#include "utils/Files.hpp"

Files::Files(std::string fullpath) {
	int stat_ret = stat(fullpath.c_str(), &_stat);
	if (stat_ret == -1) {
		_isOk = false;
		return;
	}
}

Files::Files(std::string path, std::string filename) {
}
