//v6.0

#if defined _singleplayer_included
	#endinput
#endif
#define _singleplayer_included
#pragma library PawnForSinglePlayer

#include <core>
#include <file>
#include <float>
#include <string>
#include <time>

#pragma tabsize 4

native SP_GetGameVersion();
native SP_GetBlurLevel();
native SP_GetGameTime(&hour,&minute);
native Float:SP_GetFPS();
native Float:SP_GetGameSpeed();
native Float:SP_GetGravity();
native SP_GetBikeFrontWheelCounter();
native Float:SP_GetBikeFrontWheelDist();
native SP_GetBikeRearWheelCounter();
native Float:SP_GetBikeRearWheelDist();
native SP_GetCarLess3WheelCounter();
native SP_GetCarTwoWheelCounter();
native Float:SP_GetCarTwoWheelDist();
native bool:SP_GetCrossHair(&Float:x,&Float:y);//return value - crosshair::enabled/disabled
native SP_GetDoesNotGetTired();
native Float:SP_GetFPSMoveHeading();
native SP_GetLastTimeBigGunFired();
native SP_GetLastTimeEaten();
native SP_GetPlayerMoney();
native SP_GetWantedLevel();
native SP_SetMaximumWantedLevel(MaxWantedLevel);
native SP_SetWantedLevel(WantedLevel);
native SP_SetWantedLevelNoDrop(WantedLevel);
native SP_GivePlayerParachute();
native SP_SetDoesNotGetTired(bool:DoesNotGetTired);
native SP_SetLastTimeBigGunFired(time);
native SP_SetLastTimeEaten(time);
native SP_SetPlayerMoney(money);
native SP_StreamParachuteWeapon(bool:AllowParachute);
native SP_RemoveWeaponModel(ped,model);
native SP_RemoveAllWeapons(ped);
native SP_GetAreaCode(ped);
native Float:SP_GetArmor(ped);
native bool:SP_GetCanBeShotInVehicle(ped);
native bool:SP_GetCantBeKnockedOffBike(ped);
native Float:SP_GetCurrentRotation(ped);
native SP_GetCurrentWeaponSlot(ped);
native Float:SP_GetDistFromCentreOfMassBase(ped);
native Float:SP_GetElasticity(ped);
native SP_GetFightingStyle(ped);
native Float:SP_GetHealth(ped,bool:CurrentVehicle=false);
native Float:SP_GetMass(ped);
native SP_GetModelIndex(ped);
native SP_GetMovementSpeed(ped,&Float:x,&Float:y,&Float:z,bool:CurrentVehicle=true);
native SP_GetOccupiedSeat(ped);
native SP_GetPosition(ped,&Float:x,&Float:y,&Float:z,bool:GetCurrentVehiclePosition=true);//if GetCurrentVehiclePosition false then the ped position will be given, if true the position of the car the player is in will be given
native SP_GetRunState(ped);
native bool:SP_GetStayInSamePlace(ped);
native Float:SP_GetTargetRotation(ped);
native bool:SP_GetTestForShotInVehicle(ped);
native Float:SP_GetTurnMass(ped,bool:CurrentVehicle=false);
native SP_GetTurnSpeed(ped,&Float:x,&Float:y,&Float:z,bool:CurrentVehicle=false);
native bool:SP_GetUnderwater(ped);
native SP_GetType(ped);
native SP_GiveWeapon(ped,WeaponType,Ammo,WeaponSkill);
native bool:SP_IsBackfaceCulled(ped);
native bool:SP_IsDucking(ped);
native bool:SP_IsFullyVisible(ped);
native bool:SP_IsInWater(ped);
native bool:SP_IsOnFire(ped);
native bool:SP_IsOnScreen(ped);
native SP_IsStatic(ped);
native bool:SP_IsStaticWaitingForCollision(ped);
native bool:SP_IsVisible(ped);
native bool:SP_IsWearingGoggles(ped);
native SP_Respawn(ped,Float:x,Float:y,Float:z,bool:CameraCut);
native SP_SetAreaCode(ped,areacode);
native SP_SetArmor(ped,Float:armor);
native SP_SetBackfaceCulled(ped,bool:backfaceculled);
native Float:SP_GetBuoyancyConstant(ped);
native SP_SetBuoyancyConstant(ped,Float:constante);
native SP_SetCanBeShotInVehicle(ped,bool:Shot);
native SP_SetCantBeKnockedOffBike(ped,bool:CantBeKnockedOffBike);
native SP_SetCurrentRotation(ped,Float:rot);
native SP_SetCurrentWeaponSlot(ped,slot);
native SP_SetDucking(ped,bool:Duck);
native SP_SetElasticity(ped,Float:elasticity);
native SP_SetFightingStyle(ped,FightingStyle);
native SP_SetFootBlood(ped,FootBlood);
native SP_SetGogglesState(ped,bool:IsWearingThem);
native SP_SetHealth(ped,Float:health,bool:CurrentVehicle=false);
native SP_SetIsStanding(ped,bool:standing);
native SP_SetLighting(ped,Float:lighting);
native Float:SP_GetLighting(ped);
native SP_SetMass(ped,Float:mass,bool:CurrentVehicle=false);
native SP_SetModelIndex(ped,index);
native SP_SetMoveSpeed(ped,Float:x,Float:y,Float:z,bool:CurrentVehicle=true);
native SP_SetOccupiedSeat(ped,seat);
native SP_SetOnFire(ped,bool:OnFire);
native SP_SetOrientation(ped,Float:x,Float:y,Float:z,bool:CurrentVehicle=false);
native SP_SetOxygenLevel(ped,Float:level);
native SP_SetPosition(ped,Float:x,Float:y,Float:z,bool:SetCurrentVehiclePosition=false);
native SP_SetStaticWaitingForCollision(ped,bool:Static);
native SP_SetStayInSamePlace(ped,bool:Stay);
native SP_SetTargetRotation(ped,Float:rot);
native SP_SetTestForShotInVehicle(ped,bool:Test);
native SP_SetTurnMass(ped,Float:mass,bool:CurrentVehicle=false);
native SP_SetTurnSpeed(ped,Float:x,Float:y,Float:z,bool:CurrentVehicle=false);
native SP_SetUnderwater(ped,bool:UnderWater);
native SP_SetUsesCollision(ped,bool:UsesCollision);
native SP_SetVisible(ped,bool:Visible);
native SP_Teleport(ped,Float:x,Float:y,Float:z,bool:CurrentVehile=false);//curr position+tp position
native SP_SetGravity(Float:gravity);//0.00981 is a kinda default gravity (europe)
native SP_SetBlurLevel(blurlevel);//0..255
native SP_SetGameSpeed(Float:gamespeed);//1.0 = 100%

