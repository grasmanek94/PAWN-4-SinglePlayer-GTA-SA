#include "StdInc.h"
#include "d3d9/d3drender.h"
CD3DFont * some_font = new CD3DFont("Tahoma",12,FW_BOLD);

CGameSA* pGame = NULL;

CGame * GetGameInterface( )
{
    DEBUG_TRACE("CGame * GetGameInterface()");

    pGame = new CGameSA;
	
    return (CGame *)pGame;
}

void MemSet ( void* dwDest, int cValue, uint uiAmount )
{
    memset ( dwDest, cValue, uiAmount );
}

void MemCpy ( void* dwDest, const void* dwSrc, uint uiAmount )
{
    memcpy ( dwDest, dwSrc, uiAmount );
}

bool GetDebugIdEnabled ( uint uiDebugId )
{
    return false;  
}

void LogEvent ( uint uiDebugId, const char* szType, const char* szContext, const char* szBody )
{
 
}

//called on application load
#include <sdx.h>
bool STUFF_INITED = false;

std::string string_format(const std::string fmt, ...) 
{
	int size = 512;
	std::string str;
	va_list ap;
	while (1) {
		str.resize(size);
		va_start(ap, fmt);
		int n = vsnprintf((char *)str.c_str(), size, fmt.c_str(), ap);
		va_end(ap);
		if (n > -1 && n < size) {
			str.resize(n);
			return str;
		}
		if (n > -1)
			size = n + 1;
		else
			size *= 2;
	}
	return str;
}

void Log(const std::string fmt, ...) 
{
	int size = 512;
	std::string str;
	va_list ap;
	while (1) {
		str.resize(size);
		va_start(ap, fmt);
		int n = vsnprintf((char *)str.c_str(), size, fmt.c_str(), ap);
		va_end(ap);
		if (n > -1 && n < size) {
			str.resize(n);
			std::cout << str << std::endl;
			return;
		}
		if (n > -1)
			size = n + 1;
		else
			size *= 2;
	}
	std::cout << str << std::endl;
}

