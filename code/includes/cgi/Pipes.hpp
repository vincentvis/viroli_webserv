#pragma once

#include "utils/Exception.hpp"

enum { READ_FD, WRITE_FD };

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
