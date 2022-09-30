#pragma once

#include <dirent.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define AUTOINDEX_TEMPLATE \
	"<!DOCTYPE html>\n\
<html lang=\"en\">\n\
<head>\n\
    <meta charset=\"UTF-8\">\n\
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\
    <title>Directory listing for ${TITLE}</title>\n\
</head>\n\
<body>\n\
	<h1>Index of ${ROOT}</h1>\n\
	<hr>\n\
	<ul>\n\
	${LIST}\n\
	</ul>\n\
	<hr>\n\
	<p><i>We think directory listings are a bad idea</i></p>\n\
    \n\
</body>\n\
</html>"

class Autoindex {
	public:
		Autoindex(const std::string &root);
		~Autoindex();

	private:
		Autoindex();
		Autoindex(const Autoindex &other);
		Autoindex        &operator=(const Autoindex &other);
		bool              template_replace(std::string match, const std::string &value);

		const std::string _root;
		std::string       _template;
};
