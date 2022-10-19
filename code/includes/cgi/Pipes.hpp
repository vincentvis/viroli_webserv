#pragma once

#include "utils/Defines.hpp"
#include "utils/Exception.hpp"
#include <string>
#include <unistd.h>

enum { READ_FD, WRITE_FD, SYS_ERR = -1 };

class Pipes {
	public:
		Pipes();
		~Pipes();

		void closeForParent();
		void closeForChild();
		void tryClose(int fd);
		void openPipes();

		int  toCgi[2];
		int  toServer[2];
};