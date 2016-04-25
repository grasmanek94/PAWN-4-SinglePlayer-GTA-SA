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

// externals
unsigned long	ulFullScreenRefreshRate;
extern D3DC9	orig_Direct3DCreate9 = NULL;;

// externals
#pragma data_seg( ".d3d9_shared" )
proxyIDirect3DDevice9	*pDirect3DDevice9 = NULL;
HRESULT_VOID			orig_Direct3DShaderValidatorCreate9 = NULL;
IDirect3DDevice9		*origIDirect3DDevice9;
IDirect3DDevice9		*pRwD3DDevice = (IDirect3DDevice9 *) * (DWORD *)__RwD3DDevice;
D3DPRESENT_PARAMETERS	pPresentParam;
D3DMATRIX				m_mViewMatrix, m_mProjMatrix, m_mWorldMatrix;
CDirect3DData			*pD3DData =	new CDirect3DData();
#pragma data_seg()
bool					bD3DRenderInit;
bool					bD3DWindowModeSet;
bool					g_isRequesting_RwD3D9ChangeVideoMode;


// create a render object
CD3DRender				*render = new CD3DRender( 128 );

///////////////////////////////////////////////////////////////////////////////
// Common D3D functions.
///////////////////////////////////////////////////////////////////////////////

static WCHAR *ToWChar ( char *str )
{
	static WCHAR	buffer[1024];
	_wcsset( buffer, 0 );
	MultiByteToWideChar( CP_ACP, 0, str, strlen(str), buffer, 1024 );
	return buffer;
}

// by s0beit, GHOSTER, Azorbix
HRESULT GenerateShader ( IDirect3DDevice9 *Device, IDirect3DPixelShader9 **pShader, float alpha, float red, float green,
						 float blue )
{
	traceLastFunc( "GenerateShader()" );

	char		szShader[256];
	ID3DXBuffer *pShaderBuffer = NULL;
	sprintf_s( szShader, sizeof(szShader), "ps.1.1\ndef c0, %f, %f, %f, %f\nmov r0,c0", red, green, blue, alpha );
	if ( FAILED(D3DXAssembleShader(szShader, sizeof(szShader), NULL, NULL, 0, &pShaderBuffer, NULL)) )
	{
		// Log( "Shader fail." ); - yeah it does sometimes
		return E_FAIL;
	}

	if ( FAILED(Device->CreatePixelShader((const DWORD *)pShaderBuffer->GetBufferPointer(), pShader)) )
	{
		return E_FAIL;
	}

	return S_OK;
}

// Function taken from the MTA:SA source code (MTA10/core/CGraphics.cpp)
void CalcScreenCoors ( D3DXVECTOR3 *vecWorld, D3DXVECTOR3 *vecScreen )
{
	/** C++-ifyed function 0x71DA00, formerly called by CHudSA::CalcScreenCoors **/
	// Get the static view matrix as D3DXMATRIX
	D3DXMATRIX	m ( (float *)(0xB6FA2C) );

	// Get the static virtual screen (x,y)-sizes
	DWORD		*dwLenX = ( DWORD * ) ( 0xC17044 );
	DWORD		*dwLenY = ( DWORD * ) ( 0xC17048 );

	//DWORD *dwLenZ = (DWORD*)(0xC1704C);
	//double aspectRatio = (*dwLenX) / (*dwLenY);
	// Do a transformation
	vecScreen->x = ( vecWorld->z * m._31 ) + ( vecWorld->y * m._21 ) + ( vecWorld->x * m._11 ) + m._41;
	vecScreen->y = ( vecWorld->z * m._32 ) + ( vecWorld->y * m._22 ) + ( vecWorld->x * m._12 ) + m._42;
	vecScreen->z = ( vecWorld->z * m._33 ) + ( vecWorld->y * m._23 ) + ( vecWorld->x * m._13 ) + m._43;

	// Get the correct screen coordinates
	double	fRecip = (double)1.0 / vecScreen->z;	//(vecScreen->z - (*dwLenZ));
	vecScreen->x *= (float)( fRecip * (*dwLenX) );
	vecScreen->y *= (float)( fRecip * (*dwLenY) );
}
void CalcWorldCoors ( D3DXVECTOR3 *vecScreen, D3DXVECTOR3 *vecWorld )
{
	// Get the static view matrix as D3DXMATRIX
	D3DXMATRIX	m ( (float *)(0xB6FA2C) );

	// Invert the view matrix
	D3DXMATRIX minv;
	memset ( &minv, 0, sizeof ( D3DXMATRIX ) );
	m._44 = 1.0f;
	D3DXMatrixInverse ( &minv, NULL, &m );

	DWORD		*dwLenX = ( DWORD * ) ( 0xC17044 );
	DWORD		*dwLenY = ( DWORD * ) ( 0xC17048 );

	// Reverse screen coordinates
	double fRecip = (double)1.0 / vecScreen->z;
	vecScreen->x /= (float)(fRecip * (*dwLenX) );
	vecScreen->y /= (float)(fRecip * (*dwLenY) );

	// Do an (inverse) transformation
	vecWorld->x = ( vecScreen->z * minv._31 ) + ( vecScreen->y * minv._21 ) + ( vecScreen->x * minv._11 ) + minv._41;
	vecWorld->y = ( vecScreen->z * minv._32 ) + ( vecScreen->y * minv._22 ) + ( vecScreen->x * minv._12 ) + minv._42;
	vecWorld->z = ( vecScreen->z * minv._33 ) + ( vecScreen->y * minv._23 ) + ( vecScreen->x * minv._13 ) + minv._43;
}

float		fpsDisplay, fpsBuf1, fpsBuf2, fpsBuf3, fpsBuf4;
uint32_t	fps_time = GetTickCount();
int			fpsFrameCounter;
float getFPS ( void )
{
	// increment frame counter
	fpsFrameCounter++;

	// get fps
	if ( (GetTickCount() - 250) > fps_time )
	{
		// for averaging
		fpsBuf4 = fpsBuf3;
		fpsBuf3 = fpsBuf2;
		fpsBuf2 = fpsBuf1;

		// set new FPS
		fpsBuf1 = ( fpsFrameCounter * 4 );
		fpsDisplay = ( fpsBuf1 + fpsBuf2 + fpsBuf3 + fpsBuf4 ) / 4.0;

		// reset counter
		fpsFrameCounter = 0;

		// reset tick
		fps_time = GetTickCount();
	}

	return fpsDisplay;
}

