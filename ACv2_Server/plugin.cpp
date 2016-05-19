#include "plugin.h"
#include "Utility.h"
#include "Network/BitStream.h"
#include "../Shared/Network/CRPC.h"
#include "GDK/sampgdk.h"
#include "GDK/sampgdk.h"
#include "Callback.h"
#include "SDK/samp-sdk/amx/amx.h"
#include "CServerUpdater.h"
#include "CAntiCheatHandler.h"
#include "Hooks.h"

void **PluginData;

cell AMX_NATIVE_CALL IsPlayerUsingSAMPACProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(1, "IsPlayerUsingSampAC");

	// Make sure the player is connected
	if (!IsPlayerConnected(pParams[1])) return 0;

	// Return the result if the player is connected to AC or not.
	return CAntiCheatHandler::IsConnected(pParams[1]);
}

cell AMX_NATIVE_CALL MD5_MemoryProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(3, "MD5_Memory");

	// Make sure the player is connected
	if (!IsPlayerConnected(pParams[1])) return 0;

	// pParams[1] = playerid
	// pParams[2] = address
	// pParams[3] = size

	// Prepare to send the info to the client.
	RakNet::BitStream bitStream;

	// Write header to packet
	bitStream.Write((unsigned char)PACKET_RPC);
	bitStream.Write(MD5_MEMORY_REGION);

	// All cell's need to be casted as an int. 
	bitStream.Write((int)pParams[2]);
	bitStream.Write((int)pParams[3]);

	// Send the RPC to the client and return.
	return Network::PlayerSend(pParams[1], &bitStream);
}

cell AMX_NATIVE_CALL SetPlayerCanEnableACProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(2, "SetPlayerCanEnableAC");

	// Make sure the player is connected
	if (!IsPlayerConnected(pParams[1])) return 0;

	// Toggle if the player can enable AC or not.
	CAntiCheat::ToggleCanEnableAC(pParams[1], !!pParams[2]);

	return 1;
}

cell AMX_NATIVE_CALL CanPlayerEnableACProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(1, "CanPlayerEnableAC");

	// Make sure the player is connected
	if (!IsPlayerConnected(pParams[1])) return 0;

	// Return if the player can enable AC or not.
	return CAntiCheat::CanEnableAC(pParams[1]);
}

cell AMX_NATIVE_CALL IsACEnabledProc(AMX* pAmx, cell* pParams)
{
	// Return if AC is on or not
	return Callback::ACToggle;
}

cell AMX_NATIVE_CALL GetPlayerHardwareIDProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(3, "GetPlayerHardwareID");

	// Get the player's CAntiCheat class pointer.
	CAntiCheat* ac = CAntiCheatHandler::GetAntiCheat(pParams[1]);

	// Make sure the player is connected
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	// Get the address of the variable in the 2nd parameter.
	cell* cVarAddress = NULL;
	amx_GetAddr(pAmx, pParams[2], &cVarAddress);

	// Return the result of setting the 2nd parameter to the hardware ID string.
	return amx_SetString(cVarAddress, ac->GetPlayerHardwareID().c_str(), 0, 0, pParams[3]);
}

cell AMX_NATIVE_CALL TogglePlayerSwitchReloadProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(2, "TogglePlayerSwitchReload");

	// Get CAntiCheat pointer
	CAntiCheat* ac = CAntiCheatHandler::GetAntiCheat(pParams[1]);

	// Make sure the player is connected
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	ac->ToggleSwitchReload(!!pParams[2]);

	return 1;
}

cell AMX_NATIVE_CALL SetPlayerFPSLimitProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(2, "SetPlayerFPSLimit");

	// Get CAntiCheat pointer
	CAntiCheat* ac = CAntiCheatHandler::GetAntiCheat(pParams[1]);

	// Make sure the player is connected and the frame limit isn't 0
	if (!IsPlayerConnected(pParams[1]) || ac == NULL || pParams[2] == 0) return 0;

	// Set our internal recorder of the player's FPS limit.
	ac->SetFPSLimit(pParams[2]);

	// Create the RPC data to send to the client.
	RakNet::BitStream bsData;

	// Write header to packet
	bsData.Write((unsigned char)PACKET_RPC);
	bsData.Write(SET_FRAME_LIMIT);

	// Write new frame limit to packet
	bsData.Write((unsigned short)pParams[2]);

	return Network::PlayerSend(pParams[1], &bsData);
}

