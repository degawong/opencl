/*
 * @Author: your name
 * @Date: 2021-08-16 14:45:34
 * @LastEditTime: 2021-08-20 14:05:40
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \arcsoft_algorithm\algorithm\opencl\opencl_management\opencl_management.cpp
 */

#include <regex>
#include <string>
#include <memory>
#include <fstream>

#include <CL/cl.h>

#include <opencl_management.hpp>

namespace harpocrates {
	const char* _build_option_ = { "-cl-fast-relaxed-math -cl-mad-enable -cl-denorms-are-zero" };

	opencl::opencl() : __cl_res{ CL_SUCCESS }, __cq{ nullptr }, __ct{ nullptr }, __cdi{ nullptr }, __cpi{ nullptr } {
		__get_cpi();
		__get_cdi();
		__create_context();
	}
	opencl::~opencl() {
		auto res = __release_operation();
		if (res != 0) /* */;
	}
	std::string opencl::compile_error() {
		return __compile_error();
	}
	int opencl::set_buildoption(std::string build_option) {
		__build_option = build_option;
		return 0;
	}
	int opencl::wait_queue(std::vector<int> work_size, int work_demension) {
		size_t* work_units_per_kernel = { nullptr };
		do {
			if(work_size.size() != work_demension) { __cl_res = -1; break; }
			work_units_per_kernel = new size_t[work_demension]{ 0 };
			if (work_units_per_kernel == nullptr) { __cl_res = -1; break; }
			for (auto i = 0; i < work_demension; ++i) {
				work_units_per_kernel[i] = work_size[i];
			}
			// std::to_string(std::hash<std::string>{}.operator()(space_name));
			__cl_res = clEnqueueNDRangeKernel(__cq, __ck[__add_namespace()], work_demension, nullptr, work_units_per_kernel, nullptr, 0, nullptr, nullptr);
			if (__cl_res != CL_SUCCESS) break;
		} while (false);
		delete[] work_units_per_kernel;
		return static_cast<int>(__cl_res);
	}
	int opencl::read_memory(int index, size_t size, void* pointer) {
		do {
			auto mi = __add_namespace(index);
			__cl_res = clEnqueueReadBuffer(__cq, __cm[mi], CL_TRUE, 0, size, pointer, 0, nullptr, nullptr);
			if (__cl_res != CL_SUCCESS) break;
		} while (false);
		return static_cast<int>(__cl_res);
	}
	// include dummy parameter
	int opencl::set_argument(int index, size_t size, void* pointer, int flag) {
		do {
			__cl_res = clSetKernelArg(__ck[__add_namespace()], index, size, (pointer == nullptr ? (&__cm[__add_namespace(index)]) : pointer));
			if (__cl_res != CL_SUCCESS) break;
		} while (false);
		return static_cast<int>(__cl_res);
	}
	int opencl::create_buffer(int index, size_t size, void* pointer, int flag) {
		do {
			auto mi = __add_namespace(index);
			if (__cm.count(mi) == 1) break;
			__cm[mi] = clCreateBuffer(__ct, flag, size, pointer, &__cl_res);
			if (__cl_res != CL_SUCCESS) break;
		} while (false);
		return static_cast<int>(__cl_res);
	}
	int opencl::transfer_data(int index, size_t size, void* pointer, int flag, int offset) {
		do {
			auto mi = __add_namespace(index);
			auto dp = static_cast<unsigned char*>(
				clEnqueueMapBuffer(__cq, __cm[mi], CL_TRUE, flag, offset, size, 0, nullptr, nullptr, &__cl_res)
			);
			if (__cl_res != CL_SUCCESS) break;
			(flag == CL_MAP_WRITE) ? std::memcpy(pointer, dp, size) : std::memcpy(dp, pointer, size);
			__cl_res = clEnqueueUnmapMemObject(__cq, __cm[mi], dp, 0, nullptr, nullptr);
			if (__cl_res != CL_SUCCESS) break;
		} while (false);
		return static_cast<int>(__cl_res);
	}
	int opencl::simple_argument(int index, size_t size, void* pointer, int flag) {
		do {
			auto mi = __add_namespace(index);
			if (__cm.count(mi) == 1) break;
			auto ki = __add_namespace();
			__cm[mi] = clCreateBuffer(__ct, flag, size, pointer, &__cl_res);
			if (__cl_res != CL_SUCCESS) break;
			__cl_res = clSetKernelArg(__ck[ki], index, sizeof(cl_mem), (&__cm[mi]));
			if (__cl_res != CL_SUCCESS) break;
		} while (false);
		return static_cast<int>(__cl_res);
	}
	int opencl::create_kernel(std::string kernel_name) {
		do {
			__kernel_name = kernel_name;
			auto ki = __add_namespace();
			if (__ck.count(ki) == 1) break;
			__ck[ki] = clCreateKernel(__cp[__algorithm_name], __kernel_name.c_str(), &__cl_res);
			if (__cl_res != CL_SUCCESS) break;
		} while (false);
		return static_cast<int>(__cl_res);
	}
	int opencl::create_sourceprogram(std::string algorithm_name, std::string path) {
		do {
			__algorithm_name = algorithm_name;
			__cl_res = __create_sourceprogram(path);
			if (__cl_res != CL_SUCCESS) break;
		} while (false);
		return static_cast<int>(__cl_res);
	}
	int opencl::create_binaryprogram(std::string algorithm_name, std::string path) {
		do {
			__algorithm_name = algorithm_name;
			__cl_res = __create_binaryprogram(path);
			if (__cl_res != CL_SUCCESS) break;
		} while (false);
		return static_cast<int>(__cl_res);
	}
	int opencl::clear(std::string algorithm_name, std::string kernel_name, int argument_count) {
		do {
			auto cm = algorithm_name + std::string{ "::" } + kernel_name + std::string{ "::" };
			for (auto i = 0; i < argument_count; ++i) {
				auto key = cm + std::to_string(i);
				__cl_res += clReleaseMemObject(__cm[key]) + (__cm.erase(key) - 1);
			}
			if (__cl_res != CL_SUCCESS) break;
			auto ck = algorithm_name + std::string{ "::" } + kernel_name;
			__cl_res = clReleaseKernel(__ck[ck]) + (__ck.erase(ck) - 1);
			if (__cl_res != CL_SUCCESS) break;
			__cl_res = clReleaseProgram(__cp[algorithm_name]) + (__cp.erase(algorithm_name) - 1);
			if (__cl_res != CL_SUCCESS) break;
		} while (false);
		return static_cast<int>(__cl_res);
	}
	int opencl::__get_cdi() {
		do {
			cl_uint device_count = 0;
			if (__cl_res != CL_SUCCESS) break;
			if (__cpi == nullptr) { __cl_res = -1; break; }
			__cl_res = clGetDeviceIDs(__cpi, CL_DEVICE_TYPE_GPU, 0, nullptr, &device_count);
			if (__cl_res != CL_SUCCESS) break;
			__cl_res = clGetDeviceIDs(__cpi, CL_DEVICE_TYPE_GPU, 1, &__cdi, nullptr);
			if (__cl_res != CL_SUCCESS) break;
		} while (false);
		return static_cast<int>(__cl_res);
	}
	int opencl::__get_cpi() {
		cl_platform_id* cpi{ nullptr };
		do {
			char vendor[50]{ 0 };
			cl_uint platform_count = 0;
			__cl_res = clGetPlatformIDs(0, nullptr, &platform_count);
			if (__cl_res != CL_SUCCESS) break;
			cpi = new cl_platform_id[platform_count]{ nullptr };
			if (cpi == nullptr) break;
			__cl_res = clGetPlatformIDs(platform_count, cpi, nullptr);
			if (__cl_res != CL_SUCCESS) break;
			for (cl_uint i = 0; i < platform_count; ++i) {
				__cl_res = clGetPlatformInfo(cpi[i], CL_PLATFORM_VENDOR, sizeof(vendor), &vendor, nullptr);
				if (__cl_res != CL_SUCCESS) break;
				auto v = std::string{ vendor };
				std::transform(v.begin(), v.end(), v.begin(), ::tolower);
				if (std::regex_search(std::string{ v }, std::regex{_cpi_})) {
					__cpi = cpi[i]; break;
				}
			}
		} while (false);
		delete[] cpi;
		return static_cast<int>(__cl_res);
	}
	int opencl::__build_program() {
		return clBuildProgram(__cp[__algorithm_name], 1, &__cdi, __build_option.c_str(), nullptr, nullptr);
	}
	std::string opencl::__compile_error() {
		std::string s{};
		size_t error_size = 0;
		char* information = nullptr;
		do {
			auto res = clGetProgramBuildInfo(__cp[__algorithm_name], __cdi, CL_PROGRAM_BUILD_LOG, 0, 0, &error_size);
			if (res != 0) break;
			information = new char[error_size] {0};
			if (res != 0) { res = -1; break; }
			res = clGetProgramBuildInfo(__cp[__algorithm_name], __cdi, CL_PROGRAM_BUILD_LOG, error_size, information, nullptr);
			if (res != 0) break;
			s = std::string{ information, information + error_size };
		} while (false);
		delete[] information;
		return s;
	}
	int opencl::__create_context() {
		cl_context_properties properties[5]{ 0 };
		do {
			__ct = clCreateContext(properties, 1, &__cdi, nullptr, nullptr, &__cl_res);
			if (__cl_res != CL_SUCCESS) break;
			__cq = clCreateCommandQueue(__ct, __cdi, CL_QUEUE_PROFILING_ENABLE, &__cl_res);
			if (__cl_res != CL_SUCCESS) break;
		} while (false);
		return static_cast<int>(__cl_res);
	}
	int opencl::__release_operation() {
		do {
			for (const auto& [_, cm] : __cm) {
				__cl_res += clReleaseMemObject(cm);
			}
			if (__cl_res != CL_SUCCESS) break;
			for (const auto& [_, ck] : __ck) {
				__cl_res += clReleaseKernel(ck);
			}
			if (__cl_res != CL_SUCCESS) break;
			for (const auto& [_, cp] : __cp) {
				__cl_res += clReleaseProgram(cp);
			}
			if (__cl_res != CL_SUCCESS) break;
			__cl_res = clReleaseCommandQueue(__cq);
			if (__cl_res != CL_SUCCESS) break;
			__cl_res = clReleaseContext(__ct);
			if (__cl_res != CL_SUCCESS) break;
			__cl_res = clReleaseDevice(__cdi);
			if (__cl_res != CL_SUCCESS) break;
		} while (false);
		return static_cast<int>(__cl_res);
	}
	std::string opencl::__add_namespace() {
		return __algorithm_name + std::string{ "::" } + __kernel_name;
	}
	std::string opencl::__add_namespace(int index) {
		return __algorithm_name + std::string{ "::" } + __kernel_name + std::string{ "::" } + std::to_string(index);
	}
	int opencl::__create_binaryprogram(std::string path) {
		std::vector<unsigned char> binary_data{ 0 };
		unsigned char* binary_kernel{ nullptr };
		do {
			size_t binary_size{ 0 };
			if (__cp.count(__algorithm_name) == 1) break;
			std::ifstream source_file(path + __algorithm_name + ".cl");
			std::string source_stream{ std::istreambuf_iterator<char>(source_file), (std::istreambuf_iterator<char>()) };
			auto source_point = source_stream.data();
			auto source_size = source_stream.size() + 1;
			__cp[__algorithm_name] = clCreateProgramWithSource(__ct, 1, (const char**)(&source_point), &source_size, &__cl_res);
			if (__cl_res != CL_SUCCESS) break;
			__cl_res = __build_program();
			if (__cl_res != CL_SUCCESS) break;
			__cl_res = clGetProgramInfo(__cp[__algorithm_name], CL_PROGRAM_BINARY_SIZES, sizeof(size_t), &binary_size, nullptr);
			if (__cl_res != CL_SUCCESS) break;
			binary_kernel = new unsigned char[binary_size] {0};
			if (binary_kernel == nullptr) { __cl_res = -1; break; }
			__cl_res = clGetProgramInfo(__cp[__algorithm_name], CL_PROGRAM_BINARIES, sizeof(char*), &binary_kernel, nullptr);
			if (__cl_res != CL_SUCCESS) break;
			// if u want to keep it local
			//binary_file << binary_data;
			//binary_data.resize(2 * binary_size);
			//for (auto i = 0; i < binary_size; ++i) {
			//	binary_data[2 * i + 0] = binary_kernel[i];
			//	binary_data[2 * i + 1] = ',';
			//}
			//auto binary_path = std::string{ path + __algorithm_name + ".kernel" };
			//std::ofstream binary_file{ binary_path, std::ofstream::out };
			//binary_file.write(reinterpret_cast<char*>(binary_kernel), binary_size);
			//std::copy(binary_data.begin(), binary_data.end(), std::ostream_iterator<unsigned char>(binary_file));
			__cl_res = clReleaseProgram(__cp[__algorithm_name]);
			if (__cl_res != CL_SUCCESS) break;
			const unsigned char * bd = binary_kernel;
			__cp[__algorithm_name] = clCreateProgramWithBinary(__ct, 1, &__cdi, &binary_size, reinterpret_cast<const unsigned char**>(&bd), nullptr, &__cl_res);
			if (__cl_res != CL_SUCCESS) break;
			__cl_res = __build_program();
			if (__cl_res != CL_SUCCESS) break;
		} while (false);
		delete[] binary_kernel;
		return static_cast<int>(__cl_res);
	}
	int opencl::__create_sourceprogram(std::string path) {
		do {
			if (__cp.count(__algorithm_name) == 1) break;
			std::ifstream source_file(path + __algorithm_name + ".cl");
			std::string source_stream{ std::istreambuf_iterator<char>(source_file), (std::istreambuf_iterator<char>()) };
			auto source_point = source_stream.data();
			auto source_size = source_stream.size() + 1;
			__cp[__algorithm_name] = clCreateProgramWithSource(__ct, 1, (const char**)(&source_point), &source_size, &__cl_res);
			if (__cl_res != CL_SUCCESS) break;
			__cl_res = __build_program();
			if (__cl_res != CL_SUCCESS) break;
		} while (false);
		return static_cast<int>(__cl_res);
	}
}