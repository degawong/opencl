/*
 * @Author: your name
 * @Date: 2021-08-16 14:45:34
 * @LastEditTime: 2021-08-20 14:05:29
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \arcsoft_algorithm\algorithm\opencl\opencl_management\opencl_management.hpp
 */

#pragma once

#include <map>
#include <string>
#include <vector>

#include <CL/cl.h>

namespace harpocrates {

	class opencl {
	public:
		opencl();
		virtual ~opencl();
	public:
		std::string compile_error();
	public:
		int set_buildoption(std::string build_option);
		int read_memory(int index, size_t size, void* pointer);
		int wait_queue(std::vector<int> work_size, int work_demension);
		int set_argument(int index, size_t size, void* pointer, int flag);
		int create_buffer(int index, size_t size, void* pointer, int flag);
		int simple_argument(int index, size_t size, void* pointer, int flag);
		int create_kernel(std::string kernel_name);
		int create_sourceprogram(std::string algorithm_name, std::string path);
		int create_binaryprogram(std::string algorithm_name, std::string path);
		int clear(std::string algorithm_name, std::string kernel_name, int argument_count);
		int transfer_data(int index, size_t size, void* pointer, int flag, int offset = 0);
	protected:
		int __get_cdi();
		int __get_cpi();
		int __build_program();
		std::string __compile_error();
		int __create_context();
		int __release_operation();
		std::string __add_namespace();
		std::string __add_namespace(int index);
		int __create_binaryprogram(std::string path);
		int __create_sourceprogram(std::string path);
	private:
		cl_int __cl_res;
		cl_context __ct;
		cl_device_id __cdi;
		cl_platform_id __cpi;
		cl_command_queue __cq;
		std::string __build_option;
		std::string __kernel_name;
		std::string __algorithm_name;
		std::map<std::string, cl_mem> __cm;
		std::map<std::string, cl_kernel> __ck;
		std::map<std::string, cl_program> __cp;
	};
}