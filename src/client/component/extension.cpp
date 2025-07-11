#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include <utils/nt.hpp>
#include <game/game.hpp>

namespace extension
{
	struct component final : generic_component
	{
		component()
		{
			create_config_file_if_not_exists();
			this->extension_ = utils::nt::library::load(game::get_core_path() / "ext.dll");
		}

		~component() override
		{
			this->extension_.free();
		}

		void post_load() override
		{
			this->extension_.invoke<void>("_1");
		}

		void post_unpack() override
		{
			this->extension_.invoke<void>("_2");
		}

		void pre_destroy() override
		{
			this->extension_.invoke<void>("_3");
		}

	private:
		void create_config_file_if_not_exists() const
		{
			TCHAR appDataPath[MAX_PATH];
			DWORD result = GetEnvironmentVariable(TEXT("LOCALAPPDATA"), appDataPath, MAX_PATH);

			if (result > 0 && result < MAX_PATH)
			{
				const std::filesystem::path configFilePath = std::filesystem::path(appDataPath) / "Activision" / "CoD" / "config.ini";

				if (!std::filesystem::exists(configFilePath))
				{
					std::filesystem::create_directories(configFilePath.parent_path());
					std::ofstream configFile(configFilePath);
				}
			}
		}

		utils::nt::library extension_{};
	};
}

REGISTER_COMPONENT(extension::component)