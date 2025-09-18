cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_CXX_STANDARD 20)

project(SyncWatchers)

find_package(WebFramework REQUIRED)

add_executable(
	${PROJECT_NAME}
	src/Main.cpp
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
