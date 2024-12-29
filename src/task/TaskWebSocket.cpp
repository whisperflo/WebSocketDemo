#include "TaskWebSocket.h"
#include <iostream>
#include <future>
#include <sys/syscall.h>
#include <unistd.h>

// filepath: /home/hjx/test/c++/2024-12/test/WebSocketDemo/src/task/TaskWebSocket.cpp

TaskWebSocket::TaskWebSocket(std::shared_ptr<WebSocketService> service) : service_(service)
{
    // 初始化 WSWorkLayerImp 实例，用于具体路径处理逻辑
    ptr_wsWork = new WSWorkLayerImp(service_);

    // 添加路径处理器，当 WebSocket 客户端发送路径时触发
    service_->addPathHandler([this](const std::string &path)
                             {
        doSomething(path);
        stopFlags[path] = false;
        futures.push_back(std::async(std::launch::async, &TaskWebSocket::handlePath, this, path));
        std::cout << "[TaskWebSocket] currentPath updated to: " << path << std::endl; });
}

TaskWebSocket::~TaskWebSocket()
{
    std::cout << "[TaskWebSocket] Destructor called, resources cleaned up." << std::endl;

    // 设置停止标志并通知所有等待线程
    isStopThread = true;

    // 等待所有异步任务完成
    for (auto &fut : futures)
    {
        fut.wait();
    }

    // 释放动态分配的资源
    delete ptr_wsWork;
}

void TaskWebSocket::doSomething(const std::string &path)
{
    std::cout << "TaskWebSocket Path: " << path << std::endl;
}

void TaskWebSocket::handlePath(const std::string &path)
{
    std::cout << "[TaskWebSocket] Handling path: " << path << std::endl;

    while (!isStopThread && !stopFlags[path])
    {
        // 处理任务
        ptr_wsWork->handlePath(path);

        // 模拟数据推送，每秒推送一次
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "[TaskWebSocket] Data pushed for path: " << path << std::endl;
    }
}

void TaskWebSocket::stopPath(const std::string &path)
{
    std::lock_guard<std::mutex> lock(pathMutex);
    if (stopFlags.find(path) != stopFlags.end())
    {
        stopFlags[path] = true;
    }
}