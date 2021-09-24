### CL_PLATFORM_VENDOR

|     PLATFORM     |    VALUE    |
| :---------: | :--------------------: |
|  QC  | QUALCOMM |
| INTEL | Intel |
| NVIDIA | NVIDIA |
|  MALI  | Imagination |

### CL_BUILD_STATUS

```c++
cl_int clBuildProgram(
	cl_program program,
    cl_uint num_devices, 
    const cl_device_id *device_list,
    const char *options, 
    void (*pfn_notify)(cl_program, void *user_data),    
    void *user_data
)
```

|     VALUE     |    MEANING    |
| :---------: | :--------------------: |
| CL_INVALID_PROGRAM | 如果`program`不是有效的`program`对象 |
| CL_INVALID_VALUE | 如果`device_list`为空且`num_devices`大于零，或者`device_list`不为`NULL`且`num_devices`为零 |
| CL_INVALID_VALUE | 如果`pfn_notify`为`NULL`，但`user_data`不为NULL |
| CL_INVALID_DEVICE | 如果`device_list`中列出的`OpenCL`设备不在与`program`关联的设备列表中 |
| CL_INVALID_BINARY | 如果程序是用`clCreateWithProgramBinary`创建的，并且`device_list`中列出的设备没有加载有效的程序二进制文件 |
| CL_INVALID_BUILD_OPTIONS | 如果`options`指定的生成选项无效 |
| CL_INVALID_OPERATION | 如果先前调用`clBuildProgram`对于`program`为`device_list`中列出的任何设备生成可执行程序的过程尚未完成 |
| CL_COMPILER_NOT_AVAILABLE | 如果`program`是用`clCreateProgramWithSource`创建的，并且编译器不可用，即表4.3中指定的可用`CL_DEVICE_COMPILER_`设置为`CL_FALSE` |
| CL_BUILD_PROGRAM_FAILURE | 如果生成程序可执行文件失败。如果`clBuildProgram`在生成完成之前不返回，将返回此错误 |
| CL_INVALID_OPERATION | 如果`program`附加了内核对象 |
| CL_OUT_OF_HOST_MEMORY | 如果在主机上分配`OpenCL`实现所需的资源失败 |