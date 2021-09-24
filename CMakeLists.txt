cmake_minimum_required(VERSION 3.0)

project(
	opencl
)

set(CMAKE_CXX_STANDARD 20)

enable_testing(
)

find_package(
	OpenCL
	QUIET
)

if(NOT ${OpenCL_FOUND})
	return()
endif(NOT ${OpenCL_FOUND})

set_property(
	GLOBAL
	PROPERTY
	USE_FOLDERS
	on
)

set(
	BENCHMARK_ENABLE_INSTALL OFF
)

set(
	BENCHMARK_ENABLE_TESTING OFF
)

set(
	BENCHMARK_ENABLE_GTEST_TESTS OFF
)

set(
	BENCHMARK_ENABLE_ASSEMBLY_TESTS OFF
)

set(
	gtest_force_shared_crt ON CACHE BOOL "always use mdd" FORCE
)

add_subdirectory(
	benchmark
)

add_subdirectory(
	googletest
)

add_subdirectory(
	opencl_interface
)

add_subdirectory(
	opencl_management
)

add_subdirectory(
	opencl_algorithm
)

add_subdirectory(
	opencl_test
)

add_subdirectory(
	opencl_benchmark
)