namespace SA
{
	namespace MOD
	{
		namespace Console
		{
			namespace var
			{
				extern CD3DFont* font;
				extern bool g_Console;
			};
		};
	};
};

void PAWN_DXRENDER_STATE(bool init);
void proxyID3DDevice9_UnInitOurShit ( void )
{
	traceLastFunc( "proxyID3DDevice9_UnInitOurShit()" );

	render->Invalidate();
	for(std::vector<CD3DFont>::size_type i = 0; i != fonts.size(); i++) 
	{
		fonts[i].Invalidate();
	}
	if(SA::MOD::Console::var::g_Console)
		SA::MOD::Console::var::font->Invalidate();

	static int	proxyIDirect3DDevice9_init = 0;
	PAWN_DXRENDER_STATE(false);
	bD3DRenderInit = false;
}


void proxyID3DDevice9_InitOurShit ( D3DPRESENT_PARAMETERS *pPresentationParameters )
{
	traceLastFunc( "proxyID3DDevice9_InitOurShit()" );

	render->Initialize( origIDirect3DDevice9 );
	for(std::vector<CD3DFont>::size_type i = 0; i != fonts.size(); i++) 
	{
		fonts[i].Initialize( origIDirect3DDevice9 );
	}
	if(SA::MOD::Console::var::g_Console)
		SA::MOD::Console::var::font->Initialize( origIDirect3DDevice9 );

	PAWN_DXRENDER_STATE(true);
	bD3DRenderInit = true;
}

// window/fullscreen init/update
#if defined __DEBUGx
bool	g_InitWindowMode_ForceUpdate_Active = false;
bool	pppWantsWindowedMode = false;
bool	pppWantsWindowedMode_titlebar = true;
bool	g_isRequestingWindowModeToggle = false;
RsGlobalType			*g_RsGlobal = (RsGlobalType *)0xC17040;
#endif

