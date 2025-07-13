#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "game/game.hpp"
#include "utils/concurrency.hpp"
#include "version.hpp"

#include "command.hpp"
#include "scheduler.hpp"

#include <utils/hook.hpp>

#include "utils/properties.hpp"
#include "utils/string.hpp"

namespace branding {
    namespace {
        utils::concurrency::container<std::string> branding_name;
        utils::concurrency::container<std::string> console_prefix;

        void setup_branding() {
            const std::string loaded_branding = utils::properties::load("branding").value_or("^1BOIII ReImagined^7");

            branding_name.access([&](std::string &value) {
                value = loaded_branding;
            });
        }

        const char *get_branding() {
            const std::string loaded_branding = branding_name.copy();
            return utils::string::va("%s", loaded_branding.c_str());
        }

        void set_branding(const std::string &value) {
            branding_name.access([&](std::string &name) {
                if (value == name) {
                    return;
                }

                name = value;
                utils::properties::store("branding", value);
            });
        }

        void reset_branding() {
            set_branding("^1BOIII ReImagined^7");
        }

        void setup_console_prefix() {
            const std::string loaded_console_prefix = utils::properties::load("console_prefix").value_or(
                "BOIII ReImagined");

            console_prefix.access([&](std::string &value) {
                value = loaded_console_prefix;
            });
        }

        const char *get_console_prefix() {
            const std::string loaded_console_prefix = console_prefix.copy();
            return utils::string::va("%s > ", loaded_console_prefix.c_str());
        }

        void set_console_prefix(const std::string &value) {
            console_prefix.access([&](std::string &name) {
                if (value == name) {
                    return;
                }

                name = value;
                utils::properties::store("console_prefix", value);
            });
        }

        void reset_console_prefix() {
            set_console_prefix("BOIII ReImagined");
        }
    }

    namespace {
        void draw_branding() {
            constexpr auto x = 4;
            constexpr auto y = 0;
            constexpr auto scale = 0.5f;
            float color[4] = {0.666f, 0.666f, 0.666f, 0.666f};

            const auto *font = reinterpret_cast<uint32_t*(*)()>(0x141CAC8E0_g)();
            if (!font) return;

            game::R_AddCmdDrawText(
                utils::string::va("%s: %s", get_branding(), VERSION),
                std::numeric_limits<int>::max(),
                font,
                static_cast<float>(x),
                y + static_cast<float>(font[2]) * scale,
                scale, scale, 0.0f,
                color,
                game::ITEM_TEXTSTYLE_NORMAL
            );
        }
    }

    struct component final : client_component {
        component() {
            setup_branding();
            setup_console_prefix();
        }

        void post_unpack() override {
            scheduler::loop(draw_branding, scheduler::renderer);

            // Change window title prefix
            utils::hook::copy_string(0x14303F3D8_g, "BOIII ReImagined");

            // Change ingame console prefix
            utils::hook::call(0x141339970_g, get_console_prefix);

            // Register branding command
            command::add("branding", [](const command::params &params) {
                if (params.size() < 2 || params.size() > 3) {
                    printf("Usage: branding <name|console/consolePrefix|reset> [value]\n");
                    return;
                }

                const std::string &subcmd = params[1];
                const std::string &value = params[2];

                if (subcmd == std::string("name")) {
                    set_branding(value);
                } else if (subcmd == std::string("console") || subcmd == std::string("consolePrefix")) {
                    set_console_prefix(value);
                } else if (subcmd == std::string("reset")) {
                    if (value == std::string("name")) {
                        reset_branding();
                    } else if (value == std::string("console") || value == std::string("consolePrefix")) {
                        reset_console_prefix();
                    } else if (value == std::string("all")) {
                        reset_branding();
                        reset_console_prefix();
                    } else {
                        printf("Unknown reset subcommand\n");
                        printf("Usage: branding reset <name|console/consolePrefix|all>\n");
                    }
                } else {
                    printf("Unknown branding subcommand\n");
                }
            });
        }
    };
}

REGISTER_COMPONENT(branding::component)