cell AMX_NATIVE_CALL GetPlayerFPSLimitProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(1, "GetPlayerFPSLimit");

	// Get CAntiCheat pointer
	CAntiCheat* ac = CAntiCheatHandler::GetAntiCheat(pParams[1]);

	// Make sure the player is connected and connected to AC
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	// return the player's frame limit.
	return ac->GetFPSLimit();
}

cell AMX_NATIVE_CALL SetPlayerCrouchBugProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(2, "SetPlayerCrouchBug");

	// Get CAntiCheat pointer.
	CAntiCheat* ac = CAntiCheatHandler::GetAntiCheat(pParams[1]);

	// Make sure the player is connected 
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	ac->ToggleCrouchBug(pParams[2]);

	return 1;
}

cell AMX_NATIVE_CALL TogglePlayerLiteFootProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(2, "TogglePlayerLiteFoot");

	// Get CAntiCheat pointer
	CAntiCheat* ac = CAntiCheatHandler::GetAntiCheat(pParams[1]);

	// Make sure the player is connected 
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	// Toggle lite foot.
	ac->ToggleLiteFoot(!!pParams[2]);

	return 1;
}

cell AMX_NATIVE_CALL GetPlayerLiteFootProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(1, "GetPlayerLiteFoot");

	// Get CAntiCheat pointer
	CAntiCheat* ac = CAntiCheatHandler::GetAntiCheat(pParams[1]);

	// Make sure the player is connected 
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	return ac->GetLiteFoot();
}


cell AMX_NATIVE_CALL GetPlayerCrouchBugProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(1, "GetPlayerCrouchBug");

	// Get CAntiCheat pointer
	CAntiCheat* ac = CAntiCheatHandler::GetAntiCheat(pParams[1]);

	// Make sure the player is connected 
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	return ac->GetCrouchBug();
}


cell AMX_NATIVE_CALL GetPlayerSwitchReloadProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(1, "GetPlayerSwitchReload");

	// Get CAntiCheat pointer
	CAntiCheat* ac = CAntiCheatHandler::GetAntiCheat(pParams[1]);

	// Make sure the player is connected 
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	return ac->GetSwitchReload();
}

cell AMX_NATIVE_CALL TogglePlayerUnlimitedSprintProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(2, "TogglePlayerUnlimitedSprint");

	// Get CAntiCheat pointer
	CAntiCheat* ac = CAntiCheatHandler::GetAntiCheat(pParams[1]);

	// Make sure the player is connected 
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	ac->ToggleUnlimitedSprint(!!pParams[2]);

	return 1;
}

cell AMX_NATIVE_CALL GetPlayerUnlimitedSprintProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(1, "GetPlayerUnlimitedSprint");

	// Get CAntiCheat pointer
	CAntiCheat* ac = CAntiCheatHandler::GetAntiCheat(pParams[1]);

	// Make sure the player is connected 
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	return ac->GetUnlimitedSprint();
}

cell AMX_NATIVE_CALL TogglePlayerMacroLimitsProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(2, "TogglePlayerMacroLimits");

	// Get CAntiCheat pointer
	CAntiCheat* ac = CAntiCheatHandler::GetAntiCheat(pParams[1]);

	// Make sure the player is connected 
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	ac->ToggleMacroLimitations(!!pParams[2]);

	return 1;
}

cell AMX_NATIVE_CALL GetPlayerMacroLimitsProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(1, "GetPlayerMacroLimits");

	// Get CAntiCheat pointer
	CAntiCheat* ac = CAntiCheatHandler::GetAntiCheat(pParams[1]);

	// Make sure the player is connected 
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	return ac->GetMacroLimitations();
}

cell AMX_NATIVE_CALL TogglePlayerSprintOnAllSurfacesProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(2, "TogglePlayerSprintOnAllSurfaces");

	// Get CAntiCheat pointer
	CAntiCheat* ac = CAntiCheatHandler::GetAntiCheat(pParams[1]);

	// Make sure the player is connected 
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	ac->ToggleSprintOnAllSurfaces(!!pParams[2]);

	return 1;
}

cell AMX_NATIVE_CALL GetPlayerSprintOnAllSurfacesProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(1, "GetPlayerSprintOnAllSurfaces");

	// Get CAntiCheat pointer
	CAntiCheat* ac = CAntiCheatHandler::GetAntiCheat(pParams[1]);

	// Make sure the player is connected 
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	return ac->GetSprintOnAllSurfaces();
}

cell AMX_NATIVE_CALL TogglePlayerVehicleBlipsProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(2, "TogglePlayerVehicleBlips");

	// Get CAntiCheat pointer
	CAntiCheat* ac = CAntiCheatHandler::GetAntiCheat(pParams[1]);

	// Make sure the player is connected 
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	ac->ToggleVehicleBlips(!!pParams[2]);

	return 1;
}