void proxyID3DDevice9_InitWindowMode ( D3DPRESENT_PARAMETERS *pPresentationParameters )
{
	traceLastFunc( "proxyID3DDevice9_InitWindowMode()" );
	#if defined __DEBUGx
	// window mode toggle, flips pppWantsWindowedMode bit
	if(!pppWantsWindowedMode)
	{
		if ( g_isRequestingWindowModeToggle )
		{
			g_isRequestingWindowModeToggle = false;
			pppWantsWindowedMode = true;
		}

		// m_ForceChange settings
		bool	m_ForceUpdate;

		// set to force the desired video mode if needed
		if ( pppWantsWindowedMode != (bool)pPresentationParameters->Windowed || pppWantsWindowedMode != (bool)g_pGTAPresent->Windowed )
		{
			// we need to force an update
			m_ForceUpdate = true;
		}

		// basic presentation parameters we want
		g_pGTAPresent->Flags = 0;
		g_pGTAPresent->PresentationInterval = 0;
		pPresentationParameters->Flags = 0;
		pPresentationParameters->PresentationInterval = 0;

		// get the current video mode & the correct back buffer size
		int			m_dwVideoMode = g_pCSettingsSAInterface->dwVideoMode;
		VideoMode	m_currentVideoMode;
		GTAfunc_getVideoModeInfo( &m_currentVideoMode, m_dwVideoMode );

		int presentWidth = m_currentVideoMode.width;
		int presentHeight = m_currentVideoMode.height;

		// set windowed or fullscreen
		if ( g_isRequesting_RwD3D9ChangeVideoMode && m_ForceUpdate && !g_InitWindowMode_ForceUpdate_Active )
		{
			g_isRequesting_RwD3D9ChangeVideoMode = false;
			g_InitWindowMode_ForceUpdate_Active = true;
			if ( pppWantsWindowedMode )
			{
				// set to windowed
				if ( GTAfunc_RwD3D9ChangeVideoMode(0) != 1 )
				{
					Log( "proxyID3DDevice9_InitWindowMode() failed to GTAfunc_RwD3D9ChangeVideoMode(0) #1" );
					g_InitWindowMode_ForceUpdate_Active = false;
					return;
				}
				else
				{
					*(int *)0xC920CC = 1;	// _?windowed
					g_RsGlobal->ps->fullscreen = 0;
					if ( isBadPtr_writeAny(g_pGTAPresent, sizeof(D3DPRESENT_PARAMETERS)) )
					{
						Log( "proxyID3DDevice9_InitWindowMode() g_pGTAPresent pointer bad. Continuing anyways." );
					}
					else
					{
						g_pGTAPresent->Windowed = 1;
						g_pGTAPresent->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
						g_pGTAPresent->BackBufferWidth = presentWidth;
						g_pGTAPresent->BackBufferHeight = presentHeight;
					}

					if ( isBadPtr_writeAny(pPresentationParameters, sizeof(D3DPRESENT_PARAMETERS)) )
					{
						Log( "proxyID3DDevice9_InitWindowMode() pPresentationParameters pointer bad. Continuing anyways." );
					}
					else
					{
						pPresentationParameters->Windowed = 1;
						pPresentationParameters->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
						pPresentationParameters->BackBufferWidth = presentWidth;
						pPresentationParameters->BackBufferHeight = presentHeight;
					}

					if ( GTAfunc_RwD3D9ChangeVideoMode(0) != 1 )
					{
						Log( "proxyID3DDevice9_InitWindowMode() failed to GTAfunc_RwD3D9ChangeVideoMode(0) #2" );
						g_InitWindowMode_ForceUpdate_Active = false;
						return;
					}

					//GTAfunc_setCurrentVideoMode(0);
				}
			}
			else if ( !pppWantsWindowedMode )
			{
				// set to fullscreen
				if ( GTAfunc_RwD3D9ChangeVideoMode(0) != 1 )
				{
					Log( "proxyID3DDevice9_InitWindowMode() failed to GTAfunc_RwD3D9ChangeVideoMode(0) #3" );
					g_InitWindowMode_ForceUpdate_Active = false;
					return;
				}
				else
				{
					*(int *)0xC920CC = 0;	// _?windowed
					g_RsGlobal->ps->fullscreen = 1;
					if ( isBadPtr_writeAny(g_pGTAPresent, sizeof(D3DPRESENT_PARAMETERS)) )
					{
						Log( "proxyID3DDevice9_InitWindowMode() g_pGTAPresent pointer bad. Continuing anyways." );
					}
					else
					{
						g_pGTAPresent->Windowed = 0;
						g_pGTAPresent->FullScreen_RefreshRateInHz = ulFullScreenRefreshRate;
						g_pGTAPresent->BackBufferWidth = presentWidth;
						g_pGTAPresent->BackBufferHeight = presentHeight;
					}

					if ( isBadPtr_writeAny(pPresentationParameters, sizeof(D3DPRESENT_PARAMETERS)) )
					{
						Log( "proxyID3DDevice9_InitWindowMode() pPresentationParameters pointer bad. Continuing anyways." );
					}
					else
					{
						pPresentationParameters->Windowed = 0;
						pPresentationParameters->FullScreen_RefreshRateInHz = ulFullScreenRefreshRate;
						pPresentationParameters->BackBufferWidth = presentWidth;
						pPresentationParameters->BackBufferHeight = presentHeight;
					}

					if ( GTAfunc_RwD3D9ChangeVideoMode(m_dwVideoMode) != 1 )
					{
						Log( "proxyID3DDevice9_InitWindowMode() failed to GTAfunc_RwD3D9ChangeVideoMode(m_dwVideoMode) #1, m_dwVideoMode=%d",
						 m_dwVideoMode );
						g_InitWindowMode_ForceUpdate_Active = false;
						return;
					}

					//GTAfunc_setCurrentVideoMode(m_dwVideoMode);
				}
			}

			g_InitWindowMode_ForceUpdate_Active = false;
		}
		else if ( m_ForceUpdate && pppWantsWindowedMode && !g_InitWindowMode_ForceUpdate_Active )
		{
			g_InitWindowMode_ForceUpdate_Active = true;
			*(int *)0xC920CC = 1;			// _?windowed
			g_RsGlobal->ps->fullscreen = 0;
			if ( isBadPtr_writeAny(g_pGTAPresent, sizeof(D3DPRESENT_PARAMETERS)) )
			{
				Log( "proxyID3DDevice9_InitWindowMode() g_pGTAPresent pointer bad. Continuing anyways." );
			}
			else
			{
				g_pGTAPresent->Windowed = 1;
				g_pGTAPresent->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
				g_pGTAPresent->BackBufferWidth = presentWidth;
				g_pGTAPresent->BackBufferHeight = presentHeight;
			}

			if ( isBadPtr_writeAny(pPresentationParameters, sizeof(D3DPRESENT_PARAMETERS)) )
			{
				Log( "proxyID3DDevice9_InitWindowMode() pPresentationParameters pointer bad. Continuing anyways." );
			}
			else
			{
				pPresentationParameters->Windowed = 1;
				pPresentationParameters->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
				pPresentationParameters->BackBufferWidth = presentWidth;
				pPresentationParameters->BackBufferHeight = presentHeight;
			}

			g_InitWindowMode_ForceUpdate_Active = false;
		}

		// window mode stuff
		if ( pppWantsWindowedMode
		 &&	 pPresentationParameters->BackBufferWidth != 0
		 &&	 pPresentationParameters->BackBufferHeight != 0 )
		{
			// title bar or not?
			if ( pppWantsWindowedMode_titlebar )
			{
				RECT	um;		// damn near killed um

				// add caption bar, etc
				SetWindowLong( pPresentationParameters->hDeviceWindow, GWL_STYLE,
							   WS_POPUP | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE );

				// update caption bar, etc
				SetWindowPos( pPresentationParameters->hDeviceWindow, HWND_NOTOPMOST, 0, 0, 0, 0,
							  SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE );

				// the client area of the window must be the same size as the back buffer
				GetClientRect( pPresentationParameters->hDeviceWindow, &um );
				if ( um.right == pPresentationParameters->BackBufferWidth
				 &&	 um.bottom == pPresentationParameters->BackBufferHeight ) goto proxyID3DDevice9_InitWindowMode_end;
				SetWindowPos( pPresentationParameters->hDeviceWindow, HWND_NOTOPMOST, 0, 0,
							  pPresentationParameters->BackBufferWidth + (pPresentationParameters->BackBufferWidth - um.right),
								  pPresentationParameters->BackBufferHeight + (pPresentationParameters->BackBufferHeight - um.bottom),
									  SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOMOVE );
				if ( pool_actor != NULL )
				{
					int x, y;
					x = GetSystemMetrics( SM_CXSCREEN );
					y = GetSystemMetrics( SM_CYSCREEN );
					SetWindowPos( pPresentationParameters->hDeviceWindow, HWND_NOTOPMOST,
								  (x / 2) - (pPresentationParameters->BackBufferWidth / 2),
								  (y / 2) - (pPresentationParameters->BackBufferHeight / 2),
								  pPresentationParameters->BackBufferWidth + (pPresentationParameters->BackBufferWidth - um.right),
							  pPresentationParameters->BackBufferHeight +
									  (pPresentationParameters->BackBufferHeight - um.bottom), SWP_SHOWWINDOW );
				}
			}
			else
			{
				// center the window
				SetWindowPos( pPresentationParameters->hDeviceWindow, HWND_NOTOPMOST, 0, 0, 0, 0,
							  SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE );
			}
		}
	}
proxyID3DDevice9_InitWindowMode_end: ;
	// always make sure our window_mode is synced with the game's
	pppWantsWindowedMode = ( g_RsGlobal->ps->fullscreen == 0 );
	#endif
}

#if defined __DEBUGx
void toggleWindowedMode ( void )
{
	traceLastFunc( "toggleWindowedMode()" );
	g_isRequestingWindowModeToggle = true;
	g_isRequesting_RwD3D9ChangeVideoMode = true;
	proxyID3DDevice9_InitWindowMode( &pPresentParam );
}
#endif

///////////////////////////////////////////////////////////////////////////////
// End of common D3D functions.
///////////////////////////////////////////////////////////////////////////////

proxyIDirect3DDevice9::proxyIDirect3DDevice9 ( IDirect3DDevice9 *pOriginal )
{
	//Log("proxyIDirect3DDevice9 constructor called. Original IDirect3DDevice9 interface address is 0x%p", pOriginal);
	origIDirect3DDevice9 = pOriginal;
	bD3DRenderInit = false;
}

proxyIDirect3DDevice9::~proxyIDirect3DDevice9 ( void )
{
	Log( "proxyIDirect3DDevice9 destructor called." );
}

HRESULT proxyIDirect3DDevice9::QueryInterface ( REFIID riid, void **ppvObj )
{
	HRESULT hRes;

	*ppvObj = NULL;
	hRes = origIDirect3DDevice9->QueryInterface( riid, ppvObj );
	if ( hRes == NOERROR )
		*ppvObj = this;

	return hRes;
}

