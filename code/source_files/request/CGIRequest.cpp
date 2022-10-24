#include "request/CGIRequest.hpp"
#include "cgi/Cgi.hpp"

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

void CGIRequest::GETRequest(ClientFD &Client) {
	std::cout << "\033[32m[CGI] " << Client._request.getMethod() << " "
			  << Client._request.getUri() << std::endl;
	std::cout << "\033[4mQUERY: " << Client._request.getQuery() << "\033[0m" << std::endl;

	Cgi exec(Client._request.getFileStat());
	if (exec.isDone()) {
		//
	}
}

void CGIRequest::POSTRequest(ClientFD &Client) {
	Client._request.printAttributesInRequestClass();
	std::cout << "\033[32m[CGI] " << Client._request.getMethod() << " "
			  << Client._request.getUri() << std::endl;
	std::cout << "\033[4mBODY: " << Client._request.getBody() << "\033[0m" << std::endl;
}

void CGIRequest::DELETERequest(ClientFD &Client) {
	(void)Client;
	std::cout << "this is a DELETE CGI Request" << std::endl; // REMOVE LATER
}

CGIRequest::~CGIRequest() {
}
