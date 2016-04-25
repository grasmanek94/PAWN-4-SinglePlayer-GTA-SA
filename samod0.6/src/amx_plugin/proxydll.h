#pragma once

#include <boost/filesystem.hpp>
#include <amx.h>

double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
    LARGE_INTEGER li;
    if(!QueryPerformanceFrequency(&li))
	{
	Log("QueryPerformanceFrequency failed!");
	}
	else
	{
		PCFreq = double(li.QuadPart)/1000.0;

		QueryPerformanceCounter(&li);
		CounterStart = li.QuadPart;
	}
}

double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart-CounterStart)/PCFreq;
}

/////////////////////////////
extern IDirect3DDevice9		*origIDirect3DDevice9;

font_ptr TextCreate(const char * szFontName,int FontHeight,DWORD dwCreateFlags)
{
    CD3DFont* ptr = new CD3DFont(szFontName,FontHeight,dwCreateFlags);
    ptr->Initialize(origIDirect3DDevice9);
    return font_ptr(ptr);
}

std::vector<font_ptr> fonts;

//////////////////
std::vector<IDirect3DTexture9*> Textures;
std::vector<ID3DXSprite*>		Sprites;
std::vector<D3DXMATRIX>			D3DXMatrixes;
std::vector<D3DXVECTOR2>		D3DXVectors2;
std::vector<D3DXVECTOR3>		D3DXVectors3;
std::vector<RECT>				D3DXRectangles;

#define LocalPed pPedSelf
#define LocalPedSA pPedSelfSA

CVehicle				*pVehicle = NULL;
CVehicle				*gsVehicle = NULL;
CVehicle				*gssVehicle = NULL;
CPed					*gsPed = NULL;
CEntitySAInterface		*pVehicleSA = NULL;

#define PlayerInfo (pPedSelf->GetPlayerInfo())

#define FUNCTION __declspec(dllexport)

HWND& GTAWindowHandle = *reinterpret_cast<HWND*>(0xC17054);

AMX_NATIVE_INFO custom_Natives[];

std::list <AMX*> reload_scripts_amx;
CColPoint * GlobalCollision = NULL;

#include <plugincommon.h>
#include <plugininternal.h>
#include <plugins.h>
#include <script.h>

#include <functions.h>

