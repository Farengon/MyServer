cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME "Demo")

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)

include_directories(./include)

aux_source_directory(./src/netspace/client CLIENT)
aux_source_directory(./src/netspace/server SERVER)
aux_source_directory(./src/netspace/aux NET_AUX)

add_executable("server" ./src/run_server.cpp ${SERVER} ${NET_AUX})
add_executable("client" ./src/run_client.cpp ${CLIENT} ${NET_AUX})
target_link_libraries("client" ncurses)