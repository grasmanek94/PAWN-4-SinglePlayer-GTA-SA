/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        multiplayer_sa/CMultiplayerSA_RwResources.cpp
*  PORPOISE:    Record usage of certain Renderware resources
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#include "StdInc.h"

namespace
{
    SRwResourceStats ms_Stats;
}


//////////////////////////////////////////////////////////////////////////////////////////
//
// RwTextureCreate
//
//////////////////////////////////////////////////////////////////////////////////////////
void OnMY_RwTextureCreate_Pre( DWORD calledFrom )
{
}

void OnMY_RwTextureCreate_Post( RwTexture* pTexture, DWORD calledFrom )
{
    ms_Stats.uiTextures++;
}

// Hook info
#define HOOKPOS_RwTextureCreate_US                         0x7F37C0
#define HOOKPOS_RwTextureCreate_EU                         0x7F3800
#define HOOKSIZE_RwTextureCreate_US                        5
#define HOOKSIZE_RwTextureCreate_EU                        5
DWORD RETURN_RwTextureCreate_US =                          0x7F37C5;
DWORD RETURN_RwTextureCreate_EU =                          0x7F3805;
DWORD RETURN_RwTextureCreate_BOTH =                        0;
void _declspec(naked) HOOK_RwTextureCreate()
{
    _asm
    {
        pushad
        push    [esp+32+4*0]
        call    OnMY_RwTextureCreate_Pre
        add     esp, 4*1
        popad

        push    [esp+4*1]
        call    inner
        add     esp, 4*1

        pushad
        push    [esp+32+4*0]
        push    eax
        call    OnMY_RwTextureCreate_Post
        add     esp, 4*1+4
        popad

        retn

inner:
        mov     eax,dword ptr ds:[00C97B24h]
        jmp     RETURN_RwTextureCreate_BOTH
    }
}


//////////////////////////////////////////////////////////////////////////////////////////
//
// RwTextureDestroy
//
//////////////////////////////////////////////////////////////////////////////////////////
void OnMY_RwTextureDestroy( RwTexture* pTexture, DWORD calledFrom )
{
    if ( pTexture->refs == 1 )
        ms_Stats.uiTextures--;
}

// Hook info
#define HOOKPOS_RwTextureDestroy_US                         0x7F3820
#define HOOKPOS_RwTextureDestroy_EU                         0x7F3860
#define HOOKSIZE_RwTextureDestroy_EU                        5
#define HOOKSIZE_RwTextureDestroy_US                        5
DWORD RETURN_RwTextureDestroy_US =                          0x7F3825;
DWORD RETURN_RwTextureDestroy_EU =                          0x7F3865;
DWORD RETURN_RwTextureDestroy_BOTH =                        0;
void _declspec(naked) HOOK_RwTextureDestroy ()
{
    _asm
    {
        push    esi
        mov         esi,dword ptr [esp+8]
        pushad
        push    [esp+32+4*1]
        push    esi
        call    OnMY_RwTextureDestroy
        add     esp, 4*1+4
        popad

        jmp     RETURN_RwTextureDestroy_BOTH
    }
}


//////////////////////////////////////////////////////////////////////////////////////////
//
// RwRasterCreate
//
//////////////////////////////////////////////////////////////////////////////////////////
void OnMY_RwRasterCreate( DWORD calledFrom )
{
    ms_Stats.uiRasters++;
}

// Hook info
#define HOOKPOS_RwRasterCreate_US                         0x7FB230
#define HOOKPOS_RwRasterCreate_EU                         0x7FB270
#define HOOKSIZE_RwRasterCreate_US                        5
#define HOOKSIZE_RwRasterCreate_EU                        5
DWORD RETURN_RwRasterCreate_US =                          0x7FB235;
DWORD RETURN_RwRasterCreate_EU =                          0x7FB275;
DWORD RETURN_RwRasterCreate_BOTH =                        0;
void _declspec(naked) HOOK_RwRasterCreate ()
{
    _asm
    {
        mov         eax,dword ptr ds:[00C97B24h]

        pushad
        push    [esp+32+4*0]
        call    OnMY_RwRasterCreate
        add     esp, 4*1
        popad

        jmp     RETURN_RwRasterCreate_BOTH
    }
}


//////////////////////////////////////////////////////////////////////////////////////////
//
// RwRasterDestroy
//
//////////////////////////////////////////////////////////////////////////////////////////
void OnMY_RwRasterDestroy( DWORD calledFrom )
{
    ms_Stats.uiRasters--;
}

