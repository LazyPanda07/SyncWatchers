cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/bin CACHE STRING "")
set(CMAKE_BUILD_TYPE Release CACHE STRING "")

project(SyncWatchersProject VERSION 1.0.0)

list(APPEND CMAKE_MODULE_PATH $ENV{WEB_FRAMEWORK_SDK})

if (UNIX)
	list(APPEND CMAKE_MODULE_PATH /usr/lib/web-framework)
endif()

find_package(WebFramework REQUIRED)

add_subdirectory(SyncWatchers)
add_subdirectory(Core)

install(DIRECTORY configs DESTINATION ${CMAKE_INSTALL_PREFIX})
