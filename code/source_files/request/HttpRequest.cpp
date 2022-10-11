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
	std::string uri = Client._location->getRoot();
	if (uri.empty()) {
		uri = Client._config->getRoot();
	}
	uri = uri + Client._request.getUri();

	//	std::cout << "Location root: " << Client._location->getRoot() << std::endl;
	//	std::cout << "Config root: " << Client._config->getRoot() << std::endl;
	//	std::cout << "URI: " << uri << std::endl;
//		std::cout << uri.c_str() << std::endl;
	int fd = open(uri.c_str(), O_RDONLY);
	//error check
	if (fd == -1)
		std::cout << fd << "FD ERROR" << std::endl;

	Client._fileFD = reinterpret_cast <FileFD *>(Server::addPollable(Client._server, fd, FILEPOLL, POLLIN));
	Client._fileFD->setRequestInterface(this, &Client);
//	Client._response.initResponse("200", Client._config, Client._request);
//	Client._response.createResponse();
}

void HttpRequest::processResponse(ClientFD *Client, std::string Data){
	Client->_response.setMessageBody(Data);
	Client->_response.initResponse("200", Client->_config, Client->_request);
	Client->_response.createResponse();
	Client->setStateSend();
}

void HttpRequest::POSTRequest(ClientFD &Client) {
	Client._response.initResponse("200", Client._config, Client._request);
	Client._response.createResponse();
}

void HttpRequest::DELETERequest(ClientFD &Client) {
	Client._response.initResponse("200", Client._config, Client._request);
	Client._response.createResponse();
}

HttpRequest::~HttpRequest() {
}
