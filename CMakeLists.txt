cmake_minimum_required(VERSION 3.1.0)

project(player_backend)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Multimedia Network REQUIRED)

include_directories(common player_server)
add_executable(player_server
	player_server/server.cpp 
	player_server/server_socket.cpp 
	player_server/play_worker.cpp 
	player_server/main.cpp 
	common/commands.cpp 
	common/multithread_printer.cpp
)
add_executable(player_client
	player_client/client_socket.cpp 
	player_client/client.cpp 
	player_client/main.cpp 
	player_client/keyboard_worker.cpp
	common/commands.cpp 
	common/multithread_printer.cpp
)

target_link_libraries(player_server Qt5::Network Qt5::Multimedia)
target_link_libraries(player_client Qt5::Network Qt5::Multimedia)
