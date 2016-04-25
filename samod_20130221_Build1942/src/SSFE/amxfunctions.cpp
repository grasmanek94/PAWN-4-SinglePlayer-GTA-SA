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

CPed * LocalScriptPed = NULL;

static cell AMX_NATIVE_CALL n_GetPlayerPos(AMX *amx, cell *params)
{
	LocalScriptPed = pPools->GetPed(params[1]);
	if(!LocalScriptPed)
		return 0;
    cell* cptr;
    amx_GetAddr(amx, params[2], &cptr);
    *cptr = amx_ftoc(LocalScriptPed->GetPosition()->fX);
    amx_GetAddr(amx, params[3], &cptr);
    *cptr = amx_ftoc(LocalScriptPed->GetPosition()->fY);
    amx_GetAddr(amx, params[4], &cptr);
    *cptr = amx_ftoc(LocalScriptPed->GetPosition()->fZ);
	return 1;
}

#define PAWNFUNC(a) static cell AMX_NATIVE_CALL a(AMX *amx, cell *params)

namespace GangZone
{
	int Add(SColor Color, float X1, float Y1, float X2, float Y2);
	void Show(int ID,bool Show);
	void Flashing(int ID,bool Flashing);
	void SetColor(int ID,SColor Color);
	void SetPosition(int ID,float X1, float Y1, float X2, float Y2);
	void Remove(int ID);
}

