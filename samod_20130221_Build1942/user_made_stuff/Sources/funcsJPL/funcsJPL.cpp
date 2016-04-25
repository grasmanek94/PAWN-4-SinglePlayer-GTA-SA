/*
*	This project made by grasmanek94 and edited by JuaezPL [2013]
*/
#include <windows.h>
#include <plugin.h>
#include "functions.h"

extern void * pAMXFunctions;

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() 
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load( void **ppData ) 
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload( )
{

}

static cell AMX_NATIVE_CALL n_AddPickup( AMX* amx, cell* params )
{
	return Functions::AddPickup(params[1], params[2], amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5]));
}

static cell AMX_NATIVE_CALL n_DestroyPickup( AMX* amx, cell* params )
{
	Functions::DestroyPickup(params[1]);
	return 1;
}

static cell AMX_NATIVE_CALL n_SetPickupMoney( AMX* amx, cell* params )
{
	Functions::SetPickupMoney(params[1], params[2]);
	return 1;
}

static cell AMX_NATIVE_CALL n_IsPickupPickedUp( AMX* amx, cell* params )
{
	return Functions::IsPickupPickedUp(params[1]);
}

static cell AMX_NATIVE_CALL n_SpawnCar( AMX* amx, cell* params )
{
	Functions::SpawnCar(params[1]);
	return 1;
}

static cell AMX_NATIVE_CALL n_ShowGarageGXT( AMX* amx, cell* params )
{
	char *tmp;
	amx_StrParam(amx, params[1], tmp);

	Functions::ShowGarageGXT(tmp, params[2], params[3], params[4]);
	return 1;
}

AMX_NATIVE_INFO AMXNatives[ ] =
{
	{"JPL_AddPickup", n_AddPickup},
	{"JPL_DestroyPickup", n_DestroyPickup},
	{"JPL_SetPickupMoney", n_SetPickupMoney},
	{"JPL_IsPickupPickedUp", n_IsPickupPickedUp},
	{"JPL_SpawnCar", n_SpawnCar},
	{"JPL_ShowGarageGXT", n_ShowGarageGXT},
	{0,                0}
};

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad( AMX *amx ) 
{
	return amx_Register( amx, AMXNatives, -1 );
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload( AMX *amx ) 
{
	return AMX_ERR_NONE;
}
