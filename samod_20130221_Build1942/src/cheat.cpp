/*

	PROJECT:		 samod 2
	LICENSE:		See LICENSE in the top level directory
	COPYRIGHT:		Copyright Gamer_Z

	 samod 2 is available from http://gpb.googlecode.com/

	 samod 2 is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	 samod 2 is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with  samod 2.  If not, see <http://www.gnu.org/licenses/>.

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
struct pool					*pool_object;

static struct cheat_state	__cheat_state;
struct cheat_state			*cheat_state = NULL;

extern CVehicle				*pVehicle;
extern CEntitySAInterface			*pVehicleSA;

int			m_InitStages = 0;
/*int OldCheatState = NULL;
int NewCheatState = NULL;*/

/*
void MemSet ( void* dwDest, int cValue, unsigned int uiAmount );
void MemCpy ( void* dwDest, const void* dwSrc, unsigned int  uiAmount );

void MemSet ( void* dwDest, int cValue, unsigned int uiAmount )
{
    memset ( dwDest, cValue, uiAmount );
}

void MemCpy ( void* dwDest, const void* dwSrc, unsigned int uiAmount )
{
    memcpy ( dwDest, dwSrc, uiAmount );
}

template < class T, class U >
void MemPut ( U ptr, const T value )
{
    if ( *(T*)ptr != value )
        MemCpy ( (void*)ptr, &value, sizeof ( T ) );
}
//
// Use Mem*Fast for the remaining memory regions
//

inline
void MemCpyFast ( void* dwDest, const void* dwSrc, unsigned int  uiAmount )
{
    memcpy ( dwDest, dwSrc, uiAmount );
}

inline
void MemSetFast ( void* dwDest, int cValue, unsigned int  uiAmount )
{
    memset ( dwDest, cValue, uiAmount );
}

template < class T, class U >
void MemPutFast ( U ptr, const T value )
{
    *(T*)ptr = value;
}

template < class T, class U >
void MemSubFast ( U ptr, const T value )
{
    *(T*)ptr -= value;
}
*/

#include <patch.h>

void cheat_hook ( HWND wnd )
{
	traceLastFunc( "cheat_hook()" );

	// get the time
	static uint64_t time_last;
	__time_current = __time_get();
	g_timeDiff = TIME_TO_DOUBLE(time_get_c() - time_last);

	keyhook_maybe_install( wnd );

	pool_actor = *(struct pool **)ACTOR_POOL_POINTER;
	if ( pool_actor == NULL || pool_actor->start == NULL || pool_actor->size <= 0 )
		return;

	pool_vehicle = *(struct pool **)VEHICLE_POOL_POINTER;
	if ( pool_vehicle == NULL || pool_vehicle->start == NULL || pool_vehicle->size <= 0 )
		return;

	pool_object = *(struct pool **)OBJECT_POOL_POINTER;
	if ( pool_object == NULL || pool_object->start == NULL || pool_object->size <= 0 )
		return;

	if(m_InitStages != 3)
	{
		struct actor_info	*actor_info;
		actor_info = actor_info_get( ACTOR_SELF, ACTOR_ALIVE );

		if(actor_info != NULL)
		{
			if ( m_InitStages == 0 )
			{
				pGameInterface = new CGameSA();
				pPools = pGameInterface->GetPools();
				pGameInterface->GetPools()->AddPed( (DWORD *)actor_info );
				cheat_hookers_installhooks();

				m_InitStages++;
			}
			else if ( m_InitStages == 1 )
			{
				loadAllWeaponModels();
				m_InitStages++;
			}
			else if( m_InitStages == 2)
			{
				if(m_game_reloaded == true)
				{
					Init3();
					m_InitStages++;
					m_game_reloaded = false;
				}
			}
		}
	}else
	if(!gta_menu_active())
	{
		if(m_game_reloaded == true)
		{
			reloadScripts();
			m_game_reloaded = false;
		}
		pPedSelf = pPools->GetPedFromRef( CPOOLS_PED_SELF_REF );
		if(pPedSelf != NULL)
		{
			pPedSelfSA = pPedSelf->GetPedInterface();
			pVehicle = pPedSelf->GetVehicle();
			if(pVehicle != NULL)
			{
				pVehicleSA = pVehicle->GetInterface();
			}
		}
		FrameTick((float)g_timeDiff);
		keyhook_run();
	}
	else
	{
		keyhook_clear_states();
	}
	time_last = time_get_c();
	traceLastFunc( "not_samod()" );
}
