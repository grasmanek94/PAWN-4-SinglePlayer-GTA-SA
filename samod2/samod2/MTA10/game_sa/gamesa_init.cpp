#include "StdInc.h"
#ifdef _DEBUG
#pragma comment(lib,"./Debug/multiplayer.lib")
#else
#pragma comment(lib,"./RELEASE_/multiplayer.lib")
#endif
CGameSA* pGame = NULL;

void					keyhook_maybe_install ( );
void					keyhook_uninstall ( void );

static WNDPROC		orig_wndproc;
static HWND			orig_wnd;

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

namespace internal__samod
{
	void RenderStuffInit();
	void RenderStuffUnInit();
	void OnPluginLoaded();
	void OnBeginScene_Before();
	void OnBeginScene_After(HRESULT result);
	void OnEndScene_Before();
	void OnEndScene_After(HRESULT result);
	void OnPresent_Before(CONST RECT* &pSourceRect,CONST RECT* &pDestRect,HWND &hDestWindowOverride,CONST RGNDATA* &pDirtyRegion);
	void OnPresent_After(CONST RECT* &pSourceRect,CONST RECT* &pDestRect,HWND &hDestWindowOverride,CONST RGNDATA* &pDirtyRegion, HRESULT &result);
	void OnUnload();
	void OnRelease_Before();
	void OnRelease_After(ULONG count);
	void OnReset_Before(D3DPRESENT_PARAMETERS* &pPresentationParameters);
	void OnReset_After(D3DPRESENT_PARAMETERS* &pPresentationParameters, HRESULT result);
	void ResetStrokes();
	void OnMouseDown(unsigned char c);
	void OnMouseUp(unsigned char c);
	void OnKeyDown(unsigned char c);
	void OnKeyUp(unsigned char c);
}

void RenderChangestate(bool init)
{
	if(init)
	{	
		//init all things that need init
		if(FAILED(render->Initialize( gl_pmyIDirect3DDevice9 )))
		{}
		internal__samod::RenderStuffInit();
		STUFF_INITED = true;
		return;
	}
	internal__samod::RenderStuffUnInit();
	//uninitialize all things
	render->Invalidate();
	STUFF_INITED = false;
}

void SDX::OnLoad()
{
	SetUnhandledExceptionFilter( unhandledExceptionFilter );	
	GetGameInterface()->GetGameVersion();
	InitMultiplayerInterface();
	//
	internal__samod::OnPluginLoaded();
}

void SDX::OnBeginScene_Before()
{
	internal__samod::OnBeginScene_Before();
}

void SDX::OnBeginScene_After(HRESULT result)
{
	internal__samod::OnBeginScene_After(result);
}

void SDX::OnEndScene_Before()
{
	internal__samod::OnEndScene_Before();
}

void SDX::OnEndScene_After(HRESULT result)
{
	internal__samod::OnEndScene_After(result);
}

bool ModInited = false;
void InitIfNotInited()
{
	if(!ModInited)
	{
		pGame->Initialize();
		pGame->InitLocalPlayer(  );
		ModInited = true;
	}
}

void SDX::OnPresent_Before(CONST RECT* &pSourceRect,CONST RECT* &pDestRect,HWND &hDestWindowOverride,CONST RGNDATA* &pDirtyRegion)
{
	if(!STUFF_INITED)
		RenderChangestate(true);
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
			keyhook_maybe_install (  );
			if(!SUCCEEDED(render->BeginRender()))
				return;
			internal__samod::OnPresent_Before(pSourceRect,pDestRect,hDestWindowOverride,pDirtyRegion);
			render->EndRender();
		}
	}
}

void SDX::OnPresent_After(CONST RECT* &pSourceRect,CONST RECT* &pDestRect,HWND &hDestWindowOverride,CONST RGNDATA* &pDirtyRegion, HRESULT &result)
{
	internal__samod::OnPresent_After(pSourceRect,pDestRect,hDestWindowOverride,pDirtyRegion,result);
}

//called on application shutdown (not working yet)
void SDX::OnUnload()
{
	if(STUFF_INITED)
		RenderChangestate(false);
}

//called on device reset
void SDX::OnRelease_Before()
{
	//deallocate all srpites, surfaces, etc
	if(STUFF_INITED)
		RenderChangestate(false);
	internal__samod::OnRelease_Before();
}

void SDX::OnRelease_After(ULONG count)
{
	//if count is bigger than 0, allocate again
	if(!STUFF_INITED && count)
		RenderChangestate(true);
	internal__samod::OnRelease_After(count);
}

void SDX::OnReset_Before(D3DPRESENT_PARAMETERS* &pPresentationParameters)
{
	//uninitialize our shit
	if(STUFF_INITED)
		RenderChangestate(false);
	internal__samod::OnReset_Before(pPresentationParameters);
}

void SDX::OnReset_After(D3DPRESENT_PARAMETERS* &pPresentationParameters, HRESULT result)
{
	if(!STUFF_INITED)
		RenderChangestate(true);
	//initialize our shit again
	internal__samod::OnReset_After(pPresentationParameters,result);
}

static LRESULT CALLBACK wnd_proc ( HWND wnd, UINT umsg, WPARAM wparam, LPARAM lparam )
{
	switch ( umsg )
	{
	case WM_LBUTTONDOWN:
		internal__samod::OnMouseDown(0);
		break;
	case WM_LBUTTONUP:
		internal__samod::OnMouseUp(0);
		break;

	case WM_RBUTTONDOWN:
		internal__samod::OnMouseDown(2);
		break;
	case WM_RBUTTONUP:
		internal__samod::OnMouseUp(2);
		break;

	case WM_MBUTTONDOWN:
		internal__samod::OnMouseDown(1);
		break;
	case WM_MBUTTONUP:
		internal__samod::OnMouseUp(1);
		break;

	case WM_KEYDOWN:
		internal__samod::OnKeyDown(wparam);
		break;
	case WM_KEYUP:
		internal__samod::OnKeyUp(wparam);
		break;
	}
	return CallWindowProc( orig_wndproc, wnd, umsg, wparam, lparam );
}

void keyhook_maybe_install (  )
{
	HWND wnd = (HWND)*((DWORD*)0xC97C1C);
	if ( orig_wndproc == NULL || wnd != orig_wnd )
	{
		internal__samod::ResetStrokes();
		keyhook_uninstall();
		orig_wndproc = ( WNDPROC ) ( UINT_PTR ) SetWindowLong( wnd, GWL_WNDPROC, (LONG) (UINT_PTR) wnd_proc );
		orig_wnd = wnd;
	}
}

void keyhook_uninstall ( void )
{
	if ( orig_wnd != NULL )
	{
		SetWindowLong( orig_wnd, GWL_WNDPROC, (LONG) (UINT_PTR) orig_wndproc );
		orig_wnd = NULL;
	}
}