ULONG proxyIDirect3DDevice9::AddRef ( void )
{
	return origIDirect3DDevice9->AddRef();
}

ULONG proxyIDirect3DDevice9::Release ( void )
{
	ULONG	count = origIDirect3DDevice9->Release();

	if ( count == 0 )
	{
		delete( this );
	}

	return count;
}

HRESULT proxyIDirect3DDevice9::TestCooperativeLevel ( void )
{
	return origIDirect3DDevice9->TestCooperativeLevel();
}

UINT proxyIDirect3DDevice9::GetAvailableTextureMem ( void )
{
	return origIDirect3DDevice9->GetAvailableTextureMem();
}

HRESULT proxyIDirect3DDevice9::EvictManagedResources ( void )
{
	return origIDirect3DDevice9->EvictManagedResources();
}

HRESULT proxyIDirect3DDevice9::GetDirect3D ( IDirect3D9 **ppD3D9 )
{
	return origIDirect3DDevice9->GetDirect3D( ppD3D9 );
}

HRESULT proxyIDirect3DDevice9::GetDeviceCaps ( D3DCAPS9 *pCaps )
{
	return origIDirect3DDevice9->GetDeviceCaps( pCaps );
}

HRESULT proxyIDirect3DDevice9::GetDisplayMode ( UINT iSwapChain, D3DDISPLAYMODE *pMode )
{
	return origIDirect3DDevice9->GetDisplayMode( iSwapChain, pMode );
}

HRESULT proxyIDirect3DDevice9::GetCreationParameters ( D3DDEVICE_CREATION_PARAMETERS *pParameters )
{
	return origIDirect3DDevice9->GetCreationParameters( pParameters );
}

HRESULT proxyIDirect3DDevice9::SetCursorProperties ( UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9 *pCursorBitmap )
{
	return origIDirect3DDevice9->SetCursorProperties( XHotSpot, YHotSpot, pCursorBitmap );
}

void proxyIDirect3DDevice9::SetCursorPosition ( int X, int Y, DWORD Flags )
{
	origIDirect3DDevice9->SetCursorPosition( X, Y, Flags );
}

BOOL proxyIDirect3DDevice9::ShowCursor ( BOOL bShow )
{
	return origIDirect3DDevice9->ShowCursor( bShow );
}

HRESULT proxyIDirect3DDevice9::CreateAdditionalSwapChain ( D3DPRESENT_PARAMETERS *pPresentationParameters,
														   IDirect3DSwapChain9 **pSwapChain )
{
	return origIDirect3DDevice9->CreateAdditionalSwapChain( pPresentationParameters, pSwapChain );
}

HRESULT proxyIDirect3DDevice9::GetSwapChain ( UINT iSwapChain, IDirect3DSwapChain9 **pSwapChain )
{
	return origIDirect3DDevice9->GetSwapChain( iSwapChain, pSwapChain );
}

UINT proxyIDirect3DDevice9::GetNumberOfSwapChains ( void )
{
	return origIDirect3DDevice9->GetNumberOfSwapChains();
}

HRESULT proxyIDirect3DDevice9::Reset ( D3DPRESENT_PARAMETERS *pPresentationParameters )
{
	traceLastFunc( "proxyIDirect3DDevice9::Reset()" );

	// new PROPER reset code
	//
	// return for the reset
	HRESULT hRes_orig_Reset;

	// this should always be done if a reset is requested
	// and we have assets initialized on the proxy device
	while ( bD3DRenderInit )
		proxyID3DDevice9_UnInitOurShit();

	// always attempt to reset orig device if we are asked to
	// since GTA is getting the state of the original device
	hRes_orig_Reset = origIDirect3DDevice9->Reset( pPresentationParameters );

	// handle the return from original Reset()
	if ( hRes_orig_Reset == D3D_OK )
	{
		// variable for checking the pointers
		short	badPointerCount_g_pGTAPresent = 0;
		bool	badPointerBreak_g_pGTAPresent = false;
		short	badPointerCount_pPresentationParameters = 0;
		bool	badPointerBreak_pPresentationParameters = false;

		while ( !bD3DRenderInit )
		{
			proxyID3DDevice9_InitOurShit( pPresentationParameters );
			if ( !bD3DRenderInit )
				Sleep( 100 );
		}

		// init our window mode
		proxyID3DDevice9_InitWindowMode( pPresentationParameters );

		// update the global Present Param struct AFTER original reset, only if it's ok
		pPresentParam = *pPresentationParameters;

		// Update our data.
		pD3DData->StoreViewport( 0, 0, pPresentationParameters->BackBufferWidth,
								 pPresentationParameters->BackBufferHeight );
	}

	// reporting problems is about all we can do here.
	// we would normally just end the application gracefully right
	// now, but GTA should be handling this on its own, so whatever.
	else if ( hRes_orig_Reset == D3DERR_DEVICELOST )
	{
		Log( "origIDirect3DDevice9::Reset() returned D3DERR_DEVICELOST" );
	}
	else if ( hRes_orig_Reset == D3DERR_DRIVERINTERNALERROR )
	{
		Log( "WTF: origIDirect3DDevice9::Reset() returned D3DERR_DRIVERINTERNALERROR" );
	}
	else if ( hRes_orig_Reset == D3DERR_OUTOFVIDEOMEMORY )
	{
		Log( "WTF: origIDirect3DDevice9::Reset() returned D3DERR_OUTOFVIDEOMEMORY" );
	}
	else
	{
		Log( "WTF: origIDirect3DDevice9::Reset() returned value %d", hRes_orig_Reset );
	}

	// bai
	return hRes_orig_Reset;
}

HRESULT proxyIDirect3DDevice9::Present ( CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride,
										 CONST RGNDATA *pDirtyRegion )
{
	traceLastFunc( "proxyIDirect3DDevice9::Present()" );

	////////////////////////////////////////////////
	traceLastFunc( "renderHandler()" );
	// set FPS limit for vsync mode
	//memcpy_safe( (void *)0xC1704C, &set.fps_limit, sizeof(int) );

	if ( !bD3DRenderInit )
	{
		proxyID3DDevice9_InitOurShit( &pPresentParam );
		#if defined __DEBUGx
		toggleWindowedMode ( );
		#endif
	}
	traceLastFunc( "renderHandler() 2" );
	bool isBeginRenderWIN = SUCCEEDED( render->BeginRender() );
	if ( isBeginRenderWIN )
	{
		cheat_hook ( pPresentParam.hDeviceWindow );
		render->EndRender();
	}
	traceLastFunc( "renderHandler() 3" );
	////////////////////////////////////////////////

	return origIDirect3DDevice9->Present( pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion );
}

