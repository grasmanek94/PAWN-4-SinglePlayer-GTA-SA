/*

PROJECT:		SAMOD 2
LICENSE:		See LICENSE in the top level directory
COPYRIGHT:		Copyright Gamer_Z

SA_MOD is available from http://gpb.googlecode.com

SA_MOD is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

SA_MOD is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with SA_MOD.  If not, see <http://www.gnu.org/licenses/>.

*/
#include <main.h>
#include <patch.h>
#include <inject.h>

#define VERSION "13084"

#pragma comment(linker, "/VERSION:" VERSION)

std::map				<std::string,std::string>				samod2_config;

HINSTANCE				g_hOrigDll = NULL;
HMODULE					g_hDllModule = NULL;
char					g_szWorkingDirectory[MAX_PATH];
FILE					*g_flLog = NULL;
FILE					*g_flLogAll = NULL;
char					g_szLastFunc[256];

#if defined __DEBUGx
extern RsGlobalType				*g_RsGlobal;
bool					g_dbgwin_IsDebugPresent = false;		
#endif

CSettingsSAInterface	*g_pCSettingsSAInterface = (CSettingsSAInterface *)CLASS_CMenuManager;
D3DPRESENT_PARAMETERS	*g_pGTAPresent = (D3DPRESENT_PARAMETERS *)0xC9C040;

CGameSA					*pGameInterface = NULL;
CPools					*pPools = NULL;
CPed					*pPedSelf = NULL;
CPedSAInterface			*pPedSelfSA = NULL;

bool m_game_reloaded = false;

#define pGame pGameInterface

#include <proxydll.h>

// to store information about the Windows OS
t_WindowsInfo			WindowsInfo;

void traceLastFunc ( const char *szFunc )
{
	_snprintf_s( g_szLastFunc, sizeof(g_szLastFunc)-1, szFunc );
}

unsigned int LoadSAModConfig();
#include <console.h>

void Log ( const char *fmt, ... )
{
	if ( !g_szWorkingDirectory ) return;

	SYSTEMTIME	time;
	va_list		ap;

	if ( g_flLogAll == NULL )
	{
		char	filename_all[512];
		snprintf( filename_all, sizeof(filename_all), "%s\\%s", g_szWorkingDirectory, "samod2_all.log" );

		g_flLogAll = fopen( filename_all, "a" );
		if ( g_flLogAll == NULL )
			return;
	}

	GetLocalTime( &time );
	fprintf( g_flLogAll, "[%02d-%02d-%02d || %02d:%02d:%02d.%03d] ", time.wDay, time.wMonth, time.wYear, time.wHour,
		time.wMinute, time.wSecond, time.wMilliseconds );
	char tempstorage[512];
	va_start( ap, fmt );
	vfprintf( g_flLogAll, fmt, ap );
	vsprintf(tempstorage,fmt,ap);
	va_end( ap );
	sprintf(SA::MOD::Console::var::string,"{FFFF0000}[LOG]{FFFFFFFF}%s",tempstorage);
	SA::MOD::Console::AddLine(SA::MOD::Console::var::string,false);
	fprintf( g_flLogAll, "\n" );
	fflush( g_flLogAll );
}

static int init ( void )
{
	traceLastFunc( "init()" );

	if ( g_hOrigDll == NULL )
	{
		if ( GetModuleFileName(g_hDllModule, g_szWorkingDirectory, sizeof(g_szWorkingDirectory) - 32) != 0 )
		{
			if ( strrchr(g_szWorkingDirectory, '\\') != NULL )
				*strrchr( g_szWorkingDirectory, '\\' ) = 0;
			else
				strcpy( g_szWorkingDirectory, "." );
		}
		else
		{
			strcpy( g_szWorkingDirectory, "." );
		}

		// Hello World
		Log( "Initializing %s", NAME );
		Log( "Compiled: %s CL:%d", COMPILE_DT, COMPILE_VERSION );

		// log windows version for people that forget to report it
		WindowsInfo.osPlatform = (int) * (DWORD *)GTAvar_osPlatform;
		WindowsInfo.osVer = (int) * (DWORD *)GTAvar_osVer;
		WindowsInfo.winVer = (int) * (DWORD *)GTAvar_winVer;
		WindowsInfo.winMajor = (int) * (DWORD *)GTAvar_winMajor;
		if ( WindowsInfo.osPlatform == 2 )
			Log( "OS: Windows Version %d.%d.%d", WindowsInfo.winMajor, WindowsInfo.winVer, WindowsInfo.osVer );
		else
			Log( "OS: Not Windows (%d.%d.%d)", WindowsInfo.winMajor, WindowsInfo.winVer, WindowsInfo.osVer );


#pragma warning( disable : 4127 )
		if ( sizeof(struct vehicle_info) != 2584 )
		{
			Log( "sizeof(struct vehicle_info) == %d, aborting.", sizeof(struct vehicle_info) );
			return 0;
		}

		if ( sizeof(struct actor_info) != 1988 )
		{
			Log( "sizeof(struct actor_info) == %d, aborting.", sizeof(struct actor_info) );
			return 0;
		}
#pragma warning( default : 4127 )

		// get actual d3d9.dll and proxy original D3D9Device
		g_hOrigDll = LoadLibrary( "proxyloader_d3d9.dll" );
		char	filename[MAX_PATH];
		if ( g_hOrigDll == NULL )
		{	
			Log( "No proxyloader_d3d9.dll in current directory, loading system d3d9.dll.." );
			GetSystemDirectory( filename, (UINT) (MAX_PATH - strlen("\\d3d9.dll") - 1) );

			strcat( filename, "\\d3d9.dll" );
			g_hOrigDll = LoadLibrary( filename );
			if ( g_hOrigDll == NULL )
			{
				Log( "Failed to load %s", filename );
				return 0;
			}
			Log( "Succesfully loaded %s", filename );
		}
		orig_Direct3DCreate9 = ( D3DC9 ) GetProcAddress( g_hOrigDll, "Direct3DCreate9" );
		Log( "load->orig_Direct3DCreate9" );
		if ( orig_Direct3DCreate9 == NULL )
		{
			Log( "%s does not export Direct3DCreate9!?", filename );
			FreeLibrary( g_hOrigDll );
			return 0;
		}
		Log( "End of dll main" );

	}

	return 1;
}

