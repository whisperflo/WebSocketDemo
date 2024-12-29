#include "WebSocketService.h"
#include "TaskWebSocket.h"
#include <memory>

int main()
{
    // 创建 WebSocketService 的指针对象
    std::shared_ptr<WebSocketService> wsService = std::make_shared<WebSocketService>();

    // 创建 TaskWebSocket 对象，传入 WebSocketService 指针
    std::shared_ptr<TaskWebSocket> taskWs = std::make_shared<TaskWebSocket>(wsService);
    while (true)
    {
        // 避免阻塞
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    return 0;
}