AMX_NATIVE_INFO custom_Natives[] =
{
	{   "print",					PawnFunc::n_print },
	{	"SP_GetGameVersion"	,	PawnFunc::GetGameVersion	}	,
	{	"SP_GetBlurLevel"	,	PawnFunc::GetBlurLevel	}	,
	{	"SP_GetGameTime"	,	PawnFunc::GetGameTime	}	,
	{	"SP_GetFPS"	,	PawnFunc::GetFPS	}	,
	{	"SP_GetGameSpeed"	,	PawnFunc::GetGameSpeed	}	,
	{	"SP_GetGravity"	,	PawnFunc::GetGravity	}	,
	{	"SP_GetBikeFrontWheelCounter"	,	PawnFunc::GetBikeFrontWheelCounter	}	,
	{	"SP_GetBikeFrontWheelDist"	,	PawnFunc::GetBikeFrontWheelDist	}	,
	{	"SP_GetBikeRearWheelCounter"	,	PawnFunc::GetBikeRearWheelCounter	}	,
	{	"SP_GetBikeRearWheelDist"	,	PawnFunc::GetBikeRearWheelDist	}	,
	{	"SP_GetCarLess3WheelCounter"	,	PawnFunc::GetCarLess3WheelCounter	}	,
	{	"SP_GetCarTwoWheelCounter"	,	PawnFunc::GetCarTwoWheelCounter	}	,
	{	"SP_GetCarTwoWheelDist"	,	PawnFunc::GetCarTwoWheelDist	}	,
	{	"SP_GetCrossHair"	,	PawnFunc::GetCrossHair	}	,
	{	"SP_GetDoesNotGetTired"	,	PawnFunc::GetDoesNotGetTired	}	,
	{	"SP_GetFPSMoveHeading"	,	PawnFunc::GetFPSMoveHeading	}	,
	{	"SP_GetLastTimeBigGunFired"	,	PawnFunc::GetLastTimeBigGunFired	}	,
	{	"SP_GetLastTimeEaten"	,	PawnFunc::GetLastTimeEaten	}	,
	{	"SP_GetPlayerMoney"	,	PawnFunc::GetPlayerMoney	}	,
	{	"SP_GetWantedLevel"	,	PawnFunc::GetWantedLevel	}	,
	{	"SP_SetMaximumWantedLevel"	,	PawnFunc::SetMaximumWantedLevel	}	,
	{	"SP_SetWantedLevel"	,	PawnFunc::SetWantedLevel	}	,
	{	"SP_SetWantedLevelNoDrop"	,	PawnFunc::SetWantedLevelNoDrop	}	,
	{	"SP_GivePlayerParachute"	,	PawnFunc::GivePlayerParachute	}	,
	{	"SP_SetDoesNotGetTired"	,	PawnFunc::SetDoesNotGetTired	}	,
	{	"SP_SetLastTimeBigGunFired"	,	PawnFunc::SetLastTimeBigGunFired	}	,
	{	"SP_SetLastTimeEaten"	,	PawnFunc::SetLastTimeEaten	}	,
	{	"SP_SetPlayerMoney"	,	PawnFunc::SetPlayerMoney	}	,
	{	"SP_StreamParachuteWeapon"	,	PawnFunc::StreamParachuteWeapon	}	,
	{	"SP_RemoveWeaponModel"	,	PawnFunc::RemoveWeaponModel	}	,
	{	"SP_RemoveAllWeapons"	,	PawnFunc::RemoveAllWeapons	}	,
	{	"SP_GetAreaCode"	,	PawnFunc::GetAreaCode	}	,
	{	"SP_GetArmor"	,	PawnFunc::GetArmor	}	,
	{	"SP_GetCanBeShotInVehicle"	,	PawnFunc::GetCanBeShotInVehicle	}	,
	{	"SP_GetCantBeKnockedOffBike"	,	PawnFunc::GetCantBeKnockedOffBike	}	,
	{	"SP_GetCurrentRotation"	,	PawnFunc::GetCurrentRotation	}	,
	{	"SP_GetCurrentWeaponSlot"	,	PawnFunc::GetCurrentWeaponSlot	}	,
	{	"SP_GetDistFromCentreOfMassBase"	,	PawnFunc::GetDistFromCentreOfMassToBase	}	,
	{	"SP_GetElasticity"	,	PawnFunc::GetElasticity	}	,
	{	"SP_GetFightingStyle"	,	PawnFunc::GetFightingStyle	}	,
	{	"SP_GetHealth"	,	PawnFunc::GetHealth	}	,
	{	"SP_GetMass"	,	PawnFunc::GetMass	}	,
	{	"SP_GetModelIndex"	,	PawnFunc::GetModelIndex	}	,
	{	"SP_GetMovementSpeed"	,	PawnFunc::GetMovementSpeed	}	,
	{	"SP_GetOccupiedSeat"	,	PawnFunc::GetOccupiedSeat	}	,
	{	"SP_GetPosition"	,	PawnFunc::GetPosition	}	,
	{	"SP_GetRunState"	,	PawnFunc::GetRunState	}	,
	{	"SP_GetStayInSamePlace"	,	PawnFunc::GetStayInSamePlace	}	,
	{	"SP_GetTargetRotation"	,	PawnFunc::GetTargetRotation	}	,
	{	"SP_GetTestForShotInVehicle"	,	PawnFunc::GetTestForShotInVehicle	}	,
	{	"SP_GetTurnMass"	,	PawnFunc::GetTurnMass	}	,
	{	"SP_GetTurnSpeed"	,	PawnFunc::GetTurnSpeed	}	,
	{	"SP_GetUnderwater"	,	PawnFunc::GetUnderwater	}	,
	{	"SP_GetType"	,	PawnFunc::GetType	}	,
	{	"SP_GiveWeapon"	,	PawnFunc::GiveWeapon	}	,
	{	"SP_IsBackfaceCulled"	,	PawnFunc::IsBackfaceCulled	}	,
	{	"SP_IsDucking"	,	PawnFunc::IsDucking	}	,
	{	"SP_IsFullyVisible"	,	PawnFunc::IsFullyVisible	}	,
	{	"SP_IsInWater"	,	PawnFunc::IsInWater	}	,
	{	"SP_IsOnFire"	,	PawnFunc::IsOnFire	}	,
	{	"SP_IsOnScreen"	,	PawnFunc::IsOnScreen	}	,
	{	"SP_IsStatic"	,	PawnFunc::IsStatic	}	,
	{	"SP_IsStaticWaitingForCollision"	,	PawnFunc::IsStaticWaitingForCollision	}	,
	{	"SP_IsVisible"	,	PawnFunc::IsVisible	}	,
	{	"SP_IsWearingGoggles"	,	PawnFunc::IsWearingGoggles	}	,
	{	"SP_Respawn"	,	PawnFunc::Respawn	}	,
	{	"SP_SetAreaCode"	,	PawnFunc::SetAreaCode	}	,
	{	"SP_SetArmor"	,	PawnFunc::SetArmor	}	,
	{	"SP_SetBackfaceCulled"	,	PawnFunc::SetBackfaceCulled	}	,
	{	"SP_GetBuoyancyConstant"	,	PawnFunc::GetBuoyancyConstant	}	,
	{	"SP_SetBuoyancyConstant"	,	PawnFunc::SetBuoyancyConstant	}	,
	{	"SP_SetCanBeShotInVehicle"	,	PawnFunc::SetCanBeShotInVehicle	}	,
	{	"SP_SetCantBeKnockedOffBike"	,	PawnFunc::SetCantBeKnockedOffBike	}	,
	{	"SP_SetCurrentRotation"	,	PawnFunc::SetCurrentRotation	}	,
	{	"SP_SetCurrentWeaponSlot"	,	PawnFunc::SetCurrentWeaponSlot	}	,
	{	"SP_SetDucking"	,	PawnFunc::SetDucking	}	,
	{	"SP_SetElasticity"	,	PawnFunc::SetElasticity	}	,
	{	"SP_SetFightingStyle"	,	PawnFunc::SetFightingStyle	}	,
	{	"SP_SetFootBlood"	,	PawnFunc::SetFootBlood	}	,
	{	"SP_SetGogglesState"	,	PawnFunc::SetGogglesState	}	,
	{	"SP_SetHealth"	,	PawnFunc::SetHealth	}	,
	{	"SP_SetIsStanding"	,	PawnFunc::SetIsStanding	}	,
	{	"SP_SetLighting"	,	PawnFunc::SetLighting	}	,
	{	"SP_GetLighting"	,	PawnFunc::GetLighting	}	,
	{	"SP_SetMass"	,	PawnFunc::SetMass	}	,
	{	"SP_SetModelIndex"	,	PawnFunc::SetModelIndex	}	,
	{	"SP_SetMoveSpeed"	,	PawnFunc::SetMoveSpeed	}	,
	{	"SP_SetOccupiedSeat"	,	PawnFunc::SetOccupiedSeat	}	,
	{	"SP_SetOnFire"	,	PawnFunc::SetOnFire	}	,
	{	"SP_SetOrientation"	,	PawnFunc::SetOrientation	}	,
	{	"SP_SetOxygenLevel"	,	PawnFunc::SetOxygenLevel	}	,
	{	"SP_SetPosition"	,	PawnFunc::SetPosition	}	,
	{	"SP_SetStatic"	,	PawnFunc::SetStatic	}	,
	{	"SP_SetStaticWaitingForCollision"	,	PawnFunc::SetStaticWaitingForCollision	}	,
	{	"SP_SetStayInSamePlace"	,	PawnFunc::SetStayInSamePlace	}	,
	{	"SP_SetTargetRotation"	,	PawnFunc::SetTargetRotation	}	,
	{	"SP_SetTestForShotInVehicle"	,	PawnFunc::SetTestForShotInVehicle	}	,
	{	"SP_SetTurnMass"	,	PawnFunc::SetTurnMass	}	,
	{	"SP_SetTurnSpeed"	,	PawnFunc::SetTurnSpeed	}	,
	{	"SP_SetUnderwater"	,	PawnFunc::SetUnderwater	}	,
	{	"SP_SetUsesCollision"	,	PawnFunc::SetUsesCollision	}	,
	{	"SP_SetVisible"	,	PawnFunc::SetVisible	}	,
	{	"SP_Teleport"	,	PawnFunc::Teleport	}	,
	{	"SP_SetGravity"	,	PawnFunc::SetGravity	}	,
	{	"SP_SetBlurLevel"	,	PawnFunc::SetBlurLevel	}	,
	{	"SP_SetGameSpeed"	,	PawnFunc::SetGameSpeed	}	,
	{   "SP_AddFont"		,	PawnFunc::AddFont		}	,
	{   "SP_DrawText"		,	PawnFunc::DXDrawText	}	,
	{   "format"		,	PawnFunc::format		}	,
	{   "SP_GetScreenInfo" ,	PawnFunc::GetScreenInfo }   ,
	{   "GetTickCount"  ,   PawnFunc::_GetTickCount }   ,
	{   "SP_DrawLine"      ,   PawnFunc::DXDrawLine    }   ,
	//vehicle
	{   "SP_IsPlayerInVehicle",  PawnFunc::IsPlayerInVehicle }  ,
	{   "SP_AddVehicleUpgrade",  PawnFunc::AddVehicleUpgrade }  ,
	{   "SP_AreVehicleDoorsLocked",  PawnFunc::AreVehicleDoorsLocked }  ,
	{   "SP_AreVehicleDoorsUndamageable",  PawnFunc::AreVehicleDoorsUndamageable }  ,
	{   "SP_BurstVehicleTyre",  PawnFunc::BurstVehicleTyre }  ,
	{   "SP_CarHasRoof",  PawnFunc::CarHasRoof }  ,
	{   "SP_ExtinguishCarFire",  PawnFunc::ExtinguishCarFire }  ,
	{   "SP_FixVehicle",  PawnFunc::FixVehicle }  ,
	{   "SP_GetCanVehicleBeDamaged",  PawnFunc::GetCanVehicleBeDamaged }  ,
	{   "SP_CanVehicleBeTargetedByBazoka",  PawnFunc::CanVehicleBeTargetedByBazooka }  ,
	{   "SP_CanVehiclePetrolTankBeShot",  PawnFunc::CanVehiclePetrolTankBeShot }  ,
	{   "SP_DoesVehColorChangeOnPaintjob",  PawnFunc::GetVehColorChangesWhenPaintjob }  ,
	{   "SP_GetVehicleColor",  PawnFunc::GetVehicleColor }  ,
	{   "SP_SetVehicleColor", PawnFunc::SetVehicleColor } ,
	{   "SP_GetCurrentGear",  PawnFunc::GetCurrentGear }  ,
	{   "SP_GetVehicleGasPedal",  PawnFunc::GetVehicleGasPedal }  ,
	{   "SP_GetVehicleBreakPedal" , PawnFunc::GetVehicleBreakPedal } ,
	{   "SP_SetVehicleHandlingData",  PawnFunc::SetVehicleHandlingData }  ,
	{   "SP_SetVehclePaintjob",  PawnFunc::SetVehclePaintjob }  ,
	{   "SP_SetVehclePaintjobDictionary",  PawnFunc::SetVehclePaintjobDictionary }  ,
	{   "SP_SetChangeColourOnPaintjob",  PawnFunc::SetChangeColourOnPaintjob }  ,
	{   "SP_GetVehicleHandlingDataFloat", PawnFunc::GetVehicleHandlingDataFloat },
	{   "SP_GetVehicleHandlingDataInt",  PawnFunc::GetVehicleHandlingDataInt },
	{   "SP_GetVehicleCenterOfMass"  , PawnFunc::GetVehicleCenterOfMass } ,

	{   "SP_DrawBox"  , PawnFunc::DXd3dbox } ,
	{   "SP_DrawBoxBorder"  , PawnFunc::DXdedboxborder } ,
	{   "SP_GetCursorPosition" , PawnFunc::GetCursorPosition },
	{   "SP_IsKeyPressed" , PawnFunc::KEYPRESSED },
	{   "SP_IsKeyDown" , PawnFunc::KEYDOWN },
	//otherdx
	{   "SP_CreateTexture", PawnFunc::DXCreateTexture  },
	{   "SP_CreateSprite", PawnFunc::DXCreateSprite  },
	{   "SP_CreateTextureHolder", PawnFunc::DXCreateTextureHolder  },
	{   "SP_CreateSpriteHolder", PawnFunc::DXCreateSpriteHolder  },
	{   "SP_CreateD3DMatrixHolder", PawnFunc::DXCreateD3DXMatrixHolder  },
	{   "SP_CreateVector2Holder", PawnFunc::DXCreateD3DXVector2Holder  },
	{   "SP_Vector2Set", PawnFunc::DXD3DXVector2Set  },
	{   "SP_Vector2Get", PawnFunc::DXD3DXVector2Get  },
	{   "SP_D3DMatrixTransformation2D", PawnFunc::DXD3DXMatrixTransformation2D  },
	{   "SP_Sprite_Begin", PawnFunc::DXSprite_Begin  },
	{   "SP_Sprite_End", PawnFunc::DXSprite_End  },
	{   "SP_Sprite_SetTransform", PawnFunc::DXSprite_SetTransform  },
	{   "SP_Sprite_Draw", PawnFunc::DXSprite_Draw  },
	{   "SP_CreateVector3Holder", PawnFunc::DXCreateD3DXVector3Holder  },
	{   "SP_Vector3Set", PawnFunc::DXD3DXVector3Set  },
	{   "SP_Vector3Get", PawnFunc::DXD3DXVector3Get  },
	{   "SP_CreateRectangleHolder", PawnFunc::DXCreateD3DXRectangleHolder  },
	{   "SP_RectangleSet", PawnFunc::DXD3DXRectangleSet  },
	{   "SP_RectangleGet", PawnFunc::DXD3DXRectangleGet  },
	{   "SP_GetBackBufferHeight", PawnFunc::GetBackBufferHeight },
	{   "SP_GetBackBufferWidth", PawnFunc::GetBackBufferWidth },
	{   "SP_Texture_Release", PawnFunc::DXTextureRelease },
	{   "SP_Sprite_Release", PawnFunc::DXSpriteRelease },
	{   "SP_IsTextureNull", PawnFunc::DXTextureIsNull },
	{   "SP_IsSpriteNull", PawnFunc::DXSpriteIsNull },

	{   "SP_ShowCursor" , PawnFunc::ShowCursor },
	{   "SP_EmulateKeyPress" , PawnFunc::EmulateKeyPressINPUT },
	{   "SP_GetLastError"   , PawnFunc::PWNGetLastError },
	{   "printf",	PawnFunc::printf_f},
	{   "SP_GetVehicleMatrix" , PawnFunc::ppGetVehicleMatrix},
	{   "SP_SetVehicleMatrix" , PawnFunc::ppSetVehicleMatrix},
	{   "SP_RotateVehicleMatrix" , PawnFunc::ppRotateVehicleMatrix},
	{   "SP_GetVehicleRotation", PawnFunc::ppGetVehicleRotation},
	{   "SP_GetVehicleEulerRotation", PawnFunc::ppGetVehicleRotationXYZ},
	{   "SP_ReloadMe", PawnFunc::ReloadMe},
	{   "SP_ReloadScript", PawnFunc::ReloadScript},
	{   "SP_LoadScript", PawnFunc::pLoadScript},
	{   "SP_UnloadScript", PawnFunc::pUnloadScript},
	{   "SP_LoadDll", PawnFunc::ppLoadDLL },
	{   "SP_UnloadDll", PawnFunc::ppUnloadDLL },
	{   "SP_ProcessLineOfSight", PawnFunc::ppProcessLineOfSight},
	{   "SP_TERMINATE_GTA",PawnFunc::Terminate},
	{   "SP_DrawRadarArea",PawnFunc::DrawArea},

	//5.7
	{   "SP_IsValidPed",PawnFunc::IsValidPed},
	{   "SP_IsValidVehicle",PawnFunc::IsValidVehicle},
	{   "SP_GetPedCount",PawnFunc::GetPedCount},
	{   "SP_GetVehicleCount",PawnFunc::GetVehicleCount},
	{   "SP_GetPedPoolSize",PawnFunc::GetPedPoolSize},
	{   "SP_GetVehiclePoolSize",PawnFunc::GetVehiclePoolSize},
	{   "SP_AddVehicle",PawnFunc::AddVehicle},
	{   "SP_GetPedVehicle",PawnFunc::GetPedVehicle},

	//6.0
	{   "SP_DestroyVehicle", PawnFunc::vDestroy },
	{   "SP_GetDriver", PawnFunc::vGetDriver },
	{   "SP_GetPassenger", PawnFunc::vGetPassenger },
	{   "SP_IsBeingDriven", PawnFunc::vIsBeingDriven },
	{   "SP_CanPlayerEnterCar", PawnFunc::vCanPedEnterCar },
	{   "SP_CanPedJumpOutCar", PawnFunc::vCanPedJumpOutCar },
	{   "SP_RemoveVehicleUpgrade", PawnFunc::vRemoveUpgrade },
	{   "SP_CanPedLeanOutFromCar", PawnFunc::vCanPedLeanOut },
	{   "SP_CanPedStepOutCar", PawnFunc::vCanPedStepOutCar },
	{   "SP_LockVehicleDoors", PawnFunc::vLockDoors },
	{   "SP_SetCarDoorsUndamageable", PawnFunc::vSetDoorsUndamagable },
	{   "SP_GetCarBaseType", PawnFunc::vGetBaseType },
	{   "SP_SetCarBodyDirtLevel", PawnFunc::vSetBodyDirtLevel },
	{   "SP_GetCarBodyDirtLevel", PawnFunc::vGetBodyDirtLevel },
	{   "SP_GetCarHeightAboveRoad", PawnFunc::vGetHeightAboveRoad },
	{   "SP_GetSteerAngle", PawnFunc::vGetSteerAngle },
	{   "SP_IsCarOnItsSide", PawnFunc::vIsOnItsSide },
	{   "SP_IsCarLawEnforcment", PawnFunc::vIsLawEnforcment },
	{   "SP_IsPedPassenger", PawnFunc::vIsPedPassenger },
	{   "SP_IsCarTouchingSphere", PawnFunc::vIsSphereTouching },
	{   "SP_IsCarUpsideDown", PawnFunc::vIsUpsideDown },
	{   "SP_IsCarEngineBroken", PawnFunc::vIsEngineBroken },
	{   "SP_SetCarEngineBroken", PawnFunc::vSetEngineBroken },
	{   "SP_IsCarScriptLocked", PawnFunc::vIsScriptLocked },
	{   "SP_SetCarScriptLocked", PawnFunc::vSetScriptLocked },
	{   "SP_PlaceBikeOnRoad", PawnFunc::vPlaceBikeOnRoadProperly },
	{   "SP_PlaceAutomobileOnRoad", PawnFunc::vPlaceAutomobileOnRoadProperly },
	{   "SP_IsCarSirenOrAlarmActive", PawnFunc::vIsSirenOrAlarmActive },
	{   "SP_SetCarSirenOrAlarmActive", PawnFunc::vSetSirenOrAlarmActive },
	{   "SP_GetCarAlpha", PawnFunc::vGetAlpha },
	{   "SP_SetCarAlpha", PawnFunc::vSetAlpha },
	{   "SP_FuckCarCompletely", PawnFunc::vFuckCompletely },
	{   "SP_GetCarAeroplaneCompStatus", PawnFunc::vGetAeroplaneCompStatus },
	{   "SP_GetCarDoorStatus", PawnFunc::vGetDoorStatus },
	{   "SP_GetCarEngineStatus", PawnFunc::vGetEnginestatus },
	{   "SP_GetCarLightStatus", PawnFunc::vGetLightStatus },
	{   "SP_GetCarPanelStatus", PawnFunc::vGetPanelStatus },
	{   "SP_GetCarWheelStatus", PawnFunc::vGetWheelStatus },
	{   "SP_SetCarAeroplaneCompStatus", PawnFunc::vSetAeroplaneCompStatus },
	{   "SP_SetCarDoorStatus", PawnFunc::vSetDoorStatus },
	{   "SP_SetCarEngineStatus", PawnFunc::vSetEngineStatus },
	{   "SP_SetCarLightStatus", PawnFunc::vSetLightStatus },
	{   "SP_SetCarPanelStatus", PawnFunc::vSetPanelStatus },
	{   "SP_SetCarWheelStatus", PawnFunc::vSetWheelStatus },
	{   "SP_FadeCarOut", PawnFunc::vFadeOut },
	{   "SP_IsCarFadingOut", PawnFunc::vIsFadingOut },
	{   "SP_SetVehicleTowLink", PawnFunc::vSetTowLink },
	{   "SP_BreakVehicleTowLink", PawnFunc::vBreaktowLink },
	{   "SP_GetTowedVehicle", PawnFunc::vGetTowedVehicle },
	{   "SP_GetTowedByVehicle", PawnFunc::vGetTowedByVehicle },
	{   "SP_SetCarWinchType", PawnFunc::vSetWinchType },
	{   "SP_DropCarPickUpStuffWithWinch", PawnFunc::vReleasePickedUpEntityWithWinch },
	{   "SP_SetRopeHeightForHeli", PawnFunc::vSetRopeHeightForHeli },
	{   "SP_GetVehiclePaintjob", PawnFunc::vGetPaintjob },
	{   "SP_IsCarDamaged", PawnFunc::vIsDamaged },
	{   "SP_IsCarDrowning", PawnFunc::vIsDrowning },
	{   "SP_IsCarEngineOn", PawnFunc::vIsEngineOn },
	{   "SP_IsCarHandbrakeOn", PawnFunc::vIsHandbrakeOn },
	{   "SP_DoesCarLeaveSkidMarks", PawnFunc::vGetAlwaysLeaveSkidMarks },
	{   "SP_GetCarComedyControls", PawnFunc::vGetComedyControls },
	{   "SP_GetCarGunSwitchedOff", PawnFunc::vGetGunSwitchedOff },
	{   "SP_GetCarLightsOn", PawnFunc::vGetLightsOn },
	{   "SP_GetCarOverrideLights", PawnFunc::vGetOverrideLights },
	{   "SP_GetCarTakeLessDamage", PawnFunc::vGetTakeLessDamage },
	{   "SP_GetCarTyresDontBurst", PawnFunc::vGetTyresDontBurst },
	{   "SP_GetCarAdjustablePropertyVal", PawnFunc::vGetAdjustablePropertyValue },
	{   "SP_GetHeliRotorSpeed", PawnFunc::vGetHeliRotorSpeed },
	{   "SP_GetCarExplodeTime", PawnFunc::vGetExplodeTime },
	{   "SP_SetCarDoesLeaveSkidMarks", PawnFunc::vSetAlwaysLeaveSkidMarks },
	{   "SP_SetCarCanBeDamaged", PawnFunc::vSetCanBeDamaged },
	{   "SP_SetCarCanBeTargetByBazooka", PawnFunc::vSetCanBeTargettedByHeatSeekingMissiles },
	{   "SP_SetCarCanShootPetrolTank", PawnFunc::vSetCanShootPetrolTank },
	{   "SP_SetCarComedyControls", PawnFunc::vSetComedyControls },
	{   "SP_SetCarEngine", PawnFunc::vSetEngineOn },
	{   "SP_SetCarGunSwitchedOff", PawnFunc::vSetGunSwitchedOff },
	{   "SP_SetCarHandbrake", PawnFunc::vSetHandbrakeOn },
	{   "SP_SetCarLights", PawnFunc::vSetLightsOn },
	{   "SP_SetCarTakeLessDamage", PawnFunc::vSetTakeLessDamage },
	{   "SP_SetCarTyresDontBurst", PawnFunc::vSetTyresDontBurst },
	{   "SP_SetCarAdjustablePropertyVal", PawnFunc::vSetAdjustablePropertyValue },
	{   "SP_SetHeliRotorSpeed", PawnFunc::vSetHeliRotorSpeed },
	{   "SP_SetTaxiLight", PawnFunc::vSetTaxiLightOn },
	{   "SP_GetBikeWheelStatus", PawnFunc::vGetBikeWheelStatus },
	{   "SP_SetBikeWheelStatus", PawnFunc::vSetBikeWheelStatus },
	{   "SP_IsVehicleOnScreen", PawnFunc::vIsOnScreen },
	{   "SP_IsVehicleVisible", PawnFunc::vIsVisible },
	{   "SP_GetVehicleHealth", PawnFunc::vGetHealth },
	{   "SP_SetVehicleHealth", PawnFunc::vSetHealth },
	{   "SP_SetCarLandingGearDown", PawnFunc::vSetLandingGearDown },
	{   "SP_IsCarLandingGearDown", PawnFunc::vIsLandingGearDown },
	{   "SP_GetCarLandingGearPosition", PawnFunc::vGetLandingGearPosition },
	{   "SP_SetCarLandingGearPosition", PawnFunc::vSetLandingGearPosition },
	{   "SP_GetCarTurretRotation", PawnFunc::vGetTurretRotation },
	{   "SP_SetCarTurretRotation", PawnFunc::vSetTurretRotation },
	{   "SP_GetCarNumberGettingIn", PawnFunc::vGetNumberGettingIn },
	{   "SP_GetCarPassengerCount", PawnFunc::vGetPassengerCount },
	{   "SP_GetCarMaxPassengerCount", PawnFunc::vGetMaxPassengerCount },
	{   "SP_IsCarSmokeTrailEnabled", PawnFunc::vIsSmokeTrailEnabled },
	{   "SP_SetCarSmokeTrailEnabled", PawnFunc::vSetSmokeTrailEnabled },
	{   "SP_GetVehicleGravity", PawnFunc::vGetGravity },
	{   "SP_SetVehicleGravity", PawnFunc::vSetGravity },
	{   "SP_GetCarHeadLightColor", PawnFunc::vGetHeadLightColor },
	{   "SP_SetCarHeadLightColor", PawnFunc::vSetHeadLightColor },
	{   "SP_SetCarWheelVisibility", PawnFunc::vSetWheelVisibility },
	{   "SP_IsHeliSearchLightVisible", PawnFunc::vIsHeliSearchLightVisible },
	{   "SP_SetHeliSearchLightVisible", PawnFunc::vSetHeliSearchLightVisible },
	{   "SP_UpdateCarMovingCollision", PawnFunc::vUpdateMovingCollision },
	{   "SP_SetVehicleMoveSpeed", PawnFunc::vSetMoveSpeed },
	{   "SP_GetVehicleMoveSpeed", PawnFunc::vGetMoveSpeed },
	{   "SP_SetCarOrientation", PawnFunc::vSetOrientation },
	{   "SP_GetCarDamageMagnitude", PawnFunc::vGetDamageImpulseMagnitude },
	{   "SP_SetCarDamageMagnitude", PawnFunc::vSetDamageImpulseMagnitude },
	{   "SP_SetVehicleTurnSpeed", PawnFunc::vSetTurnSpeed },
	{   "SP_GetVehicleTurnSpeed", PawnFunc::vGetTurnSpeed },
	{   "SP_SetVehicleMass", PawnFunc::vSetMass },
	{   "SP_GetVehicleMass", PawnFunc::vGetMass },
	{   "SP_SetVehicleTurnMass", PawnFunc::vSetTurnMass },
	{   "SP_GetVehicleTurnMass", PawnFunc::vGetTurnMass },
	{   "SP_SetVehicleElasticity", PawnFunc::vSetElasticity },
	{   "SP_GetVehicleElasticity", PawnFunc::vGetElasticity },
	{   "SP_SetCarBuoyancyConstant", PawnFunc::vSetBuoyancyConstant },
	{   "SP_GetCarBuoyancyConstant", PawnFunc::vGetBuoyancyConstant },
	{   "SP_ResetVehicleLastDamage", PawnFunc::vResetLastDamage },
	{   "SP_GetVehicleLighting", PawnFunc::vGetLighting },
	{   "SP_SetVehicleLighting", PawnFunc::vSetLighting },
	{   "SP_SetVehiclePosition", PawnFunc::vSetPosition },
	{   "SP_TeleportVehicle", PawnFunc::vTeleport },
	{   "SP_FixBoatOrientation", PawnFunc::vFixBoatOrientation },
	{   "SP_GetVehiclePosition", PawnFunc::vGetPosition },
	{   "SP_SetVehicleRoll", PawnFunc::vSetRoll },
	{   "SP_SetVehicleDirection", PawnFunc::vSetDirection },
	{   "SP_SetVehicleWas", PawnFunc::vSetWas },
	{   "SP_GetVehicleModel", PawnFunc::vGetModelIndex },
	{   "SP_GetVehicleCentreDistance", PawnFunc::vGetDistanceFromCentreOfMassToBaseOfModel },
	{   "SP_SetVehicleStatus", PawnFunc::vSetEntityStatus },
	{   "SP_SetVehicleVisible", PawnFunc::vSetVisible },
	{   "SP_GetVehicleStatus", PawnFunc::vGetEntityStatus },
	{   "SP_GetVehicleAreaCode", PawnFunc::vGetAreaCode },
	{   "SP_SetVehicleAreaCode", PawnFunc::vSetAreaCode },
	{   "SP_GetVehicleImmunities", PawnFunc::vGetImmunities },
	{   "SP_IsVehicleUnderwater", PawnFunc::vGetUnderwater },
	{   "SP_SetVehicleUnderwater", PawnFunc::vSetUnderwater },

	//keys
	{   "KeyHook_DOWN", PawnFunc::kh_DOWN },
	{   "KeyHook_UP", PawnFunc::kh_UP },
	{   "KeyHook_PRESSED", PawnFunc::kh_PRESSED },
	{   "KeyHook_RELEASED", PawnFunc::kh_RELEASED },
	{   "KeyHook_CONSUME", PawnFunc::kh_CONSUME },

	//peds
	{   "SP_AddPed", PawnFunc::AddPed },
	{ NULL, NULL }
};

