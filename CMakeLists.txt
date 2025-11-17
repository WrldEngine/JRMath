cmake_minimum_required(VERSION 3.10)
project(jrmath_lib)

set(CMAKE_BUILD_TYPE Release)

find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES 
    jni/org_main_JRMathLib.cpp
    jni/gpu/gpuOperators.cpp
)

add_library(jnimathlib SHARED ${SOURCE_FILES})

set(OpenCL_INCLUDE_DIR "C:/Users/user/vcpkg/installed/x64-windows/include")
set(OpenCL_LIBRARY "C:/Users/user/vcpkg/installed/x64-windows/lib/OpenCL.lib")
include_directories(${OpenCL_INCLUDE_DIR})
target_link_libraries(jnimathlib PRIVATE ${OpenCL_LIBRARY})