LONG WINAPI unhandledExceptionFilter ( struct _EXCEPTION_POINTERS *ExceptionInfo )
{
	Log( " ---------------------------------------------------------------------" );
	//Log( " %s has crashed.", NAME );
	//Log( " Base address: 0x%p", g_hDllModule );
	Log( " Exception at address: 0x%p, Last function processed: unknown", ExceptionInfo->ExceptionRecord->ExceptionAddress/*, g_szLastFunc*/ );

	int m_ExceptionCode = ExceptionInfo->ExceptionRecord->ExceptionCode;
	int m_exceptionInfo_0 = ExceptionInfo->ExceptionRecord->ExceptionInformation[0];
	int m_exceptionInfo_1 = ExceptionInfo->ExceptionRecord->ExceptionInformation[1];
	int m_exceptionInfo_2 = ExceptionInfo->ExceptionRecord->ExceptionInformation[2];
	switch ( m_ExceptionCode )
	{
	case EXCEPTION_ACCESS_VIOLATION:
		Log( " Cause: EXCEPTION_ACCESS_VIOLATION" );
		if ( m_exceptionInfo_0 == 0 )
		{
			// bad read
			Log( " Attempted to read from: 0x%08x", m_exceptionInfo_1 );
		}
		else if ( m_exceptionInfo_0 == 1 )
		{
			// bad write
			Log( " Attempted to write to: 0x%08x", m_exceptionInfo_1 );
		}
		else if ( m_exceptionInfo_0 == 8 )
		{
			// user-mode data execution prevention (DEP)
			Log( " Data Execution Prevention (DEP) at: 0x%08x", m_exceptionInfo_1 );
		}
		else
		{
			// unknown, shouldn't happen
			Log( " Unknown access violation at: 0x%08x", m_exceptionInfo_1 );
		}
		break;

	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		Log( " Cause: EXCEPTION_ARRAY_BOUNDS_EXCEEDED" );
		break;

	case EXCEPTION_BREAKPOINT:
		Log( " Cause: EXCEPTION_BREAKPOINT" );
		break;

	case EXCEPTION_DATATYPE_MISALIGNMENT:
		Log( " Cause: EXCEPTION_DATATYPE_MISALIGNMENT" );
		break;

	case EXCEPTION_FLT_DENORMAL_OPERAND:
		Log( " Cause: EXCEPTION_FLT_DENORMAL_OPERAND" );
		break;

	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		Log( " Cause: EXCEPTION_FLT_DIVIDE_BY_ZERO" );
		break;

	case EXCEPTION_FLT_INEXACT_RESULT:
		Log( " Cause: EXCEPTION_FLT_INEXACT_RESULT" );
		break;

	case EXCEPTION_FLT_INVALID_OPERATION:
		Log( " Cause: EXCEPTION_FLT_INVALID_OPERATION" );
		break;

	case EXCEPTION_FLT_OVERFLOW:
		Log( " Cause: EXCEPTION_FLT_OVERFLOW" );
		break;

	case EXCEPTION_FLT_STACK_CHECK:
		Log( " Cause: EXCEPTION_FLT_STACK_CHECK" );
		break;

	case EXCEPTION_FLT_UNDERFLOW:
		Log( " Cause: EXCEPTION_FLT_UNDERFLOW" );
		break;

	case EXCEPTION_ILLEGAL_INSTRUCTION:
		Log( " Cause: EXCEPTION_ILLEGAL_INSTRUCTION" );
		break;

	case EXCEPTION_IN_PAGE_ERROR:
		Log( " Cause: EXCEPTION_IN_PAGE_ERROR" );
		if ( m_exceptionInfo_0 == 0 )
		{
			// bad read
			Log( " Attempted to read from: 0x%08x", m_exceptionInfo_1 );
		}
		else if ( m_exceptionInfo_0 == 1 )
		{
			// bad write
			Log( " Attempted to write to: 0x%08x", m_exceptionInfo_1 );
		}
		else if ( m_exceptionInfo_0 == 8 )
		{
			// user-mode data execution prevention (DEP)
			Log( " Data Execution Prevention (DEP) at: 0x%08x", m_exceptionInfo_1 );
		}
		else
		{
			// unknown, shouldn't happen
			Log( " Unknown access violation at: 0x%08x", m_exceptionInfo_1 );
		}

		// log NTSTATUS
		Log( " NTSTATUS: 0x%08x", m_exceptionInfo_2 );

		/*
		NT_SUCCESS(Status)
			Evaluates to TRUE if the return value specified by Status is a success type (0 - 0x3FFFFFFF) or an informational type (0x40000000 - 0x7FFFFFFF).
		NT_INFORMATION(Status)
			Evaluates to TRUE if the return value specified by Status is an informational type (0x40000000 - 0x7FFFFFFF).
		NT_WARNING(Status)
			Evaluates to TRUE if the return value specified by Status is a warning type (0x80000000 - 0xBFFFFFFF).
		NT_ERROR(Status)
			Evaluates to TRUE if the return value specified by Status is an error type (0xC0000000 - 0xFFFFFFFF)
		*/
		break;

	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		Log( " Cause: EXCEPTION_INT_DIVIDE_BY_ZERO" );
		break;

	case EXCEPTION_INT_OVERFLOW:
		Log( " Cause: EXCEPTION_INT_OVERFLOW" );
		break;

	case EXCEPTION_INVALID_DISPOSITION:
		Log( " Cause: EXCEPTION_INVALID_DISPOSITION" );
		break;

	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
		Log( " Cause: EXCEPTION_NONCONTINUABLE_EXCEPTION" );
		break;

	case EXCEPTION_PRIV_INSTRUCTION:
		Log( " Cause: EXCEPTION_PRIV_INSTRUCTION" );
		break;

	case EXCEPTION_SINGLE_STEP:
		Log( " Cause: EXCEPTION_SINGLE_STEP" );
		break;

	case EXCEPTION_STACK_OVERFLOW:
		Log( " Cause: EXCEPTION_STACK_OVERFLOW" );
		break;

	case DBG_CONTROL_C:
		Log( " Cause: DBG_CONTROL_C (WTF!)" );
		break;

	default:
		Log( " Cause: %08x", m_ExceptionCode );
	}

	Log( " EAX: 0x%08x || ESI: 0x%08x", ExceptionInfo->ContextRecord->Eax, ExceptionInfo->ContextRecord->Esi );
	Log( " EBX: 0x%08x || EDI: 0x%08x", ExceptionInfo->ContextRecord->Ebx, ExceptionInfo->ContextRecord->Edi );
	Log( " ECX: 0x%08x || EBP: 0x%08x", ExceptionInfo->ContextRecord->Ecx, ExceptionInfo->ContextRecord->Ebp );
	Log( " EDX: 0x%08x || ESP: 0x%08x", ExceptionInfo->ContextRecord->Edx, ExceptionInfo->ContextRecord->Esp );

	Log( " ---------------------------------------------------------------------" );

	return EXCEPTION_CONTINUE_SEARCH;
}

