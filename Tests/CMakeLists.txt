cmake_minimum_required(VERSION 3.27.0)

project(Tests)

add_custom_target(
	${PROJECT_NAME} ALL
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	COMMAND dotnet publish Tests.sln -o ${CMAKE_BINARY_DIR}/Tests /p:DebugType=None /p:DebugSymbols=false /p:Configuration=Release /p:Platform="Any CPU"
)