HRESULT proxyIDirect3DDevice9::GetBackBuffer ( UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type,
											   IDirect3DSurface9 **ppBackBuffer )
{
	return origIDirect3DDevice9->GetBackBuffer( iSwapChain, iBackBuffer, Type, ppBackBuffer );
}

HRESULT proxyIDirect3DDevice9::GetRasterStatus ( UINT iSwapChain, D3DRASTER_STATUS *pRasterStatus )
{
	return origIDirect3DDevice9->GetRasterStatus( iSwapChain, pRasterStatus );
}

HRESULT proxyIDirect3DDevice9::SetDialogBoxMode ( BOOL bEnableDialogs )
{
	return origIDirect3DDevice9->SetDialogBoxMode( bEnableDialogs );
}

void proxyIDirect3DDevice9::SetGammaRamp ( UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP *pRamp )
{
	origIDirect3DDevice9->SetGammaRamp( iSwapChain, Flags, pRamp );
}

void proxyIDirect3DDevice9::GetGammaRamp ( UINT iSwapChain, D3DGAMMARAMP *pRamp )
{
	origIDirect3DDevice9->GetGammaRamp( iSwapChain, pRamp );
}

HRESULT proxyIDirect3DDevice9::CreateTexture ( UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format,
											   D3DPOOL Pool, IDirect3DTexture9 **ppTexture, HANDLE *pSharedHandle )
{
	return origIDirect3DDevice9->CreateTexture( Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle );
}

HRESULT proxyIDirect3DDevice9::CreateVolumeTexture ( UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage,
													 D3DFORMAT Format, D3DPOOL Pool,
													 IDirect3DVolumeTexture9 **ppVolumeTexture, HANDLE *pSharedHandle )
{
	return origIDirect3DDevice9->CreateVolumeTexture( Width, Height, Depth, Levels, Usage, Format, Pool,
													  ppVolumeTexture, pSharedHandle );
}

HRESULT proxyIDirect3DDevice9::CreateCubeTexture ( UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format,
												   D3DPOOL Pool, IDirect3DCubeTexture9 **ppCubeTexture,
												   HANDLE *pSharedHandle )
{
	return origIDirect3DDevice9->CreateCubeTexture( EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture,
													pSharedHandle );
}

HRESULT proxyIDirect3DDevice9::CreateVertexBuffer ( UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool,
													IDirect3DVertexBuffer9 **ppVertexBuffer, HANDLE *pSharedHandle )
{
	return origIDirect3DDevice9->CreateVertexBuffer( Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle );
}

HRESULT proxyIDirect3DDevice9::CreateIndexBuffer ( UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool,
												   IDirect3DIndexBuffer9 **ppIndexBuffer, HANDLE *pSharedHandle )
{
	return origIDirect3DDevice9->CreateIndexBuffer( Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle );
}

HRESULT proxyIDirect3DDevice9::CreateRenderTarget ( UINT Width, UINT Height, D3DFORMAT Format,
													D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality,
													BOOL Lockable, IDirect3DSurface9 **ppSurface, HANDLE *pSharedHandle )
{
	return origIDirect3DDevice9->CreateRenderTarget( Width, Height, Format, MultiSample, MultisampleQuality, Lockable,
													 ppSurface, pSharedHandle );
}

HRESULT proxyIDirect3DDevice9::CreateDepthStencilSurface ( UINT Width, UINT Height, D3DFORMAT Format,
														   D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality,
														   BOOL Discard, IDirect3DSurface9 **ppSurface,
														   HANDLE *pSharedHandle )
{
	return origIDirect3DDevice9->CreateDepthStencilSurface( Width, Height, Format, MultiSample, MultisampleQuality,
															Discard, ppSurface, pSharedHandle );
}

HRESULT proxyIDirect3DDevice9::UpdateSurface ( IDirect3DSurface9 *pSourceSurface, CONST RECT *pSourceRect,
											   IDirect3DSurface9 *pDestinationSurface, CONST POINT *pDestPoint )
{
	return origIDirect3DDevice9->UpdateSurface( pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint );
}

HRESULT proxyIDirect3DDevice9::UpdateTexture ( IDirect3DBaseTexture9 *pSourceTexture,
											   IDirect3DBaseTexture9 *pDestinationTexture )
{
	return origIDirect3DDevice9->UpdateTexture( pSourceTexture, pDestinationTexture );
}

HRESULT proxyIDirect3DDevice9::GetRenderTargetData ( IDirect3DSurface9 *pRenderTarget, IDirect3DSurface9 *pDestSurface )
{
	return origIDirect3DDevice9->GetRenderTargetData( pRenderTarget, pDestSurface );
}

HRESULT proxyIDirect3DDevice9::GetFrontBufferData ( UINT iSwapChain, IDirect3DSurface9 *pDestSurface )
{
	return origIDirect3DDevice9->GetFrontBufferData( iSwapChain, pDestSurface );
}

HRESULT proxyIDirect3DDevice9::StretchRect ( IDirect3DSurface9 *pSourceSurface, CONST RECT *pSourceRect,
											 IDirect3DSurface9 *pDestSurface, CONST RECT *pDestRect,
											 D3DTEXTUREFILTERTYPE Filter )
{
	return origIDirect3DDevice9->StretchRect( pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter );
}

HRESULT proxyIDirect3DDevice9::ColorFill ( IDirect3DSurface9 *pSurface, CONST RECT *pRect, D3DCOLOR color )
{
	return origIDirect3DDevice9->ColorFill( pSurface, pRect, color );
}

HRESULT proxyIDirect3DDevice9::CreateOffscreenPlainSurface ( UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool,
															 IDirect3DSurface9 **ppSurface, HANDLE *pSharedHandle )
{
	return origIDirect3DDevice9->CreateOffscreenPlainSurface( Width, Height, Format, Pool, ppSurface, pSharedHandle );
}

HRESULT proxyIDirect3DDevice9::SetRenderTarget ( DWORD RenderTargetIndex, IDirect3DSurface9 *pRenderTarget )
{
	return origIDirect3DDevice9->SetRenderTarget( RenderTargetIndex, pRenderTarget );
}

