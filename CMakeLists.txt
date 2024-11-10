cmake_minimum_required(VERSION 3.27.0)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/bin CACHE STRING "")
set(CMAKE_BUILD_TYPE Release CACHE STRING "")
set(WEB_FRAMEWORK_TAG v3.0.8 CACHE STRING "")

project(SyncWatchersProject VERSION 1.0.0)

set(WEB_FRAMEWORK_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/WebFrameworkLibrary)

list(APPEND CMAKE_MODULE_PATH $ENV{WEB_FRAMEWORK_SDK})
list(APPEND CMAKE_MODULE_PATH ${WEB_FRAMEWORK_LIBRARY_DIR})

if (UNIX)
	list(APPEND CMAKE_MODULE_PATH /usr/lib/web-framework)
endif()

if (EXISTS ${WEB_FRAMEWORK_LIBRARY_DIR})
	set(WEB_FRAMEWORK_SDK ${WEB_FRAMEWORK_LIBRARY_DIR})
endif()

find_package(WebFramework QUIET)

if (NOT ${WebFramework_FOUND})
	message("Can't find WebFramework locally. Build WebFramework from source")

	include(ProcessorCount)
	
	ProcessorCount(CPUCores)

	execute_process(
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		COMMAND ${CMAKE_COMMAND} -E make_directory build
	)

	execute_process(
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/build
		COMMAND git clone https://github.com/LazyPanda07/WebFramework.git -b ${WEB_FRAMEWORK_TAG} --recursive
	)

	execute_process(
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/build/WebFramework
		COMMAND ${CMAKE_COMMAND} -E make_directory build
	)

	execute_process(
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/build/WebFramework/build
		COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/WebFrameworkLibrary ..
	)
	
	execute_process(
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/build/WebFramework/build
		COMMAND ${CMAKE_COMMAND} --build . --target install --config ${CMAKE_BUILD_TYPE} --parallel ${CPUCores}
	)

	set(WEB_FRAMEWORK_SDK ${WEB_FRAMEWORK_LIBRARY_DIR})

	find_package(WebFramework REQUIRED)
endif()

add_subdirectory(SyncWatchers)
add_subdirectory(Core)

install(DIRECTORY configs DESTINATION ${CMAKE_INSTALL_PREFIX})
