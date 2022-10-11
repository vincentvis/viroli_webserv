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
	/* create path */
	std::string uri = Client._location->getRoot();
	if (uri.empty()) {
		uri = Client._config->getRoot();
	}
	uri = uri + Client._request.getUri();

	/* add fileFd to poll */
	int fd = open(uri.c_str(), O_RDONLY);
	if (fd == -1)
		std::cout << fd << ": FD ERROR" << std::endl; //throw error
	Client._fileFD = reinterpret_cast<FileFD *>(
		Server::addPollable(Client._server, fd, FILEPOLL, POLLIN));
	Client._fileFD->setRequestInterface(this, &Client);
}

/* called in ClientFD after fileFD is read */
void HttpRequest::processResponse(ClientFD *Client, std::string Data, int ErrorStatus) {
	if (ErrorStatus != 0){
		std::cout << "create error page response" << std::endl;
	}
	else {
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
