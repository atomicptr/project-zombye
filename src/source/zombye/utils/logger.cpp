#include <zombye/utils/logger.hpp>

std::string zombye::format_time(std::string format) {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), format.c_str());

    return ss.str();
}

std::fstream zombye::init_log_system() {
    std::fstream logger;

    auto filename = format_time("zombye-%Y-%m-%d-%X.log");

    logger.open("logs/" + filename, std::fstream::out | std::fstream::app);

    return std::move(logger);
}

void zombye::log(std::string msg) {
    zombye::log(LOG_INFO, msg);
}

void zombye::log(log_level level, std::string msg) {
    static auto logger = init_log_system();

    std::stringstream ss;
    ss << format_time("[%Y-%m-%d %X]");

    switch(level) {
        case LOG_INFO:
            ss << "[INFO]";
            break;
        case LOG_DEBUG:
            ss << "[DEBUG]";
            break;
        case LOG_WARNING:
            ss << "[WARN]";
            break;
        case LOG_ERROR:
            ss << "[ERROR]";
            break;
    }

    ss << " " << msg << std::endl;

    logger << ss.str();
    std::cout << ss.str();
}