native print(text[]);
native printf(const format[], {Float,_}:...);

native GetTickCount();

native format(output[], len, const format[], {Float,_}:...);

/* Font Weights */
#define FW_DONTCARE         0
#define FW_THIN             100
#define FW_EXTRALIGHT       200
#define FW_LIGHT            300
#define FW_NORMAL           400
#define FW_MEDIUM           500
#define FW_SEMIBOLD         600
#define FW_BOLD             700
#define FW_EXTRABOLD        800
#define FW_HEAVY            900

#define FW_ULTRALIGHT       FW_EXTRALIGHT
#define FW_REGULAR          FW_NORMAL
#define FW_DEMIBOLD         FW_SEMIBOLD
#define FW_ULTRABOLD        FW_EXTRABOLD
#define FW_BLACK            FW_HEAVY

native SP_AddFont(fontname[],size,flags);

//only in DirectXRenderer
//color = 0xAARRGGBB;
native SP_DrawText(FontID,Float:X,Float:Y,color,text[],bool:UseColorParsing=false);//doesn't like empty "text", color parsing = "{FFFFFFFF}Hello {00000000}World {FF00FF00}!".
native SP_DrawLine(Float:sX,Float:sY,Float:sZ,Float:eX,Float:eY,Float:eZ,color);
native SP_DrawBox(Float:x,Float:y,Float:w,Float:h,color);
native SP_DrawBoxBorder(Float:x,Float:y,Float:w,Float:h,border_color,box_color);
/////////////////////////
native SP_GetScreenInfo(&depth,&refreshRate,&height,&width);
native SP_GetCursorPosition(&x,&y);
native SP_ShowCursor(bool:show);

native SP_IsKeyPressed(key);
native SP_IsKeyDown(key);//can be used for mouse buttons too

#define KEYPRESS_DOWN 0
#define KEYPRESS_UP 1
native SP_EmulateKeyPress(KeyPressType,key,bool:IsKeyExtended=false,bool:HardwareScanCode=false);//use IsKeyExtended for special keys or if the key you send is not the one GTA receives..
native SP_GetLastError();
//vehicle
native SP_IsPlayerInVehicle();
native SP_AddVehicleUpgrade(vehicle,modelid);
native SP_AreVehicleDoorsLocked(vehicle);
native SP_AreVehicleDoorsUndamageable(vehicle);
native SP_BurstVehicleTyre(vehicle,tyre);
native SP_CarHasRoof(vehicle);
native SP_ExtinguishCarFire(vehicle);
native SP_FixVehicle(vehicle);
native SP_GetCanVehicleBeDamaged(vehicle);
native SP_CanVehicleBeTargetedByBazoka(vehicle);
native SP_CanVehiclePetrolTankBeShot(vehicle);
native SP_DoesVehColorChangeOnPaintjob(vehicle);
native SP_GetVehicleColor(vehicle,&color1,&color2,&color3,&color4);
native SP_GetCurrentGear(vehicle);
native Float:SP_GetVehicleGasPedal(vehicle);
native Float:SP_GetVehicleBreakPedal(vehicle);

#define VHD_ABS (0) //bool enabled
#define VHD_AnimGroup (1) //byte animgroup
#define VHD_BrakeBias (2) //float
#define VHD_BrakeDecelleration (3) //float

#define CAR_DRIVE_TYPE_FOURWHEEL (52)
#define CAR_DRIVE_TYPE_FWD (70)
#define CAR_DRIVE_TYPE_RWD (82)
#define SetCarDriveType (4) //type

#define CAR_ENGINE_TYPE_DIESEL (68)
#define CAR_ENGINE_TYPE_ELECTRIC (69)
#define CAR_ENGINE_TYPE_PETROL (80)
#define VHD_CarEngineType (5) //type

#define VHD_CenterOfMass (6) //float float float
#define VHD_CollisionDamageMultiplier (7) //float
#define VHD_DragCoeff (8) //float
#define VHD_EngineAccelleration (9) //float
#define VHD_EngineInertia (10) //float
#define VHD_HandlingFlags (11) //flags

#define LIGHT_TYPE_BIG (2)
#define LIGHT_TYPE_LONG (0)
#define LIGHT_TYPE_SMALL (1)
#define LIGHT_TYPE_TALL (3)
#define VHD_HeadLight (12) // type

#define VHD_Mass (13) //float
#define VHD_MaxVelocity (14) //float
#define VHD_ModelFlags (15) //flags
#define VHD_Monetary (16) // amount
#define VHD_NumbeOfGears (17) //amount
#define VHD_PercentSubmerged (18) //percent int
#define VHD_SeatOffsetDistance (19) //float
#define VHD_SteeringLock (20) //float
#define VHD_SuspensionMultiplier (21) //float
#define VHD_SuspensionDamping (22) //float
#define VHD_SuspensionForceLevel (23) //float
#define VHD_SuspensionFrontRearBias (24) //float
#define VHD_SuspensionHighSpeedDaming (25) //float
#define VHD_SuspensionLowerLimit (26) //float
#define VHD_SuspensionUpperLimit (27) //float
#define VHD_TailLight (28) //LIGHT_TYPE
#define VHD_TractionBias (29) //float
#define VHD_TractionLoss (30) //float
#define VHD_TractionMultiplier (31) //float
#define VHD_TurnMass (32) //float