namespace TextDraw
{
	int Add(float x, float y, float scaleX, float scaleY, BYTE font, BYTE aligment, unsigned char r, unsigned char g, unsigned char b, unsigned char a, BYTE outline, BYTE shadow, char *text, bool show);
	void Show(int ID,bool Show);
	void SetColor(int ID, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void SetPosition(int ID, float x, float y);
	void Remove(int ID);
	void SetScale(int ID, float x, float y);
	void SetAlignment(int ID, BYTE aligment);
	void SetShadow(int ID, BYTE shadow);
	void SetOutline(int ID, BYTE outline);
	void SetFont(int ID, BYTE font);
	void SetText(int ID, char * text);
	void SetSelectable(int ID, bool selectable);
	void UseBox(int ID, bool use);
	void SetBoxColor(int ID, DWORD color);
	void SetBackgroundColor(int ID, DWORD color);
	void SetProportional(int ID, BYTE proportional);
};

PAWNFUNC(n_TextDrawCreate)
{
	char * text;
	amx_StrParam(amx,params[3],text);
	return TextDraw::Add(amx_ctof(params[1]),amx_ctof(params[2]),1.0f,1.0f,0,0,0xFF,0xFF,0xFF,0xFF,0,0,text,false);
}
PAWNFUNC(n_TextDrawDestroy)
{
	TextDraw::Remove(params[1]);
	return 1;
}
PAWNFUNC(n_TextDrawLetterSize)
{
	TextDraw::SetScale(params[1],amx_ctof(params[2]),amx_ctof(params[3]));
	return 1;
}
PAWNFUNC(n_TextDrawTextSize)
{
	TextDraw::SetScale(params[1],amx_ctof(params[2]),amx_ctof(params[3]));
	return 1;
}
PAWNFUNC(n_TextDrawUseBox)
{
	TextDraw::UseBox(params[1],params[2]);
	return 1;
}
PAWNFUNC(n_TextDrawBoxColor)
{
	TextDraw::SetBoxColor(params[1],params[2]);
	return 1;
}
PAWNFUNC(n_TextDrawBackgroundColor)
{
	TextDraw::SetBackgroundColor(params[1],params[2]);
	return 1;
}
PAWNFUNC(n_TextDrawAlignment)
{
	TextDraw::SetAlignment(params[1],params[2]);
	return 1;
}
PAWNFUNC(n_TextDrawColor)
{
	TextDraw::SetColor(params[1],((params[2] & 0xFF000000) >> 24),((params[2] & 0xFF0000) >> 16),((params[2] & 0xFF00) >> 8),params[2] & 0xFF);
	return 1;
}
PAWNFUNC(n_TextDrawSetShadow)
{
	TextDraw::SetShadow(params[1],params[2]);
	return 1;
}
PAWNFUNC(n_TextDrawSetOutline)
{
	TextDraw::SetOutline(params[1],params[2]);
	return 1;
}
PAWNFUNC(n_TextDrawSetProportional)
{
	TextDraw::SetProportional(params[1],params[2]);
	return 1;
}
PAWNFUNC(n_TextDrawFont)
{
	TextDraw::SetFont(params[1],params[2]);
	return 1;
}
PAWNFUNC(n_TextDrawSetSelectable)
{
	TextDraw::SetSelectable(params[1],params[2]);
	return 1;
}
PAWNFUNC(n_TextDrawShowForPlayer)
{
	TextDraw::Show(params[2],true);
	return 1;
}
PAWNFUNC(n_TextDrawHideForPlayer)
{
	TextDraw::Show(params[2],false);
	return 1;
}
PAWNFUNC(n_TextDrawShowForAll)
{
	TextDraw::Show(params[1],true);
	return 1;
}
PAWNFUNC(n_TextDrawHideForAll)
{
	TextDraw::Show(params[1],false);
	return 1;
}
PAWNFUNC(n_TextDrawSetString)
{
	char * text;
	amx_StrParam(amx,params[2],text);
	TextDraw::SetText(params[1],text);
	return 1;
}
///
PAWNFUNC(n_PlayerTextDrawCreate)
{
	char * text;
	amx_StrParam(amx,params[4],text);
	return TextDraw::Add(amx_ctof(params[1]),amx_ctof(params[3]),1.0f,1.0f,0,0,0xFF,0xFF,0xFF,0xFF,0,0,text,false);
}
PAWNFUNC(n_PlayerTextDrawDestroy)
{
	TextDraw::Remove(params[2]);
	return 1;
}
PAWNFUNC(n_PlayerTextDrawLetterSize)
{
	TextDraw::SetScale(params[2],amx_ctof(params[3]),amx_ctof(params[4]));
	return 1;
}
PAWNFUNC(n_PlayerTextDrawTextSize)
{
	TextDraw::SetScale(params[2],amx_ctof(params[3]),amx_ctof(params[4]));
	return 1;
}
PAWNFUNC(n_PlayerTextDrawUseBox)
{
	TextDraw::UseBox(params[2],params[3]);
	return 1;
}
PAWNFUNC(n_PlayerTextDrawBoxColor)
{
	TextDraw::SetBoxColor(params[2],params[3]);
	return 1;
}
PAWNFUNC(n_PlayerTextDrawBackgroundColor)
{
	TextDraw::SetBackgroundColor(params[2],params[3]);
	return 1;
}
PAWNFUNC(n_PlayerTextDrawAlignment)
{
	TextDraw::SetAlignment(params[2],params[3]);
	return 1;
}
PAWNFUNC(n_PlayerTextDrawColor)
{
	TextDraw::SetColor(params[2],((params[3] & 0xFF000000) >> 24),((params[3] & 0xFF0000) >> 16),((params[3] & 0xFF00) >> 8),params[3] & 0xFF);
	return 1;
}
PAWNFUNC(n_PlayerTextDrawSetShadow)
{
	TextDraw::SetShadow(params[2],params[3]);
	return 1;
}
PAWNFUNC(n_PlayerTextDrawSetOutline)
{
	TextDraw::SetOutline(params[2],params[3]);
	return 1;
}
PAWNFUNC(n_PlayerTextDrawSetProportional)
{
	TextDraw::SetProportional(params[2],params[3]);
	return 1;
}
PAWNFUNC(n_PlayerTextDrawFont)
{
	TextDraw::SetFont(params[2],params[3]);
	return 1;
}
PAWNFUNC(n_PlayerTextDrawSetSelectable)
{
	TextDraw::SetSelectable(params[2],params[3]);
	return 1;
}
PAWNFUNC(n_PlayerTextDrawShow)
{
	TextDraw::Show(params[2],true);
	return 1;
}
PAWNFUNC(n_PlayerTextDrawHide)
{
	TextDraw::Show(params[2],false);
	return 1;
}
PAWNFUNC(n_PlayerTextDrawSetString)
{
	char * text;
	amx_StrParam(amx,params[3],text);
	TextDraw::SetText(params[2],text);
	return 1;
}

PAWNFUNC(n_GangZoneCreate)
{
	return GangZone::Add(SColor(0),amx_ctof(params[1]),amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4]));
}

PAWNFUNC(n_GangZoneDestroy)
{
	GangZone::Remove(params[1]);
	return 1;
}

