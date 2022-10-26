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
//	CheckMethod(Client);
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
	std::string path = Client._config->getRoot(Client._location);
	std::string uri  = Client._request.getUri();
	if (*path.rbegin() != '/' && (uri.empty() == false && uri.at(0) != '/')) {
		path += "/";
	}
	path += uri;
	int fd;

	if (Client._request.uriIsDir()) {
		if (Client._config->getAutoIndex(Client._location) == "on") {
			try {
				Autoindex autoindex(path);
				Client._response.addHeader(Utils::contentType_string, "text/html");
				Client._response.generateResponse(&Client, autoindex.getHtml(), "200");
				return;
			} catch (const Utils::AutoindexException &e) {
				Client._response.generateErrorResponse(&Client, "404");
				return;
			} catch (const std::exception &e) {
				// all other exceptions?
				// but what to do?
				// internal server error for now
				Client._response.generateErrorResponse(&Client,  "500");
				return;
			}
		}
		std::vector<std::string> indexes = Client._config->getIndex(Client._location);
		std::vector<std::string>::const_iterator it  = indexes.begin();
		std::vector<std::string>::const_iterator end = indexes.end();
		std::string                              tmp;
		if (*path.rbegin() != '/') {
			path += "/";
		}
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
	if (fd == -1) {
		Client._response.generateErrorResponse(&Client, "404");
		return;
	}
	/* add fileFd to poll */
	Client._fileFD = reinterpret_cast<FileFD *>(
		Server::addPollable(Client._server, fd, FILEPOLL, POLLIN));
//	if (!Client.getBodyStr().empty()){
//		Client._fileFD->setData(Client.getBodyStr());
//	}
	Client._fileFD->setRequestInterface(this, &Client);
}

void HttpRequest::POSTRequest(ClientFD &Client) {
	//	200 turn into 201 if the file is valid
	//		std::string uri = Client._location->getRoot();
	//		if (uri.empty()) {
	//			uri = Client._config->getRoot();
	//		}
	//		uri  = uri + Client._request.getUri();
//	std::cout << "POSTRequest:: Client request body.size(): " << Client._request.getBody().size() << std::endl;
	std::string path = Client._config->getRoot(Client._location);
	std::string uri  = Client._request.getUri();
	if (*path.rbegin() != '/' && (uri.empty() == false && uri.at(0) != '/')) {
		path += "/";
	}
	path += uri;
//	std::cout << path << std::endl;
	int fd = open(path.c_str(), O_TRUNC | O_CREAT | O_WRONLY, S_IRWXU); // change

	if (fd == -1) {
		Client._response.generateErrorResponse(&Client,  "404");
	} else {
		//		set location in response header
		Client._fileFD = reinterpret_cast<FileFD *>(
			Server::addPollable(Client._server, fd, FILEPOLL, POLLOUT));
		if (!Client.getBodyStr().empty()) {
			Client._fileFD->setData(Client._request.getBody());
		}
		Client._fileFD->setRequestInterface(this, &Client);
	}
}

//If a DELETE method is successfully applied, the origin server SHOULD send
// a 202 (Accepted) status code if the action will likely succeed but has not yet been enacted,
//	a 204 (No Content) status code if the action has been enacted and no further information is to be supplied, or
//	a 200 (OK) status code if the action has been enacted and the response message includes a representation describing the status.


void HttpRequest::DELETERequest(ClientFD &Client) {
	std::cout << "fileName" <<  Client._request.getFileStat().getFilename() << std::endl;
	std::cout << "pathName" <<  Client._request.getFileStat().getPath() << std::endl;
	std::cout << "getExtention" <<  Client._request.getFileStat().getExtension()) << std::endl;
	std::cout << "getFull" <<  Client._request.getFileStat().getFull() << std::endl;
//	std::cout << fileName <<std::endl;
//	if (Client._request.getFileStat().isDir() == true)
//		std::cout << Client._request.getFileStat().getPath() << std::endl;

//	if(remove(fileName) != 0){
//		Client._response.generateErrorResponse(405); //?
//	}
//	else{
//		Client._response.generateResponse(Client, 200);
//	}
}

HttpRequest::~HttpRequest() {
}
