#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include <discord_rpc.h>

#include "scheduler.hpp"

namespace discord
{
	namespace
	{
		void ready(const DiscordUser* request)
		{
			SetEnvironmentVariableA("discord_user", request->userId);

			printf("Discord: Ready: %s - %s\n", request->userId, request->username);

			DiscordRichPresence discord_presence{};
			ZeroMemory(&discord_presence, sizeof(discord_presence));

			discord_presence.instance = 1;
			discord_presence.state = "BOIII ReImagined";

			discord_presence.partySize = 0;
			discord_presence.partyMax = 0;
			discord_presence.startTimestamp = 0;

			Discord_UpdatePresence(&discord_presence);
		}

		void errored(const int error_code, const char* message)
		{
			printf("Discord: Error (%i): %s\n", error_code, message);
		}
	}

	class component final : public client_component
	{
	public:
		void post_load() override
		{
			DiscordEventHandlers handlers;
			ZeroMemory(&handlers, sizeof(handlers));
			handlers.ready = ready;
			handlers.errored = errored;
			handlers.disconnected = errored;
			handlers.joinGame = nullptr;
			handlers.spectateGame = nullptr;
			handlers.joinRequest = nullptr;

			Discord_Initialize("1242737353177563187", &handlers, 1, nullptr);

			this->initialized_ = true;

			scheduler::loop(Discord_RunCallbacks, scheduler::pipeline::async, 1s);
		}

		void pre_destroy() override
		{
			if (this->initialized_)
			{
				Discord_Shutdown();
			}
		}

	private:
		bool initialized_ = false;
	};
}

#ifndef DEV_BUILD
REGISTER_COMPONENT(discord::component)
#endif
