//functions
#include <main.h>
#include <proxydll.h>
#include "scmopcode/scm.h"

char* format_amxstring(AMX *amx, cell *params, int parm, int &len);
int set_amxstring(AMX *amx,cell amx_addr,const char *source,int max);

extern struct pool					*pool_actor;
extern struct pool					*pool_vehicle;

namespace Functions
{
	void ConvertMatrixToEulerAngles ( const CMatrix& Matrix, float& fX, float& fY, float& fZ )
	{
		// Convert the given matrix to a padded matrix
		CMatrix_Padded matrixPadded ( Matrix );

		// Grab its pointer and call gta's func
		CMatrix_Padded* pMatrixPadded = &matrixPadded;
		DWORD dwFunc = FUNC_CMatrix__ConvertToEulerAngles;

		float* pfX = &fX;
		float* pfY = &fY;
		float* pfZ = &fZ;
		int iUnknown = 21;
		_asm
		{
			push    iUnknown
			push    pfZ
			push    pfY
			push    pfX
			mov     ecx, pMatrixPadded
			call    dwFunc
		}
	}
};//namespace Functions

//Pawn Functions
#define PWNFUNC(a) static cell AMX_NATIVE_CALL a(AMX *amx, cell *params)

namespace PawnFunc
{
	PWNFUNC(GetGameVersion)
	{		
		return pGameInterface->FindGameVersion();
	}
	PWNFUNC(GetBlurLevel)
	{
		return pGameInterface->GetBlurLevel();
	}
	PWNFUNC(GetGameTime)
	{
		cell* cptra = NULL;
		cell* cptrb = NULL;
		amx_GetAddr(amx, params[1], &cptra);
		amx_GetAddr(amx, params[2], &cptrb);
		pGameInterface->GetClock()->Get((BYTE*)cptra,(BYTE*)cptrb);
		return 1;
	}
	PWNFUNC(GetFPS)
	{
		float temp = pGameInterface->GetFPS();
		return amx_ftoc(temp);
	}
	PWNFUNC(GetGameSpeed)
	{
		float temp = pGameInterface->GetGameSpeed();
		return amx_ftoc(temp);
	}
	PWNFUNC(GetGravity)
	{
		float temp = pGameInterface->GetGravity();
		return amx_ftoc(temp);
	}
	PWNFUNC(GetBikeFrontWheelCounter)
	{
		return pGameInterface->GetPlayerInfo()->GetBikeFrontWheelCounter();
	}
	PWNFUNC(GetBikeFrontWheelDist)
	{
		float temp = pGameInterface->GetPlayerInfo()->GetBikeFrontWheelDist();
		return amx_ftoc(temp);
	}
	PWNFUNC(GetBikeRearWheelCounter)
	{
		return pGameInterface->GetPlayerInfo()->GetBikeRearWheelCounter();
	}
	PWNFUNC(GetBikeRearWheelDist)
	{
		float temp = pGameInterface->GetPlayerInfo()->GetBikeRearWheelDist();
		return amx_ftoc(temp);
	}
	PWNFUNC(GetCarLess3WheelCounter)
	{
		return pGameInterface->GetPlayerInfo()->GetCarLess3WheelCounter();
	}
	PWNFUNC(GetCarTwoWheelCounter)
	{
		return pGameInterface->GetPlayerInfo()->GetCarTwoWheelCounter();
	}
	PWNFUNC(GetCarTwoWheelDist)
	{
		float temp = pGameInterface->GetPlayerInfo()->GetCarTwoWheelDist();
		return amx_ftoc(temp);
	}
	PWNFUNC(GetCrossHair)
	{
		bool IsEnabled = true;
		float PosX = 0.0f;
		float PosY = 0.0f;
		pGameInterface->GetPlayerInfo()->GetCrossHair(IsEnabled,PosX,PosY);
		cell* cptra;
		cell* cptrb;
		amx_GetAddr(amx, params[1], &cptra);
		amx_GetAddr(amx, params[2], &cptrb);
		*cptra = amx_ftoc(PosX);
		*cptrb = amx_ftoc(PosY);
		return IsEnabled;
	}
	PWNFUNC(GetDoesNotGetTired)
	{
		return pGameInterface->GetPlayerInfo()->GetDoesNotGetTired();
	}
	PWNFUNC(GetFPSMoveHeading)
	{
		float temp = pGameInterface->GetPlayerInfo()->GetFPSMoveHeading();
		return amx_ftoc(temp);
	}
	PWNFUNC(GetLastTimeBigGunFired)
	{
		return pGameInterface->GetPlayerInfo()->GetLastTimeBigGunFired();
	}
	PWNFUNC(GetLastTimeEaten)
	{
		return pGameInterface->GetPlayerInfo()->GetLastTimeEaten();
	}
	PWNFUNC(GetPlayerMoney)
	{
		return pGameInterface->GetPlayerInfo()->GetPlayerMoney();
	}
	PWNFUNC(GetWantedLevel)
	{
		return pGameInterface->GetPlayerInfo()->GetWanted()->GetWantedLevel();
	}
	PWNFUNC(SetMaximumWantedLevel)
	{
		pGameInterface->GetPlayerInfo()->GetWanted()->SetMaximumWantedLevel(params[1]);
		return 1;
	}
	PWNFUNC(SetWantedLevel)
	{
		pGameInterface->GetPlayerInfo()->GetWanted()->SetWantedLevel(params[1]);
		return 1;
	}
	PWNFUNC(SetWantedLevelNoDrop)
	{
		pGameInterface->GetPlayerInfo()->GetWanted()->SetWantedLevelNoDrop(params[1]);
		return 1;
	}
	PWNFUNC(GivePlayerParachute)
	{
		pGameInterface->GetPlayerInfo()->GivePlayerParachute();
		return 1;
	}
	PWNFUNC(SetDoesNotGetTired)
	{
		pGameInterface->GetPlayerInfo()->SetDoesNotGetTired((bool)params[1]);
		return 1;
	}
	PWNFUNC(SetLastTimeBigGunFired)
	{
		pGameInterface->GetPlayerInfo()->SetLastTimeBigGunFired(params[1]);
		return 1;
	}
	PWNFUNC(SetLastTimeEaten)
	{
		pGameInterface->GetPlayerInfo()->SetLastTimeEaten(params[1]);
		return 1;
	}
	PWNFUNC(SetPlayerMoney)
	{
		pGameInterface->GetPlayerInfo()->SetPlayerMoney(params[1]);
		return 1;
	}
	PWNFUNC(StreamParachuteWeapon)
	{
		pGameInterface->GetPlayerInfo()->StreamParachuteWeapon((bool)params[1]);
		return 1;
	}
	PWNFUNC(RemoveWeaponModel)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->ClearWeapon ( static_cast < eWeaponType > (params[2]));
		gsPed->RemoveWeaponModel ( static_cast < eWeaponType > (params[2]) );

