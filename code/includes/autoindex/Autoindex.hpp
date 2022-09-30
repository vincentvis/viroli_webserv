#pragma once

#include <dirent.h>
#include <iostream>
#include <sys/dir.h>
#include <sys/types.h>
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

#define LIST_START   "\t\t<li>"
#define LIST_END     "</li></a>\n"
#define A_HREF_OPEN  "<a href=\""
#define A_HREF_CLOSE "\">"
#define A_CLOSE      "</a>"

class Autoindex {
	public:
		Autoindex(const std::string &root);
		~Autoindex();

	private:
		Autoindex();
		Autoindex(const Autoindex &other);
		Autoindex        &operator=(const Autoindex &other);
		bool              template_replace(std::string match, const std::string &value);
		std::string       createListItem(std::string name, bool append_slash);

		const std::string _root;
		std::string       _template;
};
