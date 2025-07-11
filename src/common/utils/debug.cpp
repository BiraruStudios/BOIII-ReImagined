#include "debug.hpp"

#include <fstream>
#include <mutex>
#include <ctime>
#include <iomanip>
#include <filesystem>

#include "nt.hpp"
#include "properties.hpp"

namespace utils::debug {
    static std::string get_log_file_path() {
        const auto preferred_dir = properties::get_boiii_path();
        const auto preferred_log = preferred_dir / "debug.log";
        const auto fallback_log = "debug.log";

        std::error_code ec;
        if (std::filesystem::exists(preferred_dir, ec) && !ec) {
            return preferred_log.string();
        }

        return fallback_log;
    }

    static std::string logFilePath = get_log_file_path();
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
}
