#include "WebSocketService.h"

WebSocketService::WebSocketService()
{
    std::cout << "Initializing WebSocket server..." << std::endl;
    // 创建条件变量和互斥量
    std::condition_variable cv;
    std::mutex cv_m;
    bool ready = false;
    m_server.init_asio();
    m_server.set_reuse_addr(true);

    m_server.set_open_handler([this](connection_hdl hdl)
                              {
        auto con = m_server.get_con_from_hdl(hdl);
        std::string currentPath = con->get_resource(); // 获取路径
        std::cout << "New connection on path: " << currentPath << std::endl;

        {
            std::lock_guard<std::mutex> lock(m_connectionsMutex);
            m_connections[currentPath].insert(hdl);
        }

        for (const auto &handler : pathHandlers)
        {
            handler(currentPath);
        }
        std::cout << "Current connection count: " << getConnectionCount() << std::endl; });

    m_server.set_close_handler([this](connection_hdl hdl)
                               {
        auto con = m_server.get_con_from_hdl(hdl);
        std::string path = con->get_resource();

        {
            std::lock_guard<std::mutex> lock(m_connectionsMutex);
            m_connections[path].erase(hdl);
            if (m_connections[path].empty())
            {
                m_connections.erase(path);
            }
        }
        std::cout << "Connection closed on path: " << path << std::endl;
        std::cout << "Current connection count: " << getConnectionCount() << std::endl; });

    m_server.set_message_handler([this](connection_hdl hdl, server::message_ptr msg)
                                 {
        auto con = m_server.get_con_from_hdl(hdl);
        std::string path = con->get_resource();
        std::string payload = msg->get_payload();  // 获取消息内容
        std::cout << "Received message on path " << path << ": " << payload << std::endl; });

    m_server.set_fail_handler([this](connection_hdl hdl)
                              {
        auto con = m_server.get_con_from_hdl(hdl);
        std::string path = con->get_resource();
        std::cout << "Connection failed on path: " << path << std::endl; });

    m_server.set_interrupt_handler([this](connection_hdl hdl)
                                   {
        auto con = m_server.get_con_from_hdl(hdl);
        std::string path = con->get_resource();
        std::cout << "Connection interrupted on path: " << path << std::endl; });
    std::cout << "WebSocket server initialized." << std::endl;
    std::cout << "Listening on port 9002..." << std::endl;
    m_server.listen(9002);
    std::cout << "Start accepting connections..." << std::endl;
    m_server.start_accept();

    // 在构造函数中创建线程
    server_thread = std::thread([&]()
                                {
        pid_t tid = syscall(SYS_gettid);
        std::cout << "[WebSocketService] WebSocket service thread is run, tid: " << tid << std::endl;
        {
            std::lock_guard<std::mutex> lk(cv_m);
            ready = true;
        }
        cv.notify_one();

        while (true)
        {
            
            std::cout << "[WebSocketService] echo_server run!" << std::endl;
            m_server.run();
        } });

    {
        std::unique_lock<std::mutex> lk(cv_m);
        cv.wait(lk, [&]
                { return ready; });
    }
}

void WebSocketService::addPathHandler(std::function<void(const std::string &)> handler)
{
    pathHandlers.push_back(handler);
}
WebSocketService::~WebSocketService()
{
    m_server.stop();
}
void WebSocketService::send(const std::string &path, const std::string &message)
{
    auto it = m_connections.find(path);
    if (it != m_connections.end())
    {
        for (const auto &hdl : it->second)
        {
            try
            {
                m_server.send(hdl, message, websocketpp::frame::opcode::text);
                std::cout << "Sent message to path " << path << std::endl;
            }
            catch (const websocketpp::exception &e)
            {
                std::cerr << "Error sending message to path " << path << ": " << e.what() << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "No connections found for path " << path << std::endl;
    }
}
size_t WebSocketService::getConnectionCount() const
{
    std::lock_guard<std::mutex> lock(m_connectionsMutex);
    size_t count = 0;
    for (const auto &pair : m_connections)
    {
        count += pair.second.size();
    }
    return count;
}
/* std::string WebSocketService::getCurrentPath() const
{
    return currentPath;
} */