// Hook info
#define HOOKPOS_RwRasterDestroy_US                         0x7FB020
#define HOOKPOS_RwRasterDestroy_EU                         0x7FB060
#define HOOKSIZE_RwRasterDestroy_US                        5
#define HOOKSIZE_RwRasterDestroy_EU                        5
DWORD RETURN_RwRasterDestroy_US =                          0x7FB025;
DWORD RETURN_RwRasterDestroy_EU =                          0x7FB065;
DWORD RETURN_RwRasterDestroy_BOTH =                        0;
void _declspec(naked) HOOK_RwRasterDestroy ()
{
    _asm
    {
        push    esi
        mov         esi,dword ptr [esp+8]
        pushad
        push    [esp+32+4*1]
        call    OnMY_RwRasterDestroy
        add     esp, 4*1
        popad

        jmp     RETURN_RwRasterDestroy_BOTH
    }
}


//////////////////////////////////////////////////////////////////////////////////////////
//
// RwGeometryCreate
//
//////////////////////////////////////////////////////////////////////////////////////////
void OnMY_RwGeometryCreate_Pre( DWORD calledFrom )
{
}

void OnMY_RwGeometryCreate_Post( RwGeometry* pGeometry, DWORD calledFrom )
{
    ms_Stats.uiGeometries++;
}

// Hook info
#define HOOKPOS_RwGeometryCreate_US                         0x74CA90
#define HOOKPOS_RwGeometryCreate_EU                         0x74CAE0
#define HOOKSIZE_RwGeometryCreate_US                        7
#define HOOKSIZE_RwGeometryCreate_EU                        7
DWORD RETURN_RwGeometryCreate_US =                          0x74CA97;
DWORD RETURN_RwGeometryCreate_EU =                          0x74CAE7;
DWORD RETURN_RwGeometryCreate_BOTH =                        0;
void _declspec(naked) HOOK_RwGeometryCreate()
{
    _asm
    {
        pushad
        push    [esp+32+4*0]
        call    OnMY_RwGeometryCreate_Pre
        add     esp, 4*1
        popad

        push    [esp+4*3]
        push    [esp+4*3]
        push    [esp+4*3]
        call    inner
        add     esp, 4*3

        pushad
        push    [esp+32+4*0]
        push    eax
        call    OnMY_RwGeometryCreate_Post
        add     esp, 4*1+4
        popad

        retn

inner:
        mov     eax, [esp+04h]
        sub     esp, 8
        jmp     RETURN_RwGeometryCreate_BOTH
    }
}


//////////////////////////////////////////////////////////////////////////////////////////
//
// RwGeometryDestroy
//
//////////////////////////////////////////////////////////////////////////////////////////
void OnMY_RwGeometryDestroy( DWORD calledFrom, RwGeometry* pGeometry )
{
    if ( pGeometry->refs == 1 )
        ms_Stats.uiGeometries--;
}

// Hook info
#define HOOKPOS_RwGeometryDestroy_US                         0x74CCC0
#define HOOKPOS_RwGeometryDestroy_EU                         0x74CD10
#define HOOKSIZE_RwGeometryDestroy_US                        5
#define HOOKSIZE_RwGeometryDestroy_EU                        5
DWORD RETURN_RwGeometryDestroy_US =                          0x74CCC5;
DWORD RETURN_RwGeometryDestroy_EU =                          0x74CD15;
DWORD RETURN_RwGeometryDestroy_BOTH =                        0;
void _declspec(naked) HOOK_RwGeometryDestroy ()
{
    _asm
    {
        pushad
        push    [esp+32+4*1]
        push    [esp+32+4*1]
        call    OnMY_RwGeometryDestroy
        add     esp, 4*2
        popad

        push    esi
        mov         esi,dword ptr [esp+8]
        jmp     RETURN_RwGeometryDestroy_BOTH
    }
}


//////////////////////////////////////////////////////////////////////////////////////////
//
// CMultiplayerSA::GetRwResourceStats
//
//
//////////////////////////////////////////////////////////////////////////////////////////
void CMultiplayerSA::GetRwResourceStats ( SRwResourceStats& outStats )
{
    outStats = ms_Stats;
}


//////////////////////////////////////////////////////////////////////////////////////////
//
// CMultiplayerSA::InitHooks_RwResources
//
// Setup hooks for RwResources
//
//////////////////////////////////////////////////////////////////////////////////////////
void CMultiplayerSA::InitHooks_RwResources ( void )
{
    memset ( &ms_Stats, 0, sizeof ( ms_Stats ) );

    EZHookInstall ( RwTextureCreate );
    EZHookInstall ( RwTextureDestroy );
    EZHookInstall ( RwRasterCreate );
    EZHookInstall ( RwRasterDestroy );
    EZHookInstall ( RwGeometryCreate );
    EZHookInstall ( RwGeometryDestroy );
}
