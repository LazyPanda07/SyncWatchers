cmake_minimum_required(VERSION 3.27.0)

project(SyncWatchers)

add_executable(
	${PROJECT_NAME}
	src/Main.cpp
	src/StateExecutor.cpp
)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(FILES config.json DESTINATION .)
install(FILES web.json DESTINATION .)
