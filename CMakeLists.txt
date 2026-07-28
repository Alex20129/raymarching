CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

PROJECT(raymarching)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

# SET(CMAKE_CXX_FLAGS "-O0 -Wall -Wextra -Wpedantic")
SET(CMAKE_CXX_FLAGS "-O3 -ffast-math -march=native")

LINK_LIBRARIES(pthread png zmq)

SET(SERVER_EXECUTABLE_NAME "${CMAKE_PROJECT_NAME}_server")
SET(CLIENT_EXECUTABLE_NAME "${CMAKE_PROJECT_NAME}_client")

ADD_EXECUTABLE(${SERVER_EXECUTABLE_NAME}
	server_main.cpp
	basicObjects.cpp
	basicObjects.hpp
	commonVectorFun.cpp
	commonVectorFun.hpp
	prng.cpp
	prng.hpp
	scene.cpp
	scene.hpp)

ADD_EXECUTABLE(${CLIENT_EXECUTABLE_NAME}
	client_main.cpp
	basicObjects.cpp
	basicObjects.hpp
	commonVectorFun.cpp
	commonVectorFun.hpp
	prng.cpp
	prng.hpp
	scene.cpp
	scene.hpp)

SET_PROPERTY(TARGET ${CLIENT_EXECUTABLE_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
