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
#include "DataStructure.h"
class TaskWebSocket
{
public:
    TaskWebSocket(std::shared_ptr<WebSocketService> service);
    ~TaskWebSocket();

private:
    void doSomething(const std::string &path);

private:
    std::shared_ptr<WebSocketService> service_;
    std::thread ws_thread;
    std::thread server_thread;
    std::string currentPath;
    bool isStopThread;
    WSWorkLayerImp *ptr_wsWork;
    std::mutex pathMutex; // 保护 currentPath 的互斥锁
    std::mutex handle_mutex;
    std::condition_variable handle_cv;
    taskInfo taskinfo;
    taskProc taskproc;
};

#endif // TASKWEBSOCKET_H