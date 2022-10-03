#pragma once

#include "utils/Exception.hpp"
#include <dirent.h>
#include <iostream>
#include <sys/dir.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
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
${LIST}\
	</ul>\n\
	<hr>\n\
	<p><i>We think directory listings are a bad idea</i></p>\n\
    \n\
</body>\n\
</html>"


#define AUTOINDEX_LI(dir) \
	std::string("\t\t<li><a href=\"./") + (dir) + "\">" + (dir) + "</a></li>\n"
#define AUTOINDEX_LI_DIR(dir) \
	std::string("\t\t<li><a href=\"./") + (dir) + "/\">" + (dir) + "</a></li>\n"

class Autoindex {
	public:
		Autoindex(const std::string &root);
		~Autoindex();
		std::string getHtml();

	private:
		Autoindex();
		Autoindex(const Autoindex &other);
		Autoindex        &operator=(const Autoindex &other);
		bool              template_replace(std::string match, const std::string &value);

		const std::string _root;
		std::string       _template;
};
