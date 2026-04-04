CMAKE_MINIMUM_REQUIRED(VERSION 3.12)

PROJECT(raymarching)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)

SET(CMAKE_CXX_FLAGS "-O0 -Wall")

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

FIND_PACKAGE(Qt5 COMPONENTS Widgets REQUIRED)

LINK_LIBRARIES(pthread Qt5::Widgets)

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}
	main.cpp
	widget.cpp
	widget.hpp
	basicObjects.cpp
	basicObjects.hpp
	commonVectorFun.cpp
	commonVectorFun.hpp
	controlwidget.cpp
	controlwidget.hpp
	octree.cpp
	octree.hpp
	prng.cpp
	prng.hpp
	scene.cpp
	scene.hpp
	widget.ui
	controlwidget.ui)
