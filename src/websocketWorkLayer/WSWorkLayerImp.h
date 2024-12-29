#ifndef WSWORKLAYERIMP_H
#define WSWORKLAYERIMP_H

#include "WebSocketService.h"
#include "DataStructure.h"
#include <memory>
#include <string> // 添加此行以确保std::string被正确识别

class WSWorkLayerImp
{
public:
    WSWorkLayerImp(std::shared_ptr<WebSocketService> service);

    // void processData();
    void processData(const std::string &path);
    PathType getPathType(const std::string &path);
    void handlePath(const std::string &path);
    void assemblePcsOverViewData(std::vector<PCS> group1, std::vector<PCS> group2, std::ostringstream &payloadStream);
    void sendPcsOverViewData(std::ostringstream &payloadStream);

private:
    std::shared_ptr<WebSocketService> service_;
};

#endif // WSWORKLAYERIMP_H