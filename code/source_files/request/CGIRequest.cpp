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
		COUT_DEBUGMSG << "We should definitely see this..\n";
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
	std::cout << "\033[32m[CGI] " << Client._request.getMethod() << " "
			  << Client._request.getUri() << std::endl;
	std::cout << "\033[4mQUERY: " << Client._request.getQuery() << "\033[0m" << std::endl;
}

void CGIRequest::POSTRequest(ClientFD &Client) {
	Client._request.printAttributesInRequestClass();
	std::cout << "\033[32m[CGI] " << Client._request.getMethod() << " "
			  << Client._request.getUri() << std::endl;
	std::cout << "\033[4mQUERY: " << Client._request.getQuery() << "\033[0m" << std::endl;
}

void CGIRequest::DELETERequest(ClientFD &Client) {
	(void)Client;
	std::cout << "this is a DELETE CGI Request" << std::endl; // REMOVE LATER
}

CGIRequest::~CGIRequest() {
}