bool RenderChangestate(bool init)
{
	bool ret = true;//failed init
	if(init)
	{	
		//init all things that need init
		if(FAILED(render->Initialize( gl_pmyIDirect3DDevice9 )))
			ret = false;
		if(FAILED(some_font->Initialize(gl_pmyIDirect3DDevice9)))
			ret = false;
		if(!ret) 
			RenderChangestate(false);
		return ret;
	}
	//uninitialize all things
	some_font->Invalidate();
	render->Invalidate();
	return false;
}

void SDX::OnLoad()
{
	SetUnhandledExceptionFilter( unhandledExceptionFilter );	
	//pGame->FindGameVersion();
    // No intro movies kthx
    if ( GetGameInterface()->GetGameVersion() == VERSION_US_10 )
    {
        MemPut < DWORD > ( 0x748EF8, 0x748AE7 );
        MemPut < DWORD > ( 0x748EFC, 0x748B08 );
        MemPut < BYTE > ( 0x748B0E, 5 );
    }
    else if ( GetGameInterface()->GetGameVersion() == VERSION_EU_10 )
    {
        MemPut < DWORD > ( 0x748F48, 0x748B37 );
        MemPut < DWORD > ( 0x748F4C, 0x748B58 );
        MemPut < BYTE > ( 0x748B5E, 5 );
    }
    // Allow all screen aspect ratios
    MemPut < WORD > ( 0x745BC9, 0x9090 );

    // Allow all screen aspect ratios in multi-monitor dialog
    MemPut < WORD > ( 0x7459E1, 0x9090 );

    // Remove 14ms wait (Was done every other frame for some reason)
    MemPut < BYTE > ( 0x53E94C, 0x00 );

    // Disable the loading screen tune.
    if ( GetGameInterface()->GetGameVersion() == VERSION_US_10 )
        MemSet ( (void *)0x748CF6, 0x90, 5 );
    else if ( GetGameInterface()->GetGameVersion() == VERSION_EU_10 )
        MemSet ( (void *)0x748D46, 0x90, 5 );

    // Double the size of CPlaceable matrix array to fix a crash after CMatrixLinkList::AddToList1
    MemPut < int > ( 0x54F3A1, 1800 );

    // Aircraft Max Height checks are at 0x6D2614 and 0x6D2625 edit the check to use our own float.
    //MemPut ( 0x6D2614, &m_fAircraftMaxHeight );
    //MemPut ( 0x6D2625, &m_fAircraftMaxHeight );

    // Move birds up a bit so they don't fly through solid objects quite so often
    MemPut < float > ( 0x71240e, 10.f + 10.f );
    MemPut < float > ( 0x712413, 2.f  + 10.f );
    MemPut < float > ( 0x712447, 13.f + 10.f );
    MemPut < float > ( 0x71244c, 4.f  + 10.f );

	// Allow turning on vehicle lights even if the engine is off
	MemSet ( (void *)0x6E1DBC, 0x90, 8 );

	// Fix vehicle back lights both using light state 3 (SA bug)
	MemPut < BYTE > ( 0x6E1D4F, 2 );

	// increase the number of vehicles types (not actual vehicles) that can be loaded at once
	MemPutFast < int > ( 0x8a5a84, 127 );

	// Increase double link limit from 3200 ro 8000
	MemPut < int > ( 0x00550F82, 8000 );

	// DISABLE wanted levels for military zones
	MemPut < BYTE > ( 0x72DF0D, 0xEB );

	// THROWN projectiles throw more accurately
	MemPut < BYTE > ( 0x742685, 0x90 );
	MemPut < BYTE > ( 0x742686, 0xE9 );

	// DISABLE CRoadBlocks::GenerateRoadblocks
	MemPut < BYTE > ( 0x4629E0, 0xC3 );

	// DISABLE CGameLogic__SetPlayerWantedLevelForForbiddenTerritories
	MemPut < BYTE > ( 0x441770, 0xC3 );

	// ALLOW picking up of all vehicles (GTA doesn't allow picking up of 'locked' script created vehicles)
	MemPut < BYTE > ( 0x6A436C, 0x90 );
	MemPut < BYTE > ( 0x6A436D, 0x90 );

	// Satchel crash fix
	// C89110: satchel (bomb) positions pointer?
	// C891A8+4: satchel (model) positions pointer? gets set to NULL on player death, causing an access violation
	// C891A8+12: satchel (model) disappear time (in SystemTime format). 738F99 clears the satchel when VAR_SystemTime is larger.
	MemSet ( (LPVOID)0x738F3A, 0x90, 83 );

	// Prevent gta stopping driveby players from falling off
	MemSet ( (LPVOID)0x6B5B17, 0x90, 6 );

    // PREVENT CJ smoking and drinking like an addict
    //005FBA26   EB 29            JMP SHORT gta_sa.005FBA51
    MemPut < BYTE > ( 0x5FBA26, 0xEB );

	// Increase VehicleStruct pool size
	MemPut < BYTE > ( 0x5B8342 + 0, 0x33 );     // xor eax, eax
	MemPut < BYTE > ( 0x5B8342 + 1, 0xC0 );
	MemPut < BYTE > ( 0x5B8342 + 2, 0xB0 );     // mov al, 0xFF
	MemPut < BYTE > ( 0x5B8342 + 3, 0xFF );
	MemPut < BYTE > ( 0x5B8342 + 4, 0x8B );     // mov edi, eax
	MemPut < BYTE > ( 0x5B8342 + 5, 0xF8 );

	// ALLOW more than 8 players (crash with more if this isn't done)
	//0060D64D   90               NOP
	//0060D64E   E9 9C000000      JMP gta_sa.0060D6EF
	MemPut < BYTE > ( 0x60D64D, 0x90 );
	MemPut < BYTE > ( 0x60D64E, 0xE9 );

	// PREVENT the camera from messing up for drivebys for vehicle drivers
	MemPut < BYTE > ( 0x522423, 0x90 );
	MemPut < BYTE > ( 0x522424, 0x90 );

	// HACK to allow boats to be rotated
	/*
	006F2089   58               POP EAX
	006F208A   90               NOP
	006F208B   90               NOP
	006F208C   90               NOP
	006F208D   90               NOP
	*/
	MemPut < BYTE > ( 0x6F2089, 0x58 );
	MemSet ((void *)0x6F208A,0x90,4);

	// INCREASE CEntyInfoNode pool size
	//00550FB9   68 F4010000      PUSH 1F4
	/*
	MemPut < BYTE > ( 0x550FBA, 0xE8 );
	MemPut < BYTE > ( 0x550FBB, 0x03 );
	*/
	MemPut < BYTE > ( 0x550FBA, 0x00 );
	MemPut < BYTE > ( 0x550FBB, 0x10 );

	// Trains may infact go further than Los Santos
	MemPut < BYTE > ( 0x4418E0, 0xC3 );

	// Let us sprint everywhere (always return 0 from CSurfaceData::isSprint)
	MemPut < DWORD > ( 0x55E870, 0xC2C03366 );
	MemPut < WORD > ( 0x55E874, 0x0004 );

	// Allow turning on vehicle lights even if the engine is off
	MemSet ( (void *)0x6E1DBC, 0x90, 8 );

	// Fix vehicle back lights both using light state 3 (SA bug)
	MemPut < BYTE > ( 0x6E1D4F, 2 );

	// Avoid GTA setting vehicle first color to white after changing the paintjob
	MemSet ( (void *)0x6D65C5, 0x90, 11 );

	// Disable GTA vehicle detachment at rotation awkwardness
	MemPut < BYTE > ( 0x547441, 0xE9 );
	MemPut < BYTE > ( 0x547442, 0xFA );
	MemPut < BYTE > ( 0x547443, 0x02 );
	MemPut < BYTE > ( 0x547444, 0x00 );
	MemPut < BYTE > ( 0x547445, 0x00 );
	//
}