native SP_SetVehicleHandlingData(vehicle,VHD_TYPE,{Float,_}:...);
native Float:SP_GetVehicleHandlingDataFloat(vehicle,VHD_TYPE);
native SP_GetVehicleHandlingDataInt(vehicle,VHD_TYPE);
native SP_GetVehicleCenterOfMass(vehicle,&Float:x,&Float:y,&Float:z);
native SP_SetVehclePaintjob(vehicle,paintjobid);
native SP_SetVehclePaintjobDictionary(vehicle,paintjobidtexture);
native SP_SetChangeColourOnPaintjob(vehicle,bool:changecolor);
native SP_SetVehicleColor(vehicle,color1,color2,color3,color4);

//
native SP_CreateTexture(filename[],TextureHolder:texture);
native SP_CreateSprite(SpriteHolder:sprite);
native TextureHolder:SP_CreateTextureHolder();
native SpriteHolder:SP_CreateSpriteHolder();
native D3DMAT:SP_CreateD3DMatrixHolder();
native VECT2:SP_CreateVector2Holder();
native SP_Vector2Set(VECT2:id,Float:x,Float:y);
native SP_Vector2Get(VECT2:id,&Float:x,&Float:y);
native SP_D3DMatrixTransformation2D(D3DMAT:pOut,VECT2:pScalingCenter,Float:ScalingRotation,VECT2:pScaling,VECT2:pRotationCenter,Float:Rotation,VECT2:pTranslation);
native SP_Sprite_Begin(SpriteHolder:sprite,flags);
native SP_Sprite_End(SpriteHolder:sprite);
native SP_Sprite_SetTransform(SpriteHolder:sprite,D3DMAT:pTransform);
native SP_Sprite_Draw(SpriteHolder:sprite,TextureHolder:texture,RECT:pSrcRect,VECT3:pCenter,VECT3:pPosition,color);
native VECT3:SP_CreateVector3Holder();
native SP_Vector3Set(VECT3:id,Float:x,Float:y,Float:z);
native SP_Vector3Get(VECT3:id,&Float:x,&Float:y,&Float:z);
native RECT:SP_CreateRectangleHolder();
native SP_RectangleSet(RECT:id,bottom,left,right,top);
native SP_RectangleGet(RECT:id,&bottom,&left,&right,&top);
native SP_GetBackBufferHeight();
native SP_GetBackBufferWidth();
native SP_Texture_Release(TextureHolder:texture);
native SP_Sprite_Release(SpriteHolder:sprite);
native SP_IsTextureNull(TextureHolder:texture);
native SP_IsSpriteNull(SpriteHolder:sprite);

native SP_GetVehicleMatrix(vehicle,&Float:vRightX,&Float:vRightY,&Float:vRightZ,&Float:vFrontX,&Float:vFrontY,&Float:vFrontZ,&Float:vUpX,&Float:vUpY,&Float:vUpZ,&Float:vPosX,&Float:vPosY,&Float:vPosZ);
native SP_SetVehicleMatrix(vehicle,Float:vRightX,Float:vRightY,Float:vRightZ,Float:vFrontX,Float:vFrontY,Float:vFrontZ,Float:vUpX,Float:vUpY,Float:vUpZ,Float:vPosX,Float:vPosY,Float:vPosZ);
native SP_RotateVehicleMatrix(vehicle,Float:vRightX,Float:vRightY,Float:vRightZ,Float:vFrontX,Float:vFrontY,Float:vFrontZ,Float:vUpX,Float:vUpY,Float:vUpZ,Float:vPosX,Float:vPosY,Float:vPosZ,Float:rotX,Float:rotY,Float:rotZ,Float:rotTheta,&Float:ovRightX,&Float:ovRightY,&Float:ovRightZ,&Float:ovFrontX,&Float:ovFrontY,&Float:ovFrontZ,&Float:ovUpX,&Float:ovUpY,&Float:ovUpZ,&Float:ovPosX,&Float:ovPosY,&Float:ovPosZ);

#define ROTATION_VECTOR_RIGHT (0)
#define ROTATION_VECTOR_FRONT (1)
#define ROTATION_VECTOR_UP (2)
#define ROTATION_VECTOR_POS (3)
native SP_GetVehicleRotation(vehicle,rotation_vector,&Float:XY,&Float:YX,&Float:ZY,&Float:YZ,&Float:ZX,&Float:XZ);
native SP_GetVehicleEulerRotation(vehicle,&Float:x,&Float:y,&Float:z);

native SP_ReloadMe();
native SP_ReloadScript(scriptname[]);
native SP_LoadScript(scriptname[]);
native SP_UnloadScript(scriptname[]);

native SP_LoadDll(library_name[],bool:IsAmxPlugin=true);//all filenames must be unique
native SP_UnloadDll(library_name[]);

native SP_ProcessLineOfSight(Float:fX,Float:fY,Float:fZ,Float:eX,Float:eY,Float:eZ,&Float:cX,&Float:cY,&Float:cZ);
native SP_TERMINATE_GTA();

native SP_DrawRadarArea(Float:minx,Float:miny,Float:maxx,Float:maxy,color_bgra);

#define PED_PLAYER (0)

native SP_IsValidPed(ped);
native SP_IsValidVehicle(vehicle);
native SP_GetPedCount();
native SP_GetVehicleCount();
native SP_GetPedPoolSize();
native SP_GetVehiclePoolSize();
native SP_AddVehicle(model,Float:X,Float:Y,Float:Z,Float:A,color_body,color_stripe,color_body2,color_stripe2);
native SP_GetPedVehicle(ped);

forward OnScriptInit();
forward OnScriptExit();

#define PLAYER_STATE_NONE (0)
#define PLAYER_STATE_ONFOOT (1)
#define PLAYER_STATE_VEHICLE (2)

//forward OnPedHealthChange(Float:oldHealth,Float:newHealth);
//forward OnVehicleHealthChange(Float:oldHealth,Float:newHealth);
//forward OnPedStateChange(oldstate,newstate);
//forward OnPedDead();
//forward OnPedVehicleEnter();
//forward OnPedVehicleExit();
forward DirectXRenderer(Float:TimeDiff);
forward InitDXRenderer();
forward UnInitDXRenderer();
forward OnConsoleCommand(command[]);
forward OnGameLoad();
forward OnGangZoneDrawHookRun();//draw radar areas here
forward OnGameRender();//draw Textdraws here
//keys
/*
 * Virtual Keys, Standard Set
 */
