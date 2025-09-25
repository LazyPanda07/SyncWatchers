cmake_minimum_required(VERSION 3.27.0)

project(CoreDatabaseExecutor)

add_library(
	${PROJECT_NAME} SHARED
	src/InviteLinkExecutor.cpp
	src/RoomsExecutor.cpp
	src/UsersExecutor.cpp
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	Utils
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/Events/src
	${CMAKE_SOURCE_DIR}/Utils/src
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(FILES core_database_web.json DESTINATION .)
install(FILES core_database_config.json DESTINATION .)
