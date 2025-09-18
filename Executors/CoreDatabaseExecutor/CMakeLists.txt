cmake_minimum_required(VERSION 3.27.0)

project(CoreDatabaseExecutor)

add_library(
	${PROJECT_NAME} SHARED
	src/CoreDatabaseExecutor.cpp
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(FILES ${PROJECT_SOURCE_DIR}/core_database.json DESTINATION .)