#define VK_LBUTTON        0x01
#define VK_RBUTTON        0x02
#define VK_CANCEL         0x03
#define VK_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */

//#if(_WIN32_WINNT >= 0x0500)
#define VK_XBUTTON1       0x05    /* NOT contiguous with L & RBUTTON */
#define VK_XBUTTON2       0x06    /* NOT contiguous with L & RBUTTON */
//#endif /* _WIN32_WINNT >= 0x0500 */

/*
 * 0x07 : unassigned
 */

#define VK_BACK           0x08
#define VK_TAB            0x09

/*
 * 0x0A - 0x0B : reserved
 */

#define VK_CLEAR          0x0C
#define VK_RETURN         0x0D

#define VK_SHIFT          0x10
#define VK_CONTROL        0x11
#define VK_MENU           0x12
#define VK_PAUSE          0x13
#define VK_CAPITAL        0x14

#define VK_KANA           0x15
#define VK_HANGEUL        0x15  /* old name - should be here for compatibility */
#define VK_HANGUL         0x15
#define VK_JUNJA          0x17
#define VK_FINAL          0x18
#define VK_HANJA          0x19
#define VK_KANJI          0x19

#define VK_ESCAPE         0x1B

#define VK_CONVERT        0x1C
#define VK_NONCONVERT     0x1D
#define VK_ACCEPT         0x1E
#define VK_MODECHANGE     0x1F

#define VK_SPACE          0x20
#define VK_PRIOR          0x21
#define VK_NEXT           0x22
#define VK_END            0x23
#define VK_HOME           0x24
#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28
#define VK_SELECT         0x29
#define VK_PRINT          0x2A
#define VK_EXECUTE        0x2B
#define VK_SNAPSHOT       0x2C
#define VK_INSERT         0x2D
#define VK_DELETE         0x2E
#define VK_HELP           0x2F

/*
 * VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
 * 0x40 : unassigned
 * VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
 */

#define VK_LWIN           0x5B
#define VK_RWIN           0x5C
#define VK_APPS           0x5D

/*
 * 0x5E : reserved
 */

#define VK_SLEEP          0x5F

#define VK_NUMPAD0        0x60
#define VK_NUMPAD1        0x61
#define VK_NUMPAD2        0x62
#define VK_NUMPAD3        0x63
#define VK_NUMPAD4        0x64
#define VK_NUMPAD5        0x65
#define VK_NUMPAD6        0x66
#define VK_NUMPAD7        0x67
#define VK_NUMPAD8        0x68
#define VK_NUMPAD9        0x69
#define VK_MULTIPLY       0x6A
#define VK_ADD            0x6B
#define VK_SEPARATOR      0x6C
#define VK_SUBTRACT       0x6D
#define VK_DECIMAL        0x6E
#define VK_DIVIDE         0x6F
#define VK_F1             0x70
#define VK_F2             0x71
#define VK_F3             0x72
#define VK_F4             0x73
#define VK_F5             0x74
#define VK_F6             0x75
#define VK_F7             0x76
#define VK_F8             0x77
#define VK_F9             0x78
#define VK_F10            0x79
#define VK_F11            0x7A
#define VK_F12            0x7B
#define VK_F13            0x7C
#define VK_F14            0x7D
#define VK_F15            0x7E
#define VK_F16            0x7F
#define VK_F17            0x80
#define VK_F18            0x81
#define VK_F19            0x82
#define VK_F20            0x83
#define VK_F21            0x84
#define VK_F22            0x85
#define VK_F23            0x86
#define VK_F24            0x87

/*
 * 0x88 - 0x8F : unassigned
 */

#define VK_NUMLOCK        0x90
#define VK_SCROLL         0x91

/*
 * NEC PC-9800 kbd definitions
 */
#define VK_OEM_NEC_EQUAL  0x92   // '=' key on numpad

/*
 * Fujitsu/OASYS kbd definitions
 */
#define VK_OEM_FJ_JISHO   0x92   // 'Dictionary' key
#define VK_OEM_FJ_MASSHOU 0x93   // 'Unregister word' key
#define VK_OEM_FJ_TOUROKU 0x94   // 'Register word' key
#define VK_OEM_FJ_LOYA    0x95   // 'Left OYAYUBI' key
#define VK_OEM_FJ_ROYA    0x96   // 'Right OYAYUBI' key

/*
 * 0x97 - 0x9F : unassigned
 */

/*
 * VK_L* & VK_R* - left and right Alt, Ctrl and Shift virtual keys.
 * Used only as parameters to GetAsyncKeyState() and GetKeyState().
 * No other API or message will distinguish left and right keys in this way.
 */
#define VK_LSHIFT         0xA0
#define VK_RSHIFT         0xA1
#define VK_LCONTROL       0xA2
#define VK_RCONTROL       0xA3
#define VK_LMENU          0xA4
#define VK_RMENU          0xA5

//#if(_WIN32_WINNT >= 0x0500)
#define VK_BROWSER_BACK        0xA6
#define VK_BROWSER_FORWARD     0xA7
#define VK_BROWSER_REFRESH     0xA8
#define VK_BROWSER_STOP        0xA9
#define VK_BROWSER_SEARCH      0xAA
#define VK_BROWSER_FAVORITES   0xAB
#define VK_BROWSER_HOME        0xAC

#define VK_VOLUME_MUTE         0xAD
#define VK_VOLUME_DOWN         0xAE
#define VK_VOLUME_UP           0xAF
#define VK_MEDIA_NEXT_TRACK    0xB0
#define VK_MEDIA_PREV_TRACK    0xB1
#define VK_MEDIA_STOP          0xB2
#define VK_MEDIA_PLAY_PAUSE    0xB3
#define VK_LAUNCH_MAIL         0xB4
#define VK_LAUNCH_MEDIA_SELECT 0xB5
#define VK_LAUNCH_APP1         0xB6
#define VK_LAUNCH_APP2         0xB7

