#include <main.h>
#include <amx.h>

extern CPed					*pPedSelf;
extern CVehicle				*pVehicle;

void Log ( const char *fmt, ... );
#define CHECK_PARAMS(n) { if (params[0] != (n * sizeof(cell))) { Log("SCRIPT: Bad parameter count (Count is %d, Should be %d): ", params[0] / sizeof(cell), n); return 0; } }

char* format_amxstring(AMX *amx, cell *params, int parm, int &len);
int set_amxstring(AMX *amx,cell amx_addr,const char *source,int max);

// native SetTimerEx(funcname[], interval, repeating, parameter)
static cell AMX_NATIVE_CALL n_SetTimerEx(AMX *amx, cell *params)
{
	/*if (params[0] < 4 * sizeof (cell))
	{
		logprintf("SCRIPT: Bad parameter count (%d < 4): ", params[0]);
		return 0;
	}
	else if (params[0] > 260 * sizeof (cell))
	{
		logprintf("SCRIPT: Bad parameter count (%d > 260): ", params[0]);
		return 0;
	}

	char* szFuncName;
	amx_StrParam(amx, params[1], szFuncName);
	return pNetGame->GetTimers()->NewEx(szFuncName, params[2], params[3], params, amx);*/
	return 1;
}

static cell AMX_NATIVE_CALL n_SSFE_EMPTY(AMX *amx, cell *params)
{
	return 1;
}

static cell AMX_NATIVE_CALL n_GetPlayerPos(AMX *amx, cell *params)
{
    cell* cptr;
    amx_GetAddr(amx, params[2], &cptr);
    *cptr = amx_ftoc(pPedSelf->GetPosition()->fX);
    amx_GetAddr(amx, params[3], &cptr);
    *cptr = amx_ftoc(pPedSelf->GetPosition()->fY);
    amx_GetAddr(amx, params[4], &cptr);
    *cptr = amx_ftoc(pPedSelf->GetPosition()->fZ);
	return 1;
}
//----------------------------------------------------------------------------------
#include <cmath>

#define PI 3.1415926535897932384626433832795f

static cell AMX_NATIVE_CALL n_asin(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	float fResult = (float)(asin(amx_ctof(params[1])) * 180 / PI);
	return amx_ftoc(fResult);
}

static cell AMX_NATIVE_CALL n_acos(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	float fResult = (float)(acos(amx_ctof(params[1])) * 180 / PI);
	return amx_ftoc(fResult);
}

static cell AMX_NATIVE_CALL n_atan(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	float fResult = (float)(atan(amx_ctof(params[1])) * 180 / PI);
	return amx_ftoc(fResult);
}

static cell AMX_NATIVE_CALL n_atan2(AMX *amx, cell *params)
{
	CHECK_PARAMS(2);
	float fResult = (float)(atan2(amx_ctof(params[1]), amx_ctof(params[2])) * 180 / PI);
	return amx_ftoc(fResult);
}

