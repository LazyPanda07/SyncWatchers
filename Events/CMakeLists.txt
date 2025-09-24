cmake_minimum_required(VERSION 3.27.0)

project(Events)

add_library(
	${PROJECT_NAME} SHARED
	src/EventsManager.cpp
	src/Server.cpp
)

if (UNIX)
	add_definitions(-D__LINUX__)
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION .)
