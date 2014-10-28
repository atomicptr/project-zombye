#ifndef __ZOMBYE_UTILS_LOGGER_HPP__
#define __ZOMBYE_UTILS_LOGGER_HPP__

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <utility>

namespace zombye {
    enum log_level {
        LOG_INFO,
        LOG_DEBUG,
        LOG_WARNING,
        LOG_ERROR
    };

    std::fstream init_log_system();

    std::string format_time(std::string);

    void log(std::string);
    void log(log_level, std::string);
}

#endif