HRESULT proxyIDirect3DDevice9::GetRenderTarget ( DWORD RenderTargetIndex, IDirect3DSurface9 **ppRenderTarget )
{
	return origIDirect3DDevice9->GetRenderTarget( RenderTargetIndex, ppRenderTarget );
}

HRESULT proxyIDirect3DDevice9::SetDepthStencilSurface ( IDirect3DSurface9 *pNewZStencil )
{
	return origIDirect3DDevice9->SetDepthStencilSurface( pNewZStencil );
}

HRESULT proxyIDirect3DDevice9::GetDepthStencilSurface ( IDirect3DSurface9 **ppZStencilSurface )
{
	return origIDirect3DDevice9->GetDepthStencilSurface( ppZStencilSurface );
}

HRESULT proxyIDirect3DDevice9::BeginScene ( void )
{
	traceLastFunc( "proxyIDirect3DDevice9::BeginScene()" );

	// return original function
	HRESULT ret = origIDirect3DDevice9->BeginScene();
	traceLastFunc( "end of proxyIDirect3DDevice9::BeginScene()" );
	return ret;
}

HRESULT proxyIDirect3DDevice9::EndScene ( void )
{
	return origIDirect3DDevice9->EndScene();
}

HRESULT proxyIDirect3DDevice9::Clear ( DWORD Count, CONST D3DRECT *pRects, DWORD Flags, D3DCOLOR Color, float Z,
									   DWORD Stencil )
{
	return origIDirect3DDevice9->Clear( Count, pRects, Flags, Color, Z, Stencil );
}

HRESULT proxyIDirect3DDevice9::SetTransform ( D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix )
{
	// Store the matrix
	pD3DData->StoreTransform ( State, pMatrix );

	return origIDirect3DDevice9->SetTransform( State, pMatrix );
}

HRESULT proxyIDirect3DDevice9::GetTransform ( D3DTRANSFORMSTATETYPE State, D3DMATRIX *pMatrix )
{
	return origIDirect3DDevice9->GetTransform( State, pMatrix );
}

HRESULT proxyIDirect3DDevice9::MultiplyTransform ( D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix )
{
	return origIDirect3DDevice9->MultiplyTransform( State, pMatrix );
}

HRESULT proxyIDirect3DDevice9::SetViewport ( CONST D3DVIEWPORT9 *pViewport )
{
	// Store matrix
	pD3DData->StoreViewport ( pViewport->X, pViewport->Y, pViewport->Width, pViewport->Height );

	return origIDirect3DDevice9->SetViewport( pViewport );
}

HRESULT proxyIDirect3DDevice9::GetViewport ( D3DVIEWPORT9 *pViewport )
{
	return origIDirect3DDevice9->GetViewport( pViewport );
}

HRESULT proxyIDirect3DDevice9::SetMaterial ( CONST D3DMATERIAL9 *pMaterial )
{
	return origIDirect3DDevice9->SetMaterial( pMaterial );
}

HRESULT proxyIDirect3DDevice9::GetMaterial ( D3DMATERIAL9 *pMaterial )
{
	return origIDirect3DDevice9->GetMaterial( pMaterial );
}

HRESULT proxyIDirect3DDevice9::SetLight ( DWORD Index, CONST D3DLIGHT9 *pLight )
{
	return origIDirect3DDevice9->SetLight( Index, pLight );
}

HRESULT proxyIDirect3DDevice9::GetLight ( DWORD Index, D3DLIGHT9 *pLight )
{
	return origIDirect3DDevice9->GetLight( Index, pLight );
}

HRESULT proxyIDirect3DDevice9::LightEnable ( DWORD Index, BOOL Enable )
{
	return origIDirect3DDevice9->LightEnable( Index, Enable );
}

HRESULT proxyIDirect3DDevice9::GetLightEnable ( DWORD Index, BOOL *pEnable )
{
	return origIDirect3DDevice9->GetLightEnable( Index, pEnable );
}

HRESULT proxyIDirect3DDevice9::SetClipPlane ( DWORD Index, CONST float *pPlane )
{
	return origIDirect3DDevice9->SetClipPlane( Index, pPlane );
}

HRESULT proxyIDirect3DDevice9::GetClipPlane ( DWORD Index, float *pPlane )
{
	return origIDirect3DDevice9->GetClipPlane( Index, pPlane );
}

HRESULT proxyIDirect3DDevice9::SetRenderState ( D3DRENDERSTATETYPE State, DWORD Value )
{
	return origIDirect3DDevice9->SetRenderState( State, Value );
}

HRESULT proxyIDirect3DDevice9::GetRenderState ( D3DRENDERSTATETYPE State, DWORD *pValue )
{
	return origIDirect3DDevice9->GetRenderState( State, pValue );
}

HRESULT proxyIDirect3DDevice9::CreateStateBlock ( D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9 **ppSB )
{
	return origIDirect3DDevice9->CreateStateBlock( Type, ppSB );
}

HRESULT proxyIDirect3DDevice9::BeginStateBlock ( void )
{
	return origIDirect3DDevice9->BeginStateBlock();
}

HRESULT proxyIDirect3DDevice9::EndStateBlock ( IDirect3DStateBlock9 **ppSB )
{
	return origIDirect3DDevice9->EndStateBlock( ppSB );
}

HRESULT proxyIDirect3DDevice9::SetClipStatus ( CONST D3DCLIPSTATUS9 *pClipStatus )
{
	return origIDirect3DDevice9->SetClipStatus( pClipStatus );
}

HRESULT proxyIDirect3DDevice9::GetClipStatus ( D3DCLIPSTATUS9 *pClipStatus )
{
	return origIDirect3DDevice9->GetClipStatus( pClipStatus );
}

HRESULT proxyIDirect3DDevice9::GetTexture ( DWORD Stage, IDirect3DBaseTexture9 **ppTexture )
{
	return origIDirect3DDevice9->GetTexture( Stage, ppTexture );
}

HRESULT proxyIDirect3DDevice9::SetTexture ( DWORD Stage, IDirect3DBaseTexture9 *pTexture )
{
	return origIDirect3DDevice9->SetTexture( Stage, pTexture );
}

HRESULT proxyIDirect3DDevice9::GetTextureStageState ( DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD *pValue )
{
	return origIDirect3DDevice9->GetTextureStageState( Stage, Type, pValue );
}

