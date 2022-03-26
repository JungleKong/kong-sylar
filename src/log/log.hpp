#pragma once

#include <string>
#include <stdint.h>
#include <memory>
#include <sstream>
#include <fstream>
#include <vector>
#include <list> 
#include <map>
#include <iostream>

#include "utils/util.hpp"

#include "utils/singleton.hpp"

// 使用流的方式输出日志
#define KONG_LOG_LEVEL(logger, level) \
    if(logger->getLevel() <= level) \
        kong::LogEventWrap(kong::LogEvent::ptr(new kong::LogEvent(logger, level, \
        __FILE__, __LINE__, 0, kong::GetThreadId(), kong::GetFiberId(), \
        time(0), "kong::Thread::GetName()"))).getSS()

#define KONG_LOG_DEBUG(logger) KONG_LOG_LEVEL(logger, kong::LogLevel::DEBUG)
#define KONG_LOG_INFO(logger) KONG_LOG_LEVEL(logger, kong::LogLevel::INFO)
#define KONG_LOG_WARN(logger) KONG_LOG_LEVEL(logger, kong::LogLevel::WARN)
#define KONG_LOG_ERROR(logger) KONG_LOG_LEVEL(logger, kong::LogLevel::ERROR)
#define KONG_LOG_FATAL(logger) KONG_LOG_LEVEL(logger, kong::LogLevel::FATAL)

// 使用格式化字符串的方式输出日志
#define KONG_LOG_FMT_LEVEL(logger, level, fmt, ...) \
    if(logger->getLevel() <= level) \
        kong::LogEventWrap(kong::LogEvent::ptr(new kong::LogEvent(logger, level, \
        __FILE__, __LINE__, 0, kong::GetThreadId(), kong::GetFiberId(), \
        time(0), "kong::Thread::GetName()"))).getEvent()->format(fmt, __VA_ARGS__)

#define KONG_LOG_FMT_DEBUG(logger, fmt, ...) KONG_LOG_FMT_LEVEL(logger, kong::LogLevel::DEBUG, fmt, __VA_ARGS__)
#define KONG_LOG_FMT_INFO(logger, fmt, ...) KONG_LOG_FMT_LEVEL(logger, kong::LogLevel::INFO, fmt, __VA_ARGS__)
#define KONG_LOG_FMT_WARN(logger, fmt, ...) KONG_LOG_FMT_LEVEL(logger, kong::LogLevel::WARN, fmt, __VA_ARGS__)
#define KONG_LOG_FMT_ERROR(logger, fmt, ...) KONG_LOG_FMT_LEVEL(logger, kong::LogLevel::ERROR, fmt, __VA_ARGS__)
#define KONG_LOG_FMT_FATAL(logger, fmt, ...) KONG_LOG_FMT_LEVEL(logger, kong::LogLevel::FATAL, fmt, __VA_ARGS__)


namespace kong {

class Logger;
class LoggerManeger;

// 定义日志级别
class LogLevel {
public:
    enum Level {
        UNKNOW = 0,
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5,
    };

    static const char* ToString(LogLevel::Level level);

    static LogLevel::Level FromString(const std::string& str);
};
    


// 定义日志事件
class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level, 
        const char* file, int32_t line, uint32_t elapse, 
        uint32_t threadId, uint32_t fiberId, uint64_t time, 
        const std::string& threadName);

    const char* getFile() const {return m_file; }
    size_t getLine() const {return m_line; }
    uint32_t getThreadId() const {return m_threadId; }
    const std::string& getThreadName() const {return m_threadName; }
    uint32_t getFiberId() const {return m_fiberId; }
    uint64_t getTime() const {return m_time; }
    uint32_t getElapse() const {return m_elapse; }
    std::string getContent() const {return m_ss.str(); }
    std::stringstream& getSS() {return m_ss; }
    LogLevel::Level getLevel() const {return m_level; }   
    std::shared_ptr<Logger> getLogger() const {return m_logger; }

    /**
     * @brief 格式化写入日志内容
     * 
     * @param fmt 
     * @param ... 
     */
    void format(const char* fmt, ...);
    void format(const char* fmt, va_list al);
private:
    /// 文件名
    const char* m_file = nullptr;
    /// 行号
    size_t m_line = 0;
    /// 线程id
    uint32_t m_threadId = 0;
    /// 线程名
    std::string m_threadName;
    /// 协程id
    uint32_t m_fiberId = 0;
    /// 时间戳
    uint64_t m_time = 0;
    /// 程序启动到现在的秒数
    uint32_t m_elapse = 0;
    /// 日志内容流
    std::stringstream m_ss;
    /// 日志级别
    LogLevel::Level m_level;
    /// 日志器
    std::shared_ptr<Logger> m_logger; 
};

