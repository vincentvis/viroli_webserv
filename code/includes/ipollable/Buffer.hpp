#pragma once

#include "utils/Defines.hpp"
#include <vector>

class Buffer {
	public:
		static Buffer     &getInstance();
		std::vector<char> &getBuff();

	private:
		Buffer();
		std::vector<char> _buff;
};
