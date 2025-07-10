#pragma once

#include <string>
#include <sstream>

namespace utils::debug
{
    bool log(const std::string& msg);
    bool log(const char* msg);
    bool log(int value);
    bool log(double value);
    bool log(const std::ostringstream& stream);
    void flush();
}