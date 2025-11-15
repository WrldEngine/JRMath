cmake_minimum_required(VERSION 3.10)
project(jrmath_lib)

set(CMAKE_BUILD_TYPE Release)
find_package(JNI REQUIRED)

include_directories(${JNI_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(SOURCE_FILES jni/org_main_JRMathLib.cpp)
add_library(jnimathlib SHARED ${SOURCE_FILES})