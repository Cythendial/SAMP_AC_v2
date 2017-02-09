#include <queue>
#include "BanHandler.h"
#include "GDK/sampgdk.h"
#include "CAntiCheat.h"
#include "CAntiCheatHandler.h"
#include "CThreadSync.h"
#include <curl/curl.h>
#include "Utility.h"
#include "plugin.h"
#include "CServerUpdater.h"

namespace Thread_BanHandler
{
	bool AddCheater(std::string reason, std::string md5, std::string hwid, std::string name, std::string ip, std::string server_name, int server_port, bool queue);
	void CheckCheater(unsigned int playerid, std::string name, std::string hwid, std::string ip);
}

namespace Queue_BanHandler
{
	struct queued_bans_struct
	{
		std::string name;
		std::string ip;
		std::string reason;
		std::string md5;
		std::string hwid;
		std::string server_name;
		int server_port;
		queued_bans_struct(std::string arg_name, std::string arg_ip, std::string arg_reason,
			std::string arg_md5, std::string arg_hwid,
			std::string arg_server_name, int arg_server_port) : name(arg_name), ip(arg_ip), reason(arg_reason), md5(arg_md5), hwid(arg_hwid), server_name(arg_server_name), server_port(arg_server_port) {}
	};

	std::queue<queued_bans_struct> queued_bans;

	void AddBanToQueue(std::string name, std::string ip, std::string reason, std::string md5, std::string hwid, std::string server_name, int server_port)
	{
		if (hwid.compare(queued_bans.back().hwid) != 0)
		{
			queued_bans.push(queued_bans_struct(name, ip, reason, md5, hwid, server_name, server_port));
		}
	}

	void ReapplyQueuedBan()
	{
		bool success = Thread_BanHandler::AddCheater(queued_bans.front().reason, queued_bans.front().md5, queued_bans.front().hwid, queued_bans.front().name,
			queued_bans.front().ip, queued_bans.front().server_name, queued_bans.front().server_port, false);

		if (success)
		{
			queued_bans.pop();
			if (queued_bans.empty())
			{
				CAntiCheat* ac;

				for (int i = 0; i < MAX_PLAYERS; ++i)
				{
					if (!sampgdk::IsPlayerConnected(i) || sampgdk::IsPlayerNPC(i))
						continue;

					if (CAntiCheatHandler::IsConnected(i))
					{
						ac = CAntiCheatHandler::GetAntiCheat(i);
						if (ac != NULL)
						{
							BanHandler::CheckCheater(i);
						}
					}
				}
			}
		}
	}

	void SAMPGDK_CALL CheckQueuedBans(int timerid, void *params)
	{
		if (!queued_bans.empty())
		{
			boost::thread ReapplyQueuedBan_Thread(&ReapplyQueuedBan);
		}
	}
}

namespace Thread_BanHandler
{
	// Ban functions, call them in a separate thread because they get/send data from/to the web server which takes notable time to finish.

