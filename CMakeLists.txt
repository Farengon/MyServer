cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME "Demo")

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)

include_directories(./include)

aux_source_directory(./src/client CLIENT)
aux_source_directory(./src/server SERVER)
aux_source_directory(./src/aux AUX)
aux_source_directory(./src/poker POKER)

add_executable("server" ./src/run_server.cpp ${SERVER} ${AUX})
add_executable("client" ./src/run_client.cpp ${CLIENT} ${AUX} ${POKER})
target_link_libraries("client" ncurses)