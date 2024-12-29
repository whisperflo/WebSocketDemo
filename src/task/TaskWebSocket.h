#ifndef TASKWEBSOCKET_H
#define TASKWEBSOCKET_H

#include "WebSocketService.h"
#include "WSWorkLayerImp.h"
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <string>
#include <iostream>
#include <future>
#include <sys/syscall.h>
#include <unistd.h>
#include "DataStructure.h"

class TaskWebSocket
{
public:
    TaskWebSocket(std::shared_ptr<WebSocketService> service);
    ~TaskWebSocket();

private:
    void doSomething(const std::string &path);
    void handlePath(const std::string &path);
    void stopPath(const std::string &path);

private:
    std::shared_ptr<WebSocketService> service_;
    // std::thread ws_thread;
    std::string currentPath;
    bool isStopThread;
    WSWorkLayerImp *ptr_wsWork;
    std::mutex pathMutex; // 保护 currentPath 的互斥锁
    std::vector<std::future<void>> futures;
    std::map<std::string, std::atomic<bool>> stopFlags; // 每个路径的停止标志
};

#endif // TASKWEBSOCKET_H