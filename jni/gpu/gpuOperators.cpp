#include <iostream>
#include <vector>
#include "gpuOperators.h"

#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>

#define assert_sz(vec1, vec2) { if (vec1.size() != vec2.size()) { std::cout << "Arrays not equal" << std::endl; exit(1); } } 

std::string getErrorString(cl_int error)
{
    switch(error) {
        // run-time and JIT compiler errors
        case 0: return "CL_SUCCESS";
        case -1: return "CL_DEVICE_NOT_FOUND";
        case -2: return "CL_DEVICE_NOT_AVAILABLE";
        case -3: return "CL_COMPILER_NOT_AVAILABLE";
        case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
        case -5: return "CL_OUT_OF_RESOURCES";
        case -6: return "CL_OUT_OF_HOST_MEMORY";
        case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
        case -8: return "CL_MEM_COPY_OVERLAP";
        case -9: return "CL_IMAGE_FORMAT_MISMATCH";
        case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
        case -11: return "CL_BUILD_PROGRAM_FAILURE";
        case -12: return "CL_MAP_FAILURE";
        case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
        case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
        case -15: return "CL_COMPILE_PROGRAM_FAILURE";
        case -16: return "CL_LINKER_NOT_AVAILABLE";
        case -17: return "CL_LINK_PROGRAM_FAILURE";
        case -18: return "CL_DEVICE_PARTITION_FAILED";
        case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

        // compile-time errors
        case -30: return "CL_INVALID_VALUE";
        case -31: return "CL_INVALID_DEVICE_TYPE";
        case -32: return "CL_INVALID_PLATFORM";
        case -33: return "CL_INVALID_DEVICE";
        case -34: return "CL_INVALID_CONTEXT";
        case -35: return "CL_INVALID_QUEUE_PROPERTIES";
        case -36: return "CL_INVALID_COMMAND_QUEUE";
        case -37: return "CL_INVALID_HOST_PTR";
        case -38: return "CL_INVALID_MEM_OBJECT";
        case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
        case -40: return "CL_INVALID_IMAGE_SIZE";
        case -41: return "CL_INVALID_SAMPLER";
        case -42: return "CL_INVALID_BINARY";
        case -43: return "CL_INVALID_BUILD_OPTIONS";
        case -44: return "CL_INVALID_PROGRAM";
        case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
        case -46: return "CL_INVALID_KERNEL_NAME";
        case -47: return "CL_INVALID_KERNEL_DEFINITION";
        case -48: return "CL_INVALID_KERNEL";
        case -49: return "CL_INVALID_ARG_INDEX";
        case -50: return "CL_INVALID_ARG_VALUE";
        case -51: return "CL_INVALID_ARG_SIZE";
        case -52: return "CL_INVALID_KERNEL_ARGS";
        case -53: return "CL_INVALID_WORK_DIMENSION";
        case -54: return "CL_INVALID_WORK_GROUP_SIZE";
        case -55: return "CL_INVALID_WORK_ITEM_SIZE";
        case -56: return "CL_INVALID_GLOBAL_OFFSET";
        case -57: return "CL_INVALID_EVENT_WAIT_LIST";
        case -58: return "CL_INVALID_EVENT";
        case -59: return "CL_INVALID_OPERATION";
        case -60: return "CL_INVALID_GL_OBJECT";
        case -61: return "CL_INVALID_BUFFER_SIZE";
        case -62: return "CL_INVALID_MIP_LEVEL";
        case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
        case -64: return "CL_INVALID_PROPERTY";
        case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
        case -66: return "CL_INVALID_COMPILER_OPTIONS";
        case -67: return "CL_INVALID_LINKER_OPTIONS";
        case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

        // extension errors
        case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
        case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
        case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
        case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
        case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
        case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
        default: return "Unknown OpenCL error";
    }
}

#define clErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cl_int error, const char *file, int line, bool abort = true) {
    if (error != CL_SUCCESS) {
        fprintf(stderr, "GPUassert: %s %s %d\n", getErrorString(error).c_str(), file, line);
        if (abort) {
            exit(error);
        }
    }
}

inline bool check_dvs() {
    cl_uint number_of_platforms = 0;
    clGetPlatformIDs(0, NULL, &number_of_platforms);

    if (number_of_platforms == 0) {
        std::cout << " No platforms found. Check OpenCL installation!" << std::endl;
        exit(1);
    };
}

OpenCLKernelsOperator::OpenCLKernelsOperator() {
    check_dvs();

    cl_platform_id platform_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;

    this->ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    this->ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &this->dv_id, &ret_num_devices);
    this->context = clCreateContext(NULL, 1, &this->dv_id, NULL, NULL, &ret);
    this->command_queue = clCreateCommandQueueWithProperties(context, this->dv_id, 0, &ret);

    clErrchk(ret);
}

