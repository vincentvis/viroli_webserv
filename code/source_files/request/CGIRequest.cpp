#include "request/CGIRequest.hpp"
#include "cgi/Cgi.hpp"

CGIRequest::CGIRequest() {
}

CGIRequest::CGIRequest(ClientFD &Client) {
	if (Client.getRequest().getMethod() == Utils::get_string) {
		GETRequest(Client);
	}
	if (Client.getRequest().getMethod() == Utils::post_string) {
		POSTRequest(Client);
	}
	if (Client.getRequest().getMethod() == Utils::delete_string) {
		DELETERequest(Client);
	}
}

void CGIRequest::GETRequest(ClientFD &Client) {
	char tmp[80];
	std::memcpy(tmp, "/tmp/viroli_cgi_file___XXXXXXXX\0", 32);
	if (mktemp(tmp) == NULL) {
		throw Utils::ErrorPageException("502");
	}
	_tmpfilename = std::string(tmp);

	Cgi cgi(Client.getRequest().getFileStatCopy(), Client.getRequest().getMethod(),
			Client.getServer()->getPort(), Client.getConfig()->getFirstServerName(), tmp);

	if (Client.getRequest().getQuery().empty() == false) {
		cgi.setQueryString(Client.getRequest().getQuery());
	}

	cgi.execute(Client, this);
}

void CGIRequest::POSTRequest(ClientFD &Client) {
	char tmp[80];
	std::memcpy(tmp, "/tmp/viroli_cgi_file___XXXXXXXX\0", 32);
	if (mktemp(tmp) == NULL) {
		throw Utils::ErrorPageException("502");
	}
	_tmpfilename = std::string(tmp);

	Cgi cgi(Client.getRequest().getFileStatCopy(), Client.getRequest().getMethod(),
			Client.getServer()->getPort(), Client.getConfig()->getFirstServerName(), tmp);

	// cgi.setQueryString(Client.getRequest().getBody()); //remove?
	cgi.prepEnv("CONTENT_LENGTH",
				Utils::to_string(Client.getRequest().getContentLength()));
	std::map<std::string, std::string>::const_iterator contentType =
		Client.getRequest().getHeaderMap().find("Content-Type");
	if (contentType != Client.getRequest().getHeaderMap().end()) {
		cgi.prepEnv("CONTENT_TYPE", contentType->second);
	}

	if (Client.getRequest().getBody().empty() == false) {
		cgi.setQueryString(Client.getRequest().getBody());
	}

	cgi.execute(Client, this);
}

void CGIRequest::DELETERequest(ClientFD &Client) {
	(void)Client;
	std::cerr << "DELETE requests are not part of CGI (we should never see this..)"
			  << std::endl;
}

CGIRequest::~CGIRequest() {
	remove(_tmpfilename.c_str());
}
