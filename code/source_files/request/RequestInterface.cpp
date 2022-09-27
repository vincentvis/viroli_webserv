#include "request/RequestInterface.hpp"

std::map<std::string, RequestInterface::e_RequestType> RequestInterface::_MethodKeys;

RequestInterface::RequestInterface() {
	if (_MethodKeys.size() == 0) {
		_MethodKeys["GET"]    = GET;
		_MethodKeys["DELETE"] = DELETE;
		_MethodKeys["POST"]   = POST;
	}
}

//RequestInterface::RequestInterface(const Request &Req, const Config &Conf) {
//}

RequestInterface::~RequestInterface() {
}