		// Set whatever weapon to 0 ammo so we don't keep it anymore
		CWeapon* pWeapon = gsPed->GetWeapon ( static_cast < eWeaponType > (params[2]) );
		if ( pWeapon )
		{
			pWeapon->SetType ( WEAPONTYPE_UNARMED );
			pWeapon->SetAmmoInClip ( 0 );
			pWeapon->SetAmmoTotal ( 0 );
			pWeapon->Remove ();
		}
		return 1;
	}
	PWNFUNC(RemoveAllWeapons)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->ClearWeapons();
		return 1;
	}
	PWNFUNC(GetAreaCode)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		return gsPed->GetAreaCode();
	}
	PWNFUNC(GetArmor)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		float temp = gsPed->GetArmor();
		return amx_ftoc(temp);
	}
	PWNFUNC(GetCanBeShotInVehicle)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		return gsPed->GetCanBeShotInVehicle();
	}
	PWNFUNC(GetCantBeKnockedOffBike)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		return gsPed->GetCantBeKnockedOffBike();
	}
	PWNFUNC(GetCurrentRotation)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		float temp = gsPed->GetCurrentRotation()*57.2957795f;
		return amx_ftoc(temp);
	}
	PWNFUNC(GetCurrentWeaponSlot)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		return gsPed->GetPedInterface()->bCurrentWeaponSlot;
	}
	PWNFUNC(GetDistFromCentreOfMassToBase)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		float temp = gsPed->GetDistanceFromCentreOfMassToBaseOfModel();
		return amx_ftoc(temp);
	}
	PWNFUNC(GetElasticity)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		float temp = gsPed->GetElasticity();
		return amx_ftoc(temp);
	}
	PWNFUNC(GetFightingStyle)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		return gsPed->GetFightingStyle();
	}
	PWNFUNC(GetHealth)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		float temp = 0.0f;
		if(params[2] == 1)
		{
			gsVehicle = gsPed->GetVehicle();
			if(gsVehicle != NULL)
			{
				temp = gsVehicle->GetHealth();
			}else temp = gsPed->GetHealth();
		}
		else temp = gsPed->GetHealth();
		return amx_ftoc(temp);
	}
	PWNFUNC(GetMass)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		float temp = gsPed->GetMass();
		return amx_ftoc(temp);
	}
	PWNFUNC(GetModelIndex)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		return gsPed->GetModelIndex();
	}
	PWNFUNC(GetMovementSpeed)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		CVector pMs;
		if(params[5] == 1)
		{
			gsVehicle = gsPed->GetVehicle();
			if(gsVehicle != NULL)
			{
				gsVehicle->GetMoveSpeed(&pMs);
			}else gsPed->GetMoveSpeed(&pMs);
		}else gsPed->GetMoveSpeed(&pMs);
		cell* cptra;
		cell* cptrb;
		cell* cptrc;
		amx_GetAddr(amx, params[2], &cptra);
		amx_GetAddr(amx, params[3], &cptrb);
		amx_GetAddr(amx, params[4], &cptrc);
		*cptra = amx_ftoc(pMs.fX);
		*cptrb = amx_ftoc(pMs.fY);
		*cptrc = amx_ftoc(pMs.fZ);
		return 1;
	}
	PWNFUNC(GetOccupiedSeat)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		return gsPed->GetOccupiedSeat();
	}
	PWNFUNC(GetPosition)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		CVector pMs;
		if(params[5] == 1)
		{
			gsVehicle = gsPed->GetVehicle();
			if(gsVehicle != NULL)
			{
				pMs = *gsVehicle->GetPosition();
			}else pMs = *gsPed->GetPosition();
		}else pMs = *gsPed->GetPosition();
		cell* cptra;
		cell* cptrb;
		cell* cptrc;
		amx_GetAddr(amx, params[2], &cptra);
		amx_GetAddr(amx, params[3], &cptrb);
		amx_GetAddr(amx, params[4], &cptrc);
		*cptra = amx_ftoc(pMs.fX);
		*cptrb = amx_ftoc(pMs.fY);
		*cptrc = amx_ftoc(pMs.fZ);
		return 1;
	}
	PWNFUNC(GetRunState)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		return gsPed->GetRunState();
	}
	PWNFUNC(GetStayInSamePlace)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		return gsPed->GetStayInSamePlace();
	}
	PWNFUNC(GetTargetRotation)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		float temp = gsPed->GetTargetRotation();
		return amx_ftoc(temp);
	}
	PWNFUNC(GetTestForShotInVehicle)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		return gsPed->GetTestForShotInVehicle();
	}
	PWNFUNC(GetTurnMass)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		float temp = 0.0;
		if(params[2] == 1)
		{
			gsVehicle = gsPed->GetVehicle();
			if(gsVehicle != NULL)
			{
				temp = gsVehicle->GetTurnMass();
			}else temp = gsPed->GetTurnMass();
		}else temp = gsPed->GetTurnMass();
		return amx_ftoc(temp);
	}
	PWNFUNC(GetTurnSpeed)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		CVector pMs;
		if(params[5] == 1)
		{
			gsVehicle = gsPed->GetVehicle();
			if(gsVehicle != NULL)
			{
				gsVehicle->GetTurnSpeed(&pMs);
			}else gsPed->GetTurnSpeed(&pMs);
		}
		else gsPed->GetTurnSpeed(&pMs);
		cell* cptra;
		cell* cptrb;
		cell* cptrc;
		amx_GetAddr(amx, params[2], &cptra);
		amx_GetAddr(amx, params[3], &cptrb);
		amx_GetAddr(amx, params[4], &cptrc);
		*cptra = amx_ftoc(pMs.fX);
		*cptrb = amx_ftoc(pMs.fY);
		*cptrc = amx_ftoc(pMs.fZ);
		return 1;
	}
	PWNFUNC(GetUnderwater)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		return gsPed->GetUnderwater();
	}
	PWNFUNC(GetType)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		return gsPed->GetType();
	}
	/*PWNFUNC(GetWeapon)
	{
		return LocalPed->GetWeapon(static_cast < eWeaponType > (params[1]))->;
	}*/
	PWNFUNC(GiveWeapon)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		switch(params[2])
		{
		case 19:
		case 20:
		case 21:
			return 0;
		default:
			break;
		}
		gsPed->GiveWeapon(static_cast < eWeaponType > (params[2]),params[3])->SetAsCurrentWeapon();
		return 1;
	}
	PWNFUNC(IsBackfaceCulled)
	{
		//return LocalPed->IsBackfaceCulled();
		return 0;
	}
	PWNFUNC(IsDucking)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		return gsPed->IsDucking();
	}
	PWNFUNC(IsFullyVisible)
	{
		//return LocalPed->IsFullyVisible();
		return 0;
	}
	PWNFUNC(IsInWater)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		return gsPed->IsInWater();
	}
	PWNFUNC(IsOnFire)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		return gsPed->IsOnFire();
	}
	PWNFUNC(IsOnScreen)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		return gsPed->IsOnScreen();
	}
	PWNFUNC(IsStatic)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		return gsPed->IsStatic();
	}
	PWNFUNC(IsStaticWaitingForCollision)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		return gsPed->IsStaticWaitingForCollision();
	}
	PWNFUNC(IsVisible)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		return gsPed->IsVisible();
	}
	PWNFUNC(IsWearingGoggles)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		return gsPed->IsWearingGoggles();
	}
	PWNFUNC(Respawn)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->Respawn(&CVector(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4])),params[5]);
		return 1;
	}
	PWNFUNC(SetAreaCode)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->SetAreaCode((BYTE)params[2]);
		return 1;
	}
	PWNFUNC(SetArmor)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->SetArmor(amx_ctof(params[2]));
		return 1;
	}
	PWNFUNC(SetBackfaceCulled)
	{
		//LocalPed->SetBackfaceCulled((bool)params[1]);
		return 1;
	}
	PWNFUNC(GetBuoyancyConstant)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		float temp = gsPed->GetBuoyancyConstant();
		return amx_ftoc(temp);
	}
	PWNFUNC(SetBuoyancyConstant)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->SetBuoyancyConstant(amx_ctof(params[2]));
		return 1;
	}
	PWNFUNC(SetCanBeShotInVehicle)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->SetCanBeShotInVehicle((bool)params[2]);
		return 1;
	}
	PWNFUNC(SetCantBeKnockedOffBike)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->SetCantBeKnockedOffBike(params[2]);
		return 1;
	}
	PWNFUNC(SetCurrentRotation)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->SetCurrentRotation(amx_ctof(params[2]));
		return 1;
	}
	PWNFUNC(SetCurrentWeaponSlot)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->SetCurrentWeaponSlot(static_cast < eWeaponSlot > (params[2]));
		return 1;
	}
	PWNFUNC(SetDucking)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->SetDucking((bool)params[2]);
		return 1;
	}
	PWNFUNC(SetElasticity)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->SetElasticity(amx_ctof(params[2]));
		return 1;
	}
	PWNFUNC(SetFightingStyle)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->SetFightingStyle(static_cast < eFightingStyle > (params[2]),(BYTE)params[3]);
		return 1;
	}
	PWNFUNC(SetFootBlood)
	{
		//LocalPed->SetFootBlood(params[1]);
		return 1;
	}
	PWNFUNC(SetGogglesState)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->SetGogglesState((bool)params[2]);
		return 1;
	}
	PWNFUNC(SetHealth)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		if(params[3] == 1)
		{
			gsVehicle = gsPed->GetVehicle();
			if(gsVehicle != NULL)
			{
				gsVehicle->SetHealth(amx_ctof(params[2]));
			}
			else gsPed->SetHealth(amx_ctof(params[2]));
		}else gsPed->SetHealth(amx_ctof(params[2]));
		return 1;
	}
	PWNFUNC(SetIsStanding)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->SetIsStanding((bool)params[2]);
		return 1;
	}
	PWNFUNC(SetLighting)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->SetLighting(amx_ctof(params[2]));
		return 1;
	}
	PWNFUNC(GetLighting)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		float temp = gsPed->GetLighting();
		return amx_ftoc(temp);
	}
	PWNFUNC(SetMass)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		if(params[3] == 1)
		{
			gsVehicle = gsPed->GetVehicle();
			if(gsVehicle != NULL)
			{
				gsVehicle->SetMass(amx_ctof(params[2]));
			}
			else gsPed->SetMass(amx_ctof(params[2]));
		}else gsPed->SetMass(amx_ctof(params[2]));
		return 1;
	}
	PWNFUNC(SetModelIndex)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->SetModelIndex(params[2]);
		return 1;
	}
	PWNFUNC(SetMoveSpeed)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		if(params[5] == 1)
		{
			gsVehicle = gsPed->GetVehicle();
			if(gsVehicle != NULL)
			{
				gsVehicle->SetMoveSpeed(&CVector(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4])));
			}
			else gsPed->SetMoveSpeed(&CVector(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4])));
		}else gsPed->SetMoveSpeed(&CVector(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4])));
		return 1;
	}
	PWNFUNC(SetOccupiedSeat)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->SetOccupiedSeat((BYTE)params[2]);
		return 1;
	}
	PWNFUNC(SetOnFire)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->SetOnFire((bool)params[2]);
		return 1;
	}
	PWNFUNC(SetOrientation)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		if(params[5] == 1)
		{
			gsVehicle = gsPed->GetVehicle();
			if(gsVehicle != NULL)
			{
				gsVehicle->SetOrientation(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4]));
			}
			else gsPed->SetOrientation(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4]));
		}else gsPed->SetOrientation(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4]));
		return 1;
	}
	PWNFUNC(SetOxygenLevel)
	{
		//LocalPed->SetOxygenLevel(amx_ctof(params[1]));
		return 1;
	}
	PWNFUNC(SetPosition)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		if(params[5] == 1)
		{
			gsVehicle = gsPed->GetVehicle();
			if(gsVehicle != NULL)
			{
				gsVehicle->SetPosition(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4]));
			}
			else gsPed->SetPosition(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4]));
		}
		else gsPed->SetPosition(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4]));
		return 1;
	}
	PWNFUNC(SetStatic)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->SetStatic((BOOL)params[2]);
		return 1;
	}
	PWNFUNC(SetStaticWaitingForCollision)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->SetStaticWaitingForCollision((bool)params[2]);
		return 1;
	}
	PWNFUNC(SetStayInSamePlace)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->SetStayInSamePlace((bool)params[2]);
		return 1;
	}
	PWNFUNC(SetTargetRotation)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->SetTargetRotation(amx_ctof(params[2]));
		return 1;
	}
	PWNFUNC(SetTestForShotInVehicle)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->SetTestForShotInVehicle((bool)params[2]);
		return 1;
	}
	PWNFUNC(SetTurnMass)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		if(params[3] == 1)
		{
			gsVehicle = gsPed->GetVehicle();
			if(gsVehicle != NULL)
			{
				gsVehicle->SetTurnMass(amx_ctof(params[2]));
			}
			else gsPed->SetTurnMass(amx_ctof(params[2]));
		}else gsPed->SetTurnMass(amx_ctof(params[2]));
		return 1;
	}
	PWNFUNC(SetTurnSpeed)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		if(params[5] == 1)
		{
			gsVehicle = gsPed->GetVehicle();
			if(gsVehicle != NULL)
			{
				gsVehicle->SetTurnSpeed(&CVector(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4])));
			}
			else gsPed->SetTurnSpeed(&CVector(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4])));
		}
		else gsPed->SetTurnSpeed(&CVector(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4])));
		return 1;
	}
	PWNFUNC(SetUnderwater)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->SetUnderwater((bool)params[2]);
		return 1;
	}
	PWNFUNC(SetUsesCollision)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->SetUsesCollision((bool)params[2]);
		return 1;
	}
	PWNFUNC(SetVisible)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		gsPed->SetVisible((bool)params[2]);
		return 1;
	}
	PWNFUNC(Teleport)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed == NULL)
			return 0;
		if(params[5] == 1)
		{
			gsVehicle = gsPed->GetVehicle();
			if(gsVehicle != NULL)
			{
				gsVehicle->Teleport(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4]));
			}
			else gsPed->Teleport(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4]));
		}
		else gsPed->Teleport(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4]));
		return 1;
	}
	//
	PWNFUNC(SetGravity)
	{
		pGameInterface->SetGravity(amx_ctof(params[1]));
		return 1;
	}
	PWNFUNC(SetBlurLevel)
	{
		pGameInterface->SetBlurLevel(params[1]);
		return 1;
	}
	PWNFUNC(SetGameSpeed)
	{
		pGameInterface->SetGameSpeed(amx_ctof(params[1]));
		return 1;
	}
	PWNFUNC(AddFont)
	{
		char * tmp;
		amx_StrParam(amx,params[1],tmp);
		fonts.push_back(CD3DFont(tmp,params[2],params[3]));
		fonts.back().Initialize(origIDirect3DDevice9);
		return fonts.size()-1;
	}
	PWNFUNC(DXDrawText)//FontID,X,Y,Color,Text
	{
		char * tmp;
		amx_StrParam(amx,params[5],tmp);
		fonts[params[1]].Print(amx_ctof(params[2]),amx_ctof(params[3]),params[4],tmp,params[6]);
		return 1;
	}
	// native format(output[], len, const format[], {Float,_}:...)
	PWNFUNC(format)
	{
	  int len;
	  return set_amxstring(amx, params[1], format_amxstring(amx, params, 3, len), params[2]);
	}
	PWNFUNC(GetScreenInfo)
	{
		VideoMode mode;
		pGameInterface->GetSettings()->GetVideoModeInfo(&mode,pGameInterface->GetSettings()->GetCurrentVideoMode());
		cell* cptra;
		cell* cptrb;
		cell* cptrc;
		cell* cptrd;
		amx_GetAddr(amx, params[1], &cptra);
		amx_GetAddr(amx, params[2], &cptrb);
		amx_GetAddr(amx, params[3], &cptrc);
		amx_GetAddr(amx, params[4], &cptrd);
		*cptra = mode.depth;
		*cptrb = mode.refRate;
		*cptrc = mode.height;
		*cptrd = mode.width;
		return 1;
	}
	PWNFUNC(_GetTickCount)
	{
		return (int)GetCounter();
	}
	PWNFUNC(DXDrawLine)
	{
		render->DrawLine(D3DXVECTOR3(amx_ctof(params[1]),amx_ctof(params[2]),amx_ctof(params[3])),D3DXVECTOR3(amx_ctof(params[4]),amx_ctof(params[5]),amx_ctof(params[6])),params[7]);
		return 1;
	}
	PWNFUNC(n_print)
	{
		char * tmp;
		amx_StrParam(amx,params[1],tmp);
		if(strlen(tmp) == 0)tmp = "(null)";
		Log(tmp);
		return 1;
	}
	PWNFUNC(IsPlayerInVehicle)
	{
		return pVehicle != NULL;
	}
	PWNFUNC(GetVehicleModel)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetModelIndex();
		}
		return 0;
	}
	PWNFUNC(AddVehicleUpgrade)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			pGameInterface->GetModelInfo(params[2])->RequestVehicleUpgrade();
			gsVehicle->AddVehicleUpgrade(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(AreVehicleDoorsLocked)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->AreDoorsLocked();
		}
		return -1;
	}
	PWNFUNC(AreVehicleDoorsUndamageable)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->AreDoorsUndamageable();
		}
		return -1;
	}
	PWNFUNC(BurstVehicleTyre)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->BurstTyre(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(CarHasRoof)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{	
			return gsVehicle->CarHasRoof();
		}
		return -1;
	}
	PWNFUNC(ExtinguishCarFire)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{	
			gsVehicle->ExtinguishCarFire();
			return 1;
		}
		return 0;
	}
	PWNFUNC(FixVehicle)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{	
			gsVehicle->Fix();
			return 1;
		}
		return 0;
	}
	PWNFUNC(GetCanVehicleBeDamaged)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{	
			return gsVehicle->GetCanBeDamaged();
		}
		return -1;
	}
	PWNFUNC(CanVehicleBeTargetedByBazooka)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{	
			return gsVehicle->GetCanBeTargettedByHeatSeekingMissiles();
		}
		return -1;
	}
	PWNFUNC(CanVehiclePetrolTankBeShot)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{	
			return gsVehicle->GetCanShootPetrolTank();
		}
		return -1;
	}
	PWNFUNC(GetVehColorChangesWhenPaintjob)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{	
			return gsVehicle->GetChangeColourWhenRemapping();
		}
		return -1;
	}
	PWNFUNC(GetVehicleColor)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{	
			BYTE color[4];
			gsVehicle->GetColor(&color[0],&color[1],&color[2],&color[3]);
			cell* cptra;
			cell* cptrb;
			cell* cptrc;
			cell* cptrd;
			amx_GetAddr(amx, params[1], &cptra);
			amx_GetAddr(amx, params[2], &cptrb);
			amx_GetAddr(amx, params[3], &cptrc);
			amx_GetAddr(amx, params[4], &cptrd);
			*cptra = color[0];
			*cptrb = color[1];
			*cptrc = color[2];
			*cptrd = color[3];
			return color[0] << 24 | color[1] << 16 | color[2] << 8 | color[3];
		}
		return 0;
	}
	PWNFUNC(GetCurrentGear)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{	
			return gsVehicle->GetCurrentGear();
		}
		return -1;
	}
	PWNFUNC(GetVehicleGasPedal)
	{
		float temp = 0.0f;
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{	
			temp = gsVehicle->GetGasPedal();
		}
		return amx_ctof(temp);
	}
	PWNFUNC(GetVehicleBreakPedal)
	{
		float temp = 0.0f;
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{	
			temp = -gsVehicle->GetGasPedal();
		}
		return amx_ctof(temp);
	}
	PWNFUNC(SetVehicleHandlingData)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{	
			switch(params[2])
			{
				case 0:
					gsVehicle->GetHandlingData()->SetABS(params[3]);
				break;
				case 1:
					gsVehicle->GetHandlingData()->SetAnimGroup(params[3]);
				break;
				case 2:
					gsVehicle->GetHandlingData()->SetBrakeBias(amx_ctof(params[3]));
				break;
				case 3:
					gsVehicle->GetHandlingData()->SetBrakeDecelleration(amx_ctof(params[3]));
				break;
				case 4:
					gsVehicle->GetHandlingData()->SetCarDriveType(static_cast < CHandlingEntry::eDriveType > (params[3]));
				break;
				case 5:
					gsVehicle->GetHandlingData()->SetCarEngineType(static_cast < CHandlingEntry::eEngineType > (params[3]));
				break;
				case 6:
					gsVehicle->GetHandlingData()->SetCenterOfMass(CVector(amx_ctof(params[3]),amx_ctof(params[4]),amx_ctof(params[5])));
				break;
				case 7:
					gsVehicle->GetHandlingData()->SetCollisionDamageMultiplier(amx_ctof(params[3]));
				break;
				case 8:
					gsVehicle->GetHandlingData()->SetDragCoeff(amx_ctof(params[3]));
				break;
				case 9:
					gsVehicle->GetHandlingData()->SetEngineAccelleration(amx_ctof(params[3]));
				break;
				case 10:
					gsVehicle->GetHandlingData()->SetEngineInertia(amx_ctof(params[3]));
				break;
				case 11:
					gsVehicle->GetHandlingData()->SetHandlingFlags(params[3]);
				break;
				case 12:
					gsVehicle->GetHandlingData()->SetHeadLight(static_cast < CHandlingEntry::eLightType > (params[3]));
				break;
				case 13:
					gsVehicle->GetHandlingData()->SetMass(amx_ctof(params[3]));
				break;
				case 14:
					gsVehicle->GetHandlingData()->SetMaxVelocity(amx_ctof(params[3]));
				break;
				case 15:
					gsVehicle->GetHandlingData()->SetModelFlags(params[3]);
				break;
				case 16:
					gsVehicle->GetHandlingData()->SetMonetary(params[3]);
				break;
				case 17:
					gsVehicle->GetHandlingData()->SetNumberOfGears(params[3]);
				break;
				case 18:
					gsVehicle->GetHandlingData()->SetPercentSubmerged(params[3]);
				break;
				case 19:
					gsVehicle->GetHandlingData()->SetSeatOffsetDistance(amx_ctof(params[3]));
				break;
				case 20:
					gsVehicle->GetHandlingData()->SetSteeringLock(amx_ctof(params[3]));
				break;
				case 21:
					gsVehicle->GetHandlingData()->SetSuspensionAntidiveMultiplier(amx_ctof(params[3]));
				break;
				case 22:
					gsVehicle->GetHandlingData()->SetSuspensionDamping(amx_ctof(params[3]));
				break;
				case 23:
					gsVehicle->GetHandlingData()->SetSuspensionForceLevel(amx_ctof(params[3]));
				break;
				case 24:
					gsVehicle->GetHandlingData()->SetSuspensionFrontRearBias(amx_ctof(params[3]));
				break;
				case 25:
					gsVehicle->GetHandlingData()->SetSuspensionHighSpeedDamping(amx_ctof(params[3]));
				break;
				case 26:
					gsVehicle->GetHandlingData()->SetSuspensionLowerLimit(amx_ctof(params[3]));
				break;
				case 27:
					gsVehicle->GetHandlingData()->SetSuspensionUpperLimit(amx_ctof(params[3]));
				break;
				case 28:
					gsVehicle->GetHandlingData()->SetTailLight(static_cast < CHandlingEntry::eLightType > (params[3]));
				break;
				case 29:
					gsVehicle->GetHandlingData()->SetTractionBias(amx_ctof(params[3]));
				break;
				case 30:
					gsVehicle->GetHandlingData()->SetTractionLoss(amx_ctof(params[3]));
				break;
				case 31:
					gsVehicle->GetHandlingData()->SetTractionMultiplier(amx_ctof(params[3]));
				break;
				case 32:
					gsVehicle->GetHandlingData()->SetTurnMass(amx_ctof(params[3]));
				break;
			}
			return 1;
		}
		return 0;
	}
	PWNFUNC(SetVehicleColor)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetColor(params[2],params[3],params[4],params[5]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(SetVehclePaintjob)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetRemap(params[1]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(SetVehclePaintjobDictionary)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetRemapTexDictionary(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(SetChangeColourOnPaintjob)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetChangeColourWhenRemapping(params[2]);
			return 1;
		}
		return 0;
	}

	PWNFUNC(GetVehicleHandlingDataFloat)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		float temp = 0.0f;
		if(gsVehicle != NULL)
		{	
			switch(params[1])
			{
				case 2:
					temp = gsVehicle->GetHandlingData()->GetBrakeBias();
				break;
				case 3:
					temp = gsVehicle->GetHandlingData()->GetBrakeDecelleration();
				break;
				case 7:
					temp = gsVehicle->GetHandlingData()->GetCollisionDamageMultiplier();
				break;
				case 8:
					temp = gsVehicle->GetHandlingData()->GetDragCoeff();
				break;
				case 9:
					temp = gsVehicle->GetHandlingData()->GetEngineAccelleration();
				break;
				case 10:
					temp = gsVehicle->GetHandlingData()->GetEngineInertia();
				break;
				case 13:
					temp = gsVehicle->GetHandlingData()->GetMass();
				break;
				case 14:
					temp = gsVehicle->GetHandlingData()->GetMaxVelocity();
				break;
				case 19:
					temp = gsVehicle->GetHandlingData()->GetSeatOffsetDistance();
				break;
				case 20:
					temp = gsVehicle->GetHandlingData()->GetSteeringLock();
				break;
				case 21:
					temp = gsVehicle->GetHandlingData()->GetSuspensionAntidiveMultiplier();
				break;
				case 22:
					temp = gsVehicle->GetHandlingData()->GetSuspensionDamping();
				break;
				case 23:
					temp = gsVehicle->GetHandlingData()->GetSuspensionForceLevel();
				break;
				case 24:
					temp = gsVehicle->GetHandlingData()->GetSuspensionFrontRearBias();
				break;
				case 25:
					temp = gsVehicle->GetHandlingData()->GetSuspensionHighSpeedDamping();
				break;
				case 26:
					temp = gsVehicle->GetHandlingData()->GetSuspensionLowerLimit();
				break;
				case 27:
					temp = gsVehicle->GetHandlingData()->GetSuspensionUpperLimit();
				break;
				case 29:
					temp = gsVehicle->GetHandlingData()->GetTractionBias();
				break;
				case 30:
					temp = gsVehicle->GetHandlingData()->GetTractionLoss();
				break;
				case 31:
					temp = gsVehicle->GetHandlingData()->GetTractionMultiplier();
				break;
				case 32:
					temp = gsVehicle->GetHandlingData()->GetTurnMass();
				break;
			}
			return amx_ftoc(temp);
		}
		return amx_ftoc(temp);
	}

	PWNFUNC(GetVehicleHandlingDataInt)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{	
			switch(params[2])
			{
				case 0:
					return gsVehicle->GetHandlingData()->GetABS();
				case 1:
					return gsVehicle->GetHandlingData()->GetAnimGroup();
				case 4:
					return gsVehicle->GetHandlingData()->GetCarDriveType();
				case 5:
					return gsVehicle->GetHandlingData()->GetCarEngineType();
				case 11:
					gsVehicle->GetHandlingData()->GetHandlingFlags();
				case 12:
					gsVehicle->GetHandlingData()->GetHeadLight();
				case 15:
					gsVehicle->GetHandlingData()->GetModelFlags();
				case 16:
					gsVehicle->GetHandlingData()->GetMonetary();
				case 17:
					gsVehicle->GetHandlingData()->GetNUmberOfGears();
				case 18:
					gsVehicle->GetHandlingData()->GetPercentSubmerged();
				case 28:
					gsVehicle->GetHandlingData()->GetTailLight();
			}
			return -1;
		}
		return -2;
	}

	PWNFUNC(GetVehicleCenterOfMass)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{	
			cell* cptra;
			cell* cptrb;
			cell* cptrc;
			amx_GetAddr(amx, params[2], &cptra);
			amx_GetAddr(amx, params[3], &cptrb);
			amx_GetAddr(amx, params[4], &cptrc);
			*cptra = amx_ftoc(gsVehicle->GetHandlingData()->GetCenterOfMass().fX);
			*cptrb = amx_ftoc(gsVehicle->GetHandlingData()->GetCenterOfMass().fX);
			*cptrc = amx_ftoc(gsVehicle->GetHandlingData()->GetCenterOfMass().fX);
			return 1;
		}
		return 0;
	}
	PWNFUNC(DXd3dbox)
	{
		render->D3DBox(amx_ctof(params[1]),amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4]),params[5]);
		return 1;
	}
	PWNFUNC(DXdedboxborder)
	{
		render->D3DBoxBorder(amx_ctof(params[1]),amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4]),params[5],params[6]);
		return 1;
	}

	PWNFUNC(GetCursorPosition)
	{
		POINT cursor_pos;
		if (GetCursorPos(&cursor_pos) && ScreenToClient(pPresentParam.hDeviceWindow, &cursor_pos))
		{
			cell* cptra;
			cell* cptrb;
			amx_GetAddr(amx, params[1], &cptra);
			amx_GetAddr(amx, params[2], &cptrb);
			*cptra = cursor_pos.x;
			*cptrb = cursor_pos.y;
			return 1;
		}
		return 0;
	}
	PWNFUNC(ShowCursor)
	{
		origIDirect3DDevice9->ShowCursor(params[1]);
		return 1;
	}
	PWNFUNC(KEYPRESSED)
	{
		return (GetAsyncKeyState( params[1] ) < 0);
	}
	PWNFUNC(KEYDOWN)
	{
		return (GetAsyncKeyState( params[1] ) < 0);
	}
	PWNFUNC(DXCreateTexture)
	{
		char * tmp;
		amx_StrParam(amx,params[1],tmp);
		return D3DXCreateTextureFromFileA(origIDirect3DDevice9,tmp,&Textures[params[2]]);
	}
	PWNFUNC(DXCreateSprite)
	{
		D3DXCreateSprite( origIDirect3DDevice9, &Sprites[params[1]] );
		return 1;
	}
	PWNFUNC(DXCreateTextureHolder)
	{
		Textures.push_back(NULL);
		return Textures.size()-1;
	}
	PWNFUNC(DXCreateSpriteHolder)
	{
		Sprites.push_back(NULL);
		return Sprites.size()-1;
	}
	PWNFUNC(DXCreateD3DXMatrixHolder)
	{
		D3DXMatrixes.push_back(D3DXMATRIX());
		return D3DXMatrixes.size()-1;
	}
	PWNFUNC(DXCreateD3DXVector2Holder)
	{
		D3DXVectors2.push_back(D3DXVECTOR2());
		return D3DXVectors2.size()-1;
	}
	PWNFUNC(DXD3DXVector2Set)
	{
		D3DXVectors2[params[1]].x = amx_ctof(params[2]);
		D3DXVectors2[params[1]].y = amx_ctof(params[3]);
		return 1;
	}
	PWNFUNC(DXD3DXVector2Get)
	{
		cell* cptra;
		cell* cptrb;
		amx_GetAddr(amx, params[2], &cptra);
		amx_GetAddr(amx, params[3], &cptrb);
		*cptra = amx_ftoc(D3DXVectors2[params[1]].x);
		*cptrb = amx_ftoc(D3DXVectors2[params[1]].y);
		return 1;
	}
	PWNFUNC(DXD3DXMatrixTransformation2D)
	{
		D3DXMatrixTransformation2D((params[1] == -1) ? NULL : &D3DXMatrixes[params[1]],(params[2] == -1) ? NULL : &D3DXVectors2[params[2]],amx_ctof(params[3]),(params[4] == -1) ? NULL : &D3DXVectors2[params[4]],(params[5] == -1) ? NULL : &D3DXVectors2[params[5]],amx_ctof(params[6]),(params[7] == -1) ? NULL : &D3DXVectors2[params[7]]);
		return 1;
	}
	PWNFUNC(DXSprite_Begin)
	{
		return Sprites[params[1]]->Begin(params[2]);
	}
	PWNFUNC(DXSprite_End)
	{
		return Sprites[params[1]]->End();
	}
	PWNFUNC(DXSprite_SetTransform)
	{
		return Sprites[params[1]]->SetTransform(&D3DXMatrixes[params[2]]);
	}
	PWNFUNC(DXSprite_Draw)//D3DXRectangles
	{
		return Sprites[params[1]]->Draw(Textures[params[2]],(params[3] == -1) ? NULL : &D3DXRectangles[params[3]],(params[4] == -1) ? NULL : &D3DXVectors3[params[4]],(params[5] == -1) ? NULL : &D3DXVectors3[params[5]],params[6]);
	}
	PWNFUNC(DXCreateD3DXVector3Holder)
	{
		D3DXVectors3.push_back(D3DXVECTOR3());
		return D3DXVectors3.size()-1;
	}
	PWNFUNC(DXD3DXVector3Set)
	{
		D3DXVectors3[params[1]].x = amx_ctof(params[2]);
		D3DXVectors3[params[1]].y = amx_ctof(params[3]);
		D3DXVectors3[params[1]].z = amx_ctof(params[4]);
		return 1;
	}
	PWNFUNC(DXD3DXVector3Get)
	{
		cell* cptra;
		cell* cptrb;
		cell* cptrc;
		amx_GetAddr(amx, params[2], &cptra);
		amx_GetAddr(amx, params[3], &cptrb);
		amx_GetAddr(amx, params[4], &cptrc);
		*cptra = amx_ftoc(D3DXVectors3[params[1]].x);
		*cptrb = amx_ftoc(D3DXVectors3[params[1]].y);
		*cptrc = amx_ftoc(D3DXVectors3[params[1]].z);
		return 1;
	}
	PWNFUNC(DXCreateD3DXRectangleHolder)
	{
		D3DXRectangles.push_back(RECT());
		return D3DXRectangles.size()-1;
	}
	PWNFUNC(DXD3DXRectangleSet)
	{
		D3DXRectangles[params[1]].bottom = params[2];
		D3DXRectangles[params[1]].left = params[3];
		D3DXRectangles[params[1]].right = params[4];
		D3DXRectangles[params[1]].top = params[5];
		return 1;
	}
	PWNFUNC(DXD3DXRectangleGet)
	{
		cell* cptra;
		cell* cptrb;
		cell* cptrc;
		cell* cptrd;
		amx_GetAddr(amx, params[2], &cptra);
		amx_GetAddr(amx, params[3], &cptrb);
		amx_GetAddr(amx, params[4], &cptrc);
		amx_GetAddr(amx, params[5], &cptrd);
		*cptra = D3DXRectangles[params[1]].bottom;
		*cptrb = D3DXRectangles[params[1]].left;
		*cptrc = D3DXRectangles[params[1]].right;
		*cptrd = D3DXRectangles[params[1]].top;
		return 1;
	}
	PWNFUNC(GetBackBufferHeight)
	{
		return pPresentParam.BackBufferHeight;
	}
	PWNFUNC(GetBackBufferWidth)
	{
		return pPresentParam.BackBufferWidth;
	}
	PWNFUNC(DXTextureRelease)
	{
		SAFE_RELEASE(Textures[params[1]]);
		return 1;
	}
	PWNFUNC(DXSpriteRelease)
	{
		SAFE_RELEASE(Sprites[params[1]]);
		return 1;
	}
	PWNFUNC(DXTextureIsNull)
	{
		return (Textures[params[1]] == NULL) ? 1 : 0;
	}
	PWNFUNC(DXSpriteIsNull)
	{
		return (Sprites[params[1]] == NULL) ? 1 : 0;
	}
	PWNFUNC(EmulateKeyPressINPUT)
	{
		// This structure will be used to create the keyboard
		// input event.
		INPUT ip;

		// Set up a generic keyboard event.
		ip.type = INPUT_KEYBOARD;
		ip.ki.dwFlags = 0;
		if(params[4] == 1)
		{
			ip.ki.wScan = params[2]; // hardware scan code for key
			ip.ki.wVk = 0; // virtual-key code for the  key
			ip.ki.dwFlags |= KEYEVENTF_SCANCODE;
		}
		else
		{
			ip.ki.wScan = 0; // hardware scan code for key
			ip.ki.wVk = params[2]; // virtual-key code for the  key
		}

		ip.ki.time = 0;
		ip.ki.dwExtraInfo = 0;

		if(params[3] == 1)
		{
			ip.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
		}
		if(params[1] == 1)
		{
			ip.ki.dwFlags |= KEYEVENTF_KEYUP;
		}
		return SendInput(1, &ip, sizeof(INPUT));
	}
	PWNFUNC(PWNGetLastError)
	{
		return GetLastError();
	}
	PWNFUNC(printf_f)
	{
		int len;
		Log("%s",format_amxstring(amx, params, 1, len));
		return 0;
	}
	PWNFUNC(ppGetVehicleMatrix)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{		
			CMatrix VehRot;
			gsVehicle->GetMatrix(&VehRot);
			cell* cptr[12];
			for(unsigned short x = 0; x < 12; ++x)
			{
				amx_GetAddr(amx, params[x+2], &cptr[x]);
			}	
			*cptr[0] = amx_ftoc(VehRot.vRight.fX);
			*cptr[1] = amx_ftoc(VehRot.vRight.fY);
			*cptr[2] = amx_ftoc(VehRot.vRight.fZ);
			*cptr[3] = amx_ftoc(VehRot.vFront.fX);
			*cptr[4] = amx_ftoc(VehRot.vFront.fY);
			*cptr[5] = amx_ftoc(VehRot.vFront.fZ);
			*cptr[6] = amx_ftoc(VehRot.vUp.fX);
			*cptr[7] = amx_ftoc(VehRot.vUp.fY);
			*cptr[8] = amx_ftoc(VehRot.vUp.fZ);
			*cptr[9] = amx_ftoc(VehRot.vPos.fX);
			*cptr[10] = amx_ftoc(VehRot.vPos.fY);
			*cptr[11] = amx_ftoc(VehRot.vPos.fZ);
			return 1;
		}
		return 0;
	}
	PWNFUNC(ppSetVehicleMatrix)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{		
			CMatrix VehRot;
			VehRot.vRight = CVector(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4]));
			VehRot.vFront = CVector(amx_ctof(params[5]),amx_ctof(params[6]),amx_ctof(params[7]));
			VehRot.vUp =    CVector(amx_ctof(params[8]),amx_ctof(params[9]),amx_ctof(params[10]));
			VehRot.vPos =   CVector(amx_ctof(params[11]),amx_ctof(params[12]),amx_ctof(params[13]));
			gsVehicle->SetMatrix(&VehRot);
			return 1;
		}
		return 0;
	}
	PWNFUNC(ppRotateVehicleMatrix)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{		
			CMatrix VehRot;
			VehRot.vRight = CVector(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4]));
			VehRot.vFront = CVector(amx_ctof(params[5]),amx_ctof(params[6]),amx_ctof(params[7]));
			VehRot.vUp =    CVector(amx_ctof(params[8]),amx_ctof(params[9]),amx_ctof(params[10]));
			VehRot.vPos =   CVector(amx_ctof(params[11]),amx_ctof(params[12]),amx_ctof(params[13]));
			CMatrix VehRotated;
			VehRotated = VehRot.Rotate(&CVector(amx_ctof(params[14]),amx_ctof(params[15]),amx_ctof(params[16])),amx_ctof(params[17]));
			cell* cptr[12];
			for(unsigned short x = 0; x < 12; ++x)
			{
				amx_GetAddr(amx, params[x+18], &cptr[x]);
			}	
			*cptr[0] = amx_ftoc(VehRotated.vRight.fX);
			*cptr[1] = amx_ftoc(VehRotated.vRight.fY);
			*cptr[2] = amx_ftoc(VehRotated.vRight.fZ);
			*cptr[3] = amx_ftoc(VehRotated.vFront.fX);
			*cptr[4] = amx_ftoc(VehRotated.vFront.fY);
			*cptr[5] = amx_ftoc(VehRotated.vFront.fZ);
			*cptr[6] = amx_ftoc(VehRotated.vUp.fX);
			*cptr[7] = amx_ftoc(VehRotated.vUp.fY);
			*cptr[8] = amx_ftoc(VehRotated.vUp.fZ);
			*cptr[9] = amx_ftoc(VehRotated.vPos.fX);
			*cptr[10] = amx_ftoc(VehRotated.vPos.fY);
			*cptr[11] = amx_ftoc(VehRotated.vPos.fZ);
			return 1;
		}
		return 0;
	}
	PWNFUNC(ppGetVehicleRotation)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{		
			CMatrix VehRot;
			gsVehicle->GetMatrix(&VehRot);
			cell* cptr[6];
			for(unsigned short x = 0; x < 6; ++x)
			{
				amx_GetAddr(amx, params[x+3], &cptr[x]);
			}	
			float temp[6] = {0.0f};
			switch(params[2])
			{
			case 0:
				temp[0] = VehRot.vRight.GetAngleDegreesXY();
				temp[1] = VehRot.vRight.GetAngleDegreesYX();
				temp[2] = VehRot.vRight.GetAngleDegreesZY();
				temp[3] = VehRot.vRight.GetAngleDegreesYZ();
				temp[4] = VehRot.vRight.GetAngleDegreesZX();
				temp[5] = VehRot.vRight.GetAngleDegreesXZ();
			break;
			case 1:
				temp[0] = VehRot.vFront.GetAngleDegreesXY();
				temp[1] = VehRot.vFront.GetAngleDegreesYX();
				temp[2] = VehRot.vFront.GetAngleDegreesZY();
				temp[3] = VehRot.vFront.GetAngleDegreesYZ();
				temp[4] = VehRot.vFront.GetAngleDegreesZX();
				temp[5] = VehRot.vFront.GetAngleDegreesXZ();
			break;
			case 2:
				temp[0] = VehRot.vUp.GetAngleDegreesXY();
				temp[1] = VehRot.vUp.GetAngleDegreesYX();
				temp[2] = VehRot.vUp.GetAngleDegreesZY();
				temp[3] = VehRot.vUp.GetAngleDegreesYZ();
				temp[4] = VehRot.vUp.GetAngleDegreesZX();
				temp[5] = VehRot.vUp.GetAngleDegreesXZ();
			break;
			case 3:
				temp[0] = VehRot.vPos.GetAngleDegreesXY();
				temp[1] = VehRot.vPos.GetAngleDegreesYX();
				temp[2] = VehRot.vPos.GetAngleDegreesZY();
				temp[3] = VehRot.vPos.GetAngleDegreesYZ();
				temp[4] = VehRot.vPos.GetAngleDegreesZX();
				temp[5] = VehRot.vPos.GetAngleDegreesXZ();
			break;
			}
			*cptr[0] = amx_ftoc(temp[0]);
			*cptr[1] = amx_ftoc(temp[1]);
			*cptr[2] = amx_ftoc(temp[2]);
			*cptr[3] = amx_ftoc(temp[3]);
			*cptr[4] = amx_ftoc(temp[4]);
			*cptr[5] = amx_ftoc(temp[5]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(ppGetVehicleRotationXYZ)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{		
			CMatrix VehRot;
			gsVehicle->GetMatrix(&VehRot);
			cell* cptr[3];
			for(unsigned short x = 0; x < 3; ++x)
			{
				amx_GetAddr(amx, params[x+2], &cptr[x]);
			}	
			float temp[3] = {0.0f};
			Functions::ConvertMatrixToEulerAngles(VehRot,temp[0],temp[1],temp[2]);
			temp[0] *= 57.2957795f;
			temp[1] *= 57.2957795f;
			temp[2] *= 57.2957795f;
			*cptr[0] = amx_ftoc(temp[0]);
			*cptr[1] = amx_ftoc(temp[1]);
			*cptr[2] = amx_ftoc(temp[2]);
			return 1;
		}
		return 0;
	}
	
	PWNFUNC(ReloadMe)
	{
		reload_scripts_amx.push_back(amx);
		return 1;
	}
	PWNFUNC(ReloadScript)
	{
		char * tmp;
		amx_StrParam(amx,params[1],tmp);
		for(std::vector<ScriptInfo>::iterator it = scripts.begin(); it != scripts.end(); ++it) 
		{
			if(stricmp(it->Identifier.c_str(),tmp) == 0)
			{
				reload_scripts_amx.push_back(&it->amx);
				return 1;
			}
		}
		return 0;
	}
	PWNFUNC(pLoadScript)
	{
		char * tmp;
		amx_StrParam(amx,params[1],tmp);
		return LoadScript(std::string(tmp));
	}
	PWNFUNC(pUnloadScript)
	{
		char * tmp;
		amx_StrParam(amx,params[1],tmp);
		return UnloadScript(std::string(tmp));
	}
	PWNFUNC(ppLoadDLL)
	{
		char * tmp;
		amx_StrParam(amx,params[1],tmp);
		return LoadPlugin(std::string(tmp),params[2]);
	}
	PWNFUNC(ppUnloadDLL)
	{
		char * tmp;
		amx_StrParam(amx,params[1],tmp);
		return UnloadPlugin(std::string(tmp));
	}
	PWNFUNC(ppProcessLineOfSight)
	{
		CVector start(amx_ctof(params[1]),amx_ctof(params[2]),amx_ctof(params[3]));
		CVector end(amx_ctof(params[4]),amx_ctof(params[5]),amx_ctof(params[6]));
		CColPoint* pCollision = NULL;
		cell* cptr[3];
		for(unsigned short x = 0; x < 3; ++x)
		{
			amx_GetAddr(amx, params[x+7], &cptr[x]);
		}	
		if(pGameInterface->GetWorld()->ProcessLineOfSight(&start,&end,&pCollision,NULL))
		{
			*cptr[0] = amx_ftoc(pCollision->GetPosition()->fX);
			*cptr[1] = amx_ftoc(pCollision->GetPosition()->fY);
			*cptr[2] = amx_ftoc(pCollision->GetPosition()->fZ);
		}
		else
		{
			*cptr[0] = params[4];
			*cptr[1] = params[5];
			*cptr[2] = params[6];
		}
		if(pCollision)
		{
			pCollision->Destroy();
		}
		return 1;
	}
	PWNFUNC(Terminate)
	{
		ExitProcess(0);
		return 1;
	}
	PWNFUNC(DrawArea)
	{
		pGameInterface->GetRadar()->DrawAreaOnRadar(amx_ctof(params[1]),amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4]),params[5]);
		return 1;
	}
	PWNFUNC(IsValidPed)
	{
		gsPed = pPools->GetPed( params[1] );
		if(gsPed != NULL)
			return 1;
		return 0;
	}
	PWNFUNC(IsValidVehicle)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
			return 1;
		return 0;		
	}
	PWNFUNC(GetPedCount)
	{
		return pPools->GetPedCount();
	}
	PWNFUNC(GetVehicleCount)
	{
		return pPools->GetVehicleCount();
	}
	PWNFUNC(GetPedPoolSize)
	{
		return pool_actor->size;
	}
	PWNFUNC(GetVehiclePoolSize)
	{
		return pool_vehicle->size;
	}
	PWNFUNC(AddVehicle)
	{	
		pGameInterface->GetModelInfo ( params[1] )->AddRef ( true );
		gsVehicle = pGameInterface->GetPools()->AddVehicle((static_cast < eVehicleTypes > (params[1])));
		if(gsVehicle != NULL)
		{
			gsVehicle->SetPosition(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4]));
			//gsVehicle->SetDirection(&CVector(0.0,amx_ctof(params[5]),0.0));
			//gsVehicle->SetColor(params[6],params[7],params[8],params[9]);
			return gsVehicle->GetArrayID();
		}
		return -1;
	}
	PWNFUNC(GetPedVehicle)
	{
		gsVehicle = pPools->GetPed( params[1] )->GetVehicle();
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetArrayID();
		}
		return -1;
	}
	/*
	PWNFUNC(SomeVehicleFunctions)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return 1;
		}
		return 0;
	}
	*/
	PWNFUNC(vDestroy)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			pPools->RemoveVehicle(gsVehicle);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vGetDriver)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsPed = gsVehicle->GetDriver();
			if(gsPed != NULL)
			{
				return gsPed->GetArrayID();
			}
		}
		return -1;
	}
	PWNFUNC(vGetPassenger)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsPed = gsVehicle->GetPassenger(params[2]);
			if(gsPed != NULL)
			{
				return gsPed->GetArrayID();
			}
		}
		return -1;
	}
	PWNFUNC(vIsBeingDriven)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->IsBeingDriven();
		}
		return 0;
	}
	PWNFUNC(vCanPedEnterCar)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->CanPedEnterCar();
		}
		return 0;
	}
	PWNFUNC(vCanPedJumpOutCar)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsPed = pPools->GetPed( params[2] );
			if(gsPed != NULL)
			{
				return gsVehicle->CanPedJumpOutCar(gsPed);
			}
		}
		return 0;
	}
	PWNFUNC(vRemoveUpgrade)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->RemoveVehicleUpgrade(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vCanPedLeanOut)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsPed = pPools->GetPed( params[2] );
			if(gsPed != NULL)
			{
				return gsVehicle->CanPedLeanOut(gsPed);
			}
		}
		return 0;
	}
	PWNFUNC(vCanPedStepOutCar)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->CanPedStepOutCar(params[2]);
		}
		return 0;
	}
	PWNFUNC(vLockDoors)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->LockDoors(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetDoorsUndamagable)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetDoorsUndamageable(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vGetBaseType)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetBaseVehicleType();
		}
		return 0;
	}
	PWNFUNC(vSetBodyDirtLevel)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetBodyDirtLevel(amx_ctof(params[2]));
			return 1;
		}
		return 0;
	}
	PWNFUNC(vGetBodyDirtLevel)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		float temp = 0.0f;
		if(gsVehicle != NULL)
		{
			temp = gsVehicle->GetBodyDirtLevel();
		}
		return amx_ftoc(temp);
	}
	PWNFUNC(vGetHeightAboveRoad)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		float temp = 0.0f;
		if(gsVehicle != NULL)
		{
			temp = gsVehicle->GetHeightAboveRoad();
		}
		return amx_ftoc(temp);
	}
	PWNFUNC(vGetSteerAngle)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		float temp = 0.0f;
		if(gsVehicle != NULL)
		{
			temp = gsVehicle->GetSteerAngle();
		}
		return amx_ftoc(temp);
	}
	//GetTowBarPos
	//GetTowHitchPos
	PWNFUNC(vIsOnItsSide)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->IsOnItsSide();
		}
		return 0;
	}
	PWNFUNC(vIsLawEnforcment)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->IsLawEnforcementVehicle();
		}
		return 0;
	}
	PWNFUNC(vIsPedPassenger)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsPed = pPools->GetPed( params[2] );
			if(gsPed != NULL)
			{
				return gsVehicle->IsPassenger(gsPed);
			}
		}
		return 0;
	}
	PWNFUNC(vIsSphereTouching)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->IsSphereTouchingVehicle(&CVector(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4])),amx_ctof(params[5]));
		}
		return 0;
	}
	PWNFUNC(vIsUpsideDown)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->IsUpsideDown();
		}
		return 0;
	}
	//MakeDirty
	PWNFUNC(vIsEngineBroken)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->IsEngineBroken();
		}
		return 0;
	}
	PWNFUNC(vSetEngineBroken)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetEngineBroken(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vIsScriptLocked)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->IsScriptLocked();
		}
		return 0;
	}
	PWNFUNC(vSetScriptLocked)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetScriptLocked(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vPlaceBikeOnRoadProperly)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->PlaceBikeOnRoadProperly();
			return 1;
		}
		return 0;
	}
	PWNFUNC(vPlaceAutomobileOnRoadProperly)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->PlaceAutomobileOnRoadProperly();
			return 1;
		}
		return 0;
	}
	PWNFUNC(vIsSirenOrAlarmActive)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->IsSirenOrAlarmActive();
		}
		return 0;
	}
	PWNFUNC(vSetSirenOrAlarmActive)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetSirenOrAlarmActive(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vGetAlpha)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetAlpha();
		}
		return 0;
	}
	PWNFUNC(vSetAlpha)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetAlpha(params[2]);
			return 1;
		}
		return 0;
	}
	//BlowUp
	//BlowUpCutSceneNoExtras
	PWNFUNC(vFuckCompletely)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->GetDamageManager()->FuckCarCompletely(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vGetAeroplaneCompStatus)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetDamageManager()->GetAeroplaneCompStatus(params[2]);
		}
		return 0;
	}
	PWNFUNC(vGetDoorStatus)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetDamageManager()->GetDoorStatus(static_cast < eDoors > (params[2]));
		}
		return 0;
	}
	PWNFUNC(vGetEnginestatus)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetDamageManager()->GetEngineStatus();
		}
		return 0;
	}
	PWNFUNC(vGetLightStatus)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetDamageManager()->GetLightStatus(params[2]);
		}
		return 0;
	}
	PWNFUNC(vGetPanelStatus)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetDamageManager()->GetPanelStatus(params[2]);
		}
		return 0;
	}
	PWNFUNC(vGetWheelStatus)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetDamageManager()->GetWheelStatus(static_cast <eWheels> (params[2]));
		}
		return 0;
	}
	PWNFUNC(vSetAeroplaneCompStatus)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->GetDamageManager()->SetAeroplaneCompStatus(params[2],params[3]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetDoorStatus)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->GetDamageManager()->SetDoorStatus(static_cast <eDoors> (params[2]),params[3]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetEngineStatus)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->GetDamageManager()->SetEngineStatus(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetLightStatus)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->GetDamageManager()->SetLightStatus(params[2],params[3]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetPanelStatus)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->GetDamageManager()->SetPanelStatus(params[2],params[3]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetWheelStatus)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->GetDamageManager()->SetWheelStatus(static_cast <eWheels> (params[2]),params[3]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vFadeOut)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->FadeOut(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vIsFadingOut)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->IsFadingOut();
		}
		return 0;
	}
	PWNFUNC(vSetTowLink)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gssVehicle = pPools->GetVehicle( params[2] );
			if(gssVehicle != NULL)
			{	
				return gsVehicle->SetTowLink(gssVehicle);
			}
		}
		return 0;
	}
	PWNFUNC(vBreaktowLink)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->BreakTowLink();
		}
		return 0;
	}
	PWNFUNC(vGetTowedVehicle)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gssVehicle = gsVehicle->GetTowedVehicle();
			if(gssVehicle != NULL)
			{	
				return gssVehicle->GetArrayID();
			}
		}
		return 0;
	}
	PWNFUNC(vGetTowedByVehicle)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gssVehicle = gsVehicle->GetTowedByVehicle();
			if(gssVehicle != NULL)
			{	
				return gssVehicle->GetArrayID();
			}
		}
		return 0;
	}
	PWNFUNC(vSetWinchType)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetWinchType(static_cast < eWinchType > (params[2]));
			return 1;
		}
		return 0;
	}
	//PickupEntityWithWinch
	PWNFUNC(vReleasePickedUpEntityWithWinch)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->ReleasePickedUpEntityWithWinch();
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetRopeHeightForHeli)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetRopeHeightForHeli(amx_ctof(params[2]));
			return 1;
		}
		return 0;
	}
	PWNFUNC(vGetPaintjob)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetRemapIndex();
		}
		return 0;
	}
	PWNFUNC(vIsDamaged)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->IsDamaged();
		}
		return 0;
	}
	PWNFUNC(vIsDrowning)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->IsDrowning();
		}
		return 0;
	}
	PWNFUNC(vIsEngineOn)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->IsEngineOn();
		}
		return 0;
	}
	PWNFUNC(vIsHandbrakeOn)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->IsHandbrakeOn();
		}
		return 0;
	}
	PWNFUNC(vGetAlwaysLeaveSkidMarks)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetAlwaysLeaveSkidMarks();
		}
		return 0;
	}
	PWNFUNC(vGetComedyControls)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetComedyControls();
		}
		return 0;
	}
	PWNFUNC(vGetGunSwitchedOff)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetGunSwitchedOff();
		}
		return 0;
	}
	PWNFUNC(vGetLightsOn)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetLightsOn();
		}
		return 0;
	}
	PWNFUNC(vGetOverrideLights)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetOverrideLights();
		}
		return 0;
	}
	PWNFUNC(vGetTakeLessDamage)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetTakeLessDamage();
		}
		return 0;
	}
	PWNFUNC(vGetTyresDontBurst)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetTyresDontBurst();
		}
		return 0;
	}
	PWNFUNC(vGetAdjustablePropertyValue)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetAdjustablePropertyValue();
		}
		return 0;
	}
	PWNFUNC(vGetHeliRotorSpeed)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		float temp = 0.0f;
		if(gsVehicle != NULL)
		{
			temp = gsVehicle->GetHeliRotorSpeed();
		}
		return amx_ftoc(temp);
	}
	PWNFUNC(vGetExplodeTime)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetExplodeTime();
		}
		return 0;
	}
	PWNFUNC(vSetAlwaysLeaveSkidMarks)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetAlwaysLeaveSkidMarks(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetCanBeDamaged)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetCanBeDamaged(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetCanBeTargettedByHeatSeekingMissiles)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetCanBeTargettedByHeatSeekingMissiles(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetCanShootPetrolTank)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetCanShootPetrolTank(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetComedyControls)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetComedyControls(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetEngineOn)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetEngineOn(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetGunSwitchedOff)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetGunSwitchedOff(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetHandbrakeOn)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetHandbrakeOn(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetLightsOn)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetLightsOn(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetOverrideLights)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetOverrideLights(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetTakeLessDamage)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetTakeLessDamage(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetTyresDontBurst)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetTyresDontBurst(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetAdjustablePropertyValue)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetAdjustablePropertyValue(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetHeliRotorSpeed)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetHeliRotorSpeed(amx_ctof(params[2]));
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetTaxiLightOn)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetTaxiLightOn(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vGetBikeWheelStatus)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetBikeWheelStatus(params[2]);
		}
		return 0;
	}
	PWNFUNC(vSetBikeWheelStatus)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetBikeWheelStatus(params[2],params[3]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vIsOnScreen)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->IsOnScreen();
		}
		return 0;
	}
	PWNFUNC(vIsVisible)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->IsVisible();
		}
		return 0;
	}
	PWNFUNC(vGetHealth)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		float temp = 0.0f;
		if(gsVehicle != NULL)
		{
			temp = gsVehicle->GetHealth();
		}
		return amx_ftoc(temp);
	}
	PWNFUNC(vSetHealth)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetHealth(amx_ctof(params[2]));
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetLandingGearDown)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetLandingGearDown(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vIsLandingGearDown)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->IsLandingGearDown();
		}
		return 0;
	}
	PWNFUNC(vGetLandingGearPosition)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		float temp = 0.0f;
		if(gsVehicle != NULL)
		{
			temp = gsVehicle->GetLandingGearPosition();
		}
		return amx_ftoc(temp);
	}
	PWNFUNC(vSetLandingGearPosition)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetLandingGearPosition(amx_ctof(params[2]));
			return 1;
		}
		return 0;
	}
	PWNFUNC(vGetTurretRotation)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			float hor = 0.0f,ver = 0.0f;
			gsVehicle->GetTurretRotation(&hor,&ver);
			cell* cptr[2];
			for(unsigned short x = 0; x < 2; ++x)
			{
				amx_GetAddr(amx, params[x+2], &cptr[x]);
			}	

			*cptr[0] = amx_ftoc(hor);
			*cptr[1] = amx_ftoc(ver);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetTurretRotation)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetTurretRotation(amx_ctof(params[2]),amx_ctof(params[3]));
			return 1;
		}
		return 0;
	}
	PWNFUNC(vGetNumberGettingIn)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetNumberGettingIn();
		}
		return 0;
	}
	PWNFUNC(vGetPassengerCount)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetPassengerCount();
		}
		return 0;
	}
	PWNFUNC(vGetMaxPassengerCount)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetMaxPassengerCount();
		}
		return 0;
	}
	PWNFUNC(vIsSmokeTrailEnabled)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->IsSmokeTrailEnabled();
		}
		return 0;
	}
	PWNFUNC(vSetSmokeTrailEnabled)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetSmokeTrailEnabled(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vGetGravity)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{		
			CVector VehRot;
			gsVehicle->GetGravity(&VehRot);
			cell* cptr[3];
			for(unsigned short x = 0; x < 3; ++x)
			{
				amx_GetAddr(amx, params[x+2], &cptr[x]);
			}	
			*cptr[0] = amx_ftoc(VehRot.fX);
			*cptr[1] = amx_ftoc(VehRot.fY);
			*cptr[2] = amx_ftoc(VehRot.fZ);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetGravity)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{		
			gsVehicle->SetGravity(&CVector(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4])));
			return 1;
		}
		return 0;
	}
	PWNFUNC(vGetHeadLightColor)//ulargb
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetHeadLightColor().ulARGB;
		}
		return 0;
	}
	PWNFUNC(vSetHeadLightColor)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetHeadLightColor(SharedUtil::RGBA(params[2]));
			return 1;
		}
		return 0;
	}
	//SpawnFlyingComponent
	PWNFUNC(vSetWheelVisibility)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetWheelVisibility(static_cast < eWheels > (params[2]),params[3]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vIsHeliSearchLightVisible)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->IsHeliSearchLightVisible();
		}
		return 0;
	}
	PWNFUNC(vSetHeliSearchLightVisible)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetHeliSearchLightVisible(params[2]);
			return 1;
		}
		return 0;
	}
	//GetSpecialColModel
	PWNFUNC(vUpdateMovingCollision)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->UpdateMovingCollision(amx_ctof(params[2]));
		}
		return 0;
	}
	PWNFUNC(vSetMoveSpeed)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetMoveSpeed(&CVector(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4])));
			return 1;
		}
		return 0;
	}
	PWNFUNC(vGetMoveSpeed)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		CVector pMs;
		if(gsVehicle != NULL)
		{
			gsVehicle->GetMoveSpeed(&pMs);
			cell* cptra;
			cell* cptrb;
			cell* cptrc;
			amx_GetAddr(amx, params[2], &cptra);
			amx_GetAddr(amx, params[3], &cptrb);
			amx_GetAddr(amx, params[4], &cptrc);
			*cptra = amx_ftoc(pMs.fX);
			*cptrb = amx_ftoc(pMs.fY);
			*cptrc = amx_ftoc(pMs.fZ);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetOrientation)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetOrientation(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4]));
			return 1;
		}
		return 0;
	}
	PWNFUNC(vGetDamageImpulseMagnitude)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			float temp = 0.0f;
			temp = gsVehicle->GetDamageImpulseMagnitude();
			return amx_ftoc(temp);
		}
		return 0;
	}
	PWNFUNC(vSetDamageImpulseMagnitude)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetDamageImpulseMagnitude(amx_ctof(params[2]));
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetTurnSpeed)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetTurnSpeed(&CVector(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4])));
			return 1;
		}
		return 0;
	}
	PWNFUNC(vGetTurnSpeed)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		CVector pMs;
		if(gsVehicle != NULL)
		{
			gsVehicle->GetTurnSpeed(&pMs);
			cell* cptra;
			cell* cptrb;
			cell* cptrc;
			amx_GetAddr(amx, params[2], &cptra);
			amx_GetAddr(amx, params[3], &cptrb);
			amx_GetAddr(amx, params[4], &cptrc);
			*cptra = amx_ftoc(pMs.fX);
			*cptrb = amx_ftoc(pMs.fY);
			*cptrc = amx_ftoc(pMs.fZ);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetMass)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetMass(amx_ctof(params[2]));
			return 1;
		}
		return 0;
	}
	PWNFUNC(vGetMass)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		float temp = 0.0f;
		if(gsVehicle != NULL)
		{
			temp = gsVehicle->GetMass();
		}
		return amx_ftoc(temp);
	}
	PWNFUNC(vSetTurnMass)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetTurnMass(amx_ctof(params[2]));
			return 1;
		}
		return 0;
	}
	PWNFUNC(vGetTurnMass)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		float temp = 0.0f;
		if(gsVehicle != NULL)
		{
			temp = gsVehicle->GetTurnMass();
		}
		return amx_ftoc(temp);
	}
	PWNFUNC(vSetElasticity)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetElasticity(amx_ctof(params[2]));
			return 1;
		}
		return 0;
	}
	PWNFUNC(vGetElasticity)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		float temp = 0.0f;
		if(gsVehicle != NULL)
		{
			temp = gsVehicle->GetElasticity();
		}
		return amx_ftoc(temp);
	}
	PWNFUNC(vSetBuoyancyConstant)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetBuoyancyConstant(amx_ctof(params[2]));
			return 1;
		}
		return 0;
	}
	PWNFUNC(vGetBuoyancyConstant)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		float temp = 0.0f;
		if(gsVehicle != NULL)
		{
			temp = gsVehicle->GetBuoyancyConstant();
		}
		return amx_ftoc(temp);
	}
	PWNFUNC(vResetLastDamage)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->ResetLastDamage();
			return 1;
		}
		return 0;
	}
	PWNFUNC(vGetLighting)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		float temp = 0.0f;
		if(gsVehicle != NULL)
		{
			temp = gsVehicle->GetLighting();
		}
		return amx_ftoc(temp);
	}
	PWNFUNC(vSetLighting)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetLighting(amx_ctof(params[2]));
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetPosition)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetPosition(&CVector(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4])));
			return 1;
		}
		return 0;
	}
	PWNFUNC(vTeleport)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->Teleport(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4]));
			return 1;
		}
		return 0;
	}
	PWNFUNC(vFixBoatOrientation)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->FixBoatOrientation();
			return 1;
		}
		return 0;
	}
	PWNFUNC(vGetPosition)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		CVector * pMs;
		if(gsVehicle != NULL)
		{
			pMs = gsVehicle->GetPosition();
			cell* cptra;
			cell* cptrb;
			cell* cptrc;
			amx_GetAddr(amx, params[2], &cptra);
			amx_GetAddr(amx, params[3], &cptrb);
			amx_GetAddr(amx, params[4], &cptrc);
			*cptra = amx_ftoc(pMs->fX);
			*cptrb = amx_ftoc(pMs->fY);
			*cptrc = amx_ftoc(pMs->fZ);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetRoll)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetRoll(&CVector(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4])));
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetDirection)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetDirection(&CVector(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4])));
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetWas)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetWas(&CVector(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4])));
			return 1;
		}
		return 0;
	}
	PWNFUNC(vGetModelIndex)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetModelIndex();
		}
		return 0;
	}
	PWNFUNC(vGetDistanceFromCentreOfMassToBaseOfModel)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		float temp = 0.0f;
		if(gsVehicle != NULL)
		{
			temp = gsVehicle->GetDistanceFromCentreOfMassToBaseOfModel();
		}
		return amx_ftoc(temp);
	}
	PWNFUNC(vSetEntityStatus)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetEntityStatus(static_cast < eEntityStatus > (params[2]));
			return 1;
		}
		return 0;
	}
	PWNFUNC(vSetVisible)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetVisible(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vGetEntityStatus)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetEntityStatus();
		}
		return 0;
	}
	PWNFUNC(vGetAreaCode)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetAreaCode();
		}
		return 0;
	}
	PWNFUNC(vSetAreaCode)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetAreaCode(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(vGetImmunities)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			cell* cptr[8] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
			for(int i = 0; i < 8; ++i)
				amx_GetAddr(amx, params[i+2], &cptr[i]);
			bool immuns[8];
			gsVehicle->GetImmunities(immuns[0],immuns[1],immuns[2],immuns[3],immuns[4],immuns[5],immuns[6],immuns[7]);
			for(int i = 0; i < 8; ++i)
				*cptr[i] = immuns[i];
			return 1;
		}
		return 0;
	}
	PWNFUNC(vGetUnderwater)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			return gsVehicle->GetUnderwater();
		}
		return 0;
	}
	PWNFUNC(vSetUnderwater)
	{
		gsVehicle = pPools->GetVehicle( params[1] );
		if(gsVehicle != NULL)
		{
			gsVehicle->SetUnderwater(params[2]);
			return 1;
		}
		return 0;
	}
	PWNFUNC(kh_DOWN)
	{
		return KEY_DOWN(params[1]);
	}
	PWNFUNC(kh_UP)
	{
		return KEY_UP(params[1]);
	}
	PWNFUNC(kh_PRESSED)
	{
		return KEY_PRESSED(params[1]);
	}
	PWNFUNC(kh_RELEASED)
	{
		return KEY_RELEASED(params[1]);
	}
	PWNFUNC(kh_CONSUME)
	{
		KEY_CONSUME(params[1]);
		return 1;
	}
	PWNFUNC(AddPlayerMoney)
	{
		pGameInterface->GetPlayerInfo()->AddPlayerMoney(params[1]);
		return 1;
	}
	//
	PWNFUNC(AddPed)
	{	
		pGameInterface->GetModelInfo ( params[1] )->AddRef ( true );
		gsPed = pGameInterface->GetPools()->AddPed(static_cast < ePedModel > (params[1]));
		if(gsPed != NULL)
		{
			gsPed->SetPosition(amx_ctof(params[2]),amx_ctof(params[3]),amx_ctof(params[4]));
			return gsPed->GetArrayID();
		}
		return -1;
	}

	PWNFUNC(ScriptCarId)
	{
		return GetFromPool( (DWORD) pGameInterface->GetPools()->GetVehicle(params[1]), VEHICLE_POOL_POINTER, FUNC_GET_CAR_ID );
	}

	PWNFUNC(ScriptPedId)
	{
		return GetFromPool( (DWORD) pGameInterface->GetPools()->GetPed(params[1]), ACTOR_POOL_POINTER, FUNC_GET_ACTOR_ID );
	}

	PWNFUNC(ScriptObjectId)
	{
		return GetFromPool( (DWORD) pGameInterface->GetPools()->GetObjectA(params[1]), OBJECT_POOL_POINTER, FUNC_GET_OBJECT_ID );
	}

	PWNFUNC(GetVehIDFromScriptVehID)
	{
		return ((CVehicle *)GetFromPool( (DWORD) params[1], VEHICLE_POOL_POINTER, FUNC_GET_CAR_STRUCT ))->GetArrayID();
	}

	PWNFUNC(GetPedIDFromScriptPedID)
	{
		return ((CPed *)GetFromPool( (DWORD) params[1], ACTOR_POOL_POINTER, FUNC_GET_ACTOR_STRUCT ))->GetArrayID();
	}

	PWNFUNC(GetObjIDFromScriptObjID)
	{
		return ((CObject *)GetFromPool( (DWORD) params[1], OBJECT_POOL_POINTER, FUNC_GET_OBJECT_STRUCT ))->GetArrayID();
	}

	//#define PWNFUNC(a) static cell AMX_NATIVE_CALL a(AMX *amx, cell *params)
	PWNFUNC(ExecuteSCMOpcode)
	{
		SCRIPT_COMMAND Executor;
		Executor.OpCode = params[1];
		amx_StrParam(amx,params[2],Executor.Params);
		return ScriptCommand(&Executor,amx,params);
	}
	//
	PWNFUNC(SPX_SpawnCar)	// SpawnCar(modelid)
	{
		TextDraw::SpawnCar(params[1]);
		return 1;
	}
	PWNFUNC(SPX_DrawText)	// DrawText(Float:x, Float:Y, Float:scaleX, Float:ScaleY, font, aligment, red, green, blue, alpha, outline, shadow, text[])
	{
		char *text;
		amx_StrParam(amx, params[13], text);

		TextDraw::DrawTextA(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]), params[5], params[6], params[7], 
			params[8], params[9], params[10], params[11], params[12], text);

		return 1;
	}
	PWNFUNC(SPX_DrawBox)		// DrawBox(Float:x1, Float:y1, Float:x2, Float:y2, red, green, blue, alpha)
	{
		TextDraw::DrawBox(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]), params[5], params[6], params[7], params[8]);
		return 1;
	}

	//
	namespace Camera
	{
		PWNFUNC(VectorTrackLinear)
		{
			pGameInterface->GetCamera()->VectorTrackLinear(&CVector(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3])),&CVector(amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6])),amx_ctof(params[7]),params[8] == 1);
			return 1;
		}
		PWNFUNC(Fade)
		{
			pGameInterface->GetCamera()->Fade(amx_ctof(params[1]),params[2]);
			return 1;
		}
		PWNFUNC(Find3rdPersonCamTargetVector)
		{
			pGameInterface->GetCamera()->Find3rdPersonCamTargetVector(amx_ctof(params[1]),&CVector(amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4])),&CVector(amx_ctof(params[5]), amx_ctof(params[6]), amx_ctof(params[7])),&CVector(amx_ctof(params[8]), amx_ctof(params[9]), amx_ctof(params[10])));
			return 1;
		}

		PWNFUNC(Find3rdPersonQuickAimPitch)
		{
			float temp = pGameInterface->GetCamera()->Find3rdPersonQuickAimPitch();
			return amx_ftoc(temp);
		}

		PWNFUNC(GetActiveCam)
		{
			return pGameInterface->GetCamera()->GetActiveCam();
		}

		PWNFUNC(GetFixedModeSource)
		{	
			cell* cptra;
			cell* cptrb;
			cell* cptrc;
			amx_GetAddr(amx, params[2], &cptra);
			amx_GetAddr(amx, params[3], &cptrb);
			amx_GetAddr(amx, params[4], &cptrc);
			*cptra = amx_ftoc(pGameInterface->GetCamera()->GetCam(params[1])->GetFixedModeSource()->fX);
			*cptrb = amx_ftoc(pGameInterface->GetCamera()->GetCam(params[1])->GetFixedModeSource()->fY);
			*cptrc = amx_ftoc(pGameInterface->GetCamera()->GetCam(params[1])->GetFixedModeSource()->fZ);
			return 1;
		}

		PWNFUNC(GetFixedModeVector)
		{	
			cell* cptra;
			cell* cptrb;
			cell* cptrc;
			amx_GetAddr(amx, params[2], &cptra);
			amx_GetAddr(amx, params[3], &cptrb);
			amx_GetAddr(amx, params[4], &cptrc);
			*cptra = amx_ftoc(pGameInterface->GetCamera()->GetCam(params[1])->GetFixedModeVector()->fX);
			*cptrb = amx_ftoc(pGameInterface->GetCamera()->GetCam(params[1])->GetFixedModeVector()->fY);
			*cptrc = amx_ftoc(pGameInterface->GetCamera()->GetCam(params[1])->GetFixedModeVector()->fZ);
			return 1;
		}

		PWNFUNC(GetFront)
		{	
			cell* cptra;
			cell* cptrb;
			cell* cptrc;
			amx_GetAddr(amx, params[2], &cptra);
			amx_GetAddr(amx, params[3], &cptrb);
			amx_GetAddr(amx, params[4], &cptrc);
			*cptra = amx_ftoc(pGameInterface->GetCamera()->GetCam(params[1])->GetFront()->fX);
			*cptrb = amx_ftoc(pGameInterface->GetCamera()->GetCam(params[1])->GetFront()->fY);
			*cptrc = amx_ftoc(pGameInterface->GetCamera()->GetCam(params[1])->GetFront()->fZ);
			return 1;
		}

		PWNFUNC(GetFOV)
		{
			float temp = pGameInterface->GetCamera()->GetCam(params[1])->GetFOV();
			return amx_ftoc(temp);
		}

		PWNFUNC(GetSource)
		{	
			cell* cptra;
			cell* cptrb;
			cell* cptrc;
			amx_GetAddr(amx, params[2], &cptra);
			amx_GetAddr(amx, params[3], &cptrb);
			amx_GetAddr(amx, params[4], &cptrc);
			*cptra = amx_ftoc(pGameInterface->GetCamera()->GetCam(params[1])->GetSource()->fX);
			*cptrb = amx_ftoc(pGameInterface->GetCamera()->GetCam(params[1])->GetSource()->fY);
			*cptrc = amx_ftoc(pGameInterface->GetCamera()->GetCam(params[1])->GetSource()->fZ);
			return 1;
		}

		PWNFUNC(GetMode)
		{	
			return pGameInterface->GetCamera()->GetCam(params[1])->GetMode();
		}

		PWNFUNC(GetUp)
		{	
			cell* cptra;
			cell* cptrb;
			cell* cptrc;
			amx_GetAddr(amx, params[2], &cptra);
			amx_GetAddr(amx, params[3], &cptrb);
			amx_GetAddr(amx, params[4], &cptrc);
			*cptra = amx_ftoc(pGameInterface->GetCamera()->GetCam(params[1])->GetUp()->fX);
			*cptrb = amx_ftoc(pGameInterface->GetCamera()->GetCam(params[1])->GetUp()->fY);
			*cptrc = amx_ftoc(pGameInterface->GetCamera()->GetCam(params[1])->GetUp()->fZ);
			return 1;
		}

		PWNFUNC(SetFOV)
		{	
			pGameInterface->GetCamera()->GetCam(params[1])->SetFOV(amx_ctof(params[1]));
			return 1;
		}

		PWNFUNC(GetTargetHistoryPos)
		{	
			cell* cptra;
			cell* cptrb;
			cell* cptrc;
			amx_GetAddr(amx, params[2], &cptra);
			amx_GetAddr(amx, params[3], &cptrb);
			amx_GetAddr(amx, params[4], &cptrc);
			*cptra = amx_ftoc(pGameInterface->GetCamera()->GetCam(params[1])->GetTargetHistoryPos()->fX);
			*cptrb = amx_ftoc(pGameInterface->GetCamera()->GetCam(params[1])->GetTargetHistoryPos()->fY);
			*cptrc = amx_ftoc(pGameInterface->GetCamera()->GetCam(params[1])->GetTargetHistoryPos()->fZ);
			return 1;
		}

		PWNFUNC(GetTargetEntity)
		{	
			CEntity * ent = NULL;
			ent = pGameInterface->GetCamera()->GetCam(params[1])->GetTargetEntity();
			if(ent == NULL)
				return 0;
			cell* cptra;
			cell* cptrb;
			cell* cptrc;
			amx_GetAddr(amx, params[2], &cptra);
			amx_GetAddr(amx, params[3], &cptrb);
			amx_GetAddr(amx, params[4], &cptrc);
			*cptra = ent->GetArrayID();
			*cptrb = ent->GetEntityType();
			*cptrc = ent->GetEntityStatus();
			return 1;
		}

		PWNFUNC(GetCameraRotation)
		{
			float temp = pGameInterface->GetCamera()->GetCameraRotation();
			return amx_ftoc(temp);
		}

		PWNFUNC(GetCarZoom)
		{
			float temp = pGameInterface->GetCamera()->GetCarZoom();
			return amx_ftoc(temp);
		}

		PWNFUNC(GetFadingDirection)
		{	
			return pGameInterface->GetCamera()->GetFadingDirection();
		}

		PWNFUNC(GetLTM)
		{	
			cell* cptr[16];
			for(unsigned short x = 0; x < 16; ++x)
			{
				amx_GetAddr(amx, params[x+1], &cptr[x]);
			}	
			*cptr[0] = amx_ftoc(pGameInterface->GetCamera()->GetLTM()->at.x);
			*cptr[1] = amx_ftoc(pGameInterface->GetCamera()->GetLTM()->at.y);
			*cptr[2] = amx_ftoc(pGameInterface->GetCamera()->GetLTM()->at.z);
			*cptr[3] = pGameInterface->GetCamera()->GetLTM()->flags;
			*cptr[4] = pGameInterface->GetCamera()->GetLTM()->pad1;
			*cptr[5] = pGameInterface->GetCamera()->GetLTM()->pad2;
			*cptr[6] = pGameInterface->GetCamera()->GetLTM()->pad3;
			*cptr[7] = amx_ftoc(pGameInterface->GetCamera()->GetLTM()->pos.x);
			*cptr[8] = amx_ftoc(pGameInterface->GetCamera()->GetLTM()->pos.y);
			*cptr[9] = amx_ftoc(pGameInterface->GetCamera()->GetLTM()->pos.z);
			*cptr[10] = amx_ftoc(pGameInterface->GetCamera()->GetLTM()->right.x);
			*cptr[11] = amx_ftoc(pGameInterface->GetCamera()->GetLTM()->right.y);
			*cptr[12] = amx_ftoc(pGameInterface->GetCamera()->GetLTM()->right.z);
			*cptr[13] = amx_ftoc(pGameInterface->GetCamera()->GetLTM()->up.x);
			*cptr[14] = amx_ftoc(pGameInterface->GetCamera()->GetLTM()->up.y);
			*cptr[15] = amx_ftoc(pGameInterface->GetCamera()->GetLTM()->up.z);
			return 1;
		}

		PWNFUNC(GetMatrix)
		{	
			cell* cptr[12];
			for(unsigned short x = 0; x < 12; ++x)
			{
				amx_GetAddr(amx, params[x+1], &cptr[x]);
			}	
			CMatrix CameraMatrix;
			pGameInterface->GetCamera()->GetMatrix(&CameraMatrix);
			*cptr[0] = amx_ftoc(CameraMatrix.vFront.fX);
			*cptr[1] = amx_ftoc(CameraMatrix.vFront.fY);
			*cptr[2] = amx_ftoc(CameraMatrix.vFront.fZ);
			*cptr[3] = amx_ftoc(CameraMatrix.vPos.fX);
			*cptr[4] = amx_ftoc(CameraMatrix.vPos.fY);
			*cptr[5] = amx_ftoc(CameraMatrix.vPos.fZ);
			*cptr[6] = amx_ftoc(CameraMatrix.vRight.fX);
			*cptr[7] = amx_ftoc(CameraMatrix.vRight.fY);
			*cptr[8] = amx_ftoc(CameraMatrix.vRight.fZ);
			*cptr[9] = amx_ftoc(CameraMatrix.vUp.fX);
			*cptr[10] = amx_ftoc(CameraMatrix.vUp.fY);
			*cptr[11] = amx_ftoc(CameraMatrix.vUp.fZ);
			return 1;
		}

		PWNFUNC(GetWidescreen)
		{		
			return pGameInterface->GetCamera()->GetWidescreen();
		}

		PWNFUNC(IsFading)
		{		
			return pGameInterface->GetCamera()->IsFading();
		}

		PWNFUNC(Restore)
		{		
			pGameInterface->GetCamera()->Restore();
			return 1;
		}

		PWNFUNC(RestoreWithJumpCut)
		{		
			pGameInterface->GetCamera()->RestoreWithJumpCut();
			return 1;
		}

		PWNFUNC(SetCameraClip)
		{		
			pGameInterface->GetCamera()->SetCameraClip(params[1] != 0, params[2] != 0);
			return 1;
		}

		PWNFUNC(SetCamPositionForFixedMode)
		{		
			pGameInterface->GetCamera()->SetCamPositionForFixedMode(&CVector(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3])),&CVector(amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6])));
			return 1;
		}

		PWNFUNC(SetCarZoom)
		{		
			pGameInterface->GetCamera()->SetCarZoom(amx_ctof(params[1]));
			return 1;
		}

		PWNFUNC(SetFadeColor)
		{		
			pGameInterface->GetCamera()->SetFadeColor(params[1],params[2],params[3]);
			return 1;
		}

		PWNFUNC(SetMatrix)
		{		
			CMatrix matrix;
			matrix.vFront = CVector(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]));
			matrix.vPos = CVector(amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6]));
			matrix.vRight = CVector(amx_ctof(params[7]), amx_ctof(params[8]), amx_ctof(params[9]));
			matrix.vUp = CVector(amx_ctof(params[10]), amx_ctof(params[11]), amx_ctof(params[12]));
			pGameInterface->GetCamera()->SetMatrix(&matrix);
			return 1;
		}

		PWNFUNC(SetWidescreen)
		{		
			pGameInterface->GetCamera()->SetWidescreen(params[1] != 0);
			return 1;
		}

		PWNFUNC(TakeControl)
		{		
			pGameInterface->GetCamera()->TakeControl(&CVector(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3])),params[4]);
			return 1;
		}

		PWNFUNC(TakeControlEx)
		{		
			if(params[1] == 0)
				pGameInterface->GetCamera()->TakeControl((CEntity*)pPools->GetPed(params[2]),static_cast <eCamMode> (params[3]),params[4]);
			else
				pGameInterface->GetCamera()->TakeControl((CEntity*)pPools->GetVehicle(params[2]),static_cast <eCamMode> (params[3]),params[4]);
			return 1;
		}

		PWNFUNC(TakeControlAttachToEntity)
		{	
			if(params[1] == 0)
			{
				if(params[3] == 0)
				{
					pGameInterface->GetCamera()->TakeControlAttachToEntity((CEntity*)pPools->GetPed(params[2]),(CEntity*)pPools->GetPed(params[4]),&CVector(amx_ctof(params[5]), amx_ctof(params[6]), amx_ctof(params[7])),&CVector(amx_ctof(params[8]), amx_ctof(params[9]), amx_ctof(params[10])),amx_ctof(params[11]),params[12]);
				}
				else
				{
					pGameInterface->GetCamera()->TakeControlAttachToEntity((CEntity*)pPools->GetPed(params[2]),(CEntity*)pPools->GetVehicle(params[4]),&CVector(amx_ctof(params[5]), amx_ctof(params[6]), amx_ctof(params[7])),&CVector(amx_ctof(params[8]), amx_ctof(params[9]), amx_ctof(params[10])),amx_ctof(params[11]),params[12]);
				}
			}
			else
			{
				if(params[3] == 0)
				{
					pGameInterface->GetCamera()->TakeControlAttachToEntity((CEntity*)pPools->GetVehicle(params[2]),(CEntity*)pPools->GetPed(params[4]),&CVector(amx_ctof(params[5]), amx_ctof(params[6]), amx_ctof(params[7])),&CVector(amx_ctof(params[8]), amx_ctof(params[9]), amx_ctof(params[10])),amx_ctof(params[11]),params[12]);
				}
				else
				{
					pGameInterface->GetCamera()->TakeControlAttachToEntity((CEntity*)pPools->GetVehicle(params[2]),(CEntity*)pPools->GetVehicle(params[4]),&CVector(amx_ctof(params[5]), amx_ctof(params[6]), amx_ctof(params[7])),&CVector(amx_ctof(params[8]), amx_ctof(params[9]), amx_ctof(params[10])),amx_ctof(params[11]),params[12]);
				}
			}
			return 1;
		}

		PWNFUNC(ConeCastCollisionResolve)
		{		
			CVector pPos(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]));
			CVector pLookAt(amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6]));
			CVector pDest;
			float pDist;
			pGameInterface->GetCamera()->ConeCastCollisionResolve(&pPos,&pLookAt,&pDest,amx_ctof(params[10]),amx_ctof(params[11]),&pDist);
			cell* cptr;
			amx_GetAddr(amx, params[7], &cptr);
			*cptr = amx_ftoc(pDest.fX);
			amx_GetAddr(amx, params[8], &cptr);
			*cptr = amx_ftoc(pDest.fY);
			amx_GetAddr(amx, params[9], &cptr);
			*cptr = amx_ftoc(pDest.fZ);
			amx_GetAddr(amx, params[12], &cptr);
			*cptr = amx_ftoc(pDist);
			return 1;
		}
	};

//////////////////
};//namespace PawnFunc