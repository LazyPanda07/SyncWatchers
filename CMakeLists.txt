cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/bin CACHE STRING "")
option(BUILD_TESTS "" OFF)

project(SyncWatchersProject VERSION 1.0.0)

include(SetupWebFramework.cmake)

add_subdirectory(SyncWatchers)
add_subdirectory(Core)

if (${BUILD_TESTS})
	add_subdirectory(Tests)
endif()

install(DIRECTORY configs/ DESTINATION configs)