PAWNFUNC(n_GangZoneShowForPlayer)
{
	GangZone::Show(params[2],true);
	GangZone::SetColor(params[2],SColor(params[3]));
	return 1;
}

PAWNFUNC(n_GangZoneShowForAll)
{
	GangZone::Show(params[1],true);
	GangZone::SetColor(params[1],SColor(params[2]));
	return 1;
}

PAWNFUNC(n_GangZoneHideForPlayer)
{
	GangZone::Show(params[2],false);
	return 1;
}

PAWNFUNC(n_GangZoneHideForAll)
{
	GangZone::Show(params[1],false);
	return 1;
}

PAWNFUNC(n_GangZoneFlashForPlayer)
{
	GangZone::Flashing(params[2],true);
	GangZone::SetColor(params[2],SColor(params[3]));
	return 1;
}

PAWNFUNC(n_GangZoneFlashForAll)
{
	GangZone::Flashing(params[1],true);
	GangZone::SetColor(params[1],SColor(params[2]));
	return 1;
}

PAWNFUNC(n_GangZoneStopFlashForPlayer)
{
	GangZone::Flashing(params[2],false);
	return 1;
}

PAWNFUNC(n_GangZoneStopFlashForAll)
{
	GangZone::Flashing(params[1],false);
	return 1;
}


PAWNFUNC(n_SetPlayerPos)
{
	LocalScriptPed = pPools->GetPed(params[1]);
	if(!LocalScriptPed)
		return 0;
	LocalScriptPed->SetPosition(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4]));
	return 1;
}

PAWNFUNC(n_SetPlayerPosFindZ)
{
	LocalScriptPed = pPools->GetPed(params[1]);
	if(!LocalScriptPed)
		return 0;
	LocalScriptPed->SetPosition(amx_ctof(params[2]),amx_ctof(params[3]),pGame->GetWorld()->FindGroundZForPosition(amx_ctof(params[2]),amx_ctof(params[3])));
	return 1;
}

