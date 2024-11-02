cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/bin CACHE STRING "")
set(CMAKE_BUILD_TYPE Release CACHE STRING "")
set(CMAKE_SYSTEM_NAME Windows)

list(APPEND CMAKE_MODULE_PATH $ENV{WEB_FRAMEWORK_SDK})

if (UNIX)
	list(APPEND CMAKE_MODULE_PATH /usr/lib/web-framework)
endif()

find_package(WebFramework REQUIRED)

project(SyncWatchersProject)

add_subdirectory(SyncWatchers)
add_subdirectory(Core)
