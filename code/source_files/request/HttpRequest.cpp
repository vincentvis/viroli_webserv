#include "request/HttpRequest.hpp"
#include "request/Request.hpp"
#include "utils/Utils.hpp"

HttpRequest::HttpRequest() {
}

HttpRequest::HttpRequest(ClientFD &Client) {
	if (Client.getRequest().getMethod() == Utils::get_string) {
		GETRequest(Client);
	}
	else if (Client.getRequest().getMethod() == Utils::post_string) {
		POSTRequest(Client);
	}
	else if (Client.getRequest().getMethod() == Utils::delete_string) {
		DELETERequest(Client);
	}
}

void HttpRequest::GETRequest(ClientFD &Client) {
	std::string path = Client.getRequest().getFileStat().getFull();
	int         fd;

	if (Client.getRequest().getFileStat().isDir()) {
		if (Client.getConfig()->getAutoIndex(Client.getLocation()) == "on") {
			try {
				Autoindex autoindex(path);
				Client.getResponse().addHeader(Utils::contentType_string, "text/html");
				Client.getResponse().generateResponse(&Client, autoindex.getHtml(),
													  "200");
				return;
			} catch (const Utils::AutoindexException &e) {
				throw Utils::ErrorPageException("404");
			} catch (const std::exception &e) {
				throw Utils::ErrorPageException("500");
			}
		}
		std::vector<std::string> indexes =
			Client.getConfig()->getIndex(Client.getLocation());
		std::vector<std::string>::const_iterator it  = indexes.begin();
		std::vector<std::string>::const_iterator end = indexes.end();
		std::string                              tmp;
		fd = -1;
		while (it != end) {
			tmp = path + *it;
			fd  = open(tmp.c_str(), O_RDONLY);
			if (fd > 0) {
				path = tmp;
				Client.getRequest().setUri(Client.getRequest().getUri() + *it);
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
	Client.setFileFD(
		reinterpret_cast<FileFD *>(PollableFactory::getInstance().createPollable(
			Client.getServer(), fd, FILEPOLL, POLLIN)));
	Client.setFileStatus(true);
	Client.getFileFD()->setRequestInterface(this, &Client);
}

void HttpRequest::POSTRequest(ClientFD &Client) {
	std::string path = Client.getRequest().getFileStat().getFull();
	int         fd   = open(path.c_str(), O_TRUNC | O_CREAT | O_WRONLY,
							S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

	if (fd == -1) {
		throw Utils::ErrorPageException("404");
	} else {
		/* add fileFd to poll */
		Client.setFileFD(
			reinterpret_cast<FileFD *>(PollableFactory::getInstance().createPollable(
				Client.getServer(), fd, FILEPOLL, POLLOUT)));
		Client.setFileStatus(true);
		if (!Client.getBody().empty()) {
			Client.getFileFD()->setData(Client.getRequest().getBody());
		}
		Client.getFileFD()->setRequestInterface(this, &Client);
	}
}

void HttpRequest::DELETERequest(ClientFD &Client) {
	int r = 0;
	errno = 0;
	if (Client.getRequest().getFileStat().isReg() &&
		(r = remove(Client.getRequest().getFileStat().getFull().c_str())) == 0)
	{
		Client.getResponse().generateResponse(&Client, "204");
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
