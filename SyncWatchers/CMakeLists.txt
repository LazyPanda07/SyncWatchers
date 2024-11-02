cmake_minimum_required(VERSION 3.27.0)

project(SyncWatchers)

add_executable(
	${PROJECT_NAME}
	source/Main.cpp
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	${WEB_FRAMEWORK_LIBS}
)
