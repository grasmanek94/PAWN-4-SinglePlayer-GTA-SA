/*

	PROJECT:		mod_sa
	LICENSE:		See LICENSE in the top level directory
	COPYRIGHT:		Copyright we_sux

	mod_sa is available from http://code.google.com/p/m0d-s0beit-sa/

	mod_sa is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	mod_sa is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with mod_sa.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "main.h"
#include <psapi.h>

/* Note that this is 100% C code, not C++. :p */
#ifndef MIN
#define MIN( a, b ) ( (a) > (b) ? (b) : (a) )
#endif

// old camera thing, still used in RenderMapDot()
float						*cam_matrix = (float *)0xB6F99C;

uint64_t					__time_current;
double						g_timeDiff;

struct pool					*pool_actor;
struct pool					*pool_vehicle;

static struct cheat_state	__cheat_state;
struct cheat_state			*cheat_state = NULL;

extern CVehicle				*pVehicle;
extern CEntitySAInterface			*pVehicleSA;

int			m_InitStages = 0;
int OldCheatState = NULL;
int NewCheatState = NULL;
void cheat_hook ( HWND wnd )
{
	traceLastFunc( "cheat_hook()" );

	// get the time
	static uint64_t time_last;
	__time_current = __time_get();
	g_timeDiff = TIME_TO_DOUBLE(time_get_c() - time_last);

	// for looping
	//int i;

	// install keyhook
	//traceLastFunc( "cheat_hook() install keyhook" );
	keyhook_maybe_install( wnd );

	//traceLastFunc( "cheat_hook() cheat state" );

	// set cheat state to "off"
	//if ( NewCheatState == NULL )
	//{
		// set default cheat_state variables
		//traceLastFunc( "cheat_hook() cheat state 1" );
		//cheat_state = &__cheat_state;
		//traceLastFunc( "cheat_hook() cheat state 2" );
		//cheat_state->state = CHEAT_STATE_NONE;
	NewCheatState = CHEAT_STATE_NONE;
	//}
	//traceLastFunc( "cheat_hook() pool_actor" );
	// setup & refresh actor pool 
	pool_actor = *(struct pool **)ACTOR_POOL_POINTER;
	if ( pool_actor == NULL || pool_actor->start == NULL || pool_actor->size <= 0 )
		return;

	//traceLastFunc( "cheat_hook() pool_vehicle" );
	// setup & refresh vehicle pool
	pool_vehicle = *(struct pool **)VEHICLE_POOL_POINTER;
	if ( pool_vehicle == NULL || pool_vehicle->start == NULL || pool_vehicle->size <= 0 )
		return;

	//////////////////////////////////////////
	// looks like we have pools so continue //
	//traceLastFunc( "cheat_hook() structs" );
	struct actor_info	*actor_info;
	struct vehicle_info *vehicle_info;

	//traceLastFunc( "cheat_hook() actor_info" );
	actor_info = actor_info_get( ACTOR_SELF, ACTOR_ALIVE );
	//traceLastFunc( "cheat_hook() vehicle_info" );
	vehicle_info = vehicle_info_get( VEHICLE_SELF, 0 );

	// no vehicle, and no actor. exit. 
	if ( vehicle_info == NULL && actor_info == NULL )
	{
		//traceLastFunc( "cheat_hook() CHEAT_STATE_NONE" );
		NewCheatState = CHEAT_STATE_NONE;
	}
	else
	{
		if ( vehicle_info == NULL )
		{
			//traceLastFunc( "cheat_hook() CHEAT_STATE_ACTOR" );
			NewCheatState = CHEAT_STATE_ACTOR;
			pVehicle = NULL;
			pVehicleSA = NULL;
			//traceLastFunc( "cheat_hook() CHEAT_STATE_ACTOR 2" );
		}
		else
		{
			//traceLastFunc( "cheat_hook() CHEAT_STATE_VEHICLE" );
			NewCheatState = CHEAT_STATE_VEHICLE;
			pVehicle = pPools->GetVehicle( (DWORD *)vehicle_info );
			pVehicleSA = pVehicle->GetInterface();
		}
		// post GTA initialization initizalization routines
		//traceLastFunc( "cheat_hook() CHEAT_STATE_ACTOR 3" );
		if ( m_InitStages == 0 )
		{
			//traceLastFunc( "cheat_hook() m_InitStages: 0" );
			// setup CGame & friends
			pGameInterface = new CGameSA();
			//traceLastFunc( "cheat_hook() m_InitStages: 01" );
			pPools = pGameInterface->GetPools();
			//traceLastFunc( "cheat_hook() m_InitStages: 02" );
			//pGameInterface->Initialize(); // all this does so far is disable modshops and pay&sprays
			// this seems to fix SAMP's load hang bug
			//pGameInterface->StartGame();
			
			// we have to add ourself to the pool first so that we are always the 1st ref
			// NEW --> use the global external: pPedSelf
			pPedSelf = pGameInterface->GetPools()->AddPed( (DWORD *)actor_info );
			//traceLastFunc( "cheat_hook() m_InitStages: 03" );
			pPedSelfSA = pPedSelf->GetPedInterface();
			//traceLastFunc( "cheat_hook() m_InitStages: 04" );
			cheat_hookers_installhooks();
			// increment stage
			m_InitStages++;
		}
		else if ( m_InitStages == 1 )
		{
			// load all the weapon models44
			//traceLastFunc( "cheat_hook() m_InitStages: 15" );
			loadAllWeaponModels();

			// get funky
			//pGameInterface->GetAudio()->PlayBeatTrack(2);

			// increment stage
			m_InitStages++;
		}
		else if( m_InitStages == 2)
		{
			//if(gta_menu_active())return;
			Init3();
			m_InitStages++;
		}
	}
	if ( NewCheatState != CHEAT_STATE_NONE )
	{
		// keep this updated, cos something is switching it now
		pPedSelf = pPools->GetPedFromRef( CPOOLS_PED_SELF_REF );
		pPedSelfSA = pPedSelf->GetPedInterface();
	}
	if(m_InitStages == 3)
	{
		float fps = pGameInterface->GetFPS();
		if(0.0f < fps && fps < 32000.0f)
		{
			FrameTick(OldCheatState,NewCheatState,g_timeDiff,OldCheatState!=NewCheatState);
		}
	}
	traceLastFunc( "cheat_hook()" );
	OldCheatState = NewCheatState;
	//traceLastFunc( "cheat_hook() gta_menu_active" );
	if ( gta_menu_active() )
		keyhook_clear_states();
	else
		keyhook_run();
	//traceLastFunc( "cheat_hook() time_last" );
	time_last = time_get_c();
	//traceLastFunc( "cheat_hook() done" );
}
