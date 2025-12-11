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
    void sumArraysRaw(const int* a, const int* b, int* r, int n);
    void mulArraysRaw(const int* a, const int* b, int* r, int n);
    void matMulArraysRaw(const float* a, const float* b, float* r, int n, int m, int k);
};

class CUDAKernelsOperator {};
