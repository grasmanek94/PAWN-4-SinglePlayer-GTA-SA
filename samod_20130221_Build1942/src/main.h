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

#pragma once

#ifndef __MODMAIN_H
#define __MODMAIN_H

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS 1


#define M0D_NAME		"samod"
#define M0D_VERSION		VERSION

#define BP _asm int 3
#define NAME ""M0D_NAME """ "M0D_VERSION ""

#define COMPILE_DT ""__DATE__" "__TIME__""
#define COMPILE_VERSION _MSC_VER

#define INI_FILE				"samod.ini"

// let's do a precompiled header, why not
#pragma message( "Compiling precompiled header.\n" )

// illegal instruction size
#pragma warning( disable : 4409 )

// 'class1' : inherits 'class2::member' via dominance
#pragma warning( disable : 4250 )

// unreferenced formal parameter
#pragma warning( disable : 4100 )

// handler not registered as safe handler
#pragma warning( disable : 4733 )

// API/SDK includes
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <shellapi.h>
#include <d3dx9.h>
#include <Gdiplus.h>
#include <assert.h>
#include <algorithm>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

#include <intrin.h>
#pragma intrinsic(_ReturnAddress)

// typedefs/classes for legacy
#include "stddefs.h"
#include "stdtypes.h"
#include "CVector.h"
#include "CVector2D.h"
#include "CMatrix.h"
#include "CMatrix_Pad.h"

// public SA classes
// try to always include these in the private class's header
#include "SharedUtil.h"
#include <game/CGame.h>
#include <game/CWanted.h>

//#include <ijsify.h>
#include "SString.h"

// private SA classes
#include "CEntitySA.h"
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
#include "CAudioSA.h"
#include "CPlayerInfoSA.h"
#include "CPopulationSA.h"
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
#include "CEventGroupSA.h"
#include "CAnimManagerSA.h"
#include "CStreamingSA.h"
#include "CVisibilityPluginsSA.h"
#include "CKeyGenSA.h"
#include "CRopesSA.h"
#include "CFxSA.h"
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

// selected MTA class includes
#include "CDirect3DData.h"

// WDL includes
#include "mutex.h"

// normal includes
#include "cheat.h"


#include "d3drender.h"
#include "proxyIDirect3D9.h"
#include "proxyIDirect3DDevice9.h"

// main.cpp functions
void							traceLastFunc ( const char *szFunc );
void							Log ( const char *fmt, ... );
void							setDebugPointer ( void *ptr );
void							toggleWindowedMode ( void );

// externals
extern HMODULE					g_hDllModule;
extern char						g_szWorkingDirectory[MAX_PATH];
extern FILE						*g_flLog;
extern FILE						*g_flLogAll;
extern char						g_szLastFunc[256];
extern CSettingsSAInterface		*g_pCSettingsSAInterface;
extern D3DPRESENT_PARAMETERS	*g_pGTAPresent;
//extern RsGlobalType				*g_RsGlobal;
extern CCameraSA				g_CCamera;

// new MTA externals
extern CGameSA					*pGameInterface;
extern CPools					*pPools;
extern CPed						*pPedSelf;
extern CPedSAInterface			*pPedSelfSA;
extern CDirect3DData			*pD3DData;

// store info about Windows OS
extern t_WindowsInfo			WindowsInfo;

// externals
#include "cheat_actor.h"
#include "cheat_vehicle.h"
#include "cheat_generic.h"
#include "GTAfuncs.h"
#include "math_stuff.h"
#include "cheat_funcs.h"
#include "keyhook.h"
#include "cheat_hookers.h"

void FrameTick(float TimeDiff);
void Init3();
/*
struct CD3DInvalidator 
{
    template<class T>
    void operator()(T* ptr) const {
        //ptr->Invalidate();//caused crash? wtf?
        //delete ptr;//caused crash??...
    }
};*/
extern bool m_game_reloaded;
void reloadScripts();

//typedef std::unique_ptr<CD3DFont, CD3DInvalidator> font_ptr;
extern std::vector<CD3DFont> fonts;
#endif
