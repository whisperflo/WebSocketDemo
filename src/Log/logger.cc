#include "logger.h"
#include <sys/stat.h>
#include <iostream>

// 检查并创建日志目录
bool checkAndCreateLogDir(const std::string &dir)
{
    struct stat info;
    if (stat(dir.c_str(), &info) != 0)
    {
        // 目录不存在，创建目录
        if (mkdir(dir.c_str(), 0777) != 0)
        {
            return false;
        }
    }
    else if (!(info.st_mode & S_IFDIR))
    {
        return false; // 存在但不是目录
    }
    return true;
}

// 私有构造函数，只允许通过 getInstance() 方法创建实例
Logger::Logger(const std::string &logDir) : root(log4cpp::Category::getRoot())
{
    if (!checkAndCreateLogDir(logDir))
    {
        std::cout << "Failed to create or access log directory: " << logDir << std::endl;
        throw std::runtime_error("Log directory error");
    }
    std::cout << "logDir: " << logDir << std::endl;
    log4cpp::PatternLayout *fileLayout = new log4cpp::PatternLayout();
    fileLayout->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S}] [%p] %m%n");

    log4cpp::Appender *fileAppender = new log4cpp::FileAppender("file", logDir + "/program.log");
    fileAppender->setLayout(fileLayout);

    root.setPriority(log4cpp::Priority::DEBUG);
    root.addAppender(fileAppender); // 确保只添加一次
}

Logger::~Logger()
{
    log4cpp::Category::shutdown();
}

void Logger::info(const std::string &message)
{
    root.info(message);
}

void Logger::warn(const std::string &message)
{
    root.warn(message);
}

void Logger::debug(const std::string &message)
{
    root.debug(message);
}
