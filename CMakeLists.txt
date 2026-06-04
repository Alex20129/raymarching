CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

PROJECT(raymarching)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

# SET(CMAKE_CXX_FLAGS "-O0 -Wall -Wextra -Wpedantic")
SET(CMAKE_CXX_FLAGS "-O3")

LINK_LIBRARIES(pthread png)

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}
	main.cpp
	basicObjects.cpp
	basicObjects.hpp
	commonVectorFun.cpp
	commonVectorFun.hpp
	octree.cpp
	octree.hpp
	prng.cpp
	prng.hpp
	scene.cpp
	scene.hpp)

SET_PROPERTY(TARGET ${CMAKE_PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
