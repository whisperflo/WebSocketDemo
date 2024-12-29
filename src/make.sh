#!/bin/bash

# 如果提供了 clean 参数，删除构建目录
if [ "$1" == "clean" ]; then
    echo "Cleaning build directory..."
    rm -rf build
    exit 0
fi

# 删除旧的构建目录（如果存在）
if [ -d "build" ]; then
    echo "Removing old build directory..."
    rm -rf build
fi

# 创建新的构建目录
echo "Creating new build directory..."
mkdir build
cd build

# 运行 cmake 配置项目
echo "CMake开始编译..."
cmake .. -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_SOURCE_DIR}/src

# 编译项目
echo "正在编译中..."
if ! make; then
    echo "编译失败!"
    exit 1
fi

# 完成
echo "编译成功! 可执行文件已放置在 src 目录下。"