//#endif /* _WIN32_WINNT >= 0x0500 */

/*
 * 0xB8 - 0xB9 : reserved
 */

#define VK_OEM_1          0xBA   // ';:' for US
#define VK_OEM_PLUS       0xBB   // '+' any country
#define VK_OEM_COMMA      0xBC   // ',' any country
#define VK_OEM_MINUS      0xBD   // '-' any country
#define VK_OEM_PERIOD     0xBE   // '.' any country
#define VK_OEM_2          0xBF   // '/?' for US
#define VK_OEM_3          0xC0   // '`~' for US

/*
 * 0xC1 - 0xD7 : reserved
 */

/*
 * 0xD8 - 0xDA : unassigned
 */

#define VK_OEM_4          0xDB  //  '[{' for US
#define VK_OEM_5          0xDC  //  '\|' for US
#define VK_OEM_6          0xDD  //  ']}' for US
#define VK_OEM_7          0xDE  //  ''"' for US
#define VK_OEM_8          0xDF

/*
 * 0xE0 : reserved
 */

/*
 * Various extended or enhanced keyboards
 */
#define VK_OEM_AX         0xE1  //  'AX' key on Japanese AX kbd
#define VK_OEM_102        0xE2  //  "<>" or "\|" on RT 102-key kbd.
#define VK_ICO_HELP       0xE3  //  Help key on ICO
#define VK_ICO_00         0xE4  //  00 key on ICO

//#if(WINVER >= 0x0400)
#define VK_PROCESSKEY     0xE5
//#endif /* WINVER >= 0x0400 */

#define VK_ICO_CLEAR      0xE6


//#if(_WIN32_WINNT >= 0x0500)
#define VK_PACKET         0xE7
//#endif /* _WIN32_WINNT >= 0x0500 */

/*
 * 0xE8 : unassigned
 */

/*
 * Nokia/Ericsson definitions
 */
#define VK_OEM_RESET      0xE9
#define VK_OEM_JUMP       0xEA
#define VK_OEM_PA1        0xEB
#define VK_OEM_PA2        0xEC
#define VK_OEM_PA3        0xED
#define VK_OEM_WSCTRL     0xEE
#define VK_OEM_CUSEL      0xEF
#define VK_OEM_ATTN       0xF0
#define VK_OEM_FINISH     0xF1
#define VK_OEM_COPY       0xF2
#define VK_OEM_AUTO       0xF3
#define VK_OEM_ENLW       0xF4
#define VK_OEM_BACKTAB    0xF5

#define VK_ATTN           0xF6
#define VK_CRSEL          0xF7
#define VK_EXSEL          0xF8
#define VK_EREOF          0xF9
#define VK_PLAY           0xFA
#define VK_ZOOM           0xFB
#define VK_NONAME         0xFC
#define VK_PA1            0xFD
#define VK_OEM_CLEAR      0xFE


#define VK_KEY_0 0x30
#define VK_KEY_1 0x31
#define VK_KEY_2 0x32
#define VK_KEY_3 0x33
#define VK_KEY_4 0x34
#define VK_KEY_5 0x35
#define VK_KEY_6 0x36
#define VK_KEY_7 0x37
#define VK_KEY_8 0x38
#define VK_KEY_9 0x39

#define VK_KEY_A 0x41
#define VK_KEY_B 0x42
#define VK_KEY_C 0x43
#define VK_KEY_D 0x44
#define VK_KEY_E 0x45
#define VK_KEY_F 0x46
#define VK_KEY_G 0x47
#define VK_KEY_H 0x48
#define VK_KEY_I 0x49
#define VK_KEY_J 0x4A
#define VK_KEY_K 0x4B
#define VK_KEY_L 0x4C
#define VK_KEY_M 0x4D
#define VK_KEY_N 0x4E
#define VK_KEY_O 0x4F
#define VK_KEY_P 0x50
#define VK_KEY_Q 0x51
#define VK_KEY_R 0x52
#define VK_KEY_S 0x53
#define VK_KEY_T 0x54
#define VK_KEY_U 0x55
#define VK_KEY_V 0x56
#define VK_KEY_W 0x57
#define VK_KEY_X 0x58
#define VK_KEY_Y 0x59
#define VK_KEY_Z 0x5A

/*
 * 0xFF : reserved
 */
///

// Not sure if R* used these
#define DT_ENGINE_OK					(0)
#define DT_ENGINE_RADIATOR_BURST		(100)
#define DT_ENGINE_ENGINE_OVERHEAT		(150)
#define DT_ENGINE_ENGINE_PIPES_BURST	(200)
#define DT_ENGINE_ON_FIRE				(225)
#define DT_ENGINE_FUCKED				(250)

/*******************SSFE -- Samp Server Functions Emulator*********************/
#tryinclude <SSFE_03e>
#tryinclude "SSFE_03e.inc"