	bool AddCheater(std::string reason, std::string md5, std::string hwid, std::string name, std::string ip, std::string server_name, int server_port, bool queue)
	{
		// Suppose the ban will succeed
		bool ban_status = true;

		// Completely prepared now. Let's send data to the web server (which takes it to database)!
		CURL *curl;

		// curl handle
		curl = curl_easy_init();
		if (curl)
		{
			// Set URL to receive POST data
			curl_easy_setopt(curl, CURLOPT_URL, AC_BAN_HANDLER_ADD);

			/*
			* If you want to connect to a site who isn't using a certificate that is
			* signed by one of the certs in the CA bundle you have, you can skip the
			* verification of the server's certificate. This makes the connection
			* A LOT LESS SECURE.
			*
			* If you have a CA cert for the server stored someplace else than in the
			* default bundle, then the CURLOPT_CAPATH option might come handy for
			* you.
			*/
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

			/*
			* If the site you're connecting to uses a different host name that what
			* they have mentioned in their server certificate's commonName (or
			* subjectAltName) fields, libcurl will refuse to connect. You can skip
			* this check, but this will make the connection less secure.
			*/
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

			// Adds user agent to request
			curl_easy_setopt(curl, CURLOPT_USERAGENT, HTTP_REQUEST_USER_AGENT);

			// Escape some parameters
			char
				*escaped_name = curl_easy_escape(curl, name.c_str(), 0),
				*escaped_server_name = curl_easy_escape(curl, server_name.c_str(), 0),
				*escaped_ban_reason = curl_easy_escape(curl, reason.c_str(), 0);

			if (escaped_name && escaped_server_name && escaped_ban_reason)
			{
				// Format POST data
				char str[400];
				snprintf(str, sizeof str, "Cheater=%s&CheaterIP=%s&Hardware=%s&Reason=%s&MD5=%s&ServerName=%s&Port=%d",
					escaped_name, ip.c_str(), hwid.c_str(), escaped_ban_reason, md5.c_str(), escaped_server_name, server_port);

				// Set POST data
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str);

				// Send the request and store result into "res" variable
				CURLcode res;
				res = curl_easy_perform(curl);


				// Handle possible errors
				if (res != CURLE_OK)
				{
					//Utility::Printf("curl_easy_perform() failed: %s while trying to add player %d to ban list.", curl_easy_strerror(res), playerid);
					ban_status = false;
					if (queue)
					{
						Queue_BanHandler::AddBanToQueue(name, ip, reason, md5, hwid, server_name, server_port);
					}
				}
			}
			else
			{
				//Utility::Printf("curl_easy_escape() failed while trying to add player %d to ban list.", playerid);
				ban_status = false;
				if (queue)
				{
					Queue_BanHandler::AddBanToQueue(name, ip, reason, md5, hwid, server_name, server_port);
				}
			}


			// Clean up
			curl_free(escaped_name);
			curl_free(escaped_server_name);
			curl_free(escaped_ban_reason);
			curl_easy_cleanup(curl);
		}
		else
		{
			//Utility::Printf("failed to initialize curl handle while trying to add player %d to ban list.", playerid);
			ban_status = false;
			if (queue)
			{
				Queue_BanHandler::AddBanToQueue(name, ip, reason, md5, hwid, server_name, server_port);
			}
		}
		return ban_status;
	}

	void CheckCheater(unsigned int playerid, std::string name, std::string hwid, std::string ip)
	{
		// Create this variable which holds info whether this player is a cheater or not.
		bool ischeater = false;

		CURL *curl;
		// curl handle
		curl = curl_easy_init();
		if (curl)
		{
			// Set URL to receive POST data
			curl_easy_setopt(curl, CURLOPT_URL, AC_BAN_HANDLER_CHECK);

			// Host & Peer Verification
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

			// Adds user agent to request
			curl_easy_setopt(curl, CURLOPT_USERAGENT, HTTP_REQUEST_USER_AGENT);

			// Escape name
			char *escaped_name = curl_easy_escape(curl, name.c_str(), 0);

			if (escaped_name)
			{
				// Format POST data.
				char str[100];
				snprintf(str, sizeof str, "Name=%s&Hardware=%s&IP=%s", escaped_name, hwid.c_str(), ip.c_str());

				// Set POST data
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str);

				// Send the request and store result into "res" variable
				CURLcode res;
				res = curl_easy_perform(curl);

				// Handle possible errors
				if (res != CURLE_OK)
				{
					//Utility::Printf("curl_easy_perform() failed: %s while checking if player %d is in ban list.", curl_easy_strerror(res), playerid);
				}
				else // success
				{
					// Now let's handle response codes
					long response_code;
					curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

					switch (response_code)
					{
						case 508:
						{
							// Found in ban list.
							ischeater = true;
							break;
						}
						default:
						{
							// Was not found in ban list.
							ischeater = false;
							break;
						}
					}
				}
			}

			// Clean up
			curl_free(escaped_name);
			curl_easy_cleanup(curl);
		}
		else
		{
			//Utility::Printf("failed to initialize curl handle while trying to add player %d to ban list.", playerid);
		}

		// Return whether this is a cheater or not
		CThreadSync::OnCheaterCheckResponse__parameters *param = new CThreadSync::OnCheaterCheckResponse__parameters;
		param->playerid = playerid;
		param->ischeater = ischeater;
		pMainThreadSync->AddCallbackToQueue(&CThreadSync::OnCheaterCheckResponse, param);
	}
}

namespace BanHandler
{

	void AddCheater(unsigned int playerid, std::string reason, std::string md5)
	{
		// Find a CAntiCheat class associated with this player (this was created in Network::HandleConnection earlier in this function)
		CAntiCheat* ac = CAntiCheatHandler::GetAntiCheat(playerid);
		if (ac == NULL)
		{
			// error?
			//Utility::Printf("failed to add player %d to ban list due to CAntiCheat class error.", playerid);
			return;
		}

		// Get the player's Hardware ID.
		std::string hwid = "";
		hwid = ac->GetPlayerHardwareID();

		// Delcare 2 variables for player's name and IP
		char name[MAX_PLAYER_NAME], ip[16];

		// Get the player's name
		sampgdk::GetPlayerName(playerid, name, sizeof(name));

		// Get the player's IP
		sampgdk::GetPlayerIp(playerid, ip, sizeof ip);

		// Get server info: hostname and port
		char server_name[64];
		sampgdk::GetServerVarAsString("hostname", server_name, sizeof server_name);
		int server_port;
		server_port = sampgdk::GetServerVarAsInt("port");

		boost::thread addCheaterThread(&Thread_BanHandler::AddCheater, reason, md5, hwid, std::string(name), std::string(ip), std::string(server_name), server_port, true);
	}

	void CheckCheater(unsigned int playerid)
	{
		// Find a CAntiCheat class associated with this player (this was created in Network::HandleConnection earlier in this function)
		CAntiCheat* ac = CAntiCheatHandler::GetAntiCheat(playerid);
		if (ac == NULL)
		{
			// error?
			//Utility::Printf("failed while checking if player %d is in ban list due to CAntiCheat class error.", playerid);
			return;
		}

		// Get the player's name
		char name[MAX_PLAYER_NAME];
		sampgdk::GetPlayerName(playerid, name, sizeof name);

		// Get the player's IP
		char ip[16];
		sampgdk::GetPlayerIp(playerid, ip, sizeof ip);

		// Get the player's Hardware ID.
		std::string hwid = "";
		hwid = ac->GetPlayerHardwareID();

		boost::thread checkCheaterThread(&Thread_BanHandler::CheckCheater, playerid, std::string(name), hwid, std::string(ip));
	}

	void StartQueuedBansChecker()
	{
		sampgdk::SetTimer(60000, true, Queue_BanHandler::CheckQueuedBans, NULL);
	}
}
