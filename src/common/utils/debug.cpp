#include "debug.hpp"

#include <fstream>
#include <mutex>
#include <ctime>
#include <iomanip>

namespace utils::debug {
    static std::string logFilePath = "debug.log";
    static std::mutex logMutex;

    static std::string timestamp() {
        const auto now = std::time(nullptr);
        std::tm timeInfo{};
        localtime_s(&timeInfo, &now);
        std::ostringstream oss;
        oss << "[" << std::put_time(&timeInfo, "%Y-%m-%d %H:%M:%S") << "] ";
        return oss.str();
    }

    bool log(const std::string &msg) {
        std::lock_guard lock(logMutex);
        try {
            std::ofstream logFile(logFilePath, std::ios::app);
            if (!logFile.is_open())
                return false;

            logFile << timestamp() << msg << std::endl;
            return true;
        } catch (...) {
            return false;
        }
    }

    bool log(const char *msg) {
        return log(std::string(msg));
    }

    bool log(const int value) {
        return log(std::to_string(value));
    }

    bool log(const double value) {
        return log(std::to_string(value));
    }

    bool log(const std::ostringstream &stream) {
        return log(stream.str());
    }

    void flush() {
        std::lock_guard lock(logMutex);
        if (std::ofstream logFile(logFilePath, std::ios::app); logFile.is_open())
            logFile.flush();
    }

    void set_log_file(const std::string &path) {
        std::lock_guard lock(logMutex);
        logFilePath = path;
    }
}
