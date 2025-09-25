cmake_minimum_required(VERSION 3.27.0)

project(Utils)

add_library(
	${PROJECT_NAME} STATIC
	src/Utils.cpp
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/Events/src
)

if (UNIX)
	add_definitions(-D__LINUX__)
endif()
