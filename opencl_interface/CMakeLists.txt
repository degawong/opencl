
cmake_minimum_required(VERSION 3.0)

project(opencl_interface)

add_library(
	${PROJECT_NAME}
	INTERFACE
)

add_library(
	${PROJECT_NAME}::${PROJECT_NAME}
	ALIAS
	${PROJECT_NAME}
)

target_include_directories(
	${PROJECT_NAME}
	INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)