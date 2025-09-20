cmake_minimum_required(VERSION 3.27.0)

project(CoreDatabaseExecutor)

add_library(
	${PROJECT_NAME} SHARED
	src/InviteLinkExecutor.cpp
	src/RoomsExecutor.cpp
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(DIRECTORY ${PROJECT_SOURCE_DIR} DESTINATION . FILES_MATCHING PATTERN "*.json")
