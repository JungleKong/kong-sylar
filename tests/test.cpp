#include "../src/log/log.hpp"
#include <iostream>

int main(int argc, char** argv) {
    kong::Logger::ptr logger(new kong::Logger);
    logger->addAppender(kong::LogAppender::ptr(new kong::StdoutLogAppender));

    kong::FileLogAppender::ptr file_appender(new kong::FileLogAppender("./log.txt"));
    kong::LogFormatter::ptr fmt(new kong::LogFormatter("%d%T%p%T%m%n"));
    file_appender->setFormatter(fmt);
    file_appender->setLevel(kong::LogLevel::ERROR);

    logger->addAppender(file_appender);

    // kong::LogEvent::ptr event(new kong::LogEvent(__FILE__, __LINE__, 0, 0, 0, time(0)));

    // event->getSS() << "hello sylar log";
    // logger->log(kong::LogLevel::DEBUG, event);
    std::cout << "hello sylar log" << std::endl;

   KONG_LOG_INFO(logger) << "tests macro";
//    KONG_LOG_ERROR(logger) << "tests macro error";

//    KONG_LOG_FMT_ERROR(logger, "tests macro fmt error %s", "aa");

//    auto l = kong::LoggerMgr::GetInstance()->getLogger("xx");
//    KONG_LOG_INFO(l) << "xxx";
    return 0;
}
