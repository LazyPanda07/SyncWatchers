cmake_minimum_required(VERSION 3.27.0)

option(REMOVE_DATABASE_ON_START "" OFF)

project(SyncWatchers)

if (${REMOVE_DATABASE_ON_START})
	add_definitions(-DREMOVE_DATABASE_ON_START)
endif()

add_executable(
	${PROJECT_NAME}
	source/Main.cpp
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	${WEB_FRAMEWORK_LIBS}
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
