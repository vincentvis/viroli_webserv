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
				Client._response.addHeader("Content-Type", "text/html");
				Client._response.processResponse(&Client, autoindex.getHtml(), "200");
				return;
			} catch (const Utils::AutoindexException &e) {
				Client._response.processResponse(&Client, "", "404");
				return;
			} catch (const std::exception &e) {
				// all other exceptions?
				// but what to do?
				// internal server error for now
				Client._response.processResponse(&Client, "", "500");
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
				Client._request.setUri(Client._request.getUri() + *it);
				break;
			}
			fd = -1;
			it++;
		}
	} else {
		fd = open(path.c_str(), O_RDONLY);
	}
	if (fd == -1) {
		Client._response.processResponse(&Client, "", "404");
		return;
	}
	/* add fileFd to poll */
	Client._fileFD = reinterpret_cast<FileFD *>(
		Server::addPollable(Client._server, fd, FILEPOLL, POLLIN));
//	if (!Client.getBodyStr().empty()){
//		Client._fileFD->setData(Client.getBodyStr());
//	}
	Client._fileFD->setRequestInterface(this, &Client);
}

// Statuscode range:
// 100-199 is classed as Informational.
// 200-299 is Successful.
// 300-399 is Redirection.
// 400-499 is Client error.
// 500-599 is Server error.

///* called in ClientFD after fileFD is read */
//void HttpRequest::processResponse(ClientFD *Client, std::string messageBody,
//								  std::string StatusCode) {
//	/* check errorpages */
//	if (StatusCode.at(0) < '4') {
//		Client->_response.findAndSetContentType(Client->_request);
//		Client->_response.setMessageBody(messageBody);
//	} else {
//		Client->_response.setContentType("text/html");
//		Client->_response.generateErrorPage(StatusCode,
//											&Client->_config->getErrorPages());
//	}
//	/* generate response */
//	Client->_response.initResponse(StatusCode, Client->_config, Client->_request);
//	Client->_response.createResponse(); // thinking about merging those two
//	Client->sendResponse(Client->_index);
//}

// void HttpRequest::GETRequest(ClientFD &Client) {
//	/* create path */
//	std::string uri = Client._location->getRoot();
//	if (uri.empty()) {
//		uri = Client._config->getRoot();
//	}
//	uri   = uri + Client._request.getUri();
//
//	/* add fileFd to poll */
//	int fd = open(uri.c_str(), O_RDONLY); // O_NONBLOCK?
//	if (fd == -1) {
//		processResponse(&Client, "", "404");
//	} else {
//		Client._fileFD = reinterpret_cast<FileFD *>(
//			Server::addPollable(Client._server, fd, FILEPOLL, POLLIN));
//		Client._fileFD->setRequestInterface(this, &Client);
//	}
// }

void HttpRequest::POSTRequest(ClientFD &Client) {
	//	200 turn into 201 if the file is valid
	//		std::string uri = Client._location->getRoot();
	//		if (uri.empty()) {
	//			uri = Client._config->getRoot();
	//		}
	//		uri  = uri + Client._request.getUri();
//	std::cout << "POSTRequest:: Client request body.size(): " << Client._request.getBody().size() << std::endl;
	std::string path = Client._config->getRoot(Client._location);
	std::string uri  = Client._request.getUri();
	if (*path.rbegin() != '/' && (uri.empty() == false && uri.at(0) != '/')) {
		path += "/";
	}
	path += uri;
	std::cout << path << std::endl;
	int fd = open(path.c_str(), O_TRUNC | O_CREAT | O_WRONLY, S_IRWXU); // change

	if (fd == -1) {
		Client._response.processResponse(&Client, "", "404");
	} else {
		//		set location in response header
		Client._fileFD = reinterpret_cast<FileFD *>(
			Server::addPollable(Client._server, fd, FILEPOLL, POLLOUT));
		if (!Client.getBodyStr().empty()) {
			Client._fileFD->setData(Client._request.getBody());
		}
		Client._fileFD->setRequestInterface(this, &Client);
	}
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
