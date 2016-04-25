/*
*	This function made by JuarezPL [2013]
	Addresses only for gta sa EXE 1.0 :/
*/
#pragma once

namespace Functions
{
	int AddPickup(int modelid, int type, float x, float y, float z)
	{
		int (__cdecl *CPickups__addPickup)(float, float, float, int, int, int, int, int, char *)
		= (decltype(CPickups__addPickup))(0x456F20);

		return CPickups__addPickup(x, y, z, modelid, type, 0, 0, 0, 0);
	}

	int DestroyPickup(int pickup)
	{
		void (__cdecl *CPickups__removePickup)(int)
		= (decltype(CPickups__removePickup))(0x4573D0);

		CPickups__removePickup(pickup);

		return 0;
	}

	int SetPickupMoney(int pickup, int money)
	{
		void (__cdecl *CPickups__setMoney)(int, int)
		= (decltype(CPickups__setMoney))(0x455680);

		CPickups__setMoney(pickup, money);

		return 0;
	}

	bool IsPickupPickedUp(int pickup)
	{
		bool (__cdecl *CPickups__isPickedUp)(int)
		= (decltype(CPickups__isPickedUp))(0x454B40);

		return CPickups__isPickedUp(pickup)?true:false;
	}

	int SpawnCar(int modelID)
	{
		void (__cdecl *SpawnCar)(int)
		= (decltype(SpawnCar))(0x43A0B0);

		SpawnCar(modelID);

		return 0;
	}

	int ShowGarageGXT(char *gxt, int nr1, int time, int nr2)
	{
		void (__cdecl *CGarages__showGarageGXT)(char *, int, int, int)
		= (decltype(CGarages__showGarageGXT))(0x447B80);

		CGarages__showGarageGXT(gxt, nr1, time, nr2);

		return 0;
	}
};