AMX_NATIVE_INFO ssfe_Natives[] =
{
	{ "asin",					n_asin },
	{ "acos",					n_acos },
	{ "atan2",					n_atan2 },
	{ "atan",					n_atan },
	// Util
	{	"HTTP",	n_SSFE_EMPTY	},
	{	"CreateObject",	n_SSFE_EMPTY	},
	{	"AttachObjectToVehicle",	n_SSFE_EMPTY	},
	{	"AttachObjectToObject",	n_SSFE_EMPTY	},
	{	"AttachObjectToPlayer",	n_SSFE_EMPTY	},
	{	"SetObjectPos",	n_SSFE_EMPTY	},
	{	"GetObjectPos",	n_SSFE_EMPTY	},
	{	"SetObjectRot",	n_SSFE_EMPTY	},
	{	"GetObjectRot",	n_SSFE_EMPTY	},
	{	"IsValidObject",	n_SSFE_EMPTY	},
	{	"DestroyObject",	n_SSFE_EMPTY	},
	{	"MoveObject",	n_SSFE_EMPTY	},
	{	"StopObject",	n_SSFE_EMPTY	},
	{	"IsObjectMoving",	n_SSFE_EMPTY	},
	{	"EditObject",	n_SSFE_EMPTY	},
	{	"EditPlayerObject",	n_SSFE_EMPTY	},
	{	"SelectObject",	n_SSFE_EMPTY	},
	{	"CancelEdit",	n_SSFE_EMPTY	},
	{	"CreatePlayerObject",	n_SSFE_EMPTY	},
	{	"AttachPlayerObjectToVehicle",	n_SSFE_EMPTY	},
	{	"SetPlayerObjectPos",	n_SSFE_EMPTY	},
	{	"GetPlayerObjectPos",	n_SSFE_EMPTY	},
	{	"SetPlayerObjectRot",	n_SSFE_EMPTY	},
	{	"GetPlayerObjectRot",	n_SSFE_EMPTY	},
	{	"IsValidPlayerObject",	n_SSFE_EMPTY	},
	{	"DestroyPlayerObject",	n_SSFE_EMPTY	},
	{	"MovePlayerObject",	n_SSFE_EMPTY	},
	{	"StopPlayerObject",	n_SSFE_EMPTY	},
	{	"IsPlayerObjectMoving",	n_SSFE_EMPTY	},
	{	"AttachPlayerObjectToPlayer",	n_SSFE_EMPTY	},
	{	"SetObjectMaterial",	n_SSFE_EMPTY	},
	{	"SetPlayerObjectMaterial",	n_SSFE_EMPTY	},
	{	"SetObjectMaterialText",	n_SSFE_EMPTY	},
	{	"SetPlayerObjectMaterialText",	n_SSFE_EMPTY	},
	{	"SetSpawnInfo",	n_SSFE_EMPTY	},
	{	"SpawnPlayer",	n_SSFE_EMPTY	},
	{	"SetPlayerPos",	n_SSFE_EMPTY	},
	{	"SetPlayerPosFindZ",	n_SSFE_EMPTY	},
	{	"GetPlayerPos",	n_GetPlayerPos	},
	{	"SetPlayerFacingAngle",	n_SSFE_EMPTY	},
	{	"GetPlayerFacingAngle",	n_SSFE_EMPTY	},
	{	"IsPlayerInRangeOfPoint",	n_SSFE_EMPTY	},
	{	"GetPlayerDistanceFromPoint",	n_SSFE_EMPTY	},
	{	"IsPlayerStreamedIn",	n_SSFE_EMPTY	},
	{	"SetPlayerInterior",	n_SSFE_EMPTY	},
	{	"GetPlayerInterior",	n_SSFE_EMPTY	},
	{	"SetPlayerHealth",	n_SSFE_EMPTY	},
	{	"GetPlayerHealth",	n_SSFE_EMPTY	},
	{	"SetPlayerArmour",	n_SSFE_EMPTY	},
	{	"GetPlayerArmour",	n_SSFE_EMPTY	},
	{	"SetPlayerAmmo",	n_SSFE_EMPTY	},
	{	"GetPlayerAmmo",	n_SSFE_EMPTY	},
	{	"GetPlayerWeaponState",	n_SSFE_EMPTY	},
	{	"GetPlayerTargetPlayer",	n_SSFE_EMPTY	},
	{	"SetPlayerTeam",	n_SSFE_EMPTY	},
	{	"GetPlayerTeam",	n_SSFE_EMPTY	},
	{	"SetPlayerScore",	n_SSFE_EMPTY	},
	{	"GetPlayerScore",	n_SSFE_EMPTY	},
	{	"GetPlayerDrunkLevel",	n_SSFE_EMPTY	},
	{	"SetPlayerDrunkLevel",	n_SSFE_EMPTY	},
	{	"SetPlayerColor",	n_SSFE_EMPTY	},
	{	"GetPlayerColor",	n_SSFE_EMPTY	},
	{	"SetPlayerSkin",	n_SSFE_EMPTY	},
	{	"GetPlayerSkin",	n_SSFE_EMPTY	},
	{	"GivePlayerWeapon",	n_SSFE_EMPTY	},
	{	"ResetPlayerWeapons",	n_SSFE_EMPTY	},
	{	"SetPlayerArmedWeapon",	n_SSFE_EMPTY	},
	{	"GetPlayerWeaponData",	n_SSFE_EMPTY	},
	{	"GivePlayerMoney",	n_SSFE_EMPTY	},
	{	"ResetPlayerMoney",	n_SSFE_EMPTY	},
	{	"SetPlayerName",	n_SSFE_EMPTY	},
	{	"GetPlayerMoney",	n_SSFE_EMPTY	},
	{	"GetPlayerState",	n_SSFE_EMPTY	},
	{	"GetPlayerIp",	n_SSFE_EMPTY	},
	{	"GetPlayerPing",	n_SSFE_EMPTY	},
	{	"GetPlayerWeapon",	n_SSFE_EMPTY	},
	{	"GetPlayerKeys",	n_SSFE_EMPTY	},
	{	"GetPlayerName",	n_SSFE_EMPTY	},
	{	"SetPlayerTime",	n_SSFE_EMPTY	},
	{	"GetPlayerTime",	n_SSFE_EMPTY	},
	{	"TogglePlayerClock",	n_SSFE_EMPTY	},
	{	"SetPlayerWeather",	n_SSFE_EMPTY	},
	{	"ForceClassSelection",	n_SSFE_EMPTY	},
	{	"SetPlayerWantedLevel",	n_SSFE_EMPTY	},
	{	"GetPlayerWantedLevel",	n_SSFE_EMPTY	},
	{	"SetPlayerFightingStyle",	n_SSFE_EMPTY	},
	{	"GetPlayerFightingStyle",	n_SSFE_EMPTY	},
	{	"SetPlayerVelocity",	n_SSFE_EMPTY	},
	{	"GetPlayerVelocity",	n_SSFE_EMPTY	},
	{	"PlayCrimeReportForPlayer",	n_SSFE_EMPTY	},
	{	"PlayAudioStreamForPlayer",	n_SSFE_EMPTY	},
	{	"StopAudioStreamForPlayer",	n_SSFE_EMPTY	},
	{	"SetPlayerShopName",	n_SSFE_EMPTY	},
	{	"SetPlayerSkillLevel",	n_SSFE_EMPTY	},
	{	"GetPlayerSurfingVehicleID",	n_SSFE_EMPTY	},
	{	"GetPlayerSurfingObjectID",	n_SSFE_EMPTY	},
	{	"RemoveBuildingForPlayer",	n_SSFE_EMPTY	},
	{	"SetPlayerAttachedObject",	n_SSFE_EMPTY	},
	{	"RemovePlayerAttachedObject",	n_SSFE_EMPTY	},
	{	"IsPlayerAttachedObjectSlotUsed",	n_SSFE_EMPTY	},
	{	"EditAttachedObject",	n_SSFE_EMPTY	},
	{	"CreatePlayerTextDraw",	n_SSFE_EMPTY	},
	{	"PlayerTextDrawDestroy",	n_SSFE_EMPTY	},
	{	"PlayerTextDrawLetterSize",	n_SSFE_EMPTY	},
	{	"PlayerTextDrawTextSize",	n_SSFE_EMPTY	},
	{	"PlayerTextDrawAlignment",	n_SSFE_EMPTY	},
	{	"PlayerTextDrawColor",	n_SSFE_EMPTY	},
	{	"PlayerTextDrawUseBox",	n_SSFE_EMPTY	},
	{	"PlayerTextDrawBoxColor",	n_SSFE_EMPTY	},
	{	"PlayerTextDrawSetShadow",	n_SSFE_EMPTY	},
	{	"PlayerTextDrawSetOutline",	n_SSFE_EMPTY	},
	{	"PlayerTextDrawBackgroundColor",	n_SSFE_EMPTY	},
	{	"PlayerTextDrawFont",	n_SSFE_EMPTY	},
	{	"PlayerTextDrawSetProportional",	n_SSFE_EMPTY	},
	{	"PlayerTextDrawSetSelectable",	n_SSFE_EMPTY	},
	{	"PlayerTextDrawShow",	n_SSFE_EMPTY	},
	{	"PlayerTextDrawHide",	n_SSFE_EMPTY	},
	{	"PlayerTextDrawSetString",	n_SSFE_EMPTY	},
	{	"SetPVarInt",	n_SSFE_EMPTY	},
	{	"GetPVarInt",	n_SSFE_EMPTY	},
	{	"SetPVarString",	n_SSFE_EMPTY	},
	{	"GetPVarString",	n_SSFE_EMPTY	},
	{	"SetPVarFloat",	n_SSFE_EMPTY	},
	{	"GetPVarFloat",	n_SSFE_EMPTY	},
	{	"DeletePVar",	n_SSFE_EMPTY	},
	{	"GetPVarsUpperIndex",	n_SSFE_EMPTY	},
	{	"GetPVarNameAtIndex",	n_SSFE_EMPTY	},
	{	"GetPVarType",	n_SSFE_EMPTY	},
	{	"SetPlayerChatBubble",	n_SSFE_EMPTY	},
	{	"PutPlayerInVehicle",	n_SSFE_EMPTY	},
	{	"GetPlayerVehicleID",	n_SSFE_EMPTY	},
	{	"GetPlayerVehicleSeat",	n_SSFE_EMPTY	},
	{	"RemovePlayerFromVehicle",	n_SSFE_EMPTY	},
	{	"TogglePlayerControllable",	n_SSFE_EMPTY	},
	{	"PlayerPlaySound",	n_SSFE_EMPTY	},
	{	"ApplyAnimation",	n_SSFE_EMPTY	},
	{	"ClearAnimations",	n_SSFE_EMPTY	},
	{	"GetPlayerAnimationIndex",	n_SSFE_EMPTY	},
	{	"GetAnimationName",	n_SSFE_EMPTY	},
	{	"GetPlayerSpecialAction",	n_SSFE_EMPTY	},
	{	"SetPlayerSpecialAction",	n_SSFE_EMPTY	},
	{	"SetPlayerCheckpoint",	n_SSFE_EMPTY	},
	{	"DisablePlayerCheckpoint",	n_SSFE_EMPTY	},
	{	"SetPlayerRaceCheckpoint",	n_SSFE_EMPTY	},
	{	"DisablePlayerRaceCheckpoint",	n_SSFE_EMPTY	},
	{	"SetPlayerWorldBounds",	n_SSFE_EMPTY	},
	{	"SetPlayerMarkerForPlayer",	n_SSFE_EMPTY	},
	{	"ShowPlayerNameTagForPlayer",	n_SSFE_EMPTY	},
	{	"SetPlayerMapIcon",	n_SSFE_EMPTY	},
	{	"RemovePlayerMapIcon",	n_SSFE_EMPTY	},
	{	"AllowPlayerTeleport",	n_SSFE_EMPTY	},
	{	"SetPlayerCameraPos",	n_SSFE_EMPTY	},
	{	"SetPlayerCameraLookAt",	n_SSFE_EMPTY	},
	{	"SetCameraBehindPlayer",	n_SSFE_EMPTY	},
	{	"GetPlayerCameraPos",	n_SSFE_EMPTY	},
	{	"GetPlayerCameraFrontVector",	n_SSFE_EMPTY	},
	{	"GetPlayerCameraMode",	n_SSFE_EMPTY	},
	{	"AttachCameraToObject",	n_SSFE_EMPTY	},
	{	"AttachCameraToPlayerObject",	n_SSFE_EMPTY	},
	{	"InterpolateCameraPos",	n_SSFE_EMPTY	},
	{	"InterpolateCameraLookAt",	n_SSFE_EMPTY	},
	{	"IsPlayerConnected",	n_SSFE_EMPTY	},
	{	"IsPlayerInVehicle",	n_SSFE_EMPTY	},
	{	"IsPlayerInAnyVehicle",	n_SSFE_EMPTY	},
	{	"IsPlayerInCheckpoint",	n_SSFE_EMPTY	},
	{	"IsPlayerInRaceCheckpoint",	n_SSFE_EMPTY	},
	{	"SetPlayerVirtualWorld",	n_SSFE_EMPTY	},
	{	"GetPlayerVirtualWorld",	n_SSFE_EMPTY	},
	{	"EnableStuntBonusForPlayer",	n_SSFE_EMPTY	},
	{	"EnableStuntBonusForAll",	n_SSFE_EMPTY	},
	{	"TogglePlayerSpectating",	n_SSFE_EMPTY	},
	{	"PlayerSpectatePlayer",	n_SSFE_EMPTY	},
	{	"PlayerSpectateVehicle",	n_SSFE_EMPTY	},
	{	"StartRecordingPlayerData",	n_SSFE_EMPTY	},
	{	"StopRecordingPlayerData",	n_SSFE_EMPTY	},
	{	"SelectTextDraw",	n_SSFE_EMPTY	},
	{	"CancelSelectTextDraw",	n_SSFE_EMPTY	},
	{	"SendClientMessage",	n_SSFE_EMPTY	},
	{	"SendClientMessageToAll",	n_SSFE_EMPTY	},
	{	"SendPlayerMessageToPlayer",	n_SSFE_EMPTY	},
	{	"SendPlayerMessageToAll",	n_SSFE_EMPTY	},
	{	"SendDeathMessage",	n_SSFE_EMPTY	},
	{	"GameTextForAll",	n_SSFE_EMPTY	},
	{	"GameTextForPlayer",	n_SSFE_EMPTY	},
	{	"SetTimer",	n_SSFE_EMPTY	},
	{	"SetTimerEx",	n_SSFE_EMPTY	},
	{	"KillTimer",	n_SSFE_EMPTY	},
	{	"GetMaxPlayers",	n_SSFE_EMPTY	},
	{	"CallRemoteFunction",	n_SSFE_EMPTY	},
	{	"CallLocalFunction",	n_SSFE_EMPTY	},
	{	"SetGameModeText",	n_SSFE_EMPTY	},
	{	"SetTeamCount",	n_SSFE_EMPTY	},
	{	"AddPlayerClass",	n_SSFE_EMPTY	},
	{	"AddPlayerClassEx",	n_SSFE_EMPTY	},
	{	"AddStaticVehicle",	n_SSFE_EMPTY	},
	{	"AddStaticVehicleEx",	n_SSFE_EMPTY	},
	{	"AddStaticPickup",	n_SSFE_EMPTY	},
	{	"CreatePickup",	n_SSFE_EMPTY	},
	{	"DestroyPickup",	n_SSFE_EMPTY	},
	{	"ShowNameTags",	n_SSFE_EMPTY	},
	{	"ShowPlayerMarkers",	n_SSFE_EMPTY	},
	{	"GameModeExit",	n_SSFE_EMPTY	},
	{	"SetWorldTime",	n_SSFE_EMPTY	},
	{	"GetWeaponName",	n_SSFE_EMPTY	},
	{	"EnableTirePopping",	n_SSFE_EMPTY	},
	{	"AllowInteriorWeapons",	n_SSFE_EMPTY	},
	{	"SetWeather",	n_SSFE_EMPTY	},
	{	"SetGravity",	n_SSFE_EMPTY	},
	{	"AllowAdminTeleport",	n_SSFE_EMPTY	},
	{	"SetDeathDropAmount",	n_SSFE_EMPTY	},
	{	"CreateExplosion",	n_SSFE_EMPTY	},
	{	"EnableZoneNames",	n_SSFE_EMPTY	},
	{	"UsePlayerPedAnims",	n_SSFE_EMPTY	},
	{	"DisableInteriorEnterExits",	n_SSFE_EMPTY	},
	{	"SetNameTagDrawDistance",	n_SSFE_EMPTY	},
	{	"DisableNameTagLOS",	n_SSFE_EMPTY	},
	{	"LimitGlobalChatRadius",	n_SSFE_EMPTY	},
	{	"LimitPlayerMarkerRadius",	n_SSFE_EMPTY	},
	{	"ConnectNPC",	n_SSFE_EMPTY	},
	{	"IsPlayerNPC",	n_SSFE_EMPTY	},
	{	"IsPlayerAdmin",	n_SSFE_EMPTY	},
	{	"Kick",	n_SSFE_EMPTY	},
	{	"Ban",	n_SSFE_EMPTY	},
	{	"BanEx",	n_SSFE_EMPTY	},
	{	"SendRconCommand",	n_SSFE_EMPTY	},
	{	"GetServerVarAsString",	n_SSFE_EMPTY	},
	{	"GetServerVarAsInt",	n_SSFE_EMPTY	},
	{	"GetServerVarAsBool",	n_SSFE_EMPTY	},
	{	"GetPlayerNetworkStats",	n_SSFE_EMPTY	},
	{	"GetNetworkStats",	n_SSFE_EMPTY	},
	{	"GetPlayerVersion",	n_SSFE_EMPTY	},
	{	"CreateMenu",	n_SSFE_EMPTY	},
	{	"DestroyMenu",	n_SSFE_EMPTY	},
	{	"AddMenuItem",	n_SSFE_EMPTY	},
	{	"SetMenuColumnHeader",	n_SSFE_EMPTY	},
	{	"ShowMenuForPlayer",	n_SSFE_EMPTY	},
	{	"HideMenuForPlayer",	n_SSFE_EMPTY	},
	{	"IsValidMenu",	n_SSFE_EMPTY	},
	{	"DisableMenu",	n_SSFE_EMPTY	},
	{	"DisableMenuRow",	n_SSFE_EMPTY	},
	{	"GetPlayerMenu",	n_SSFE_EMPTY	},
	{	"TextDrawCreate",	n_SSFE_EMPTY	},
	{	"TextDrawDestroy",	n_SSFE_EMPTY	},
	{	"TextDrawLetterSize",	n_SSFE_EMPTY	},
	{	"TextDrawTextSize",	n_SSFE_EMPTY	},
	{	"TextDrawAlignment",	n_SSFE_EMPTY	},
	{	"TextDrawColor",	n_SSFE_EMPTY	},
	{	"TextDrawUseBox",	n_SSFE_EMPTY	},
	{	"TextDrawBoxColor",	n_SSFE_EMPTY	},
	{	"TextDrawSetShadow",	n_SSFE_EMPTY	},
	{	"TextDrawSetOutline",	n_SSFE_EMPTY	},
	{	"TextDrawBackgroundColor",	n_SSFE_EMPTY	},
	{	"TextDrawFont",	n_SSFE_EMPTY	},
	{	"TextDrawSetProportional",	n_SSFE_EMPTY	},
	{	"TextDrawSetSelectable",	n_SSFE_EMPTY	},
	{	"TextDrawShowForPlayer",	n_SSFE_EMPTY	},
	{	"TextDrawHideForPlayer",	n_SSFE_EMPTY	},
	{	"TextDrawShowForAll",	n_SSFE_EMPTY	},
	{	"TextDrawHideForAll",	n_SSFE_EMPTY	},
	{	"TextDrawSetString",	n_SSFE_EMPTY	},
	{	"GangZoneCreate",	n_SSFE_EMPTY	},
	{	"GangZoneDestroy",	n_SSFE_EMPTY	},
	{	"GangZoneShowForPlayer",	n_SSFE_EMPTY	},
	{	"GangZoneShowForAll",	n_SSFE_EMPTY	},
	{	"GangZoneHideForPlayer",	n_SSFE_EMPTY	},
	{	"GangZoneHideForAll",	n_SSFE_EMPTY	},
	{	"GangZoneFlashForPlayer",	n_SSFE_EMPTY	},
	{	"GangZoneFlashForAll",	n_SSFE_EMPTY	},
	{	"GangZoneStopFlashForPlayer",	n_SSFE_EMPTY	},
	{	"GangZoneStopFlashForAll",	n_SSFE_EMPTY	},
	{	"Create3DTextLabel",	n_SSFE_EMPTY	},
	{	"Delete3DTextLabel",	n_SSFE_EMPTY	},
	{	"Attach3DTextLabelToPlayer",	n_SSFE_EMPTY	},
	{	"Attach3DTextLabelToVehicle",	n_SSFE_EMPTY	},
	{	"Update3DTextLabelText",	n_SSFE_EMPTY	},
	{	"CreatePlayer3DTextLabel",	n_SSFE_EMPTY	},
	{	"DeletePlayer3DTextLabel",	n_SSFE_EMPTY	},
	{	"UpdatePlayer3DTextLabelText",	n_SSFE_EMPTY	},
	{	"ShowPlayerDialog",	n_SSFE_EMPTY	},
	{	"CreateVehicle",	n_SSFE_EMPTY	},
	{	"DestroyVehicle",	n_SSFE_EMPTY	},
	{	"IsVehicleStreamedIn",	n_SSFE_EMPTY	},
	{	"GetVehiclePos",	n_SSFE_EMPTY	},
	{	"SetVehiclePos",	n_SSFE_EMPTY	},
	{	"GetVehicleZAngle",	n_SSFE_EMPTY	},
	{	"GetVehicleRotationQuat",	n_SSFE_EMPTY	},
	{	"GetVehicleDistanceFromPoint",	n_SSFE_EMPTY	},
	{	"SetVehicleZAngle",	n_SSFE_EMPTY	},
	{	"SetVehicleParamsForPlayer",	n_SSFE_EMPTY	},
	{	"ManualVehicleEngineAndLights",	n_SSFE_EMPTY	},
	{	"SetVehicleParamsEx",	n_SSFE_EMPTY	},
	{	"GetVehicleParamsEx",	n_SSFE_EMPTY	},
	{	"SetVehicleToRespawn",	n_SSFE_EMPTY	},
	{	"LinkVehicleToInterior",	n_SSFE_EMPTY	},
	{	"AddVehicleComponent",	n_SSFE_EMPTY	},
	{	"RemoveVehicleComponent",	n_SSFE_EMPTY	},
	{	"ChangeVehicleColor",	n_SSFE_EMPTY	},
	{	"ChangeVehiclePaintjob",	n_SSFE_EMPTY	},
	{	"SetVehicleHealth",	n_SSFE_EMPTY	},
	{	"GetVehicleHealth",	n_SSFE_EMPTY	},
	{	"AttachTrailerToVehicle",	n_SSFE_EMPTY	},
	{	"DetachTrailerFromVehicle",	n_SSFE_EMPTY	},
	{	"IsTrailerAttachedToVehicle",	n_SSFE_EMPTY	},
	{	"GetVehicleTrailer",	n_SSFE_EMPTY	},
	{	"SetVehicleNumberPlate",	n_SSFE_EMPTY	},
	{	"GetVehicleModel",	n_SSFE_EMPTY	},
	{	"GetVehicleComponentInSlot",	n_SSFE_EMPTY	},
	{	"GetVehicleComponentType",	n_SSFE_EMPTY	},
	{	"RepairVehicle",	n_SSFE_EMPTY	},
	{	"GetVehicleVelocity",	n_SSFE_EMPTY	},
	{	"SetVehicleVelocity",	n_SSFE_EMPTY	},
	{	"SetVehicleAngularVelocity",	n_SSFE_EMPTY	},
	{	"GetVehicleDamageStatus",	n_SSFE_EMPTY	},
	{	"UpdateVehicleDamageStatus",	n_SSFE_EMPTY	},
	{	"GetVehicleModelInfo",	n_SSFE_EMPTY	},
	{	"SetVehicleVirtualWorld",	n_SSFE_EMPTY	},
	{	"GetVehicleVirtualWorld",	n_SSFE_EMPTY	},
	{	"sendstring",	n_SSFE_EMPTY	},
	{	"sendpacket",	n_SSFE_EMPTY	},
	{	"listenport",	n_SSFE_EMPTY	},
	{ NULL, NULL }
};

//----------------------------------------------------------------------------------

int amx_SSFEInit(AMX *amx)
{
  return amx_Register(amx, ssfe_Natives, -1);
}

//----------------------------------------------------------------------------------