/**
 * @brief 日志事件包装器
 */
class LogEventWrap {
public:

    /**
     * @brief 构造函数
     * @param[in] e 日志事件
     */
    LogEventWrap(LogEvent::ptr e);

    /**
     * @brief 析构函数
     */
    ~LogEventWrap();

    /**
     * @brief 获取日志事件
     */
    LogEvent::ptr getEvent() const { return m_event;}

    /**
     * @brief 获取日志内容流
     */
    std::stringstream& getSS();
private:
    /**
     * @brief 日志事件
     */
    LogEvent::ptr m_event;
};


// 定义日志格式
class LogFormatter {
public:
    typedef std::shared_ptr<LogFormatter> ptr;
    
    /**
     * @brief Construct a new Log Formatter object
     * 
     * @param pattern 格式模板
     * @details 
     * %m 消息
     * %p 日志级别
     * %r 累计毫秒数
     * %c 日志名称
     * %t 线程id
     * %n 换行
     * %d 时间
     * %f 文件名
     * %l 行号
     * %T 制表符
     * %F 协程id
     * %N 线程名称
     * 默认格式 "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
     */
    explicit LogFormatter(const std::string& pattern);

    /**
     * @brief 返回格式化后的日志文本
     * 
     * @param logger 日志器
     * @param level 日志级别
     * @param event 日志事件
     * @return std::string 
     */
    std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
    std::ostream& format(std::ostream& ofs, std::shared_ptr<Logger>  logger, LogLevel::Level level, LogEvent::ptr event);

    class FormatItem {
    public:
        typedef std::shared_ptr<FormatItem> ptr;

        virtual ~FormatItem()= default;
        virtual void format(std::ostream& os, std::shared_ptr<Logger>  logger, LogLevel::Level level, LogEvent::ptr event) = 0;
    };

    /// 初始化解析日志模板
    void init();
    bool isError() const {return m_error; }
    const std::string getPattern() const {return m_pattern; }
private:
    // 日志模板
    std::string m_pattern;
    // 日志解析后的格式
    std::vector<FormatItem::ptr> m_items;
    // 是否有错误
    bool m_error = false;

};

class LogAppender {
public:
    typedef std::shared_ptr<LogAppender> ptr;

    virtual ~LogAppender()= default;;

    virtual void log(std::shared_ptr<Logger>  logger, LogLevel::Level level, LogEvent::ptr event) = 0;
    void setFormatter(LogFormatter::ptr formatter) {m_formatter = formatter; }
    LogFormatter::ptr getFormatter() const {return m_formatter; }

    LogLevel::Level getLevel() const {return m_level; }
    void setLevel(LogLevel::Level level) {m_level = level; }
protected:
    LogLevel::Level m_level;
    LogFormatter::ptr m_formatter;
};


// 日志器
class Logger: public std::enable_shared_from_this<Logger> {
friend class LoggerManager;
public:
    typedef std::shared_ptr<Logger> ptr;
    Logger();
    explicit Logger(const std::string& name);
    void log(LogLevel::Level level, LogEvent::ptr event);
    void debug(LogEvent::ptr event); 
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);
    void addAppender(LogAppender::ptr appender);
    bool delAppender(LogAppender::ptr appender);
    void clearAppenders();
    const std::string& getName() const {return m_name; }
    LogLevel::Level getLevel() const {return m_level; };

private:
    std::string m_name;
    LogLevel::Level m_level;
    LogFormatter::ptr m_formatter;
    std::list<LogAppender::ptr> m_appenders;
    Logger::ptr m_root;

};

/// 输出到控制台的appender
class StdoutLogAppender: public LogAppender {
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    
    void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event)  override;
private:
};

/// 输出到文件的appender
class FileLogAppender: public LogAppender {
public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    
    explicit FileLogAppender(const std::string& filename);
    void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;
    
    /// 重新打开日志文件,若之前是打开的,则关闭后再打开,成功返回true
    bool reopen();
private:
    std::string m_filename;
    std::ofstream m_filestream;
};

class LoggerManager {
public:
    LoggerManager();

    Logger::ptr getLogger(const std::string& name);

    void init();

    Logger::ptr getRoot() const {return m_root; }

private:
    std::map<std::string, Logger::ptr> m_loggers;

    Logger::ptr m_root;
};


/// 日志器管理类单例模式
typedef kong::Singleton<LoggerManager> LoggerMgr;

}