void SDX::OnBeginScene_Before()
{

}

void SDX::OnBeginScene_After(HRESULT result)
{

}

void SDX::OnEndScene_Before()
{

}

void SDX::OnEndScene_After(HRESULT result)
{

}

CVehicle* buildings[6] = {0,0,0,0,0,0};
CVehicle* vehicle = 0;

void SDX::OnPresent_Before(CONST RECT* &pSourceRect,CONST RECT* &pDestRect,HWND &hDestWindowOverride,CONST RGNDATA* &pDirtyRegion)
{
	if(!STUFF_INITED)
		STUFF_INITED = RenderChangestate(true);
	static bool ModInited = false;
	if(!ModInited)
	{
		if(pGame->GetSystemState() == eSystemState::GS_PLAYING_GAME)
		{
			pGame->Initialize();
			pGame->InitLocalPlayer(  );
			ModInited = true;
		}
	}
	else
	{
		if(pGame->GetSystemState() == eSystemState::GS_INIT_PLAYING_GAME)
		{
			ModInited = false;
		}
		if(STUFF_INITED)
		{
			if(!SUCCEEDED(render->BeginRender()))
				return;
			//render->D3DBoxBorder(10.0f,15.0f,30.0f,50.0f,0xFF000000,0x77777777);
			//render->DrawLine(D3DXVECTOR3(0.0,0.0,0.0),D3DXVECTOR3(100.0,100.0,100.0),0x77777777);
			//render->DrawRectangleInWorld(D3DXVECTOR3(-100.0,0.0,0.0),D3DXVECTOR3(100.0,0.0,0.0),D3DXVECTOR3(-100.0,0.0,100.0),D3DXVECTOR3(100.0,0.0,100.0),0xFFFF0000,false);
			//some_font->Print(50.0,50.0,-1,string_format("POS: {FFFF0000}X(%.2f);Y(%.2f);Z(%.2f);",pGame->GetPools ()->GetPedFromRef ( (DWORD)1 )->GetPosition()->fX,pGame->GetPools ()->GetPedFromRef ( (DWORD)1 )->GetPosition()->fY,pGame->GetPools ()->GetPedFromRef ( (DWORD)1 )->GetPosition()->fZ).c_str(),true);
			{
				if(GetAsyncKeyState('Q') && GetAsyncKeyState('P'))
				{
					if(!vehicle)
					{
						pGame->GetModelInfo(520)->ModelAddRef( EModelRequestType::BLOCKING, "CVehicle::Create" );

						vehicle = pGame->GetPools()->AddVehicle(static_cast < eVehicleTypes > (520),1,1);
						vehicle->SetPosition(&CVector(*pGame->GetPools ()->GetPedFromRef ( (DWORD)1 )->GetPosition()+CVector(1.0,1.0,1.0)));


						buildings[0] = pGame->GetPools()->AddVehicle(static_cast < eVehicleTypes > (520),1,1);
						buildings[1] = pGame->GetPools()->AddVehicle(static_cast < eVehicleTypes > (520),1,1);
						buildings[2] = pGame->GetPools()->AddVehicle(static_cast < eVehicleTypes > (520),1,1);
						buildings[3] = pGame->GetPools()->AddVehicle(static_cast < eVehicleTypes > (520),1,1);
						buildings[4] = pGame->GetPools()->AddVehicle(static_cast < eVehicleTypes > (520),1,1);
						buildings[5] = pGame->GetPools()->AddVehicle(static_cast < eVehicleTypes > (520),1,1);
					}
				}
				if(GetAsyncKeyState('I') && GetAsyncKeyState('P'))
				{
					pGame->GetPlayerInfo()->SetPlayerMoney(pGame->GetPools()->GetPedCount());
				}
				if(GetAsyncKeyState('I') && GetAsyncKeyState('O'))
				{
					pGame->GetPlayerInfo()->SetPlayerMoney(pGame->GetPools()->GetVehicleCount());
				}
				if(vehicle)
				{
					buildings[0]->Teleport(vehicle->GetPosition()->fX+20.0,vehicle->GetPosition()->fY,vehicle->GetPosition()->fZ);
					buildings[1]->Teleport(vehicle->GetPosition()->fX-20.0,vehicle->GetPosition()->fY,vehicle->GetPosition()->fZ);
					buildings[2]->Teleport(vehicle->GetPosition()->fX,vehicle->GetPosition()->fY+20.0,vehicle->GetPosition()->fZ);
					buildings[3]->Teleport(vehicle->GetPosition()->fX,vehicle->GetPosition()->fY-20.0,vehicle->GetPosition()->fZ);
					buildings[4]->Teleport(vehicle->GetPosition()->fX,vehicle->GetPosition()->fY,vehicle->GetPosition()->fZ+20.0);
					buildings[5]->Teleport(vehicle->GetPosition()->fX,vehicle->GetPosition()->fY,vehicle->GetPosition()->fZ-20.0);
					some_font->Print(50.0,50.0,-1,string_format("POS: {FFFF0000}X(%.2f);Y(%.2f);Z(%.2f);",vehicle->GetPosition()->fX,vehicle->GetPosition()->fY,vehicle->GetPosition()->fZ).c_str(),true);
					some_font->Print(50.0,65.0,-1,string_format("POS: {FFFF0000}X(%.2f);Y(%.2f);Z(%.2f);",buildings[0]->GetPosition()->fX,buildings[0]->GetPosition()->fY,buildings[0]->GetPosition()->fZ).c_str(),true);
				}
			}
			render->EndRender();
		}
	}
}

void SDX::OnPresent_After(CONST RECT* &pSourceRect,CONST RECT* &pDestRect,HWND &hDestWindowOverride,CONST RGNDATA* &pDirtyRegion, HRESULT &result)
{

}

//called on application shutdown (not working yet)
void SDX::OnUnload()
{
	if(STUFF_INITED)
		STUFF_INITED = RenderChangestate(false);
}

//called on device reset
void SDX::OnRelease_Before()
{
	//deallocate all srpites, surfaces, etc
	if(STUFF_INITED)
		STUFF_INITED = RenderChangestate(false);
}

void SDX::OnRelease_After(ULONG count)
{
	//if count is bigger than 0, allocate again
	while(!STUFF_INITED && count)
		STUFF_INITED = RenderChangestate(true);
}

void SDX::OnReset_Before(D3DPRESENT_PARAMETERS* &pPresentationParameters)
{
	//uninitialize our shit
	if(STUFF_INITED)
		STUFF_INITED = RenderChangestate(false);
}

void SDX::OnReset_After(D3DPRESENT_PARAMETERS* &pPresentationParameters, HRESULT result)
{
	while(!STUFF_INITED)
		STUFF_INITED = RenderChangestate(true);
	//initialize our shit again
}