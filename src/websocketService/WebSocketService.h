#ifndef WEBSOCKETSERVICE_H
#define WEBSOCKETSERVICE_H

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <unordered_map>
#include <set>
#include <thread>
#include <chrono>
#include <functional>
#include <vector>
#include <sys/syscall.h>
#include <unistd.h>

typedef websocketpp::server<websocketpp::config::asio> server;
typedef websocketpp::connection_hdl connection_hdl;

class WebSocketService
{
public:
    WebSocketService();
    ~WebSocketService();
    // std::string getCurrentPath() const;

    void addPathHandler(std::function<void(const std::string &)> handler);
    void send(const std::string &path, const std::string &message);

private:
    void sendPcsData(connection_hdl hdl);
    void sendBmsData(connection_hdl hdl);
    size_t getConnectionCount() const;

private:
    std::unordered_map<std::string, std::set<connection_hdl, std::owner_less<connection_hdl>>> m_connections;
    server m_server;
    std::thread server_thread;
    // std::string currentPath;
    mutable std::mutex m_connectionsMutex;
    std::vector<std::function<void(const std::string &)>> pathHandlers;
};

#endif // WEBSOCKETSERVICE_H