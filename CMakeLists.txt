cmake_minimum_required(VERSION 2.8.11)

if(UNIX)
	add_definitions("-Wall -Wextra -Werror -std=c++14 -pedantic")
endif(UNIX)

add_subdirectory(server)

#add_subdirectory(client)
