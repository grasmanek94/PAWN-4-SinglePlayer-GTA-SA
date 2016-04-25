//#pragma message("Compiling precompiled header.\n")

// Pragmas
#pragma warning (disable:4409)
#pragma warning (disable:4250)

#include <windows.h>
#define MTA_CLIENT
#define SHARED_UTIL_WITH_HASH_MAP
#define SHARED_UTIL_WITH_FAST_HASH_MAP
#define SHARED_UTIL_WITH_SYS_INFO
#include "SharedUtil.h"
#include "SharedUtil.MemAccess.h"
#include <stdio.h>

#include <algorithm>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

// SDK includes
//#include <core/CCoreInterface.h>
//#include <net/CNet.h>
#include <game/CGame.h>
#include <game/CWanted.h>
#include <CMatrix_Pad.h>
#include <../version.h>
#include <ijsify.h>

// Game includes
#include "CEntitySA.h"
#include "gamesa_init.h"
#include "Common.h"
#include "CGameSA.h"
#include "CWorldSA.h"
#include "CPoolsSA.h"
#include "CClockSA.h"
#include "CFontSA.h"
#include "CRadarSA.h"
#include "CMenuManagerSA.h"
#include "CCameraSA.h"
#include "CCheckpointsSA.h"
#include "CRenderWareSA.h"
#include "CCoronasSA.h"
#include "CPickupsSA.h"
#include "CPathFindSA.h"
#include "CWeaponInfoSA.h"
#include "CExplosionManagerSA.h"
#include "CFireManagerSA.h"
#include "CHandlingManagerSA.h"
#include "CHudSA.h"
#include "C3DMarkersSA.h"
#include "CStatsSA.h"
#include "CTheCarGeneratorsSA.h"
#include "CPadSA.h"
#include "CAERadioTrackManagerSA.h"
#include "CWeatherSA.h"
#include "CTextSA.h"
#include "CPedSA.h"
#include "CPedSoundSA.h"
#include "CAudioEngineSA.h"
#include "CAudioContainerSA.h"
#include "CPlayerInfoSA.h"
#include "CPopulationSAMP.h"
#include "CSettingsSA.h"
#include "CCarEnterExitSA.h"
#include "COffsets.h"
#include "CControllerConfigManagerSA.h"
#include "CProjectileInfoSA.h"
#include "CEventListSA.h"
#include "CGaragesSA.h"
#include "CTasksSA.h"
#include "CEventDamageSA.h"
#include "CEventGunShotSA.h"
#include "CAnimManagerSA.h"
#include "CStreamingSA.h"
#include "CVisibilityPluginsSA.h"
#include "CKeyGenSA.h"
#include "CRopesSA.h"
#include "CFxSA.h"
#include "CFxSystemBPSA.h"
#include "CFxSystemSA.h"
#include "CFxManagerSA.h"
#include "HookSystem.h"
#include "CModelInfoSA.h"
#include "CPedModelInfoSA.h"
#include "CColPointSA.h"
#include "CCivilianPedSA.h"
#include "CAnimBlendAssociationSA.h"
#include "CAnimBlendAssocGroupSA.h"
#include "CAnimBlendHierarchySA.h"
#include "CAnimBlendSequenceSA.h"
#include "CAnimBlendStaticAssociationSA.h"
#include "CAnimBlockSA.h"
#include "CAutomobileSA.h"
#include "CBikeSA.h"
#include "CBoatSA.h"
#include "CBmxSA.h"
#include "CQuadBikeSA.h"
#include "CMonsterTruckSA.h"
#include "CPlaneSA.h"
#include "CTrailerSA.h"
#include "CPlayerPedSA.h"
#include "CCivilianPedSA.h"
#include "CObjectSA.h"
#include "CBuildingSA.h"
#include "CRestartSA.h"
#include "CWaterManagerSA.h"
#include "CPedDamageResponseSA.h"
#include "CPedDamageResponseCalculatorSA.h"
#include "CPointLightsSA.h"
#include "CWeaponStatSA.h"
#include "CWeaponStatManagerSA.h"
#include "CShadowDataSA.h"
#include "CBuoyancySA.h"

// Multiplayer includes
#include "multiplayersa_init.h"
#include "multiplayer_keysync.h"
#include "multiplayer_hooksystem.h"
#include "multiplayer_shotsync.h"
#include "CMultiplayerSA.h"
#include "COffsetsMP.h"
//#include "CPopulationSAMP.h"
#include "CRemoteDataSA.h"

extern CMultiplayerSA* pMultiplayer;
extern CMultiplayer* gpMultiplayer;
extern CGameSA* pGame;

#define TIMING_CHECKPOINT(a) {}
#define IS_TIMING_CHECKPOINTS() false
#define TIMING_DETAIL(a) {}