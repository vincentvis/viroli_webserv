#include "request/CGIRequest.hpp"

CGIRequest::CGIRequest() {
}

CGIRequest::CGIRequest(ClientFD &Client) {
	CheckMethod(Client);
}

void CGIRequest::CheckMethod(ClientFD &Client) {
	if (Client._request.getMethod() == Utils::get_string) {
		GETRequest(Client);
	} else if (Client._request.getMethod() == Utils::post_string) {
		POSTRequest(Client);
	} else if (Client._request.getMethod() == Utils::delete_string) {
		DELETERequest(Client);
	}
}

void CGIRequest::processResponse(ClientFD *Client, std::string messageBody,
								 std::string StatusCode) {
	/* check errorpages */
	if (StatusCode.at(0) < '4') {
		Client->_response.findAndSetContentType(Client->_request);
		Client->_response.setMessageBody(messageBody);
	} else {
		Client->_response.setContentType("text/html");
		Client->_response.generateErrorPage(StatusCode,
											&Client->_config->getErrorPages());
	}
	/* generate response */
	Client->_response.initResponse(StatusCode, Client->_config, Client->_request);
	Client->_response.createResponse();
	Client->sendResponse(Client->_index);
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

CGIRequest::~CGIRequest() {
}
