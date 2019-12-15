project(ZRainySTL)
# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 添加头文件目录
include_directories(include)

# 支持C++11
add_definitions(-std=c++11)

# 支持子目录
add_subdirectory(src)

# 指定生成目标
add_executable(ZRainySTL ut_main.cpp)

# 查找gtest静态库
find_library(GTEST_LIB gtest /usr/local/lib NO_DEFAULT_PATH)
# find_library(GTEST_MAIN__LIB gtest_main /usr/local/lib NO_DEFAULT_PATH)

# 查找pthread库
find_package(Threads)

# 添加需要链接的库文件路径
# link_libraries(${GTEST_LIB} ${GTEST_MAIN__LIB})
link_libraries(${GTEST_LIB})

# 设置要链接的库
# target_link_libraries(ZRainySTL libgtest.a libgtest_main.a)
target_link_libraries(ZRainySTL libgtest.a)
target_link_libraries(ZRainySTL ZRainySTLCore)
target_link_libraries(ZRainySTL ${CMAKE_THREAD_LIBS_INIT}) # 链接pthread库
