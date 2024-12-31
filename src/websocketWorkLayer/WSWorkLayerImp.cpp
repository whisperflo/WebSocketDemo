#include "WSWorkLayerImp.h"
#include <iostream>

WSWorkLayerImp::WSWorkLayerImp(std::shared_ptr<WebSocketService> service) : service_(service)
{
    service_->addPathHandler([this](const std::string &path)
                             {
                                //  processData();

                                 processData(path); });
}

/* void WSWorkLayerImp::processData()
{
    std::cout << "WSWorkLayerImp Processing the pointer is obtained by calling the member function get.: " << service_->getCurrentPath() << std::endl;
} */

void WSWorkLayerImp::processData(const std::string &path)
{
    std::cout << "WSWorkLayerImp Processing Path: " << path << std::endl;
}

// 将字符串转换为枚举类型
PathType WSWorkLayerImp::getPathType(const std::string &path)
{
    if (path == "/pcsData")
    {
        return PathType::PCSDATA;
    }
    else if (path == "/bmsData")
    {
        return PathType::BMSDATA;
    }
    else if (path == "/pcsStatus")
    {
        return PathType::PCSSTATUS;
    }
    else if (path == "/bmsStatus")
    {
        return PathType::BMSSTATUS;
    }
    else
    {
        return PathType::UNKNOWN;
    }
}
// 根据路径类型处理对应的功能
void WSWorkLayerImp::handlePath(const std::string &path)
{
    std::ostringstream payloadStream;
    std::string str;

    PathType pathType = getPathType(path); // 获取路径类型
    switch (pathType)
    {
    case PCSDATA:
        // 处理 /pcsData 路径的功能
        sendPcsOverViewData(payloadStream);
        str = payloadStream.str();
        service_->send(path, str);
        break;

    case BMSDATA:
        // 处理 /bmsData 路径的功能
        sendBmsOverViewData(payloadStream);
        str = payloadStream.str();
        service_->send(path, str);
        break;

    case PCSSTATUS:
        // 处理 /pcsStatus 路径的功能

        break;

    case BMSSTATUS:
        // 处理 /bmsStatus

        break;

    default:
        // 如果没有匹配的路径

        break;
    }
    // path.clear(); // 清空path
}

void WSWorkLayerImp::sendPcsOverViewData(std::ostringstream &payloadStream)
{

    // 模拟24台pcs的数据
    // 模拟第一组数据
    std::vector<PCS> group1 = {
        {"PCS 1", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"PCS 2", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"PCS 3", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"PCS 4", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"PCS 5", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"PCS 6", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"PCS 7", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"PCS 8", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"PCS 9", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"PCS 10", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"PCS 11", "errow", {{"0", "kW"}, {"0", "kW"}, {"Shutdown", ""}, {"Grid loss", ""}}},
        {"PCS 12", "errow", {{"0", "kW"}, {"0", "kW"}, {"Shutdown", ""}, {"Grid loss", ""}}}};
    // 模拟第二组设备
    std::vector<PCS> group2 = {
        {"PCS 13", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"PCS 14", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"PCS 15", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"PCS 16", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"PCS 17", "errow", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"PCS 18", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"PCS 19", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"PCS 20", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"PCS 21", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"PCS 22", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"PCS 23", "errow", {{"0", "kW"}, {"0", "kW"}, {"Shutdown", ""}, {"Grid loss", ""}}},
        {"PCS 24", "errow", {{"0", "kW"}, {"0", "kW"}, {"Shutdown", ""}, {"Grid loss", ""}}}};
    assemblePcsOverViewData(group1, group2, payloadStream);
}

void WSWorkLayerImp::assemblePcsOverViewData(std::vector<PCS> group1, std::vector<PCS> group2, std::ostringstream &payloadStream)
{

    // 创建一个JSON对象，用来存储最终的数据
    nlohmann::json pcsData;

    // 用来转换每个PCS的详细数据为JSON格式
    auto createPcsJson = [](const PCS &pcs)
    {
        nlohmann::json pcsJson;
        pcsJson["name"] = pcs.name;
        pcsJson["status"] = pcs.status;

        // 将details数据填充到PCS对象中
        nlohmann::json detailsJson;
        for (const auto &detail : pcs.details)
        {
            nlohmann::json detailJson;
            detailJson["value"] = detail.value;
            detailJson["unit"] = detail.unit;
            detailsJson.push_back(detailJson);
        }
        pcsJson["details"] = detailsJson;
        return pcsJson;
    };

    // 将group1和group2中的PCS数据分别转化为JSON格式
    nlohmann::json group1Json = nlohmann::json::array();
    for (const auto &pcs : group1)
    {
        group1Json.push_back(createPcsJson(pcs));
    }

    nlohmann::json group2Json = nlohmann::json::array();
    for (const auto &pcs : group2)
    {
        group2Json.push_back(createPcsJson(pcs));
    }

    // 将组装好的数据放入最终的JSON对象
    pcsData.push_back(group1Json);
    pcsData.push_back(group2Json);

    // 将JSON对象转化为字符串，并写入到payloadStream
    payloadStream << pcsData.dump(4); // 使用4个空格缩进格式化JSON
}

void WSWorkLayerImp::sendBmsOverViewData(std::ostringstream &payloadStream)
{

    // 模拟24台pcs的数据
    // 模拟第一组数据
    std::vector<PCS> group1 = {
        {"BMS 1", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"BMS 2", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"BMS 3", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"BMS 4", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"BMS 5", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"BMS 6", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"BMS 7", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"BMS 8", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"BMS 9", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"BMS 10", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"BMS 11", "errow", {{"0", "kW"}, {"0", "kW"}, {"Shutdown", ""}, {"Grid loss", ""}}},
        {"BMS 12", "errow", {{"0", "kW"}, {"0", "kW"}, {"Shutdown", ""}, {"Grid loss", ""}}}};
    // 模拟第二组设备
    std::vector<PCS> group2 = {
        {"BMS 13", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"BMS 14", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"BMS 15", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"BMS 16", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"BMS 17", "errow", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"BMS 18", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"BMS 19", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"BMS 20", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"BMS 21", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"BMS 22", "", {{"215.1", "kW"}, {"215.1", "kW"}, {"Run", ""}, {"No", ""}}},
        {"BMS 23", "errow", {{"0", "kW"}, {"0", "kW"}, {"Shutdown", ""}, {"Grid loss", ""}}},
        {"BMS 24", "errow", {{"0", "kW"}, {"0", "kW"}, {"Shutdown", ""}, {"Grid loss", ""}}}};
    assemblePcsOverViewData(group1, group2, payloadStream);
}