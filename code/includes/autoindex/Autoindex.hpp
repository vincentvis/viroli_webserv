#pragma once

#include <dirent.h>
#include <sstream>
#include <string>
#include <vector>

#define AUTOINDEX_TEMPLATE \
	"<!DOCTYPE html>\
<html lang=\"en\">\
<head>\
    <meta charset=\"UTF-8\">\
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
    <title>${TITLE}</title>\
</head>\
<body>\
	${BODY}\
	<p><i>We think directory listings are a bad idea<i></p>\
    \
</body>\
</html>"

class Autoindex {
	public:
		Autoindex();
		Autoindex(const std::string &root);
		~Autoindex();

	private:
		Autoindex(const Autoindex &other);
		Autoindex        &operator=(const Autoindex &other);
		void              setTitle(std::string &title);

		const std::string _root;
		std::string       _template;
};
