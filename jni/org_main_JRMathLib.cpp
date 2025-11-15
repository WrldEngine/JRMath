#include "org_main_JRMathLib.h"
#include <cmath>

JNIEXPORT jdouble JNICALL Java_org_main_JRMathLib_CMathExp
  (JNIEnv *env, jclass clazz, jdouble x)
{
    return std::exp( x );
}

JNIEXPORT jintArray JNICALL Java_org_main_JRMathLib_SumArrays
  (JNIEnv *env, jclass, jintArray a, jintArray b)
{
    jsize size = env->GetArrayLength(a);

    jint *aarr = env->GetIntArrayElements(a, NULL);
    jint *barr = env->GetIntArrayElements(b, NULL);

    jintArray resultArray = env->NewIntArray(size);
    jint *rarr = env->GetIntArrayElements(resultArray, NULL);

    for (int i = 0; i < size; i++) {
        rarr[i] = aarr[i] + barr[i];
    };

    env->ReleaseIntArrayElements(resultArray, rarr, NULL);
    env->ReleaseIntArrayElements(a, aarr, NULL);
    env->ReleaseIntArrayElements(b, barr, NULL);

    return resultArray;
}