#include "request/HttpRequest.hpp"
#include "request/Request.hpp"
#include "utils/Utils.hpp"

HttpRequest::HttpRequest() {
}

HttpRequest::HttpRequest(ClientFD &Client) {
	CheckMethod(Client);
}

void HttpRequest::CheckMethod(ClientFD &Client) {
	if (Client._request.getMethod() == Utils::get_string) {
		GETRequest(Client);
	}
	if (Client._request.getMethod() == Utils::post_string) {
		POSTRequest(Client);
	}
	if (Client._request.getMethod() == Utils::delete_string) {
		DELETERequest(Client);
	}
}

void HttpRequest::GETRequest(ClientFD &Client) {
	std::string path = Client._config->getRoot(Client._location);
	std::string uri  = Client._request.getUri();
	if (*path.rbegin() != '/' && (uri.empty() == false && uri.at(0) != '/')) {
		path += "/";
	}
	path += uri;
	int fd;

	if (Client._request.uriIsDir()) {
		if (Client._config->getAutoIndex(Client._location) == "on") {
			try {
				Autoindex autoindex(path);
				processResponse(&Client, autoindex.getHtml(), 0);
				return;
			} catch (const Utils::AutoindexException &e) {
				std::cerr << "Create 404 error response" << std::endl;
				return;
			}
		}
		std::vector<std::string> indexes = Client._config->getIndex(Client._location);
		std::vector<std::string>::const_iterator it  = indexes.begin();
		std::vector<std::string>::const_iterator end = indexes.end();
		std::string                              tmp;
		if (*path.rbegin() != '/') {
			path += "/";
		}
		fd = -1;
		while (it != end) {
			tmp = path + *it;
			fd  = open(tmp.c_str(), O_RDONLY);
			if (fd > 0) {
				path = tmp;
				break;
			}
			fd = -1;
			it++;
		}
	} else {
		fd = open(path.c_str(), O_RDONLY);
	}
	if (fd == -1)
		std::cout << fd << ": FD ERROR" << std::endl; // throw error
	Client._fileFD = reinterpret_cast<FileFD *>(
		Server::addPollable(Client._server, fd, FILEPOLL, POLLIN));
	Client._fileFD->setRequestInterface(this, &Client);
}

/* called in ClientFD after fileFD is read */
void HttpRequest::processResponse(ClientFD *Client, std::string Data, int ErrorStatus) {
	if (ErrorStatus != 0) {
		std::cout << "create error page response" << std::endl;
	} else {
		Client->_response.findAndSetContentType(Client->_request);
		Client->_response.setMessageBody(Data);
		Client->_response.initResponse("200", Client->_config, Client->_request);
		Client->_response.createResponse();
	}
	Client->sendResponse(Client->_index);
}

void HttpRequest::POSTRequest(ClientFD &Client) {
	(void)Client;
	std::cout << "POST REQUEST UNDER CONSTRUCTION" << std::endl;
	//	Client._response.setContentType("text/plain");
	//	Client._response.initResponse("200", Client._config, Client._request);
	//	Client._response.createResponse();
	//	Client.sendResponse(Client._index);
}

void HttpRequest::DELETERequest(ClientFD &Client) {
	(void)Client;
	std::cout << "DELETE REQUEST UNDER CONSTRUCTION" << std::endl;
	//	Client._response.setContentType("text/plain");
	//	Client._response.initResponse("200", Client._config, Client._request);
	//	Client._response.createResponse();
	//	Client.sendResponse(Client._index);
}

HttpRequest::~HttpRequest() {
}