native SP_DestroyVehicle(vehicle);
native SP_GetDriver(vehicle);
native SP_GetPassenger(vehicle,seat);
native bool:SP_IsBeingDriven(vehicle);
native bool:SP_CanPlayerEnterCar(vehicle);
native bool:SP_CanPedJumpOutCar(vehicle,ped);
native SP_RemoveVehicleUpgrade(vehicle,modelid);
native SP_CanPedLeanOutFromCar(vehicle,ped);
native SP_CanPedStepOutCar(vehicle,ped);
native SP_LockVehicleDoors(vehicle,bool:Locked);
native SP_SetCarDoorsUndamageable(vehicle,bool:Undamageable);
native SP_GetCarBaseType(vehicle);
native SP_SetCarBodyDirtLevel(vehicle,Float:level);
native Float:SP_GetCarBodyDirtLevel(vehicle);
native Float:SP_GetCarHeightAboveRoad(vehicle);
native Float:SP_GetSteerAngle(vehicle);
native bool:SP_IsCarOnItsSide(vehicle);
native bool:SP_IsCarLawEnforcment(vehicle);
native bool:SP_IsPedPassenger(vehicle,ped);
native bool:SP_IsCarTouchingSphere(vehicle,Float:CenterX,Float:CenterY,Float:CenterZ,Float:Radius);
native bool:SP_IsCarUpsideDown(vehicle);
native bool:SP_IsCarEngineBroken(vehicle);
native SP_SetCarEngineBroken(vehicle,bool:Broken);
native bool:SP_IsCarScriptLocked(vehicle);
native SP_SetCarScriptLocked(vehicle,bool:Locked);
native SP_PlaceBikeOnRoad(vehicle);
native SP_PlaceAutomobileOnRoad(vehicle);
native bool:SP_IsCarSirenOrAlarmActive(vehicle);
native SP_SetCarSirenOrAlarmActive(vehicle,bool:active);
native SP_GetCarAlpha(vehicle);
native SP_SetCarAlpha(vehicle,alpha);
native SP_FuckCarCompletely(vehicle);
native SP_GetCarAeroplaneCompStatus(vehicle,component);
native SP_GetCarDoorStatus(vehicle,door);
native SP_GetCarEngineStatus(vehicle);
native SP_GetCarLightStatus(vehicle,light);
native SP_GetCarPanelStatus(vehicle,panel);
native SP_GetCarWheelStatus(vehicle,tire);
native SP_SetCarAeroplaneCompStatus(vehicle,component,status);
native SP_SetCarDoorStatus(vehicle,door,status);
native SP_SetCarEngineStatus(vehicle,status);
native SP_SetCarLightStatus(vehicle,light,status);
native SP_SetCarPanelStatus(vehicle,panel,status);
native SP_SetCarWheelStatus(vehicle,tire,status);
native SP_FadeCarOut(vehicle,bool:fade);
native SP_IsCarFadingOut(vehicle);
native SP_SetVehicleTowLink(vehicle_1,vehicle_2);
native SP_BreakVehicleTowLink(vehicle);
native SP_GetTowedVehicle(vehicle);
native SP_GetTowedByVehicle(vehicle);
native SP_SetCarWinchType(vehicle,winchtype);
native SP_DropCarPickUpStuffWithWinch(vehicle);
native SP_SetRopeHeightForHeli(vehicle,Float:height);
native SP_GetVehiclePaintjob(vehicle);
native bool:SP_IsCarDamaged(vehicle);
native bool:SP_IsCarDrowning(vehicle);
native bool:SP_IsCarEngineOn(vehicle);
native bool:SP_IsCarHandbrakeOn(vehicle);
native bool:SP_DoesCarLeaveSkidMarks(vehicle);
native bool:SP_GetCarComedyControls(vehicle);
native bool:SP_GetCarGunSwitchedOff(vehicle);
native bool:SP_GetCarLightsOn(vehicle);
native bool:SP_GetCarOverrideLights(vehicle);
native bool:SP_GetCarTakeLessDamage(vehicle);
native bool:SP_GetCarTyresDontBurst(vehicle);
native SP_GetCarAdjustablePropertyVal(vehicle);
native Float:SP_GetHeliRotorSpeed(vehicle);
native SP_GetCarExplodeTime(vehicle);
native SP_SetCarDoesLeaveSkidMarks(vehicle,bool:does);
native SP_SetCarCanBeDamaged(vehicle,bool:CanBeDamaged);
native SP_SetCarCanBeTargetByBazooka(vehicle,bool:CanBeTargetted);
native SP_SetCarCanShootPetrolTank(vehicle,bool:CanBeShoot);
native SP_SetCarComedyControls(vehicle,bool:enabled);
native SP_SetCarEngine(vehicle,bool:EngineOn);
native SP_SetCarGunSwitchedOff(vehicle,bool:GunsOff);
native SP_SetCarHandbrake(vehicle,bool:HandbrakeOn);
native SP_SetCarLights(vehicle,bool:LightsOn);
native SP_SetCarTakeLessDamage(vehicle,bool:LessDamage);
native SP_SetCarTyresDontBurst(vehicle,bool:DontBurst);
native SP_SetCarAdjustablePropertyVal(vehicle,value);
native SP_SetHeliRotorSpeed(vehicle,Float:RotorSpeed);
native SP_SetTaxiLight(vehicle,bool:TurnedOn);
native SP_GetBikeWheelStatus(vehicle,wheel);
native SP_SetBikeWheelStatus(vehicle,wheel,status);
native bool:SP_IsVehicleOnScreen(vehicle);
native bool:SP_IsVehicleVisible(vehicle);
native Float:SP_GetVehicleHealth(vehicle);
native SP_SetVehicleHealth(vehicle,Float:health);
native SP_SetCarLandingGearDown(vehicle,bool:GearDown);
native bool:SP_IsCarLandingGearDown(vehicle);
native Float:SP_GetCarLandingGearPosition(vehicle);
native SP_SetCarLandingGearPosition(vehicle,Float:position);
native SP_GetCarTurretRotation(vehicle,&Float:Horizontal,&Float:Vertical);
native SP_SetCarTurretRotation(vehicle,Horizontal,Vertical);
native SP_GetCarNumberGettingIn(vehicle);
native SP_GetCarPassengerCount(vehicle);
native SP_GetCarMaxPassengerCount(vehicle);
native bool:SP_IsCarSmokeTrailEnabled(vehicle);
native SP_SetCarSmokeTrailEnabled(vehicle,bool:enabled);
native SP_GetVehicleGravity(vehicle,&Float:X,&Float:Y,&Float:Z);
native SP_SetVehicleGravity(vehicle,Float:X,Float:Y,Float:Z);
native SP_GetCarHeadLightColor(vehicle);//0xAARRGGBB
native SP_SetCarHeadLightColor(vehicle,color);
native SP_SetCarWheelVisibility(vehicle,tire,bool:visible);
native bool:SP_IsHeliSearchLightVisible(vehicle);
native SP_SetHeliSearchLightVisible(vehicle,bool:visible);
native SP_UpdateCarMovingCollision(vehicle,Float:angle);
native SP_SetVehicleMoveSpeed(vehicle,Float:X,Float:Y,Float:Z);
native SP_GetVehicleMoveSpeed(vehicle,&Float:X,&Float:Y,&Float:Z);
native SP_SetCarOrientation(vehicle,Float:X,Float:Y,Float:Z);
native Float:SP_GetCarDamageMagnitude(vehicle);
native SP_SetCarDamageMagnitude(vehicle,Float:magnitude);
native SP_SetVehicleTurnSpeed(vehicle,Float:X,Float:Y,Float:Z);
native SP_GetVehicleTurnSpeed(vehicle,&Float:X,&Float:Y,&Float:Z);
native SP_SetVehicleMass(vehicle,Float:mass);
native Float:SP_GetVehicleMass(vehicle);
native SP_SetVehicleTurnMass(vehicle,Float:mass);
native Float:SP_GetVehicleTurnMass(vehicle);
native SP_SetVehicleElasticity(vehicle,Float:elasticity);
native Float:SP_GetVehicleElasticity(vehicle);
native SP_SetCarBuoyancyConstant(vehicle,Float:constante);
native Float:SP_GetCarBuoyancyConstant(vehicle);
native SP_ResetVehicleLastDamage(vehicle);
native Float:SP_GetVehicleLighting(vehicle);
native SP_SetVehicleLighting(vehicle,Float:lighting);
native SP_SetVehiclePosition(vehicle,Float:X,Float:Y,Float:Z);
native SP_TeleportVehicle(vehicle,Float:X,Float:Y,Float:Z);
native SP_FixBoatOrientation(vehicle);
native SP_GetVehiclePosition(vehicle,&Float:X,&Float:Y,&Float:Z);
native SP_SetVehicleRoll(vehicle,Float:X,Float:Y,Float:Z);
native SP_SetVehicleDirection(vehicle,Float:X,Float:Y,Float:Z);
native SP_SetVehicleWas(vehicle,Float:X,Float:Y,Float:Z);
native SP_GetVehicleModel(vehicle);
native Float:SP_GetVehicleCentreDistance(vehicle);
native SP_SetVehicleStatus(vehicle,status);
native SP_SetVehicleVisible(vehicle,bool:visible);
native SP_GetVehicleStatus(vehicle);
native SP_GetVehicleAreaCode(vehicle);
native SP_SetVehicleAreaCode(vehicle,code);
native SP_GetVehicleImmunities(vehicle,&bool:NoClip,&bool:Frozen,&bool:BulletProof,&bool:FlameProof,&bool:unknown_1,&bool:unknown_2,&bool:CollisionProof,&bool:ExplosionProof);
native bool:SP_IsVehicleUnderwater(vehicle);
native SP_SetVehicleUnderwater(vehicle,bool:underwater);

