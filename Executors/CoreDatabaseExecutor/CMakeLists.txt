cmake_minimum_required(VERSION 3.27.0)

project(CoreDatabaseExecutor)

add_library(
	${PROJECT_NAME} SHARED
	src/InviteLinkExecutor.cpp
	src/RoomsExecutor.cpp
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(FILES ${PROJECT_SOURCE_DIR}/core_database_web.json DESTINATION .)
install(FILES ${PROJECT_SOURCE_DIR}/core_database_config.json DESTINATION .)
