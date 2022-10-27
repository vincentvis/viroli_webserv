#include "request/HttpRequest.hpp"
#include "request/Request.hpp"
#include "utils/Utils.hpp"

HttpRequest::HttpRequest() {
}

HttpRequest::HttpRequest(ClientFD &Client) {
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
	std::string path = Client._request.getFileStat().getFull();
	int fd;

	if (Client._request.getFileStat().isDir()) {
		if (Client._config->getAutoIndex(Client._location) == "on") {
			try {
				Autoindex autoindex(path);
				Client._response.addHeader(Utils::contentType_string, "text/html");
				Client._response.generateResponse(&Client, autoindex.getHtml(), "200");
				return;
			} catch (const Utils::AutoindexException &e) {
				throw Utils::ErrorPageException("404");
			} catch (const std::exception &e) {
				throw Utils::ErrorPageException("500"); // internal server error?
			}
		}
		std::vector<std::string> indexes = Client._config->getIndex(Client._location);
		std::vector<std::string>::const_iterator it  = indexes.begin();
		std::vector<std::string>::const_iterator end = indexes.end();
		std::string                              tmp;
//		if (*path.rbegin() != '/') { // this can be deleted now right?
//			path += "/";
//		}
		fd = -1;
		while (it != end) {
			tmp = path + *it;
			fd  = open(tmp.c_str(), O_RDONLY);
			if (fd > 0) {
				path = tmp;
				Client._request.setUri(Client._request.getUri() + *it);
				break;
			}
			fd = -1;
			it++;
		}
	} else {
		fd = open(path.c_str(), O_RDONLY);
	}
	if (fd == -1) { // is this going well since it will also catches the -1 of de index on?
		throw Utils::ErrorPageException("404"); // if change back to errorResponse do RETURN!
	}
	/* add fileFd to poll */
	Client._fileFD = reinterpret_cast<FileFD *>(
		Server::addPollable(Client._server, fd, FILEPOLL, POLLIN));
	Client._fileFD->setRequestInterface(this, &Client);
}
#include "config/MimeTypes.hpp"

void HttpRequest::POSTRequest(ClientFD &Client) {
	/* check if requested file is valid */


	/* check if path to upload exists// is already done ? */

	/* check if file exists */
//	Client._request.getFileStat().getFilename();
	
//	std::string con = MimeTypes::findMimeType(Client, Client._request.getFileStat().getFilename());

	int fd = open(Client._request.getFileStat().getFull().c_str(), O_TRUNC | O_CREAT | O_WRONLY, S_IRWXU); // change?
	if (fd == -1) {
		throw Utils::ErrorPageException("404");
	} else {
		//		set location in response header ?is this still necessary?
		Client._fileFD = reinterpret_cast<FileFD *>(
			Server::addPollable(Client._server, fd, FILEPOLL, POLLOUT));
		if (!Client.getBodyStr().empty()) {
			Client._fileFD->setData(Client._request.getBody());
		}
		Client._fileFD->setRequestInterface(this, &Client);
	}
}

// If a DELETE method is successfully applied, the origin server SHOULD send
// a 202 (Accepted) status code if the action will likely succeed but has not yet been enacted,
//	a 204 (No Content) status code if the action has been enacted and no further information is to be supplied, or
//	a 200 (OK) status code if the action has been enacted and the response message includes a representation describing the status.

void HttpRequest::DELETERequest(ClientFD &Client) {
	if (remove(Client._request.getFileStat().getFull().c_str()) != 0){
		throw Utils::ErrorPageException("404"); // would you agree with 404?
	}
	else {
		Client._response.generateResponse(&Client, "200");
	}
}

HttpRequest::~HttpRequest() {
}
