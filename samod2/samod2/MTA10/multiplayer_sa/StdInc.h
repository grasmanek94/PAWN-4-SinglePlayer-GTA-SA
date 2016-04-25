#pragma message("Compiling precompiled header.\n")

// Pragmas
#pragma warning (disable:4250)

#include <windows.h>
#include <stdio.h>

#include <algorithm>
#include <list>
#include <map>
#include <string>
#include <vector>

// SDK includes
#define MTA_CLIENT
#define SHARED_UTIL_WITH_HASH_MAP
#define SHARED_UTIL_WITH_FAST_HASH_MAP
#include "SharedUtil.h"
#include <game/CGame.h>
#include <CMatrix_Pad.h>

// Multiplayer includes
#include "multiplayersa_init.h"
#include "multiplayer_keysync.h"
#include "multiplayer_hooksystem.h"
#include "multiplayer_shotsync.h"
#include "CMultiplayerSA.h"
#include "COffsets.h"
#include "CPopulationSA.h"
#include "CRemoteDataSA.h"

extern CMultiplayerSA* pMultiplayer;

namespace internal__samod
{
	void DrawRadarGangOverlay();
	bool OnBreakTowLink(CVehicle * vehicle);
	void VehicleCollisionHandler(CVehicleSAInterface* pCollidingVehicle, CEntitySAInterface* pCollidedVehicle, int iModelIndex, float fDamageImpulseMag, float fCollidingDamageImpulseMag, uint16 usPieceType, CVector &vecCollisionPos, CVector &vecCollisionVelocity);
	void OnFireWeapon(CPed* ped, CVector* origin);
	void OnPostFireWeapon(CPed* ped);
	void BulletImpactHandler(CPed* pInitiator, CEntity* pVictim, const CVector* pvecStartPosition, const CVector* pvecEndPosition);
}