//new key module - 6.0

native KeyHook_DOWN(key);
native KeyHook_UP(key);
native KeyHook_PRESSED(key);
native KeyHook_RELEASED(key);
native KeyHook_CONSUME(key);

//peds

//native SP_AddPed(modelid,Float:X,Float:Y,Float:Z);

//
native SP_AddPlayerMoney(money);

//SCM OPCODE ENGINE
#define SCM_is_model_available			0x0248, "i"		// #MODEL
#define SCM_create_thread				0x00D7, "iz"		// StartIP (4F)
#define SCM_end_thread					0x004E, ""		// -/-
#define SCM_wait						0x0001, "i"		// time_in_ms
#define SCM_name_thread				0x03A4, "s"		// "MAIN"
#define SCM_set_fade_color				0x0169, "iii"	// Red(0-255), Green(0-255), Blue(0-255)
#define SCM_fade						0x016A, "ii"		// (time in ms), FADE_*
#define SCM_is_fading					0x016B, ""		// -/-
#define SCM_set_max_wanted_level		0x01F0, "i"		// MaxLevel
#define SCM_set_wasted_busted_check	0x0111, "i"		// Check(1/0)
#define SCM_set_current_time			0x00C0, "ii"		// Hours, Minutes
#define SCM_refresh_screen				0x04E4, "ff"		// x, y
#define SCM_set_camera					0x03CB, "fff"	// x, y, z
#define SCM_set_weather				0x01B6, "i"		// WEATHER_*
#define SCM_select_interior			0x04BB, "i"		// INTERIOR_*
#define SCM_play_music					0x0394, "i"		// music
#define SCM_text_styled				0x00BA, "sii"	// TEXT, time, style
#define SCM_text_now					0x00BC, "sii"	// TEXT, time, style
#define SCM_text_1number_styled		0x01E3, "siii"	// TEXT, number, time, style
#define SCM_toggle_widescreen			0x02A3, "i"		// widescreen(1/0)
#define SCM_set_camera_behind_player	0x0373, ""		// -/-
#define SCM_camera_on_vehicle			0x0158, "vii"	// var_car, ukn, ukn
#define SCM_restore_camera				0x015A, ""		// -/-
#define SCM_restore_camera_jumpcut		0x02EB, ""		// -/-
#define SCM_set_camera_position		0x015F, "ffffff" // x, y, z, vx, vy, vz
#define SCM_point_camera				0x0160, "fffi"	// x, y, z, type
#define SCM_restart_if_wasted_at		0x016C, "ffffi"	// x, y, z, a, tn
#define SCM_restart_if_busted_at		0x016D, "ffffi"	// x, y, z, a, tn
#define SCM_create_player				0x0053, "ifffv"	// 0, x, y, z, PLAYER_CHAR
#define SCM_create_actor_from_player	0x01F5, "vv"		// PLAYER_CHAR, PLAYER_ACTOR
#define SCM_rebuild_player				0x070D, "v"		// PLAYER_CHAR
#define SCM_set_player_clothes			0x087B, "vssi"	// PLAYER_CHAR, clothes_texture, model, bodypart
#define SCM_clear_player_wanted_level	0x0110, "v"		// PLAYER_CHAR
#define SCM_set_player_wanted_level	0x010D, "vi"		// PLAYER_CHAR, level
#define SCM_set_actor_health			0x0223, "vi"		// var_actor, health
#define SCM_set_actor_armour			0x035F, "vi"		// var_actor, armour
#define SCM_save_player_clothes		0x0793, ""		// -/-
#define SCM_end_thread_named			0x0459, "s"		// "MAIN"
#define SCM_show_save_screen 			0x03D8, ""		// -/-
#define SCM_save_done					0x03D9, ""		// -/-
#define SCM_create_pickup				0x0213, "iifffv"	// model, type, x, y, z,var_ pickup
#define SCM_destory_pickup				0x0215, "v"		// var_pickup
#define SCM_pickup_picked_up			0x0214, "v"		// var_pickup
#define SCM_player_defined				0x0256, "v"		// PLAYER_CHAR
#define SCM_freeze_player				0x01B4, "vi"		// PLAYER_CHAR, Freeze(1/0)
#define SCM_put_actor_at				0x00A1, "vfff"	// var_actor, x, y, z
#define SCM_set_actor_z_angle			0x0173, "vf"		// var_actor, angle
#define SCM_create_marker				0x04CE, "fffiv"	// x, y, z, icon_type, var_marker
#define SCM_request_model				0x0247, "i"		// (CAR_*|BIKE_*|BOAT_*|WEAPON_*|OBJECT_*)
#define SCM_load_requested_models		0x038B, ""		// -/-
#define SCM_give_actor_weapon			0x01B2, "vii"	// var_actor, weapon, ammo

