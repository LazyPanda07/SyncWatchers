cmake_minimum_required(VERSION 3.27.0)

project(Core)

add_library(
	${PROJECT_NAME} SHARED
	source/CreateRoom.cpp
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	${WEB_FRAMEWORK_LIBS}
)
