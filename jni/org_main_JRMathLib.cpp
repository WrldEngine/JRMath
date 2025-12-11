#include "org_main_JRMathLib.h"
#include "gpu/gpuOperators.h"
#include <cmath>
#include <vector>
#include <iostream>

JNIEXPORT jdouble JNICALL Java_org_main_JRMathLib_CMathExp
  (JNIEnv *env, jclass clazz, jdouble x)
{
    return std::exp(x);
}

JNIEXPORT jintArray JNICALL Java_org_main_JRMathLib_SumArrays
  (JNIEnv *env, jclass, jintArray a, jintArray b)
{
    jsize size = env->GetArrayLength(a);

    jint *aarr = env->GetIntArrayElements(a, NULL);
    jint *barr = env->GetIntArrayElements(b, NULL);

    jintArray resultArray = env->NewIntArray(size);
    OpenCLKernelsOperator g;

    std::vector<int> a_to_vec(size);
    std::vector<int> b_to_vec(size);

    env->GetIntArrayRegion(a, jsize{0}, size, &a_to_vec[0]);
    env->GetIntArrayRegion(b, jsize{0}, size, &b_to_vec[0]);

    std::vector<int> result = g.sumArrays(a_to_vec, b_to_vec);

    env->SetIntArrayRegion(resultArray, 0, result.size(), reinterpret_cast<const jint*>(result.data()));
    env->ReleaseIntArrayElements(a, aarr, NULL);
    env->ReleaseIntArrayElements(b, barr, NULL);

    return resultArray;
}

JNIEXPORT jintArray JNICALL Java_org_main_JRMathLib_MulArrays
  (JNIEnv *env, jclass, jintArray a, jintArray b)
{
    jsize size = env->GetArrayLength(a);

    jint *aarr = env->GetIntArrayElements(a, NULL);
    jint *barr = env->GetIntArrayElements(b, NULL);

    jintArray resultArray = env->NewIntArray(size);
    OpenCLKernelsOperator g;

    std::vector<int> a_to_vec(size);
    std::vector<int> b_to_vec(size);

    env->GetIntArrayRegion(a, jsize{0}, size, &a_to_vec[0]);
    env->GetIntArrayRegion(b, jsize{0}, size, &b_to_vec[0]);

    std::vector<int> result = g.mulArrays(a_to_vec, b_to_vec);

    env->SetIntArrayRegion(resultArray, 0, result.size(), reinterpret_cast<const jint*>(result.data()));
    env->ReleaseIntArrayElements(a, aarr, NULL);
    env->ReleaseIntArrayElements(b, barr, NULL);

    return resultArray;
}

JNIEXPORT void JNICALL Java_org_main_JRMathLib_SumBuffers
  (JNIEnv *env, jclass, jobject bufA, jobject bufB, jobject bufR, jint N)
{
    int* a = (int*) env->GetDirectBufferAddress(bufA);
    int* b = (int*) env->GetDirectBufferAddress(bufB);
    int* r = (int*) env->GetDirectBufferAddress(bufR);

    if (!a || !b || !r) {
        fprintf(stderr, "SumBuffers: direct buffer address is null\n");
        return;
    }

    OpenCLKernelsOperator g;
    g.sumArraysRaw(a, b, r, N);
}

JNIEXPORT void JNICALL Java_org_main_JRMathLib_MulBuffers
  (JNIEnv *env, jclass, jobject bufA, jobject bufB, jobject bufR, jint N)
{
    int* a = (int*) env->GetDirectBufferAddress(bufA);
    int* b = (int*) env->GetDirectBufferAddress(bufB);
    int* r = (int*) env->GetDirectBufferAddress(bufR);

    if (!a || !b || !r) {
        fprintf(stderr, "MulBuffers: direct buffer address is null\n");
        return;
    }

    OpenCLKernelsOperator g;
    g.mulArraysRaw(a, b, r, N);
}

JNIEXPORT void JNICALL Java_org_main_JRMathLib_MatMulBuffers
  (JNIEnv *env, jclass, jobject bufA, jobject bufB, jobject bufR, jint n, jint m, jint k)
{
    float* a = (float*) env->GetDirectBufferAddress(bufA);
    float* b = (float*) env->GetDirectBufferAddress(bufB);
    float* r = (float*) env->GetDirectBufferAddress(bufR);

    if (!a || !b || !r) {
        fprintf(stderr, "MatMulBuffers: direct buffer address is null\n");
        return;
    }

    OpenCLKernelsOperator g;
    g.matMulArraysRaw(a, b, r, N);
}