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
	Client._response.initResponse("200", Client._config, Client._request);
	Client._response.createResponseHeader();
}

void HttpRequest::POSTRequest(ClientFD &Client) {
	Client._response.initResponse("200", Client._config, Client._request);
	Client._response.createResponseHeader();
}

void HttpRequest::DELETERequest(ClientFD &Client) {
	Client._response.initResponse("200", Client._config, Client._request);
	Client._response.createResponseHeader();
}

HttpRequest::~HttpRequest() {
}