HRESULT proxyIDirect3DDevice9::SetTextureStageState ( DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value )
{
	return origIDirect3DDevice9->SetTextureStageState( Stage, Type, Value );
}

HRESULT proxyIDirect3DDevice9::GetSamplerState ( DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD *pValue )
{
	return origIDirect3DDevice9->GetSamplerState( Sampler, Type, pValue );
}

HRESULT proxyIDirect3DDevice9::SetSamplerState ( DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value )
{
	return origIDirect3DDevice9->SetSamplerState( Sampler, Type, Value );
}

HRESULT proxyIDirect3DDevice9::ValidateDevice ( DWORD *pNumPasses )
{
	return origIDirect3DDevice9->ValidateDevice( pNumPasses );
}

HRESULT proxyIDirect3DDevice9::SetPaletteEntries ( UINT PaletteNumber, CONST PALETTEENTRY *pEntries )
{
	return origIDirect3DDevice9->SetPaletteEntries( PaletteNumber, pEntries );
}

HRESULT proxyIDirect3DDevice9::GetPaletteEntries ( UINT PaletteNumber, PALETTEENTRY *pEntries )
{
	return origIDirect3DDevice9->GetPaletteEntries( PaletteNumber, pEntries );
}

HRESULT proxyIDirect3DDevice9::SetCurrentTexturePalette ( UINT PaletteNumber )
{
	return origIDirect3DDevice9->SetCurrentTexturePalette( PaletteNumber );
}

HRESULT proxyIDirect3DDevice9::GetCurrentTexturePalette ( UINT *PaletteNumber )
{
	return origIDirect3DDevice9->GetCurrentTexturePalette( PaletteNumber );
}

HRESULT proxyIDirect3DDevice9::SetScissorRect ( CONST RECT *pRect )
{
	return origIDirect3DDevice9->SetScissorRect( pRect );
}

HRESULT proxyIDirect3DDevice9::GetScissorRect ( RECT *pRect )
{
	return origIDirect3DDevice9->GetScissorRect( pRect );
}

HRESULT proxyIDirect3DDevice9::SetSoftwareVertexProcessing ( BOOL bSoftware )
{
	return origIDirect3DDevice9->SetSoftwareVertexProcessing( bSoftware );
}

BOOL proxyIDirect3DDevice9::GetSoftwareVertexProcessing ( void )
{
	return origIDirect3DDevice9->GetSoftwareVertexProcessing();
}

HRESULT proxyIDirect3DDevice9::SetNPatchMode ( float nSegments )
{
	return origIDirect3DDevice9->SetNPatchMode( nSegments );
}

float proxyIDirect3DDevice9::GetNPatchMode ( void )
{
	return origIDirect3DDevice9->GetNPatchMode();
}

HRESULT proxyIDirect3DDevice9::DrawPrimitive ( D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount )
{
	return origIDirect3DDevice9->DrawPrimitive( PrimitiveType, StartVertex, PrimitiveCount );
}

HRESULT proxyIDirect3DDevice9::DrawIndexedPrimitive ( D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex,
													  UINT MinVertexIndex, UINT NumVertices, UINT startIndex,
													  UINT primCount )
{

	return origIDirect3DDevice9->DrawIndexedPrimitive( PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices,
													   startIndex, primCount );
}

HRESULT proxyIDirect3DDevice9::DrawPrimitiveUP ( D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount,
												 CONST void *pVertexStreamZeroData, UINT VertexStreamZeroStride )
{
	return origIDirect3DDevice9->DrawPrimitiveUP( PrimitiveType, PrimitiveCount, pVertexStreamZeroData,
												  VertexStreamZeroStride );
}

HRESULT proxyIDirect3DDevice9::DrawIndexedPrimitiveUP ( D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex,
														UINT NumVertices, UINT PrimitiveCount, CONST void *pIndexData,
														D3DFORMAT IndexDataFormat, CONST void *pVertexStreamZeroData,
														UINT VertexStreamZeroStride )
{
	return origIDirect3DDevice9->DrawIndexedPrimitiveUP( PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount,
														 pIndexData, IndexDataFormat, pVertexStreamZeroData,
														 VertexStreamZeroStride );
}

HRESULT proxyIDirect3DDevice9::ProcessVertices ( UINT SrcStartIndex, UINT DestIndex, UINT VertexCount,
												 IDirect3DVertexBuffer9 *pDestBuffer,
												 IDirect3DVertexDeclaration9 *pVertexDecl, DWORD Flags )
{
	return origIDirect3DDevice9->ProcessVertices( SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags );
}

HRESULT proxyIDirect3DDevice9::CreateVertexDeclaration ( CONST D3DVERTEXELEMENT9 *pVertexElements,
														 IDirect3DVertexDeclaration9 **ppDecl )
{
	return origIDirect3DDevice9->CreateVertexDeclaration( pVertexElements, ppDecl );
}

HRESULT proxyIDirect3DDevice9::SetVertexDeclaration ( IDirect3DVertexDeclaration9 *pDecl )
{
	return origIDirect3DDevice9->SetVertexDeclaration( pDecl );
}

HRESULT proxyIDirect3DDevice9::GetVertexDeclaration ( IDirect3DVertexDeclaration9 **ppDecl )
{
	return origIDirect3DDevice9->GetVertexDeclaration( ppDecl );
}

HRESULT proxyIDirect3DDevice9::SetFVF ( DWORD FVF )
{
	return origIDirect3DDevice9->SetFVF( FVF );
}

HRESULT proxyIDirect3DDevice9::GetFVF ( DWORD *pFVF )
{
	return origIDirect3DDevice9->GetFVF( pFVF );
}

HRESULT proxyIDirect3DDevice9::CreateVertexShader ( CONST DWORD *pFunction, IDirect3DVertexShader9 **ppShader )
{
	return origIDirect3DDevice9->CreateVertexShader( pFunction, ppShader );
}

HRESULT proxyIDirect3DDevice9::SetVertexShader ( IDirect3DVertexShader9 *pShader )
{
	return origIDirect3DDevice9->SetVertexShader( pShader );
}

HRESULT proxyIDirect3DDevice9::GetVertexShader ( IDirect3DVertexShader9 **ppShader )
{
	return origIDirect3DDevice9->GetVertexShader( ppShader );
}

HRESULT proxyIDirect3DDevice9::SetVertexShaderConstantF ( UINT StartRegister, CONST float *pConstantData,
														  UINT Vector4fCount )
{
	return origIDirect3DDevice9->SetVertexShaderConstantF( StartRegister, pConstantData, Vector4fCount );
}

