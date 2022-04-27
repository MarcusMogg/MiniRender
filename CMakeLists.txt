# CMakeList.txt: minirender 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("minirender")
add_subdirectory("src")

# 将源代码添加到此项目的可执行文件。
add_executable (minirender 
    "minirender.cpp" 
    "minirender.h"
    "ch0.h"
    "ch1.h"
    "ch2.h" "ch3.h")

target_link_libraries( minirender
    PRIVATE
        minirenderlib
)

FILE(GLOB MYRES "resource/*")
FILE(COPY ${MYRES} DESTINATION "${PROJECT_BINARY_DIR}/resource")

# TODO: 如有需要，请添加测试并安装目标。
