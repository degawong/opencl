
cmake_minimum_required(VERSION 3.0)

project(
	opencl_algorithm
)

file(
	GLOB
	kernel_file
	${CMAKE_CURRENT_SOURCE_DIR}/kernel/*.cl
)

file(
	GLOB
	src_file
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/*.md
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/kernel/*.cl
)

add_library(
	${PROJECT_NAME}
	${src_file}
	$<TARGET_OBJECTS:opencl_management>
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	$<$<CXX_COMPILER_ID:MSVC>:${OpenCL_INCLUDE_DIR}>
)

target_link_libraries(
	${PROJECT_NAME}
	PUBLIC
	opencl_management
	$<$<CXX_COMPILER_ID:GNU>:${OpenCL_LIBRARY}>
	$<$<CXX_COMPILER_ID:MSVC>:${OpenCL_LIBRARY}>
	$<$<CXX_COMPILER_ID:Clang>:opencl_interface>
	$<$<CXX_COMPILER_ID:ARMClang>:opencl_interface>
)

target_compile_definitions(
	${PROJECT_NAME}
	PRIVATE
	_kernel_path_="${CMAKE_CURRENT_SOURCE_DIR}/kernel/"
	$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},4>,_x86_arch_,_x64_arch_>
)

set_target_properties(
	${PROJECT_NAME}
	PROPERTIES
	FOLDER
	"OpenCL"
)

source_group(
	"kernel"
	FILES
	${kernel_file}
)