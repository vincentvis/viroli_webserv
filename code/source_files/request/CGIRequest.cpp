#include "request/CGIRequest.hpp"

CGIRequest::CGIRequest() {
}

CGIRequest::CGIRequest(ClientFD &Client) {
	CheckMethod(Client);
}

void CGIRequest::CheckMethod(ClientFD &Client) {
	if (Client._request.getMethod() == Utils::get_string)
		GETRequest(Client);
	if (Client._request.getMethod() == Utils::post_string)
		POSTRequest(Client);
	DELETERequest(Client);
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