IDirect3D9 * WINAPI sys_Direct3DCreate9 ( UINT SDKVersion )
{
	pDirect3D9 = NULL;
	if ( init() )
		pDirect3D9 = new proxyIDirect3D9( orig_Direct3DCreate9(SDKVersion) );

	// do stuff that needs to happen BEFORE gta inits

	Log("Direct3DCreate9() called. Returning 0x%p.", pDirect3D9);
	return pDirect3D9;
}

LONG WINAPI unhandledExceptionFilter ( struct _EXCEPTION_POINTERS *ExceptionInfo )
{
	Log( " ---------------------------------------------------------------------" );
	Log( " %s has crashed.", NAME );
	Log( " Base address: 0x%p", g_hDllModule );
	Log( " Exception at address: 0x%p, Last function processed: %s", ExceptionInfo->ExceptionRecord->ExceptionAddress, g_szLastFunc );

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

void __cdecl LoadSCMData()
{
	m_game_reloaded = true;
}
//

bool ProcessConfigFile(std::istream& is) 
{
	std::string line;
	while(std::getline(is, line))
	{
		std::string 
			key, 
			value;
		std::istringstream inner_iss (line);
		if(std::getline(inner_iss, key,'=') && std::getline (inner_iss, value))
		{
			samod2_config[key] = value;
		}
		// read from iss 
	}
	return is.eof();
}

bool ReadConfig()
{
	samod2_config["AMX Libraries Relative Path"]	= "libraries\\pluginsa\\plugins";
	samod2_config["DLL Libraries Relative Path"]	= "libraries";
	samod2_config["AMX Scripts Relative Path"]		= "libraries\\pluginsa\\scripts";
	samod2_config["AMX Scriptfiles Relative Path"]	= "libraries\\pluginsa\\scriptfiles";

	std::ifstream ifs("samod2_config.ini");
	if(ifs.good())
	{
		return ProcessConfigFile(ifs);
	}
	return false;
}

//////////////////////
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
////////////////////////////////////////

#define SaModConfig_LoadHex(a,b); \
	if(samod2_config.find(a) != samod2_config.end()) \
{ \
	size_t insert_ox_back = samod2_config[a].find("0x"); \
	if(insert_ox_back != std::string::npos) \
{ \
	samod2_config[a].replace(samod2_config[a].find("0x"),2,""); \
	insert_ox_back = true; \
} \
	if(sscanf(samod2_config[a].c_str(), "%x", &tempvalui) == 1) \
{ \
	b = tempvalui; \
	++LoadedEntries; \
} \
}

#define SaModConfig_LoadInteger(a,b); \
	if(samod2_config.find(a) != samod2_config.end()) \
{ \
	if(sscanf(samod2_config[a].c_str(), "%d", &tempvalui) == 1) \
{ \
	b = tempvalui; \
	++LoadedEntries; \
} \
}

#define SaModConfig_LoadString(a,b); \
	if(samod2_config.find(a) != samod2_config.end()) \
{ \
	b.assign(SA::MOD::Console::string_format("%s",samod2_config[a].c_str()));\
	++LoadedEntries; \
}

#define SaModConfig_LoadCharString(a,b); \
	if(samod2_config.find(a) != samod2_config.end()) \
{ \
	sprintf(b,"%s",samod2_config[a].c_str());\
	++LoadedEntries; \
}

#define SaModConfig_LoadBool(a,b); \
	if(samod2_config.find(a) != samod2_config.end()) \
{ \
	if(samod2_config[a].find("true") != std::string::npos) \
{ \
	b = true; \
} \
		else \
{ \
	b = false; \
} \
	++LoadedEntries; \
}

unsigned int LoadSAModConfig()
{
	unsigned int tempvalui = 0;
	unsigned int LoadedEntries = 0;
	SaModConfig_LoadHex("Console Box Color",SA::MOD::Console::Colors::Box);
	SaModConfig_LoadHex("Console Box Outline Color",SA::MOD::Console::Colors::BoxOutline);
	SaModConfig_LoadHex("Console Text Box Text Color",SA::MOD::Console::Colors::TextBoxText);
	SaModConfig_LoadHex("Console Text Box Color",SA::MOD::Console::Colors::TextBox);
	SaModConfig_LoadHex("Console Text Box Outline Color",SA::MOD::Console::Colors::TextBoxOutline);
	SaModConfig_LoadHex("Console Default Text Color",SA::MOD::Console::Colors::DefaultText);
	SaModConfig_LoadHex("Console Scrollbar Background Color",SA::MOD::Console::Colors::ScrollbarBackground);
	SaModConfig_LoadHex("Console Scrollbar Outline Color",SA::MOD::Console::Colors::ScrollbarOutline);
	SaModConfig_LoadHex("Console Scroll Indicator Background Color",SA::MOD::Console::Colors::ScrollIndicatorBackground);
	SaModConfig_LoadHex("Console Scroll Indicator Outline Color",SA::MOD::Console::Colors::ScrollIndicatorOutline);
	return LoadedEntries;
}

void __cdecl GameRender();

BOOL APIENTRY DllMain ( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved )
{
	std::string commands(GetCommandLine());
	switch ( ul_reason_for_call )
	{
	case DLL_PROCESS_ATTACH:
#if defined __DEBUGx
		if(commands.find("samod_debug") != std::string::npos)
		{
			while(!IsDebuggerPresent()){}
			g_dbgwin_IsDebugPresent = true;
		}
#endif
		DisableThreadLibraryCalls( hModule );
		g_hDllModule = hModule;
		SetUnhandledExceptionFilter( unhandledExceptionFilter );

		ReadConfig();

		DWORD oldProt;
		VirtualProtect((LPVOID)0x401000, 0x4A3000, PAGE_EXECUTE_READWRITE, &oldProt);

		patch(0x46A22F, (DWORD)&ScriptTick - 0x46A233, 4);
		patch(0x469387, (DWORD)&LoadSCMData - 0x46938B, 4);
		// Allow turning on vehicle lights even if the engine is off
		if(samod2_config["Allow turning on vehicle lights even if the engine is off"].find("true") != std::string::npos)
			patch ( (void *)0x6E1DBC, 0x90, 8 );

		// Fix vehicle back lights both using light state 3 (SA bug)
		if(samod2_config["Fix vehicle back lights both using light state 3 (SA bug)"].find("true") != std::string::npos)
			patch ( 0x6E1D4F, 2, sizeof(BYTE) );
		// Avoid GTA setting vehicle first color to white after changing the paintjob
		if(samod2_config["Avoid GTA setting vehicle first color to white after changing the paintjob"].find("true") != std::string::npos)
			patch ( (void *)0x6D65C5, 0x90, 11 );
		// No intro movies kthx
		if(samod2_config["No intro movies"].find("true") != std::string::npos)
		{
			if ( *(BYTE *)0x748ADD == 0xFF && *(BYTE *)0x748ADE == 0x53 )
			{
				patch ( 0x748EF8, 0x748AE7, sizeof DWORD );
				patch ( 0x748EFC, 0x748B08, sizeof DWORD );
				patch ( 0x748B0E, 5, sizeof BYTE );
			}
			else
			{
				patch ( 0x748F48, 0x748B37, sizeof DWORD );
				patch ( 0x748F4C, 0x748B58, sizeof DWORD );
				patch ( 0x748B5E, 5, sizeof BYTE );
			}
		}
		// Remove 14ms wait
		if(samod2_config["Remove 14ms wait"].find("true") != std::string::npos)
			patch ( 0x53E94C, 0x00, sizeof BYTE );

		// Disable the loading screen tune.
		if(samod2_config["Disable the loading screen tune"].find("true") != std::string::npos)
		{
			if ( *(BYTE *)0x748ADD == 0xFF && *(BYTE *)0x748ADE == 0x53 )
				patch ( (void *)0x748CF6, 0x90, 5 );
			else
				patch ( (void *)0x748D46, 0x90, 5 );
		}

		// Increase VehicleStruct pool size
		if(samod2_config["Increase VehicleStruct pool size"].find("true") != std::string::npos)
		{
			patch ( 0x5B8342 + 0, 0x33, sizeof BYTE );     // xor eax, eax
			patch ( 0x5B8342 + 1, 0xC0, sizeof BYTE );
			patch ( 0x5B8342 + 2, 0xB0, sizeof BYTE );     // mov al, 0xFF
			patch ( 0x5B8342 + 3, 0xFF, sizeof BYTE );
			patch ( 0x5B8342 + 4, 0x8B, sizeof BYTE );     // mov edi, eax
			patch ( 0x5B8342 + 5, 0xF8, sizeof BYTE );
		}

		// Increase double link limit from 3200 to 8000
		if(samod2_config["Increase double link limit from 3200 to 8000"].find("true") != std::string::npos)
			patch ( 0x00550F82, 8000, sizeof(int) );

		// Increase the number of vehicles types (not actual vehicles) that can be loaded at once
		if(samod2_config["Increase the number of vehicles types (not actual vehicles) that can be loaded at once"].find("true") != std::string::npos)
			patch ( 0x8a5a84, 127, sizeof(int) );

		// ALLOW more than 8 players (crash with more if this isn't done)
		if(samod2_config["ALLOW more than 8 players (crash with more if this isn't done)"].find("true") != std::string::npos)
		{
			patch ( 0x60D64D, 0x90, sizeof BYTE );
			patch ( 0x60D64E, 0xE9, sizeof BYTE );
		}

		// DISABLE cinematic camera for trains
		if(samod2_config["DISABLE cinematic camera for trains"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x52A535, 0 );
		}

		// DISABLE wanted levels for military zones
		if(samod2_config["DISABLE wanted levels for military zones"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x72DF0D, 0xEB );
		}

		// THROWN projectiles throw more accurately
		if(samod2_config["THROWN projectiles throw more accurately"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x742685, 0x90 );
			MemPut < BYTE > ( 0x742686, 0xE9 );
		}

		// DISABLE CRoadBlocks::GenerateRoadblocks
		if(samod2_config["DISABLE CRoadBlocks::GenerateRoadblocks"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x4629E0, 0xC3 );
		}

		// DISABLE CPopulation__AddToPopulation
		if(samod2_config["DISABLE CPopulation__AddToPopulation"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x614720, 0x32 );
			MemPut < BYTE > ( 0x614721, 0xC0 );
			MemPut < BYTE > ( 0x614722, 0xC3 );
		}

		// DISABLE CGameLogic__SetPlayerWantedLevelForForbiddenTerritories
		if(samod2_config["DISABLE CGameLogic__SetPlayerWantedLevelForForbiddenTerritories"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x441770, 0xC3 );
		}

		// DISABLE CCrime__ReportCrime
		if(samod2_config["DISABLE CCrime__ReportCrime"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x532010, 0xC3 );
		}

		// Disable MakePlayerSafe
		if(samod2_config["Disable MakePlayerSafe"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x56E870, 0xC2 );
			MemPut < BYTE > ( 0x56E871, 0x08 );
			MemPut < BYTE > ( 0x56E872, 0x00 );
		}

		// ALLOW picking up of all vehicles (GTA doesn't allow picking up of 'locked' script created vehicles)
		if(samod2_config["ALLOW picking up of all vehicles (GTA doesn't allow picking up of 'locked' script created vehicles)"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x6A436C, 0x90 );
			MemPut < BYTE > ( 0x6A436D, 0x90 );
		}

		// Players always lean out whatever the camera mode
		// 00621983     EB 13          JMP SHORT hacked_g.00621998
		if(samod2_config["Players always lean out whatever the camera mode"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x621983, 0xEB );
		}

		// Players can fire drivebys whatever camera mode
		// 627E01 - 6 bytes
		if(samod2_config["Players can fire drivebys whatever camera mode"].find("true") != std::string::npos)
		{
			MemSet ( (LPVOID)0x627E01, 0x90, 6 );

			MemSet ( (LPVOID)0x62840D, 0x90, 6 );
		}

		// Satchel crash fix
		// C89110: satchel (bomb) positions pointer?
		// C891A8+4: satchel (model) positions pointer? gets set to NULL on player death, causing an access violation
		// C891A8+12: satchel (model) disappear time (in SystemTime format). 738F99 clears the satchel when VAR_SystemTime is larger.
		if(samod2_config["Satchel crash fix"].find("true") != std::string::npos)
		{
			MemSet ( (LPVOID)0x738F3A, 0x90, 83 );
		}

		// Prevent gta stopping driveby players from falling off
		if(samod2_config["Prevent gta stopping driveby players from falling off"].find("true") != std::string::npos)
		{
			MemSet ( (LPVOID)0x6B5B17, 0x90, 6 );
		}

		// DISABLE PLAYING REPLAYS
		if(samod2_config["DISABLE PLAYING REPLAYS"].find("true") != std::string::npos)
		{
			MemSet ((void *)0x460390, 0xC3, 1);

			MemSet ((void *)0x4600F0, 0xC3, 1);

			MemSet ((void *)0x45F050, 0xC3, 1);
		}

		// DISABLE CHEATS
		if(samod2_config["DISABLE CHEATS"].find("true") != std::string::npos)
		{
			MemSet ((void *)0x439AF0, 0xC3, 1);

			MemSet ((void *)0x438370, 0xC3, 1);
		}

		// DISABLE GARAGES
		if(samod2_config["DISABLE GARAGES"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x44AA89 + 0, 0xE9 );
			MemPut < BYTE > ( 0x44AA89 + 1, 0x28 );
			MemPut < BYTE > ( 0x44AA89 + 2, 0x01 );
			MemPut < BYTE > ( 0x44AA89 + 3, 0x00 );
			MemPut < BYTE > ( 0x44AA89 + 4, 0x00 );
			MemPut < BYTE > ( 0x44AA89 + 5, 0x90 );

			MemPut < DWORD > ( 0x44C7E0, 0x44C7C4 );
			MemPut < DWORD > ( 0x44C7E4, 0x44C7C4 );
			MemPut < DWORD > ( 0x44C7F8, 0x44C7C4 );
			MemPut < DWORD > ( 0x44C7FC, 0x44C7C4 );
			MemPut < DWORD > ( 0x44C804, 0x44C7C4 );
			MemPut < DWORD > ( 0x44C808, 0x44C7C4 );
			MemPut < DWORD > ( 0x44C83C, 0x44C7C4 );
			MemPut < DWORD > ( 0x44C840, 0x44C7C4 );
			MemPut < DWORD > ( 0x44C850, 0x44C7C4 );
			MemPut < DWORD > ( 0x44C854, 0x44C7C4 );
			MemPut < DWORD > ( 0x44C864, 0x44C7C4 );
			MemPut < DWORD > ( 0x44C868, 0x44C7C4 );
			MemPut < DWORD > ( 0x44C874, 0x44C7C4 );
			MemPut < DWORD > ( 0x44C878, 0x44C7C4 );
			MemPut < DWORD > ( 0x44C88C, 0x44C7C4 );
			MemPut < DWORD > ( 0x44C890, 0x44C7C4 );
			MemPut < DWORD > ( 0x44C89C, 0x44C7C4 );
			MemPut < DWORD > ( 0x44C8A0, 0x44C7C4 );
			MemPut < DWORD > ( 0x44C8AC, 0x44C7C4 );
			MemPut < DWORD > ( 0x44C8B0, 0x44C7C4 );

			MemPut < BYTE > ( 0x44C39A + 0, 0x0F );
			MemPut < BYTE > ( 0x44C39A + 1, 0x84 );
			MemPut < BYTE > ( 0x44C39A + 2, 0x24 );
			MemPut < BYTE > ( 0x44C39A + 3, 0x04 );
			MemPut < BYTE > ( 0x44C39A + 4, 0x00 );
			MemPut < BYTE > ( 0x44C39A + 5, 0x00 );
		}

		// Avoid garage doors closing when you change your model
		if(samod2_config["Avoid garage doors closing when you change your model"].find("true") != std::string::npos)
		{
			MemSet ((LPVOID)0x4486F7, 0x90, 4);
		}


		// Disable CStats::IncrementStat (returns at start of function)
		if(samod2_config["Disable CStats::IncrementStat (returns at start of function)"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x55C180, 0xC3 );
		}

		// DISABLE STATS DECREMENTING
		if(samod2_config["DISABLE STATS DECREMENTING"].find("true") != std::string::npos)
		{
			MemSet ((void *)0x559FD5, 0x90, 7 );
			MemSet ((void *)0x559FEB, 0x90, 7 );
		}

		// DISABLE STATS MESSAGES
		if(samod2_config["DISABLE STATS MESSAGES"].find("true") != std::string::npos)
		{
			MemSet ((void *)0x55B980, 0xC3, 1);

			MemSet ((void *)0x559760, 0xC3, 1);
		}

		// PREVENT CJ smoking and drinking like an addict
		//005FBA26   EB 29            JMP SHORT gta_sa.005FBA51
		if(samod2_config["PREVENT CJ smoking and drinking like an addict"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x5FBA26, 0xEB );
		}

		// PREVENT the camera from messing up for drivebys for vehicle drivers
		if(samod2_config["PREVENT the camera from messing up for drivebys for vehicle drivers"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x522423, 0x90 );
			MemPut < BYTE > ( 0x522424, 0x90 );
		}

		if(samod2_config["ALLOW ALT+TABBING WITHOUT PAUSING"].find("true") != std::string::npos)
		{
			LPVOID patchAddress = NULL;
			// ALLOW ALT+TABBING WITHOUT PAUSING
			if ( *(BYTE *)0x748ADD == 0xFF && *(BYTE *)0x748ADE == 0x53 )
				patchAddress = (LPVOID)0x748A8D;
			else
				patchAddress = (LPVOID)0x748ADD;

			MemSet (patchAddress, 0x90, 6);
		}

		// DISABLE SAM SITES
		if(samod2_config["DISABLE SAM SITES"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x5A07D0, 0xC3 );
		}

		// DISABLE TRAINS (AUTO GENERATED ONES)
		if(samod2_config["DISABLE TRAINS (AUTO GENERATED ONES)"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x6F7900, 0xC3 );
		}

		// Prevent TRAINS spawning with PEDs
		if(samod2_config["Prevent TRAINS spawning with PEDs"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x6F7865, 0xEB );
		}

		// DISABLE PLANES
		if(samod2_config["DISABLE PLANES"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x6CD2F0, 0xC3 );
		}

		// DISABLE EMERGENCY VEHICLES
		if(samod2_config["DISABLE EMERGENCY VEHICLES"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x42B7D0, 0xC3 );
		}

		// DISABLE CAR GENERATORS
		if(samod2_config["DISABLE CAR GENERATORS"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x6F3F40, 0xC3 );
		}

		// DISABLE CEntryExitManager::Update (they crash when you enter anyway)
		if(samod2_config["DISABLE CEntryExitManager::Update (they crash when you enter anyway)"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x440D10, 0xC3 );
		}

		// Disable MENU AFTER alt + tab
		//0053BC72   C605 7B67BA00 01 MOV BYTE PTR DS:[BA677B],1   
		if(samod2_config["Disable MENU AFTER alt + tab"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x53BC78, 0x00 );
		}

		// DISABLE HUNGER MESSAGES
		if(samod2_config["DISABLE HUNGER MESSAGES"].find("true") != std::string::npos)
		{
			MemSet ( (LPVOID)0x56E740, 0x90, 5 );
		}

		// DISABLE RANDOM VEHICLE UPGRADES
		if(samod2_config["DISABLE RANDOM VEHICLE UPGRADES"].find("true") != std::string::npos)
		{
			MemSet ( (LPVOID)0x6B0BC2, 0xEB, 1 );
		}

		// make peds always walk around
		if(samod2_config["make peds always walk around"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x53C017, 0x90 );
			MemPut < BYTE > ( 0x53C018, 0x90 );
		}

		// allow boats to be rotated

		if(samod2_config["allow boats to be rotated"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x6F2089, 0x58 );
			MemSet ((void *)0x6F208A,0x90,4);
		}

		// Prevent the game deleting _any_ far away vehicles
		if(samod2_config["Prevent the game deleting _any_ far away vehicles"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x42CD10, 0xC3 );
		}

		// DISABLE real-time shadows for peds
		if(samod2_config["DISABLE real-time shadows for peds"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x5E68A0, 0xEB );
		}

		// and some more just to be safe
		if(samod2_config["and some more just to be safe"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x542483, 0xEB );
		}

		// DISABLE weapon pickups
		if(samod2_config["DISABLE weapon pickups"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x5B47B0, 0xC3 );
		}

		// INCREASE CEntyInfoNode pool size
		if(samod2_config["INCREASE CEntyInfoNode pool size"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x550FBA, 0x00 );
			MemPut < BYTE > ( 0x550FBB, 0x10 );
		}

		//DISABLE CPad::ReconcileTwoControllersInput
		if(samod2_config["DISABLE CPad::ReconcileTwoControllersInput"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x53F530, 0xC2 );
			MemPut < BYTE > ( 0x53F531, 0x0C );
			MemPut < BYTE > ( 0x53F532, 0x00 );

			MemPut < BYTE > ( 0x53EF80, 0xC3 );

			MemPut < BYTE > ( 0x541DDC, 0xEB );
			MemPut < BYTE > ( 0x541DDD, 0x60 );
		}

		// DISABLE CWanted Helis (always return 0 from CWanted::NumOfHelisRequired)
		if(samod2_config["DISABLE CWanted Helis (always return 0 from CWanted::NumOfHelisRequired)"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x561FA4, 0x90 );
			MemPut < BYTE > ( 0x561FA5, 0x90 );
		}

		// DISABLE  CWanted__UpdateEachFrame
		if(samod2_config["DISABLE  CWanted__UpdateEachFrame"].find("true") != std::string::npos)
		{
			MemSet ( (void*)0x53BFF6, 0x90, 5 );
		}

		// DISABLE CWanted__Update
		if(samod2_config["DISABLE CWanted__Update"].find("true") != std::string::npos)
		{
			MemSet ( (void*)0x60EBCC, 0x90, 5 );
		}

		// Disable armour-increase upon entering an enforcer
		if(samod2_config["Disable armour-increase upon entering an enforcer"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x6D189B, 0x06 );
		}

		// Removes the last weapon pickups from interiors as well
		if(samod2_config["Removes the last weapon pickups from interiors as well"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x591F90, 0xC3 );
		}

		// Trains may infact go further than Los Santos
		if(samod2_config["Trains may infact go further than Los Santos"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x4418E0, 0xC3 );
		}

		// Make CTaskComplexSunbathe::CanSunbathe always return true
		if(samod2_config["Make CTaskComplexSunbathe::CanSunbathe always return true"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x632140, 0xB0 );
			MemPut < BYTE > ( 0x632141, 0x01 );
			MemPut < BYTE > ( 0x632142, 0xC3 );
		}

		// Stop CTaskSimpleCarDrive::ProcessPed from exiting passengers with CTaskComplexSequence (some timer check)
		if(samod2_config["Stop CTaskSimpleCarDrive::ProcessPed from exiting passengers with CTaskComplexSequence (some timer check)"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x644C18, 0x90 );
			MemPut < BYTE > ( 0x644C19, 0xE9 );
		}

		// Stop CPlayerPed::ProcessControl from calling CVisibilityPlugins::SetClumpAlpha
		if(samod2_config["Stop CPlayerPed::ProcessControl from calling CVisibilityPlugins::SetClumpAlpha"].find("true") != std::string::npos)
		{
			MemSet ( (void*)0x5E8E84, 0x90, 5 );
		}

		// Stop CVehicle::UpdateClumpAlpha from calling CVisibilityPlugins::SetClumpAlpha
		if(samod2_config["Stop CVehicle::UpdateClumpAlpha from calling CVisibilityPlugins::SetClumpAlpha"].find("true") != std::string::npos)
		{
			MemSet ( (void*)0x6D29CB, 0x90, 5 );
		}

		// Disable CVehicle::DoDriveByShootings
		if(samod2_config["Disable CVehicle::DoDriveByShootings"].find("true") != std::string::npos)
		{
			MemSet ( (void*)0x741FD0, 0x90, 3 );
			MemPut < BYTE > ( 0x741FD0, 0xC3 );
		}

		// Disable CTaskSimplePlayerOnFoot::PlayIdleAnimations (ret 4)
		if(samod2_config["Disable CTaskSimplePlayerOnFoot::PlayIdleAnimations (ret 4)"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x6872C0, 0xC2 );
			MemPut < BYTE > ( 0x6872C1, 0x04 );
			MemPut < BYTE > ( 0x6872C2, 0x00 );
		}

		// Let us sprint everywhere (always return 0 from CSurfaceData::isSprint)
		if(samod2_config["Let us sprint everywhere (always return 0 from CSurfaceData::isSprint)"].find("true") != std::string::npos)
		{
			MemPut < DWORD > ( 0x55E870, 0xC2C03366 );
			MemPut < WORD > ( 0x55E874, 0x0004 );
		}

		// Create pickup objects in interior 0 instead of 13
		if(samod2_config["Create pickup objects in interior 0 instead of 13"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x59FAA3, 0x00 );
		}

		// Don't get shotguns from police cars
		if(samod2_config["Don't get shotguns from police cars"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x6D19CD, 0xEB );
		}

		// Don't get golf clubs from caddies
		if(samod2_config["Don't get golf clubs from caddies"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x6D1A1A, 0xEB );
		}

		// Don't get 20 health from ambulances
		if(samod2_config["Don't get 20 health from ambulances"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x6D1762, 0x00 );
		}

		// Prevent CVehicle::RecalcTrainRailPosition from changing train speed
		if(samod2_config["Prevent CVehicle::RecalcTrainRailPosition from changing train speed"].find("true") != std::string::npos)
		{
			MemSet ((void *)0x6F701D, 0x90, 6);
			MemPut < BYTE > ( 0x6F7069, 0xEB );
		}

		// Fix for sliding over objects and vehicles (ice floor)
		if(samod2_config["Fix for sliding over objects and vehicles (ice floor)"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x5E1E72, 0xE9 );
			MemPut < BYTE > ( 0x5E1E73, 0xB9 );
			MemPut < BYTE > ( 0x5E1E74, 0x00 );
			MemPut < BYTE > ( 0x5E1E77, 0x90 );
		}

		// Disable GTA vehicle detachment at rotation awkwardness
		if(samod2_config["Disable GTA vehicle detachment at rotation awkwardness"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x547441, 0xE9 );
			MemPut < BYTE > ( 0x547442, 0xFA );
			MemPut < BYTE > ( 0x547443, 0x02 );
			MemPut < BYTE > ( 0x547444, 0x00 );
			MemPut < BYTE > ( 0x547445, 0x00 );
		}

		// Disable idle cam
		if(samod2_config["Disable idle cam"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x522C80, 0xC3 );
		}

		// Disable radar map hiding when pressing TAB (action key) while on foot
		if(samod2_config["Disable radar map hiding when pressing TAB (action key) while on foot"].find("true") != std::string::npos)
		{
			MemSet ( (void *)0x58FC3E, 0x90, 14 );
		}

		// Force triggering of the damage event for players on fire
		if(samod2_config["Force triggering of the damage event for players on fire"].find("true") != std::string::npos)
		{
			MemSet ( (void *)0x633695, 0x90, 6 );
			MemPut < BYTE > ( 0x633720, 0 );
		}

		// Make CCreepingFire::TryToStartFireAtCoors return the fire pointer rather than a bool
		if(samod2_config["Make CCreepingFire::TryToStartFireAtCoors return the fire pointer rather than a bool"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x53A459, 0x33 );
			MemPut < BYTE > ( 0x53A568, 0x8B );
			MemPut < BYTE > ( 0x53A4A9, 0x33 );
			MemPut < WORD > ( 0x53A55F, 0x9090 );
			MemPut < BYTE > ( 0x73EC06, 0x85 );
		}

		// Do not fixate camera behind spectated player if local player is dead
		if(samod2_config["Do not fixate camera behind spectated player if local player is dead"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x52A2BB, 0 );
			MemPut < BYTE > ( 0x52A4F8, 0 );
		}

		// Disable setting players on fire when they're riding burning bmx's
		if(samod2_config["Disable setting players on fire when they're riding burning bmx's"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x53A982, 0xEB );
		}

		// Allow all screen aspect ratios
		if(samod2_config["Allow all screen aspect ratios"].find("true") != std::string::npos)
		{
			MemPut < WORD > ( 0x745BC9, 0x9090 );
		}

		// Allow all screen aspect ratios in multi-monitor dialog
		if(samod2_config["Allow all screen aspect ratios in multi-monitor dialog"].find("true") != std::string::npos)
		{
			MemPut < WORD > ( 0x7459E1, 0x9090 );
		}

		// Show the GTA:SA Main menu
		// We are hiding the menu in "void CGameSA::Initialize ( void )".
		if(samod2_config["Show the GTA:SA Main menu"].find("true") != std::string::npos)
		{
			MemPutFast < BYTE > ( (0xBA6748)+0x5C, 1 );
		}

		// Force the MrWhoopee music to load even if we are not the driver.
		if(samod2_config["Force the MrWhoopee music to load even if we are not the driver"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x4F9CCE, 0xCE );
		}

		// Disable re-initialization of DirectInput mouse device by the game
		if(samod2_config["Disable re-initialization of DirectInput mouse device by the game"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x576CCC, 0xEB );
			MemPut < BYTE > ( 0x576EBA, 0xEB );
			MemPut < BYTE > ( 0x576F8A, 0xEB );
		}

		// Make sure DirectInput mouse device is set non-exclusive (may not be needed?)
		if(samod2_config["Make sure DirectInput mouse device is set non-exclusive (may not be needed?)"].find("true") != std::string::npos)
		{
			MemPut < DWORD > ( 0x7469A0, 0x909000B0 );
		}

		// Disable the GTASA main menu.
		if(samod2_config["Disable the GTASA main menu"].find("true") != std::string::npos)
		{
			MemSet ( (void *)0x57BA57, 0x90, 6 );
		}

		// Do not render the loading screen.
		if(samod2_config["Do not render the loading screen"].find("true") != std::string::npos)
		{
			MemSet ( (void *)0x590D7C, 0x90, 5 );
			MemSet ( (void *)0x590DB3, 0x90, 5 );
			MemCpy ( (void *)0x590D9F, "\xC3\x90\x90\x90\x90", 5 );
		}

		// Disable ped to player conversations.
		if(samod2_config["Disable ped to player conversations"].find("true") != std::string::npos)
		{
			MemSet ( (void *)0x53C127, 0x90, 10 );
		}

		// Clip camera also outside the world bounds
		if(samod2_config["Clip camera also outside the world bounds"].find("true") != std::string::npos)
		{
			MemSet ( (void *)0x41AD12, 0x90, 2 );
			MemSet ( (void *)0x41ADA7, 0x90, 2 );
			MemSet ( (void *)0x41ADF3, 0x90, 2 );
		}

		// Allow Player Garages to shut with players inside
		if(samod2_config["Allow Player Garages to shut with players inside"].find("true") != std::string::npos)
		{
			MemSet ( (void *)0x44C6FA, 0x90, 4 );
		}

		// Disable CPopulation::ManagePed
		if(samod2_config["Disable CPopulation::ManagePed"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x611FC0, 0xC3 );
		}

		// Stop CPopulation::Update after ManagePopulation call
		if(samod2_config["Stop CPopulation::Update after ManagePopulation call"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x616698, 0x5E );
			MemPut < BYTE > ( 0x616699, 0xC3 );
		}

		// Disable CReplay::Update
		if(samod2_config["Disable CReplay::Update"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x460500, 0xC3 );
		}

		// Disable CInterestingEvents::ScanForNearbyEntities
		if(samod2_config["Disable CInterestingEvents::ScanForNearbyEntities"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x605A30, 0xC3 );
		}

		// Disable CGangWars::Update
		if(samod2_config["Disable CGangWars::Update"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x446610, 0xC3 );
		}

		// Disable CConversations::Update
		if(samod2_config["Disable CConversations::Update"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x43C590, 0xC3 );
		}

		// Disable CPedToPlayerConversations::Update
		if(samod2_config["Disable CPedToPlayerConversations::Update"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x43B0F0, 0xC3 );
		}

		// Disable CCarCtrl::RemoveCarsIfThePoolGetsFull
		if(samod2_config["Disable CCarCtrl::RemoveCarsIfThePoolGetsFull"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x4322B0, 0xC3 );
		}

		// Disable CStreaming::StreamVehiclesAndPeds_Always
		if(samod2_config["Disable CStreaming::StreamVehiclesAndPeds_Always"].find("true") != std::string::npos)
		{
			MemPut < BYTE > ( 0x40B650, 0xC3 );
		}

		// Double the size of CPlaceable matrix array to fix a crash after CMatrixLinkList::AddToList1
		if(samod2_config["Double the size of CPlaceable matrix array to fix a crash after CMatrixLinkList::AddToList1"].find("true") != std::string::npos)
		{
			MemPut < int > ( 0x54F3A1, 1800 );
		}

		//if(sscanf(ccolor, "%x", &tempcolor) != 1)
		MakeCALL(0x58FBDB, (DWORD)&GameRender);

		LoadSAModConfig();

		samod2_config["Current Working Directory"] = "";
		samod2_config["Current Working Directory"].insert(0,"\\");
		samod2_config["Current Working Directory"].insert(0,boost::filesystem::current_path().string());
		samod2_config["Command Line Parameters"] = commands;

		break;

	case DLL_PROCESS_DETACH:

		if ( g_hOrigDll != NULL )
		{
			FreeLibrary( g_hOrigDll );
			g_hOrigDll = NULL;
			Log( "Exited\n" );
		}

		if ( g_flLogAll != NULL )
		{
			fclose( g_flLogAll );
			g_flLogAll = NULL;
		}

		UnloadScripts();
		UnloadPlugins();

		break;
	}

	return true;
}

void Init3()
{
	InitPluginInternals();
	std::string AMXFILE = (boost::filesystem::current_path() / samod2_config["AMX Scriptfiles Relative Path"] ).string();
	AMXFILE.append("\\");
	SetEnvironmentVariable("AMXFILE",AMXFILE.c_str());
	LoadPlugins();
	LoadScripts();
	StartCounter();
#if defined __DEBUGx
	std::string commands(GetCommandLine());
	if(commands.find("samod_debug") != std::string::npos)
	{
		toggleWindowedMode();
	}
#endif
}

void reloadScripts()
{
	traceLastFunc( "reloadScripts()" );
	if(SA::MOD::Console::var::g_Console)
	{
		GTAfunc_TogglePlayerControllable(false);
		SA::MOD::Console::var::g_Console = false;
	}
	for(std::vector<ScriptInfo>::iterator it = scripts.begin(); it != scripts.end(); ++it) 
	{
		if (!amx_FindPublic(&it->amx, "OnGameLoad", &PublicCall))
		{
			amx_Exec(&it->amx, NULL, PublicCall);
		}
	}
}

int PublicCall = 0;

void PAWN_DXRENDER_STATE(bool init)
{
	if(init)
	{
		for(std::vector<ScriptInfo>::iterator it = scripts.begin(); it != scripts.end(); ++it) 
		{
			if (!amx_FindPublic(&it->amx, "InitDXRenderer", &PublicCall))
			{
				amx_Exec(&it->amx, NULL, PublicCall);
			}
		}
	}
	else
	{
		for(std::vector<ScriptInfo>::iterator it = scripts.begin(); it != scripts.end(); ++it) 
		{
			if (!amx_FindPublic(&it->amx, "UnInitDXRenderer", &PublicCall))
			{
				amx_Exec(&it->amx, NULL, PublicCall);
			}
		}
	}
}

void FrameTick(float TimeDiff)
{
	traceLastFunc( "FrameTick()" );
	while(!reload_scripts_amx.empty())
	{
		for(std::vector<ScriptInfo>::iterator it = scripts.begin(); it != scripts.end(); ++it) 
		{
			if(&it->amx == reload_scripts_amx.front())
			{
				reload_scripts_amx.pop_front();
				it->Reload();
			}
		}
	}
	DoProcessTick();
	if(SA::MOD::Console::ProcessConsole())
		return;
	for(std::vector<ScriptInfo>::iterator it = scripts.begin(); it != scripts.end(); ++it) 
	{
		if (!amx_FindPublic(&it->amx, "DirectXRenderer", &PublicCall))
		{
			amx_Push(&it->amx,amx_ftoc(TimeDiff));
			amx_Exec(&it->amx, NULL, PublicCall);
		}
	}
}

void GangZoneDraw_DoPulse ()
{
	for(std::vector<ScriptInfo>::iterator it = scripts.begin(); it != scripts.end(); ++it) 
	{
		if (!amx_FindPublic(&it->amx, "OnGangZoneDrawHookRun", &PublicCall))
		{
			amx_Exec(&it->amx, NULL, PublicCall);
		}
	}
	for(auto it = GangZone::Zones.begin(); it != GangZone::Zones.end(); ++it) 
	{
		#define RADAR_FLASH_CYCLETIME 1000
		SColor color = it->second.Color;
		if(it->second.Flashing)
		{
			unsigned long ulCurrentTime = (unsigned long)GetCounter();
			if ( it->second.FlashCycleStart == 0 )
			{
				it->second.FlashCycleStart = ulCurrentTime;
			}
			// Time to end the cycle and start a new?
			else if ( ulCurrentTime >= it->second.FlashCycleStart + RADAR_FLASH_CYCLETIME )
			{
				it->second.FlashCycleStart = ulCurrentTime;
			}
			// Calculate the alpha based on the last cycle time and the cycle intervals
			float fAlphaFactor;

			// We're in the fade in part of the cycle?
			if ( ulCurrentTime >= it->second.FlashCycleStart + RADAR_FLASH_CYCLETIME / 2 )
			{
				// Calculate the alpha-factor
				fAlphaFactor = static_cast < float > ( ulCurrentTime - it->second.FlashCycleStart - RADAR_FLASH_CYCLETIME / 2 ) / ( RADAR_FLASH_CYCLETIME / 2 );
			}
			else
			{
				// Calculate the alpha-factor
				fAlphaFactor = 1.0f - static_cast < float > ( ulCurrentTime - it->second.FlashCycleStart ) / ( RADAR_FLASH_CYCLETIME / 2 );
			}

			// Multiply the alpha-factor with the alpha we're supposed to have to find what alpha to use and set it
			color.A = static_cast < unsigned char > ( fAlphaFactor * static_cast < float > ( color.A ) );
		}
		if(it->second.Show)
		{
			pGame->GetRadar ()->DrawAreaOnRadar ( it->second.X1, it->second.Y1, it->second.X2, it->second.Y2, color );
		}
	}
}

void __cdecl GameRender()
{
	traceLastFunc("GameRender()");

	__asm mov eax, 0x58D9A0
	__asm call eax

	for(std::vector<ScriptInfo>::iterator it = scripts.begin(); it != scripts.end(); ++it) 
	{
		if (!amx_FindPublic(&it->amx, "OnGameRender", &PublicCall))
		{
			amx_Exec(&it->amx, NULL, PublicCall);
		}
	}
}

//~500 native functions
//+~300 sa-mp emulator functions
//----
//~800 native functions in total