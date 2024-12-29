#include "TaskWebSocket.h"
#include <iostream>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

// filepath: /home/hjx/test/c++/2024-12/test/src/task/TaskWebSocket.cpp

TaskWebSocket::TaskWebSocket(std::shared_ptr<WebSocketService> service) : service_(service)
{
    // 初始化 WSWorkLayerImp 实例，用于具体路径处理逻辑
    ptr_wsWork = new WSWorkLayerImp(service_);

    taskinfo.taskname = "WebSocket";
    taskinfo.taskstate = taskState::ready;
    isStopThread = false;
    taskproc = taskProc::task_ready;

    // 添加路径处理器，当 WebSocket 客户端发送路径时触发
    service_->addPathHandler([this](const std::string &path)
                             {
        doSomething(path);
        {
            std::lock_guard<std::mutex> lock(pathMutex);
            currentPath = path;
            std::cout << "currentPath:" << currentPath << std::endl;
        }
        // {
        //     std::lock_guard<std::mutex> lock(handle_mutex);
        //     // taskinfo.taskstate = taskState::start;
        //     std::cout << "taskinfo.taskstate:" << 0<< std::endl;
        // }
        handle_cv.notify_all();
        std::cout << "[TaskWebSocket] currentPath updated to: " << currentPath << std::endl; });

        // 启动一个新的线程来处理 WebSocket 任务
    ws_thread = std::thread([&]()
                            {
        pid_t tid = syscall(SYS_gettid);
        std::cout << "[TaskWebSocket] websocket task thread is run, tid: " << tid << std::endl;
      
        
        while (true) {
            while (taskinfo.taskstate != taskState::start) {
                std::unique_lock<std::mutex> lock(handle_mutex);
                
                std::cout << "[TaskWebSocket] work wait "<< std::endl;
                if (!isStopThread) {
                    handle_cv.wait(lock);
                } 
                std::cout << "isStopThread:" << isStopThread << std::endl;
                if (isStopThread) {
                    return;
                }
                std::cout << "[TaskWebSocket] work start" << std::endl;
               
                taskinfo.taskstate = taskState::start;
                taskproc = taskProc::task_running;
            }
            
            // 处理任务
            ptr_wsWork->handlePath(currentPath);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        } });
}

TaskWebSocket::~TaskWebSocket()
{
    std::cout << "[TaskWebSocket] Destructor called, resources cleaned up." << std::endl;
    // 停止任务线程
    isStopThread = true;
    handle_cv.notify_all(); // 唤醒等待的线程

    // 等待任务线程结束
    if (ws_thread.joinable())
    {
        ws_thread.join();
    }

    // 等待 WebSocket 服务端线程结束
    if (server_thread.joinable())
    {
        server_thread.join();
    }

    // 释放动态分配的资源
    delete ptr_wsWork;
}

void TaskWebSocket::doSomething(const std::string &path)
{
    std::cout << "TaskWebSocket Path: " << path << std::endl;
}
