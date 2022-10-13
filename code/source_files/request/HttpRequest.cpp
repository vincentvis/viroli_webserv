#include "request/HttpRequest.hpp"
#include "request/Request.hpp"
#include "utils/Utils.hpp"

HttpRequest::HttpRequest() {
}

HttpRequest::HttpRequest(ClientFD &Client) {
	CheckMethod(Client);
}

void HttpRequest::CheckMethod(ClientFD &Client) {
	std::cout << "check method" << std::endl;
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

// Statuscode range:
// 100-199 is classed as Informational.
// 200-299 is Successful.
// 300-399 is Redirection.
// 400-499 is Client error.
// 500-599 is Server error.

/* called in ClientFD after fileFD is read */
void HttpRequest::processResponse(ClientFD *Client, std::string messageBody,
								  std::string StatusCode) {
	/* check errorpages */
	if (StatusCode.at(0) < '4') {
		Client->_response.findAndSetContentType(Client->_request);
		Client->_response.setMessageBody(messageBody);
	} else {
		Client->_response.setContentType("text/html");
		Client->_response.generateErrorPage(StatusCode);
	}
	/* generate response */
	Client->_response.initResponse(StatusCode, Client->_config, Client->_request);
	Client->_response.createResponse(); // thinking about merging those two
	Client->sendResponse(Client->_index);
}

void HttpRequest::GETRequest(ClientFD &Client) {
	/* create path */
	std::string uri = Client._location->getRoot();
	if (uri.empty()) {
		uri = Client._config->getRoot();
	}
	uri   = uri + Client._request.getUri();

	/* add fileFd to poll */
	int fd = open(uri.c_str(), O_RDONLY); // O_NONBLOCK?
	if (fd == -1) {
		processResponse(&Client, "", "404");
	} else {
		Client._fileFD = reinterpret_cast<FileFD *>(
			Server::addPollable(Client._server, fd, FILEPOLL, POLLIN));
		Client._fileFD->setRequestInterface(this, &Client);
	}
}

void HttpRequest::POSTRequest(ClientFD &Client) {

	std::string uri = Client._location->getRoot();
	if (uri.empty()) {
		uri = Client._config->getRoot();
	}
	uri  = uri + Client._request.getUri();
	std::cout << uri << std::endl;
	int fd = open(uri.c_str(), O_APPEND); // change
	if (fd == -1) {
		processResponse(&Client, "", "404");
	}
	else {
		std::cout << "yes" << std::endl;
		Client._fileFD = reinterpret_cast<FileFD *>(
			Server::addPollable(Client._server, fd, FILEPOLL, POLLOUT));
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