native SCM_EXEC(opcode,format[],{Float,_}:...);
native VAR_SCM_CAR(VehicleID);//returns a opcode 'v' for the vehicleid
native VAR_SCM_PED(PedID);//returns a opcode 'v' for the pedid
native VAR_SCM_OBJ(ObjID);//^'							'objectid
native SCM_CAR_VehicleID(VAR);//returns the VehicleID of the specified VAR
native SCM_CAR_PedID(VAR);//returns the PedID of the specified VAR
native SCM_CAR_ObjectID(VAR);//returns the ObjectID of the specified VAR

//
native SP_GameTextDraw(Float:x, Float:Y, Float:scaleX, Float:ScaleY, font, aligment, red, green, blue, alpha, outline, shadow, text[]);
native SP_GameBoxDraw(Float:x1, Float:y1, Float:x2, Float:y2, red, green, blue, alpha);
native SP_SpawnCar(modelid);

//camera
native SP_CamVectorTrackLinear(Float:ToX,Float:ToY,Float:ToZ,Float:FromX,Float:FromY,Float:FromZ,Float:time,bool:SmoothEnds);
native SP_CamFade(Float:FadeTime,FadeInOrOut);
native SP_CamFind3rdPTargetVector(Float:Distance,Float:GunMuzzleX,Float:GunMuzzleY,Float:GunMuzzleZ,Float:srcX,Float:srcY,Float:srcZ,Float:TargetX,Float:TargetY,Float:TargetZ);
native Float:SP_CamFind3rdPQuickAimPitch();
native SP_GetActiveCam();
native SP_GetCamFixedModeSource(CameraID,&Float:X,&Float:Y,&Float:Z);
native SP_GetCamFixedModeVector(CameraID,&Float:X,&Float:Y,&Float:Z);
native SP_GetCamFront(CameraID,&Float:X,&Float:Y,&Float:Z);
native Float:SP_GetCamFOV(CameraID);
native SP_GetCamSource(CameraID,&Float:X,&Float:Y,&Float:Z);
native SP_GetCamMode(CameraID);
native SP_GetCamUp(CameraID,&Float:X,&Float:Y,&Float:Z);
native SP_SetCamFOV(CameraID,Float:FOV);
native SP_GetCamTargetHistoryPos(CameraID,&Float:X,&Float:Y,&Float:Z);
native SP_GetCamTargetEntity(CameraID,&ID,&Type,&Status);
native Float:SP_GetCamRotation();
native Float:SP_GetCamCarZoom();
native SP_GetCamFadingDirection();
native SP_GetCamLTM(&Float:atX,&Float:atY,&Float:atZ,&flags,&pad1,&pad2,&pad3,&Float:posX,&Float:posY,&Float:posZ,&Float:rightX,&Float:rightY,&Float:rightZ,&Float:upX,&Float:upY,&Float:upZ);
native SP_GetCamMatrix(&Float:frontX,&Float:frontY,&Float:frontZ,&Float:posX,&Float:posY,&Float:posZ,&Float:rightX,&Float:rightY,&Float:rightZ,&Float:upX,&Float:upY,&Float:upZ);
native SP_GetCamWidescreen();
native SP_IsCamFading();
native SP_CamRestore();
native SP_CamRestoreWithJumpCut();
native SP_SetCameraClip(bool:objects,bool:vehicles);
native SP_SetCamPositionForFixedMode(Float:X,Float:Y,Float:Z,Float:upX,Float:upY,Float:upZ);
native SP_SetCamCarZoom(Float:zoom);
native SP_SetCamFadeColor(red,green,blue);
native SP_SetCamMatrix(Float:frontX,Float:frontY,Float:frontZ,Float:posX,Float:posY,Float:posZ,Float:rightX,Float:rightY,Float:rightZ,Float:upX,Float:upY,Float:upZ);
native SP_SetCamWidescreen(bool:WideScreen);
native SP_CamTakeControl(Float:X,Float:Y,Float:Z,CamSwitchStyle);
native SP_CamTakeControlEx(EntityType,EntityID,CameraMode,CamSwitchStyle);
native SP_CamTakeControlAttachToEntity(EntityType1,EntityID1,EntityType2,EntityID2,Float:offX,Float:offY,Float:offZ,Float:LookAtX,Float:LookAtY,Float:LookAtZ,Float:Tilt,CamSwitchStyle);
native SP_CamConeCastCollisionResolve(Float:posX,Float:posY,Float:posZ,Float:LookAtX,Float:LookAtY,Float:LookAtZ,&Float:DestX,&Float:DestY,&Float:DestZ,Float:rad,Float:MinDist,&Float:distance);