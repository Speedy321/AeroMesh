#include "logging.h"

int Logger::level = LoggingLevel::DEBUG;

void Logger::set_level(LoggingLevel log_level){
    level = log_level;

    String msg = "Log level changed to {";
    msg += level;
    msg += "}";
    info(msg);
}

void Logger::debug(const String msg){
    if (level <= DEBUG){
        Serial.printf("[DEBUG]  : %s\n", msg.c_str());
    }
}

void Logger::info(const String msg){
    if (level <= INFO){
        Serial.printf("[INFO]   : %s\n", msg.c_str());
    }
}

void Logger::warn(const String msg){
    if (level <= WARN){
        Serial.printf("[WARNING]: %s\n", msg.c_str());
    }
}

void Logger::error(const String msg){
    if (level <= ERROR){
        Serial.printf("[ERROR]  : %s\n", msg.c_str());
    }
}