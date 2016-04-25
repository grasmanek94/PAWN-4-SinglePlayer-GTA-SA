/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        multiplayer_sa/multiplayersa_init.cpp
*  PURPOSE:     Multiplayer module entry
*  DEVELOPERS:  Ed Lyons <eai@opencoding.net>
*               Christian Myhre Lundheim <>
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#include "StdInc.h"
#define DECLARE_PROFILER_SECTION_multiplayersa_init
#include "profiler/SharedUtil.Profiler.h"

CGame* pGameInterface = 0;
//has nothing actually to do with multiplayer, just HL HACKS
CMultiplayerSA* pMultiplayer = 0;

//-----------------------------------------------------------
// This function uses the initialized data sections of the executables
// to differentiate between versions.  MUST be called at least once
// in order for proper initialization to occur.

CMultiplayer* InitMultiplayerInterface()
{   
    // set the internal pointer to the game class
    pGameInterface = pGame;

    assert ( pGameInterface );

    // create an instance of our multiplayer class
    pMultiplayer = new CMultiplayerSA;
    pMultiplayer->InitHooks();

    // return the multiplayer class ptr
    return (CMultiplayer*) pMultiplayer;
}

//-----------------------------------------------------------