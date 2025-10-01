cmake_minimum_required(VERSION 3.27.0)

project(SyncWatchersClient)

add_custom_target(
	${PROJECT_NAME}_Android ALL
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/SyncWatchersClient
	COMMAND flutter build apk -V --release
	COMMENT "Building Android Flutter application"
)

if (WIN32)
	add_custom_target(
		${PROJECT_NAME}_Windows ALL
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/SyncWatchersClient
		COMMAND flutter build windows -V --release
		COMMENT "Building Windows Flutter application"
	)
endif()
