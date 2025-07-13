#pragma once

#include "update_cancelled.hpp"

namespace updater {
    void update();
    void run(const std::filesystem::path &base);
}
