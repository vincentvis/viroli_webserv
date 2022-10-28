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
	int         fd;

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
		throw Utils::ErrorPageException("404");
	}
	/* add fileFd to poll */
	Client._fileFD =
		reinterpret_cast<FileFD *>(PollableFactory::getInstance().createPollable(
			Client._server, fd, FILEPOLL, POLLIN));
	Client._fileFD->setRequestInterface(this, &Client);
}

void HttpRequest::POSTRequest(ClientFD &Client) {
	std::string path = Client._request.getFileStat().getFull();
	int         fd   = open(path.c_str(), O_TRUNC | O_CREAT | O_WRONLY,
							S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

	if (fd == -1) {
		throw Utils::ErrorPageException("404");
	} else {
		/* add fileFd to poll */
		Client._fileFD =
			reinterpret_cast<FileFD *>(PollableFactory::getInstance().createPollable(
				Client._server, fd, FILEPOLL, POLLOUT));
		if (!Client.getBodyStr().empty()) {
			Client._fileFD->setData(Client._request.getBody());
		}
		Client._fileFD->setRequestInterface(this, &Client);
	}
}

void HttpRequest::DELETERequest(ClientFD &Client) {
	int r = 0;
	errno = 0;
	if (Client._request.getFileStat().isReg() &&
		(r = remove(Client._request.getFileStat().getFull().c_str())) == 0)
	{
		Client._response.generateResponse(&Client, "204");
	} else {
		if (r == -1) {
			if (errno == EACCES || errno == ECANCELED || errno == EFAULT ||
				errno == EINVAL || errno == ENOENT || errno == EPERM || errno == EROFS ||
				errno == EPERM)
			{
				throw Utils::ErrorPageException("406");
			}
		} else
			throw Utils::ErrorPageException("404");
	}
}

HttpRequest::~HttpRequest() {
}
