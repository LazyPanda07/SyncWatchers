cmake_minimum_required(VERSION 3.27.0)

project(SyncWatchers)

add_executable(
	${PROJECT_NAME}
	src/Main.cpp
	src/StateExecutor.cpp
)

if (UNIX)
	target_link_options(${PROJECT_NAME} PRIVATE -rdynamic)
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(FILES config.json DESTINATION .)
install(FILES web.json DESTINATION .)
