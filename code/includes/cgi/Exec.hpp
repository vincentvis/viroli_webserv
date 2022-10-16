#pragma once

#include "cgi/Pipes.hpp"
#include <unistd.h>

class Exec {
	public:
		Exec();
		~Exec();

	private:
		Pipes pipes;
};
