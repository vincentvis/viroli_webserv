cmake_minimum_required(VERSION 3.17)
project(webserv)

#add_subdirectory(libft)
include_directories(code/includes code/source_files)
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS -std=c++98)
#set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra")

add_executable(webserv
        code/source_files/main.cpp
        code/includes/server/Server.hpp
        code/source_files/server/Server.cpp
        code/includes/utils/Utils.hpp
        code/source_files/utils/Utils.cpp
        code/includes/config/Config.hpp
        code/source_files/config/Config.cpp
        code/includes/config/ConfigParser.hpp
        code/source_files/config/ConfigParser.cpp
        code/includes/server/Location.hpp
        code/source_files/server/Location.cpp
        code/includes/ipollable/PollableFactory.hpp
        code/source_files/ipollable/PollableFactory.cpp
        code/includes/ipollable/IPollable.hpp
        code/source_files/ipollable/IPollable.cpp
        code/source_files/ipollable/FileFD.cpp
        code/includes/ipollable/FileFD.hpp
        code/source_files/ipollable/ClientFD.cpp
        code/includes/ipollable/ClientFD.hpp
        code/source_files/ipollable/ServerFD.cpp
        code/includes/ipollable/ServerFD.hpp
        code/source_files/ipollable/Buffer.cpp
        code/includes/ipollable/Buffer.hpp
        code/source_files/ipollable/CgiFD.cpp
        code/includes/ipollable/CgiFD.hpp
        code/source_files/request/Request.cpp
        code/includes/request/Request.hpp
        code/source_files/request/RequestInterface.cpp
        code/includes/request/RequestInterface.hpp
        code/source_files/request/CGIRequest.cpp
        code/includes/request/CGIRequest.hpp
        code/source_files/request/HttpRequest.cpp
        code/includes/request/HttpRequest.hpp
        code/source_files/utils/Utils.cpp
        code/includes/utils/Utils.hpp
        code/source_files/response/Response.cpp
        code/includes/response/Response.hpp
        code/includes/utils/Defines.hpp
        code/source_files/autoindex/Autoindex.cpp
        code/includes/config/MimeTypes.hpp
        code/source_files/response/HttpStatus.cpp
        code/includes/response/HttpStatus.hpp
        code/source_files/utils/FileStat.cpp
        code/includes/utils/FileStat.hpp
        code/includes/cgi/Executables.hpp
        code/includes/cgi/Cgi.hpp
        code/source_files/cgi/Cgi.cpp)

#target_link_libraries(webserv PUBLIC libft)