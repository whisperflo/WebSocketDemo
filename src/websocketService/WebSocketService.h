#ifndef WEBSOCKETSERVICE_H
#define WEBSOCKETSERVICE_H

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <unordered_map>
#include <set>
#include <thread>
#include <unistd.h>      // 提供pid_t类型定义
#include <sys/syscall.h> // 提供SYS_gettid的定义
#include <functional>
#include <vector>
#include "../Log/logger.h"

typedef websocketpp::server<websocketpp::config::asio> server;
typedef websocketpp::connection_hdl connection_hdl;

class WebSocketService
{
public:
    WebSocketService();
    ~WebSocketService();

    void send(const std::string &path, const std::string &message);
    void addPathHandler(std::function<void(const std::string &)> handler);
    bool getConnectionStatus(const std::string &path) const;
    // size_t getConnectionCount() const;

private:
    std::unordered_map<std::string, std::set<connection_hdl, std::owner_less<connection_hdl>>> m_connections;
    std::unordered_map<std::string, bool> isConnected;
    server m_server;
    mutable std::mutex m_mutex;
    std::thread server_thread;
    mutable std::mutex m_connectionsMutex;
    std::vector<std::function<void(const std::string &)>> pathHandlers;
};

#endif // WEBSOCKETSERVICE_H