//! A LOT OF BOILERPLATE, NEED CUDA IMPLEMENTATION
std::vector<int> OpenCLKernelsOperator::sumArrays(const std::vector<int>& a, const std::vector<int>& b) {
    assert_sz(a, b)

    int arr_sz = a.size();
    std::vector<int> result(arr_sz);

    std::string source_str = R"==(
    __kernel void sum_vectors(
        __global const int *vec_a,
        __global const int *vec_b,
        __global int *res,
        const int size
    )
    {
        int idx = get_global_id(0);
        if (idx > size) {
            return;
        }
        res[idx] = vec_a[idx] + vec_b[idx];
    })==";

    const char* source_c_str = source_str.c_str();
    const size_t source_size = source_str.size();

    cl_program program = clCreateProgramWithSource(
        this->context, 1,
        (const char**)&source_c_str,
        (const size_t*)&source_size,
        &this->ret
    );
    clErrchk(this->ret);

    cl_mem vec_a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, arr_sz * sizeof(int), NULL, &this->ret);
    clErrchk(this->ret);
    cl_mem vec_b_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, arr_sz * sizeof(int), NULL, &this->ret);
    clErrchk(this->ret);
    cl_mem res_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, arr_sz * sizeof(int), NULL, &this->ret);
    clErrchk(this->ret);

    clErrchk(
        clEnqueueWriteBuffer(
            this->command_queue, vec_a_mem_obj, CL_TRUE, 0,
            arr_sz*sizeof(int), a.data(), 0, NULL, NULL
        )
    );

    clErrchk(
        clEnqueueWriteBuffer(
            this->command_queue, vec_b_mem_obj, CL_TRUE, 0,
            arr_sz*sizeof(int), b.data(), 0, NULL, NULL
        )
    );

    clErrchk(clBuildProgram(program, 1, &this->dv_id, NULL, NULL, NULL));

    cl_kernel kernel = clCreateKernel(program, "sum_vectors", &this->ret);
    clErrchk(this->ret);

    clErrchk(clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&vec_a_mem_obj));
    clErrchk(clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&vec_b_mem_obj));
    clErrchk(clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&res_mem_obj));
    clErrchk(clSetKernelArg(kernel, 3, sizeof(const int), (void*)&arr_sz));

    const size_t max_local_size = 256;
    size_t global_size = arr_sz;
    size_t local_size = arr_sz;
    if (local_size > max_local_size) {
        local_size = max_local_size;
    }

    clErrchk(clEnqueueNDRangeKernel(
        command_queue, kernel, 1, NULL,
        &global_size, &local_size,
        0, NULL, NULL
    ));

    clErrchk(clEnqueueReadBuffer(
        this->command_queue, res_mem_obj, CL_TRUE, 0,
        arr_sz * sizeof(float), result.data(), 0, NULL, NULL
    ));

    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(vec_a_mem_obj);
    ret = clReleaseMemObject(vec_b_mem_obj);
    ret = clReleaseMemObject(res_mem_obj);
    ret = clReleaseCommandQueue(this->command_queue);
    ret = clReleaseContext(this->context);

    return result;
}

//! A LOT OF BOILERPLATE, NEED CUDA IMPLEMENTATION
std::vector<int> OpenCLKernelsOperator::mulArrays(const std::vector<int>& a, const std::vector<int>& b) {
    assert_sz(a, b)

    int arr_sz = a.size();
    std::vector<int> result(arr_sz);

    std::string source_str = R"==(
    __kernel void mul_vectors(
        __global const int *vec_a,
        __global const int *vec_b,
        __global int *res,
        const int size
    )
    {
        int idx = get_global_id(0);
        if (idx > size) {
            return;
        }
        res[idx] = vec_a[idx] * vec_b[idx];
    })==";

    const char* source_c_str = source_str.c_str();
    const size_t source_size = source_str.size();

    cl_program program = clCreateProgramWithSource(
        this->context, 1,
        (const char**)&source_c_str,
        (const size_t*)&source_size,
        &this->ret
    );
    clErrchk(this->ret);

    cl_mem vec_a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, arr_sz * sizeof(int), NULL, &this->ret);
    clErrchk(this->ret);
    cl_mem vec_b_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, arr_sz * sizeof(int), NULL, &this->ret);
    clErrchk(this->ret);
    cl_mem res_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, arr_sz * sizeof(int), NULL, &this->ret);
    clErrchk(this->ret);

    clErrchk(
        clEnqueueWriteBuffer(
            this->command_queue, vec_a_mem_obj, CL_TRUE, 0,
            arr_sz*sizeof(int), a.data(), 0, NULL, NULL
        )
    );

    clErrchk(
        clEnqueueWriteBuffer(
            this->command_queue, vec_b_mem_obj, CL_TRUE, 0,
            arr_sz*sizeof(int), b.data(), 0, NULL, NULL
        )
    );

    clErrchk(clBuildProgram(program, 1, &this->dv_id, NULL, NULL, NULL));

    cl_kernel kernel = clCreateKernel(program, "mul_vectors", &this->ret);
    clErrchk(this->ret);

    clErrchk(clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&vec_a_mem_obj));
    clErrchk(clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&vec_b_mem_obj));
    clErrchk(clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&res_mem_obj));
    clErrchk(clSetKernelArg(kernel, 3, sizeof(const int), (void*)&arr_sz));

    const size_t max_local_size = 256;
    size_t global_size = arr_sz;
    size_t local_size = arr_sz;
    if (local_size > max_local_size) {
        local_size = max_local_size;
    }

    clErrchk(clEnqueueNDRangeKernel(
        command_queue, kernel, 1, NULL,
        &global_size, &local_size,
        0, NULL, NULL
    ));

    clErrchk(clEnqueueReadBuffer(
        this->command_queue, res_mem_obj, CL_TRUE, 0,
        arr_sz * sizeof(float), result.data(), 0, NULL, NULL
    ));

    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(vec_a_mem_obj);
    ret = clReleaseMemObject(vec_b_mem_obj);
    ret = clReleaseMemObject(res_mem_obj);
    ret = clReleaseCommandQueue(this->command_queue);
    ret = clReleaseContext(this->context);

    return result;
}