cell AMX_NATIVE_CALL GetPlayerVehicleBlipsProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(1, "GetPlayerVehicleBlips");

	// Get CAntiCheat pointer
	CAntiCheat* ac = CAntiCheatHandler::GetAntiCheat(pParams[1]);

	// Make sure the player is connected 
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	return ac->GetVehicleBlips();
}

cell AMX_NATIVE_CALL AC_IsPlayerBannedProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(1, "AC_IsPlayerBanned");

	// Get CAntiCheat pointer
	CAntiCheat* ac = CAntiCheatHandler::GetAntiCheat(pParams[1]);

	// Make sure the player is connected 
	if (!IsPlayerConnected(pParams[1]) || ac == NULL) return 0;

	return ac->AC_IsBanned();
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return sampgdk::Supports() | SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
	// Handle sampGDK ticking.
	return sampgdk::ProcessTick();
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	// Load SampGDK
	bool load = sampgdk::Load(ppData);

	// Initialize
	Utility::Initialize(ppData);

	// hook amx_register
	InstallAmxHooks();

	// Find addresses to hook
	FindAddresses();

	PluginData = ppData;

	// Print out that we've loaded successfully.
	Utility::Printf("SA-MP Anti-Cheat v%0.2f Has loaded successfully.", CURRENT_VERSION);

	// return SampGDK load value
	return load;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	// The plugin has been unloaded, let them know we're leaving :(
	Utility::Printf("Unloaded SA-MP Anti-Cheat v%0.2f", CURRENT_VERSION);
	return sampgdk::Unload();
}

AMX_NATIVE_INFO PluginNatives[] =
{
	{ "IsPlayerUsingSampAC", IsPlayerUsingSAMPACProc },
	{ "CanPlayerEnableAC", CanPlayerEnableACProc },
	{ "SetPlayerCanEnableAC", SetPlayerCanEnableACProc },
	{ "MD5_Memory", MD5_MemoryProc },
	{ "GetPlayerHardwareID", GetPlayerHardwareIDProc },
	{ "TogglePlayerSwitchReload", TogglePlayerSwitchReloadProc },
	{ "SetPlayerFPSLimit", SetPlayerFPSLimitProc },
	{ "GetPlayerFPSLimit", GetPlayerFPSLimitProc },
	{ "SetPlayerCrouchBug", SetPlayerCrouchBugProc },
	{ "TogglePlayerLiteFoot", TogglePlayerLiteFootProc },
	{ "GetPlayerLiteFoot", GetPlayerLiteFootProc },
	{ "GetPlayerCrouchBug", GetPlayerCrouchBugProc },
	{ "GetPlayerSwitchReload", GetPlayerSwitchReloadProc },
	{ "TogglePlayerUnlimitedSprint", TogglePlayerUnlimitedSprintProc },
	{ "GetPlayerUnlimitedSprint", GetPlayerUnlimitedSprintProc },
	{ "TogglePlayerMacroLimits", TogglePlayerMacroLimitsProc },
	{ "GetPlayerMacroLimits", GetPlayerMacroLimitsProc },
	{ "TogglePlayerSprintOnAllSurfaces", TogglePlayerSprintOnAllSurfacesProc },
	{ "GetPlayerSprintOnAllSurfaces", GetPlayerSprintOnAllSurfacesProc },
	{ "TogglePlayerVehicleBlips", TogglePlayerVehicleBlipsProc },
	{ "GetPlayerVehicleBlips", GetPlayerVehicleBlipsProc },
	{ "IsACEnabled", IsACEnabledProc },
	{ "AC_IsPlayerBanned", AC_IsPlayerBannedProc },
	{ 0, 0 }
};

AMX_NATIVE_INFO* GetPluginNatives()
{
	return PluginNatives;
}

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *pAmx)
{
	// Add a new AMX script to our list of running AMX files.
	Callback::GetAMXList().push_back(pAmx);

	static bool bFirst = false;
	if (!bFirst)
	{	
		// Check for an update to this plugin version.
		CServerUpdater::CheckForUpdate();

		InstallHooks();
		Network::Initialize(PluginData);

		bFirst = true;
	}
	
	// Allow the new loaded script to use our plugin's functions.
	return amx_Register(pAmx, PluginNatives, -1);
}


PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *pAmx)
{
	// Remove the AMX scripts from our list of loaded AMX files.
	Callback::GetAMXList().remove(pAmx);
	return AMX_ERR_NONE;
}