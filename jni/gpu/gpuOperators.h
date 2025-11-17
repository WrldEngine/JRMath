#pragma once

#include <CL/cl.h>
#include <vector>

class OpenCLKernelsOperator {
    public:
    cl_context context = NULL;
    cl_int ret = NULL;
    cl_command_queue command_queue = NULL;
    cl_device_id dv_id = NULL;

    OpenCLKernelsOperator();
    std::vector<int> sumArrays(const std::vector<int>& a, const std::vector<int>& b);
    std::vector<int> mulArrays(const std::vector<int>& a, const std::vector<int>& b);
};

class CUDAKernelsOperator {};