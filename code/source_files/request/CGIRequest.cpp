#include "request/CGIRequest.hpp"

CGIRequest::CGIRequest() {
}

CGIRequest::CGIRequest(ClientFD &Client) {
	CheckMethod(Client);
}

void CGIRequest::CheckMethod(ClientFD &Client) {
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

void CGIRequest::GETRequest(ClientFD &Client) {
	(void)Client;
	std::cout << "this is a GET CGI Request" << std::endl; // REMOVE LATER
}

void CGIRequest::POSTRequest(ClientFD &Client) {
	(void)Client;
	std::cout << "this is a POST CGI Request" << std::endl; // REMOVE LATER
}

void CGIRequest::DELETERequest(ClientFD &Client) {
	(void)Client;
	std::cout << "this is a DELETE CGI Request" << std::endl; // REMOVE LATER
}

void CGIRequest::processResponse(ClientFD *Client, std::string Data, int StatusCode) {
	if (StatusCode == 0){
		Client->_response.findAndSetContentType(Client->_request);
		Client->_response.setMessageBody(Data);
	}
	Client->_response.initResponse("200", Client->_config, Client->_request);
	Client->_response.createResponse();
	Client->sendResponse(Client->_index);
}

CGIRequest::~CGIRequest() {
}
