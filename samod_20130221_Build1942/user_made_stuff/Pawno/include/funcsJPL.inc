//funcs made by JuarezPL [2013]

#if defined _funcsJPL_included
	#endinput
#endif
#define _funcsJPL_included
#pragma library funcsJPL

native JPL_AddPickup(modelid, type, Float:X, Float:Y, Float:Z);
native JPL_DestroyPickup(pickup);
native JPL_SetPickupMoney(pickup, money);
native IsPickupPickedUp(pickup);
native JPL_SpawnCar(modelid);
native JPL_ShowGarageGXT(text[], nr1, time, nr2);