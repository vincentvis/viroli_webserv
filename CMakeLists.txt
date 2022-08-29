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
        )

#target_link_libraries(webserv PUBLIC libft)