#include "request/CGIRequest.hpp"
#include "cgi/Cgi.hpp"

CGIRequest::CGIRequest() {
}

CGIRequest::CGIRequest(ClientFD &Client) {
	Cgi exec(Client._request.getFileStat(), Client._request.getMethod());

	if (Client._request.getMethod() == Utils::get_string) {
		exec.setQueryString(Client._request.getQuery());
	} else if (Client._request.getMethod() == Utils::post_string) {
		exec.setQueryString(Client._request.getBody());
		exec.setEnv("CONTENT_LENGTH",
					Utils::to_string(Client._request.getContentLength()));
	}

	// CheckMethod(Client);
}

void CGIRequest::CheckMethod(ClientFD &Client) {
	(void)Client;
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
}

void CGIRequest::POSTRequest(ClientFD &Client) {
	std::cout << "\033[32m[CGI] " << Client._request.getMethod() << " "
			  << Client._request.getUri() << std::endl;
	std::cout << "\033[4mBODY: " << Client._request.getBody() << "\033[0m" << std::endl;
}

void CGIRequest::DELETERequest(ClientFD &Client) {
	std::cout << "\033[32m[CGI] " << Client._request.getMethod() << " "
			  << Client._request.getUri() << std::endl;
}

CGIRequest::~CGIRequest() {
}
