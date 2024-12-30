cmake_minimum_required(VERSION 3.27.0)

set(WEB_FRAMEWORK_TAG v3.0.12)
set(WEB_FRAMEWORK_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/WebFrameworkLibrary CACHE STRING "")
set(CMAKE_BUILD_TYPE Release CACHE STRING "")

list(APPEND CMAKE_MODULE_PATH ${WEB_FRAMEWORK_LIBRARY_DIR})

if (UNIX)
	list(APPEND CMAKE_MODULE_PATH /usr/lib/web-framework)
endif()

find_package(WebFramework QUIET)

if (NOT DEFINED WEB_FRAMEWORK_SDK)
	set(WEB_FRAMEWORK_SDK ${WEB_FRAMEWORK_LIBRARY_DIR})
endif()

if (${WebFramework_FOUND})
	message("Use WebFramework in ${WEB_FRAMEWORK_LIBRARY_DIR}")
endif()

if (NOT ${WebFramework_FOUND})
	message("Can't find WebFramework locally. Build WebFramework ${WEB_FRAMEWORK_TAG} from source")

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
		COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${WEB_FRAMEWORK_LIBRARY_DIR} ..
	)
	
	execute_process(
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/build/WebFramework/build
		COMMAND ${CMAKE_COMMAND} --build . --target install --config Release --parallel ${CPUCores}
	)

	find_package(WebFramework REQUIRED)
endif()
