#ifndef LOGGING_H
#define LOGGING_H
#include <Arduino.h>

enum LoggingLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    SILENCE
};

class Logger{
    Logger();
    static int level;
public:
    static void set_level(LoggingLevel log_level);
    static void debug(const String msg);
    static void info(const String msg);
    static void warn(const String msg);
    static void error(const String msg);
};

#endif