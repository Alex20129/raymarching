CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(raymarching)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)

SET(CMAKE_CXX_FLAGS "-O3 -no-pie")

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

FIND_PACKAGE(Qt5 COMPONENTS Widgets REQUIRED)

LINK_LIBRARIES(pthread Qt5::Widgets)

ADD_EXECUTABLE(raymarching
    main.cpp
    widget.cpp
    widget.h
	basicObjects.cpp
	basicObjects.hpp
	commonVectorFun.cpp
	commonVectorFun.hpp
	controlwidget.cpp
	controlwidget.h
	scene.cpp
	scene.hpp
	widget.ui
	controlwidget.ui)
