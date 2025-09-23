cmake_minimum_required(VERSION 3.27.0)

project(ContentExecutor)

add_library(
	${PROJECT_NAME} SHARED
	src/DownloadContentExecutor.cpp
	src/UploadContentExecutor.cpp
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(FILES content_config.json DESTINATION .)
install(FILES content_web.json DESTINATION .)
