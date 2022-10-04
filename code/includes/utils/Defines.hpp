#pragma once

#include <iostream>
#include <map>

namespace Utils {

enum e_RequestType { GET, POST, DELETE }; // make static

static std::map<std::string, e_RequestType> _MethodKeys; // make static
//_MethodKeys["GET"]    = GET;
//_MethodKeys["POST"]   = POST;
//_MethodKeys["DELETE"] = DELETE;

} // namespace Utils