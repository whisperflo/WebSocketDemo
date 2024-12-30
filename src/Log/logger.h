#ifndef LOGGER_H
#define LOGGER_H

#include <log4cpp/Category.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <stdexcept>
#include <string>

class Logger
{
public:
    // 提供一个静态方法，用于获取唯一的 Logger 实例
    static Logger &getInstance(const std::string &logDir)
    {
        static Logger instance(logDir); // 静态实例，生命周期为程序的整个运行期
        return instance;
    }

    // 禁用拷贝构造函数和赋值操作符，确保实例唯一
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    void info(const std::string &message);
    void warn(const std::string &message);
    void debug(const std::string &message);

private:
    log4cpp::Category &root;

    // 私有构造函数，防止外部实例化
    Logger(const std::string &logDir);
    ~Logger();
};

#endif // LOGGER_H
