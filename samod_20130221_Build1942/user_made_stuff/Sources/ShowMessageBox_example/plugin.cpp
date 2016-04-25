/*
* Default Includes
*/
#include <Windows.h>
/*
* AMX Includes
*/
#include <plugin.h>

extern void * pAMXFunctions;

using namespace std;  

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() 
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load( void **ppData ) 
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload( )
{

}

static cell AMX_NATIVE_CALL ShowDummyMessageBox( AMX* amx, cell* params )
{
	MessageBoxA(NULL,"Dummy message box activated","Dummy MSGBOX",0);
	return 1;
}

AMX_NATIVE_INFO AMXNatives[ ] =
{
	{"ShowDummyMessageBox", ShowDummyMessageBox},
	{0,                0}
};

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad( AMX *amx ) 
{
	return amx_Register( amx, AMXNatives, -1 );
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload( AMX *amx ) 
{
	return AMX_ERR_NONE;
}