PAWNFUNC(n_SetPlayerFacingAngle)
{
	LocalScriptPed = pPools->GetPed(params[1]);
	if(!LocalScriptPed)
		return 0;
	LocalScriptPed->SetDirection(&CVector(amx_ctof(params[2]),0.0f,0.0f));
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

	{	"GangZoneCreate",	n_GangZoneCreate	},
	{	"GangZoneDestroy",	n_GangZoneDestroy	},
	{	"GangZoneShowForPlayer",	n_GangZoneShowForPlayer	},
	{	"GangZoneShowForAll",	n_GangZoneShowForAll	},
	{	"GangZoneHideForPlayer",	n_GangZoneHideForPlayer	},
	{	"GangZoneHideForAll",	n_GangZoneHideForAll	},
	{	"GangZoneFlashForPlayer",	n_GangZoneFlashForPlayer	},
	{	"GangZoneFlashForAll",	n_GangZoneFlashForAll	},
	{	"GangZoneStopFlashForPlayer",	n_GangZoneStopFlashForPlayer	},
	{	"GangZoneStopFlashForAll",	n_GangZoneStopFlashForAll	},

	{	"GetPlayerPos",	n_GetPlayerPos	},
	{	"SetPlayerPos",	n_SetPlayerPos	},

	{	"SetPlayerPosFindZ",	n_SetPlayerPosFindZ	},

	{	"TextDrawCreate",	n_TextDrawCreate	},
	{	"TextDrawDestroy",	n_TextDrawDestroy	},
	{	"TextDrawLetterSize",	n_TextDrawLetterSize	},
	{	"TextDrawTextSize",	n_TextDrawTextSize	},
	{	"TextDrawAlignment",	n_TextDrawAlignment	},
	{	"TextDrawColor",	n_TextDrawColor	},
	{	"TextDrawUseBox",	n_TextDrawUseBox	},
	{	"TextDrawBoxColor",	n_TextDrawBoxColor	},
	{	"TextDrawSetShadow",	n_TextDrawSetShadow	},
	{	"TextDrawSetOutline",	n_TextDrawSetOutline	},
	{	"TextDrawBackgroundColor",	n_TextDrawBackgroundColor	},
	{	"TextDrawFont",	n_TextDrawFont	},
	{	"TextDrawSetProportional",	n_TextDrawSetProportional	},
	{	"TextDrawSetSelectable",	n_TextDrawSetSelectable	},
	{	"TextDrawShowForPlayer",	n_TextDrawShowForPlayer	},
	{	"TextDrawHideForPlayer",	n_TextDrawHideForPlayer	},
	{	"TextDrawShowForAll",	n_TextDrawShowForAll	},
	{	"TextDrawHideForAll",	n_TextDrawHideForAll	},
	{	"TextDrawSetString",	n_TextDrawSetString	},
	{	"CreatePlayerTextDraw",	n_PlayerTextDrawCreate	},
	{	"PlayerTextDrawDestroy",	n_PlayerTextDrawDestroy	},
	{	"PlayerTextDrawLetterSize",	n_PlayerTextDrawLetterSize	},
	{	"PlayerTextDrawTextSize",	n_PlayerTextDrawTextSize	},
	{	"PlayerTextDrawAlignment",	n_PlayerTextDrawAlignment	},
	{	"PlayerTextDrawColor",	n_PlayerTextDrawColor	},
	{	"PlayerTextDrawUseBox",		n_PlayerTextDrawUseBox},
	{	"PlayerTextDrawBoxColor",	n_PlayerTextDrawBoxColor	},
	{	"PlayerTextDrawSetShadow",	n_PlayerTextDrawSetShadow	},
	{	"PlayerTextDrawSetOutline",	n_PlayerTextDrawSetOutline	},
	{	"PlayerTextDrawBackgroundColor",	n_PlayerTextDrawBackgroundColor	},
	{	"PlayerTextDrawFont",	n_PlayerTextDrawFont	},
	{	"PlayerTextDrawSetProportional",	n_PlayerTextDrawSetProportional	},
	{	"PlayerTextDrawSetSelectable",	n_PlayerTextDrawSetSelectable	},
	{	"PlayerTextDrawShow",	n_PlayerTextDrawShow	},
	{	"PlayerTextDrawHide",	n_PlayerTextDrawHide	},
	{	"PlayerTextDrawSetString",	n_PlayerTextDrawSetString	},
	// Util
	{	"SetPlayerFacingAngle",	n_SetPlayerFacingAngle	},
	{	"GetPlayerFacingAngle",	n_SSFE_EMPTY	},
	{	"IsPlayerInRangeOfPoint",	n_SSFE_EMPTY	},
	{	"GetPlayerDistanceFromPoint",	n_SSFE_EMPTY	},
	{	"SetPlayerHealth",	n_SSFE_EMPTY	},
	{	"GetPlayerHealth",	n_SSFE_EMPTY	},
	{	"SetPlayerArmour",	n_SSFE_EMPTY	},
	{	"GetPlayerArmour",	n_SSFE_EMPTY	},
	{	"SetPlayerAmmo",	n_SSFE_EMPTY	},
	{	"GetPlayerAmmo",	n_SSFE_EMPTY	},
	{	"GetPlayerDrunkLevel",	n_SSFE_EMPTY	},
	{	"SetPlayerDrunkLevel",	n_SSFE_EMPTY	},
	{	"GivePlayerWeapon",	n_SSFE_EMPTY	},
	{	"ResetPlayerWeapons",	n_SSFE_EMPTY	},
	{	"SetPlayerArmedWeapon",	n_SSFE_EMPTY	},
	{	"GetPlayerWeaponData",	n_SSFE_EMPTY	},
	{	"GivePlayerMoney",	n_SSFE_EMPTY	},
	{	"ResetPlayerMoney",	n_SSFE_EMPTY	},
	{	"GetPlayerMoney",	n_SSFE_EMPTY	},
	{	"GetPlayerState",	n_SSFE_EMPTY	},
	{	"GetPlayerWeapon",	n_SSFE_EMPTY	},
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
	{	"SetPlayerInterior",	n_SSFE_EMPTY	},
	{	"GetPlayerInterior",	n_SSFE_EMPTY	},
	{	"GetPlayerWeaponState",	n_SSFE_EMPTY	},
	{	"GetPlayerTargetPlayer",	n_SSFE_EMPTY	},
	{	"SetPlayerTeam",	n_SSFE_EMPTY	},
	{	"GetPlayerTeam",	n_SSFE_EMPTY	},
	{	"SetPlayerSkin",	n_SSFE_EMPTY	},
	{	"GetPlayerSkin",	n_SSFE_EMPTY	},
	{	"GetPlayerKeys",	n_SSFE_EMPTY	},
	{	"SetPlayerTime",	n_SSFE_EMPTY	},
	{	"GetPlayerTime",	n_SSFE_EMPTY	},
	{	"TogglePlayerClock",	n_SSFE_EMPTY	},
	{	"SetPlayerWeather",	n_SSFE_EMPTY	},
	{	"SetPlayerWantedLevel",	n_SSFE_EMPTY	},
	{	"GetPlayerWantedLevel",	n_SSFE_EMPTY	},
	{	"SetPlayerFightingStyle",	n_SSFE_EMPTY	},
	{	"GetPlayerFightingStyle",	n_SSFE_EMPTY	},
	{	"SetPlayerVelocity",	n_SSFE_EMPTY	},
	{	"GetPlayerVelocity",	n_SSFE_EMPTY	},
	{	"PlayCrimeReportForPlayer",	n_SSFE_EMPTY	},
	{	"SetPlayerSkillLevel",	n_SSFE_EMPTY	},
	{	"GetPlayerSurfingVehicleID",	n_SSFE_EMPTY	},
	{	"GetPlayerSurfingObjectID",	n_SSFE_EMPTY	},
	{	"RemoveBuildingForPlayer",	n_SSFE_EMPTY	},
	{	"SetPlayerAttachedObject",	n_SSFE_EMPTY	},
	{	"RemovePlayerAttachedObject",	n_SSFE_EMPTY	},
	{	"IsPlayerAttachedObjectSlotUsed",	n_SSFE_EMPTY	},
	{	"EditAttachedObject",	n_SSFE_EMPTY	},
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

	{	"IsPlayerInVehicle",	n_SSFE_EMPTY	},
	{	"IsPlayerInAnyVehicle",	n_SSFE_EMPTY	},
	{	"IsPlayerInCheckpoint",	n_SSFE_EMPTY	},
	{	"IsPlayerInRaceCheckpoint",	n_SSFE_EMPTY	},


	{	"TogglePlayerSpectating",	n_SSFE_EMPTY	},
	{	"PlayerSpectatePlayer",	n_SSFE_EMPTY	},
	{	"PlayerSpectateVehicle",	n_SSFE_EMPTY	},
	{	"StartRecordingPlayerData",	n_SSFE_EMPTY	},
	{	"StopRecordingPlayerData",	n_SSFE_EMPTY	},
	{	"SelectTextDraw",	n_SSFE_EMPTY	},
	{	"CancelSelectTextDraw",	n_SSFE_EMPTY	},
	{	"GameTextForAll",	n_SSFE_EMPTY	},
	{	"GameTextForPlayer",	n_SSFE_EMPTY	},
	{	"SetTimer",	n_SSFE_EMPTY	},
	{	"SetTimerEx",	n_SSFE_EMPTY	},
	{	"KillTimer",	n_SSFE_EMPTY	},
	{	"CallRemoteFunction",	n_SSFE_EMPTY	},
	{	"CallLocalFunction",	n_SSFE_EMPTY	},
	{	"AddStaticVehicle",	n_SSFE_EMPTY	},
	{	"AddStaticVehicleEx",	n_SSFE_EMPTY	},
	{	"AddStaticPickup",	n_SSFE_EMPTY	},
	{	"CreatePickup",	n_SSFE_EMPTY	},
	{	"DestroyPickup",	n_SSFE_EMPTY	},
	{	"SetWorldTime",	n_SSFE_EMPTY	},
	{	"GetWeaponName",	n_SSFE_EMPTY	},
	{	"EnableTirePopping",	n_SSFE_EMPTY	},
	{	"AllowInteriorWeapons",	n_SSFE_EMPTY	},
	{	"SetWeather",	n_SSFE_EMPTY	},
	{	"SetGravity",	n_SSFE_EMPTY	},
	{	"CreateExplosion",	n_SSFE_EMPTY	},
	{	"EnableZoneNames",	n_SSFE_EMPTY	},
	{	"UsePlayerPedAnims",	n_SSFE_EMPTY	},
	{	"DisableInteriorEnterExits",	n_SSFE_EMPTY	},
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
	{	"sendstring",	n_SSFE_EMPTY	},
	{	"sendpacket",	n_SSFE_EMPTY	},
	{	"listenport",	n_SSFE_EMPTY	},

	//VERY LOW PRIORITY:
	{	"SetVehicleVirtualWorld",	n_SSFE_EMPTY	},
	{	"GetVehicleVirtualWorld",	n_SSFE_EMPTY	},
	{	"SetPlayerScore",	n_SSFE_EMPTY	},
	{	"GetPlayerScore",	n_SSFE_EMPTY	},
	{	"SetPlayerColor",	n_SSFE_EMPTY	},
	{	"GetPlayerColor",	n_SSFE_EMPTY	},
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
	{	"SetPlayerMarkerForPlayer",	n_SSFE_EMPTY	},
	{	"ShowPlayerNameTagForPlayer",	n_SSFE_EMPTY	},
	{	"IsPlayerConnected",	n_SSFE_EMPTY	},
	{	"SetPlayerVirtualWorld",	n_SSFE_EMPTY	},
	{	"GetPlayerVirtualWorld",	n_SSFE_EMPTY	},
	{	"GetMaxPlayers",	n_SSFE_EMPTY	},
	{	"SetGameModeText",	n_SSFE_EMPTY	},
	{	"SetTeamCount",	n_SSFE_EMPTY	},
	{	"ShowNameTags",	n_SSFE_EMPTY	},
	{	"ShowPlayerMarkers",	n_SSFE_EMPTY	},
	{	"IsPlayerStreamedIn",	n_SSFE_EMPTY	},
	{	"SetPlayerName",	n_SSFE_EMPTY	},
	{	"GetPlayerIp",	n_SSFE_EMPTY	},
	{	"GetPlayerPing",	n_SSFE_EMPTY	},
	{	"SetPlayerShopName",	n_SSFE_EMPTY	},
	{	"PlayAudioStreamForPlayer",	n_SSFE_EMPTY	},
	{	"StopAudioStreamForPlayer",	n_SSFE_EMPTY	},
	{	"ForceClassSelection",	n_SSFE_EMPTY	},
	{	"SetPlayerChatBubble",	n_SSFE_EMPTY	},
	{	"GetVehicleRotationQuat",	n_SSFE_EMPTY	},
	{	"AllowAdminTeleport",	n_SSFE_EMPTY	},
	{	"SetDeathDropAmount",	n_SSFE_EMPTY	},
	{	"GameModeExit",	n_SSFE_EMPTY	},
	{	"AddPlayerClass",	n_SSFE_EMPTY	},
	{	"AddPlayerClassEx",	n_SSFE_EMPTY	},
	{	"SendClientMessage",	n_SSFE_EMPTY	},
	{	"SendClientMessageToAll",	n_SSFE_EMPTY	},
	{	"SendPlayerMessageToPlayer",	n_SSFE_EMPTY	},
	{	"SendPlayerMessageToAll",	n_SSFE_EMPTY	},
	{	"SendDeathMessage",	n_SSFE_EMPTY	},
	{	"EnableStuntBonusForPlayer",	n_SSFE_EMPTY	},
	{	"EnableStuntBonusForAll",	n_SSFE_EMPTY	},
	{	"GetPlayerName",	n_SSFE_EMPTY	},
	{	"SetSpawnInfo",	n_SSFE_EMPTY	},
	{	"SpawnPlayer",	n_SSFE_EMPTY	},
	{	"HTTP",	n_SSFE_EMPTY	},
	{ NULL, NULL }
};

//----------------------------------------------------------------------------------

int amx_SSFEInit(AMX *amx)
{
  return amx_Register(amx, ssfe_Natives, -1);
}

//----------------------------------------------------------------------------------

/*NOT WORKING..

#include "SSFE/textdraw.h"

CTextDraw * td = NULL;

void TDTEST()
{
	static bool created = false;
	if(!created)
	{
		TEXT_DRAW_TRANSMIT x;
		x.byteBox = 1;
		x.byteCenter = 1;
		x.byteFlags = 1;
		x.byteLeft = 0;
		x.byteOutline = 1;
		x.bytePadding = 1;
		x.byteProportional = 1;
		x.byteRight = 0;
		x.byteShadow = 1;
		x.dwBackgroundColor = 0x55555555;
		x.dwBoxColor = 0x55555555;
		x.dwLetterColor = 0xFFFFFFFF;
		x.fLetterHeight = 12.0;
		x.fLetterWidth = 6.0;
		x.fLineHeight = 12.0;
		x.fLineWidth = 6.0;
		x.fX = 320.0;
		x.fY = 240.0;
		td = new CTextDraw(&x,"TEST");
	}
	if(td != NULL)
		td->Draw();
}
*/