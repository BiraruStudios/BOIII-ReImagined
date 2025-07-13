#include "flags.hpp"
#include "string.hpp"
#include "nt.hpp"

#include <shellapi.h>
#include <unordered_set>

#include "finally.hpp"

namespace utils::flags {
    std::unordered_set<std::string> parse_flags() {
        int num_args{};
        auto *const argv = CommandLineToArgvW(GetCommandLineW(), &num_args);
        const auto _ = finally([&argv] {
            if (argv) {
                LocalFree(argv);
            }
        });

        std::unordered_set<std::string> flags{};

        for (int i = 0; i < num_args && argv; ++i) {
            std::wstring wide_flag(argv[i]);

            if (wide_flag.rfind(L"--", 0) == 0) {
                wide_flag.erase(0, 2);
            } else if (wide_flag.rfind(L"-", 0) == 0) {
                wide_flag.erase(0, 1);
            } else {
                continue;
            }

            flags.emplace(string::to_lower(string::convert(wide_flag)));
        }

        return flags;
    }

    bool has_flag(const std::string &flag) {
        static const auto enabled_flags = parse_flags();
        return enabled_flags.contains(string::to_lower(flag));
    }
}
