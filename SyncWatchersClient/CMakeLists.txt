cmake_minimum_required(VERSION 3.27.0)

project(SyncWatchersClient)

function(add_windows_target)
	if (WIN32 AND NOT TARGET ${PROJECT_NAME}_Windows)
		add_custom_target(
			${PROJECT_NAME}_Windows ALL
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/SyncWatchersClient
			COMMAND flutter build windows -v --release
			COMMAND ${CMAKE_COMMAND} -E copy_directory build/windows/x64/runner/Release/ ${CMAKE_INSTALL_PREFIX}/windows
			COMMENT "Building Windows Flutter application"
		)
	endif()
endfunction()

function(add_android_target)
	if (NOT TARGET ${PROJECT_NAME}_Android)
		add_custom_target(
			${PROJECT_NAME}_Android ALL
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/SyncWatchersClient
			COMMAND flutter build apk -v --release --split-per-abi
			COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_INSTALL_PREFIX}/android
			COMMAND ${CMAKE_COMMAND} -E copy build/app/outputs/flutter-apk/app-arm64-v8a-release.apk ${CMAKE_INSTALL_PREFIX}/android/
			COMMENT "Building Android Flutter application"
		)
	endif()
endfunction()

if (${WITH_CLIENTS})
	message("Build both clients")

	add_windows_target()
	add_android_target()
endif()

if (${WITH_WINDOWS_CLIENT})
	message("Build Windows client")

	add_windows_target()
endif()

if (${WITH_ANDROID_CLIENT})
	message("Build Android client")

	add_android_target()
endif()