HRESULT proxyIDirect3DDevice9::GetVertexShaderConstantF ( UINT StartRegister, float *pConstantData, UINT Vector4fCount )
{
	return origIDirect3DDevice9->GetVertexShaderConstantF( StartRegister, pConstantData, Vector4fCount );
}

HRESULT proxyIDirect3DDevice9::SetVertexShaderConstantI ( UINT StartRegister, CONST int *pConstantData,
														  UINT Vector4iCount )
{
	return origIDirect3DDevice9->SetVertexShaderConstantI( StartRegister, pConstantData, Vector4iCount );
}

HRESULT proxyIDirect3DDevice9::GetVertexShaderConstantI ( UINT StartRegister, int *pConstantData, UINT Vector4iCount )
{
	return origIDirect3DDevice9->GetVertexShaderConstantI( StartRegister, pConstantData, Vector4iCount );
}

HRESULT proxyIDirect3DDevice9::SetVertexShaderConstantB ( UINT StartRegister, CONST BOOL *pConstantData, UINT BoolCount )
{
	return origIDirect3DDevice9->SetVertexShaderConstantB( StartRegister, pConstantData, BoolCount );
}

HRESULT proxyIDirect3DDevice9::GetVertexShaderConstantB ( UINT StartRegister, BOOL *pConstantData, UINT BoolCount )
{
	return origIDirect3DDevice9->GetVertexShaderConstantB( StartRegister, pConstantData, BoolCount );
}

HRESULT proxyIDirect3DDevice9::SetStreamSource ( UINT StreamNumber, IDirect3DVertexBuffer9 *pStreamData,
												 UINT OffsetInBytes, UINT Stride )
{
	return origIDirect3DDevice9->SetStreamSource( StreamNumber, pStreamData, OffsetInBytes, Stride );
}

HRESULT proxyIDirect3DDevice9::GetStreamSource ( UINT StreamNumber, IDirect3DVertexBuffer9 **ppStreamData,
												 UINT *OffsetInBytes, UINT *pStride )
{
	return origIDirect3DDevice9->GetStreamSource( StreamNumber, ppStreamData, OffsetInBytes, pStride );
}

HRESULT proxyIDirect3DDevice9::SetStreamSourceFreq ( UINT StreamNumber, UINT Divider )
{
	return origIDirect3DDevice9->SetStreamSourceFreq( StreamNumber, Divider );
}

HRESULT proxyIDirect3DDevice9::GetStreamSourceFreq ( UINT StreamNumber, UINT *Divider )
{
	return origIDirect3DDevice9->GetStreamSourceFreq( StreamNumber, Divider );
}

HRESULT proxyIDirect3DDevice9::SetIndices ( IDirect3DIndexBuffer9 *pIndexData )
{
	return origIDirect3DDevice9->SetIndices( pIndexData );
}

HRESULT proxyIDirect3DDevice9::GetIndices ( IDirect3DIndexBuffer9 **ppIndexData )
{
	return origIDirect3DDevice9->GetIndices( ppIndexData );
}

HRESULT proxyIDirect3DDevice9::CreatePixelShader ( CONST DWORD *pFunction, IDirect3DPixelShader9 **ppShader )
{
	return origIDirect3DDevice9->CreatePixelShader( pFunction, ppShader );
}

HRESULT proxyIDirect3DDevice9::SetPixelShader ( IDirect3DPixelShader9 *pShader )
{
	return origIDirect3DDevice9->SetPixelShader( pShader );
}

HRESULT proxyIDirect3DDevice9::GetPixelShader ( IDirect3DPixelShader9 **ppShader )
{
	return origIDirect3DDevice9->GetPixelShader( ppShader );
}

HRESULT proxyIDirect3DDevice9::SetPixelShaderConstantF ( UINT StartRegister, CONST float *pConstantData,
														 UINT Vector4fCount )
{
	return origIDirect3DDevice9->SetPixelShaderConstantF( StartRegister, pConstantData, Vector4fCount );
}

HRESULT proxyIDirect3DDevice9::GetPixelShaderConstantF ( UINT StartRegister, float *pConstantData, UINT Vector4fCount )
{
	return origIDirect3DDevice9->GetPixelShaderConstantF( StartRegister, pConstantData, Vector4fCount );
}

HRESULT proxyIDirect3DDevice9::SetPixelShaderConstantI ( UINT StartRegister, CONST int *pConstantData, UINT Vector4iCount )
{
	return origIDirect3DDevice9->SetPixelShaderConstantI( StartRegister, pConstantData, Vector4iCount );
}

HRESULT proxyIDirect3DDevice9::GetPixelShaderConstantI ( UINT StartRegister, int *pConstantData, UINT Vector4iCount )
{
	return origIDirect3DDevice9->GetPixelShaderConstantI( StartRegister, pConstantData, Vector4iCount );
}

HRESULT proxyIDirect3DDevice9::SetPixelShaderConstantB ( UINT StartRegister, CONST BOOL *pConstantData, UINT BoolCount )
{
	return origIDirect3DDevice9->SetPixelShaderConstantB( StartRegister, pConstantData, BoolCount );
}

HRESULT proxyIDirect3DDevice9::GetPixelShaderConstantB ( UINT StartRegister, BOOL *pConstantData, UINT BoolCount )
{
	return origIDirect3DDevice9->GetPixelShaderConstantB( StartRegister, pConstantData, BoolCount );
}

HRESULT proxyIDirect3DDevice9::DrawRectPatch ( UINT Handle, CONST float *pNumSegs, CONST D3DRECTPATCH_INFO *pRectPatchInfo )
{
	return origIDirect3DDevice9->DrawRectPatch( Handle, pNumSegs, pRectPatchInfo );
}

HRESULT proxyIDirect3DDevice9::DrawTriPatch ( UINT Handle, CONST float *pNumSegs, CONST D3DTRIPATCH_INFO *pTriPatchInfo )
{
	return origIDirect3DDevice9->DrawTriPatch( Handle, pNumSegs, pTriPatchInfo );
}

HRESULT proxyIDirect3DDevice9::DeletePatch ( UINT Handle )
{
	return origIDirect3DDevice9->DeletePatch( Handle );
}

HRESULT proxyIDirect3DDevice9::CreateQuery ( D3DQUERYTYPE Type, IDirect3DQuery9 **ppQuery )
{
	return origIDirect3DDevice9->CreateQuery( Type, ppQuery );
}