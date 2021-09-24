/*
 * @Author: your name
 * @Date: 2021-08-17 20:14:15
 * @LastEditTime: 2021-08-18 08:35:36
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \arcsoft_algorithm\algorithm\opencl\opencl_algorithm\opencl_helloworld.hpp
 */

#include <opencl_management.hpp>

namespace harpocrates {
	class hello_world {
	public:
		hello_world() = default;
		~hello_world() = default;
	public:
		int apply();
	private:
		opencl __cl_handle;
	};
}
