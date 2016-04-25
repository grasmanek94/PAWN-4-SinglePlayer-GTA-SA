// proxydll.cpp
#include "proxydll.h"

// global variables
#pragma data_seg (".d3d9_shared")
myIDirect3DSwapChain9*  gl_pmyIDirect3DSwapChain9;
myIDirect3DDevice9*		gl_pmyIDirect3DDevice9;
myIDirect3D9*			gl_pmyIDirect3D9;
HINSTANCE				gl_hOriginalDll;
#pragma data_seg ()

void LoadOriginalDll(void);

//HMODULE Injected = NULL;
//bool DoTheInject = false;

#pragma warning(disable: 4996)

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
/*	if (fdwReason == DLL_PROCESS_ATTACH)
	{
#if defined _DEBUG
		while (!IsDebuggerPresent()){}
#endif
		CHAR inject[1024];
		GetEnvironmentVariable("VirtualGameSandBoxEmulatorDLLToInject", inject, 1024);
		if (strlen(inject) > 3)
		{
			Injected = LoadLibrary(inject);
#if defined _DEBUG
			MessageBoxA(NULL, "Proxy Injection success", "Inject Info", 0);
#endif
		}
#if defined _DEBUG
		else
		{
			MessageBoxA(NULL, "Proxy Injection fail", "Inject Info", 0);
		}
#endif
	}
	else if (fdwReason == DLL_PROCESS_DETACH)
	{
		FreeLibrary(Injected);
	}
	*/
	return true;
}

void LoadStuff();
// Exported function (faking d3d9.dll's one-and-only export)
IDirect3D9* WINAPI Direct3DCreate9(UINT SDKVersion)
{
	bool Load = false;
	if (!gl_hOriginalDll)
	{	
		LoadOriginalDll(); // looking for the "right d3d9.dll"
		Load = true;
	}
	
	// Hooking IDirect3D Object from Original Library
	typedef IDirect3D9 *(WINAPI* D3D9_Type)(UINT SDKVersion);
	D3D9_Type D3DCreate9_fn = (D3D9_Type) GetProcAddress( gl_hOriginalDll, "Direct3DCreate9");
    
    // Debug
	if (!D3DCreate9_fn) 
    {
        OutputDebugString("PROXYDLL: Pointer to original D3DCreate9 function not received ERROR ****\r\n");
        ::ExitProcess(0); // exit the hard way
    }
	
	// Request pointer from Original Dll. 
	IDirect3D9 *pIDirect3D9_orig = D3DCreate9_fn(SDKVersion);
	
	// Create my IDirect3D8 object and store pointer to original object there.
	// note: the object will delete itself once Ref count is zero (similar to COM objects)
	gl_pmyIDirect3D9 = new myIDirect3D9(pIDirect3D9_orig);

	LoadStuff();
	// Return pointer to hooking Object instead of "real one"
	return (gl_pmyIDirect3D9);
}

void LoadOriginalDll(void)
{
    char buffer[MAX_PATH];
    
    // Getting path to system dir and to d3d8.dll
	::GetSystemDirectory(buffer,MAX_PATH);

	// Append dll name
	strcat(buffer,"\\d3d9.dll");
	
	// try to load the system's d3d9.dll, if pointer empty
	if (!gl_hOriginalDll) gl_hOriginalDll = ::LoadLibrary(buffer);

	// Debug
	if (!gl_hOriginalDll)
	{
		OutputDebugString("PROXYDLL: Original d3d9.dll not loaded ERROR ****\r\n");
		::ExitProcess(0); // exit the hard way
	}
}

void ExitInstance() 
{    

    OutputDebugString("PROXYDLL: ExitInstance called.\r\n");
	
	// Release the system's d3d9.dll
	if (gl_hOriginalDll)
	{
		::FreeLibrary(gl_hOriginalDll);
	    gl_hOriginalDll = NULL;  
	}
}

//man, i'm hungry... *getting some food*