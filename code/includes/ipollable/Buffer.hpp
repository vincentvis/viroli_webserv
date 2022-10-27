#pragma once

#include "utils/Defines.hpp"
#include <vector>

class Buffer {
	public:
		Buffer();
		~Buffer();

		static Buffer     &getInstance();
		std::vector<char> &getBuff();

	private:
		std::vector<char> _buff;
};
