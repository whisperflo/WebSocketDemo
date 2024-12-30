# 设置 CMake 的最低版本要求
cmake_minimum_required(VERSION 3.10)

# 指定项目名称
project(WebSocketProject)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加包含目录
include_directories(${CMAKE_SOURCE_DIR}/task)
include_directories(${CMAKE_SOURCE_DIR}/websocketService)
include_directories(${CMAKE_SOURCE_DIR}/websocketWorkLayer)
include_directories(${CMAKE_SOURCE_DIR}/emu_msg)
include_directories(${CMAKE_SOURCE_DIR}/Log)
# 添加 log4cpp 头文件路径
include_directories(/usr/local/include) 
# 设置可执行文件输出目录为 src 目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
# 添加源文件
add_executable(my_program
    main.cpp
    task/TaskWebSocket.cpp
    websocketService/WebSocketService.cpp
    websocketWorkLayer/WSWorkLayerImp.cpp
    Log/logger.cc
)
# 添加 log4cpp 库路径
link_directories(/usr/local/lib) 
# 链接 log4cpp 库
target_link_libraries(my_program log4cpp)