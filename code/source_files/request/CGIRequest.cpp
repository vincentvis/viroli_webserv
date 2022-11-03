#include "request/CGIRequest.hpp"
#include "cgi/Cgi.hpp"

CGIRequest::CGIRequest() {
}

CGIRequest::CGIRequest(ClientFD &Client) {
	if (Client._request.getMethod() == Utils::get_string) {
		GETRequest(Client);
	} else if (Client._request.getMethod() == Utils::post_string) {
		POSTRequest(Client);
	} else if (Client._request.getMethod() == Utils::delete_string) {
		DELETERequest(Client);
	}
}

void CGIRequest::GETRequest(ClientFD &Client) {
	char *tmp = new char[80];
	memcpy(tmp, "/tmp/viroli_cgi_file___XXXXXXXX\0", 32);
	if (mktemp(tmp) == NULL) {
		throw Utils::ErrorPageException("502");
	}
	_tmpfilename = std::string(tmp);

	Cgi cgi(Client._request.getFileStatCopy(), Client._request.getMethod(),
			Client._server->getPort(), Client._config->getFirstServerName(), tmp);

	if (Client._request.getQuery().empty() == false) {
		cgi.setQueryString(Client._request.getQuery());
	}

	cgi.execute(Client, this, Cgi::GET);

	// should this be here?
	delete[] tmp;
}

void CGIRequest::POSTRequest(ClientFD &Client) {
	char *tmp = new char[80];
	memcpy(tmp, "/tmp/viroli_cgi_file___XXXXXXXX\0", 32);
	if (mktemp(tmp) == NULL) {
		throw Utils::ErrorPageException("502");
	}
	_tmpfilename = std::string(tmp);

	Cgi cgi(Client._request.getFileStatCopy(), Client._request.getMethod(),
			Client._server->getPort(), Client._config->getFirstServerName(), tmp);

	// cgi.setQueryString(Client._request.getBody());
	cgi.setEnv("CONTENT_LENGTH", Utils::to_string(Client._request.getContentLength()));
	std::map<std::string, std::string>::const_iterator contentType =
		Client._request.getHeaderMap().find("Content-Type");
	if (contentType != Client._request.getHeaderMap().end()) {
		cgi.setEnv("CONTENT_TYPE", contentType->second);
	}

	if (Client._request.getBody().empty() == false) {
		cgi.setQueryString(Client._request.getBody());
	}

	cgi.execute(Client, this, Cgi::POST);

	delete[] tmp;
}

void CGIRequest::DELETERequest(ClientFD &Client) {
	(void)Client;
	std::cerr << "DELETE requests are not part of CGI" << std::endl;
}

CGIRequest::~CGIRequest() {
	errno = 0;
	int x = remove(_tmpfilename.c_str());
	std::cerr << CGIPARNTCLR "CGIRequest destructor - Remove: " << x;
	if (x < 0) {
		std::cerr << " " << strerror(errno);
	}
	std::cerr << "\033[0m" << std::endl;
}
