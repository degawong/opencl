/*
 * @Author: your name
 * @Date: 2021-08-17 20:14:15
 * @LastEditTime: 2021-08-18 08:35:30
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \arcsoft_algorithm\algorithm\opencl\opencl_algorithm\opencl_helloworld.cpp
 */

#include <opencl_helloworld.hpp>

namespace harpocrates {
	int hello_world::apply() {
		auto res = 0;
		char message[16]{ 0 };
		do {
			res = __cl_handle.create_sourceprogram("hello_algorithm", _kernel_path_);
			if (res != 0) break;
			res = __cl_handle.create_kernel("hello_kernel");
			if (res != 0) break;
			res = __cl_handle.simple_argument(0, sizeof(message), nullptr, CL_MEM_WRITE_ONLY);
			if (res != 0) break;
			res = __cl_handle.wait_queue({ 16 }, 1);
			if (res != 0) break;
			res = __cl_handle.read_memory(0, sizeof(message), message);
			if (res != 0) break;
		} while (false);
		return res;
	}
}
