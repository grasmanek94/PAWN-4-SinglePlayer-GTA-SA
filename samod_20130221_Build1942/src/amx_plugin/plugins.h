#ifndef _PLUGINS_H_INCLUDED
#define _PLUGINS_H_INCLUDED
#include <proxydll.h>

typedef bool (CALLBACK *ServerPluginLoad_t)(void **data);
typedef void (CALLBACK *ServerPluginUnload_t)();
typedef unsigned int (CALLBACK *ServerPluginSupports_t)();
typedef void (CALLBACK *ServerPluginProcessTick_t)();
typedef void (CALLBACK *ServerPluginKeyHit_t)(int lparam, int wparam);

typedef int (CALLBACK *ServerPluginAmxLoad_t)(AMX *amx);
typedef int (CALLBACK *ServerPluginAmxUnload_t)(AMX *amx);

void* m_PluginData[MAX_PLUGIN_DATA];
void* m_AMXExports[MAX_PLUGIN_AMX_EXPORT];

bool LoadScript(std::string script);
bool UnloadScript(std::string script);

//#define __DEBUGx

#ifdef __DEBUGx
	#define dbg(a) ((Log) a)
#else
	#define dbg(a)
#endif

void InitPluginInternals()
{
	m_AMXExports[PLUGIN_AMX_EXPORT_Allot] = (void*)&amx_Allot;
	m_AMXExports[PLUGIN_AMX_EXPORT_Callback] = (void*)&amx_Callback;
	m_AMXExports[PLUGIN_AMX_EXPORT_Cleanup] = (void*)&amx_Cleanup;
	m_AMXExports[PLUGIN_AMX_EXPORT_Clone] = (void*)&amx_Clone;
	m_AMXExports[PLUGIN_AMX_EXPORT_Exec] = (void*)&amx_Exec;
	m_AMXExports[PLUGIN_AMX_EXPORT_FindNative] = (void*)&amx_FindNative;
	m_AMXExports[PLUGIN_AMX_EXPORT_FindPublic] = (void*)&amx_FindPublic;
	m_AMXExports[PLUGIN_AMX_EXPORT_FindPubVar] = (void*)&amx_FindPubVar;
	m_AMXExports[PLUGIN_AMX_EXPORT_FindTagId] = (void*)&amx_FindTagId;
	m_AMXExports[PLUGIN_AMX_EXPORT_Flags] = (void*)&amx_Flags;
	m_AMXExports[PLUGIN_AMX_EXPORT_GetAddr] = (void*)&amx_GetAddr;
	m_AMXExports[PLUGIN_AMX_EXPORT_GetNative] = (void*)&amx_GetNative;
	m_AMXExports[PLUGIN_AMX_EXPORT_GetPublic] = (void*)&amx_GetPublic;
	m_AMXExports[PLUGIN_AMX_EXPORT_GetPubVar] = (void*)&amx_GetPubVar;
	m_AMXExports[PLUGIN_AMX_EXPORT_GetString] = (void*)&amx_GetString;
	m_AMXExports[PLUGIN_AMX_EXPORT_GetTag] = (void*)&amx_GetTag;
	m_AMXExports[PLUGIN_AMX_EXPORT_GetUserData] = (void*)&amx_GetUserData;
	m_AMXExports[PLUGIN_AMX_EXPORT_Init] = (void*)&amx_Init;
	m_AMXExports[PLUGIN_AMX_EXPORT_InitJIT] = (void*)&amx_InitJIT;
	m_AMXExports[PLUGIN_AMX_EXPORT_MemInfo] = (void*)&amx_MemInfo;
	m_AMXExports[PLUGIN_AMX_EXPORT_NameLength] = (void*)&amx_NameLength;
	m_AMXExports[PLUGIN_AMX_EXPORT_NativeInfo] = (void*)&amx_NativeInfo;
	m_AMXExports[PLUGIN_AMX_EXPORT_NumNatives] = (void*)&amx_NumNatives;
	m_AMXExports[PLUGIN_AMX_EXPORT_NumPublics] = (void*)&amx_NumPublics;
	m_AMXExports[PLUGIN_AMX_EXPORT_NumPubVars] = (void*)&amx_NumPubVars;
	m_AMXExports[PLUGIN_AMX_EXPORT_NumTags] = (void*)&amx_NumTags;
	m_AMXExports[PLUGIN_AMX_EXPORT_Push] = (void*)&amx_Push;
	m_AMXExports[PLUGIN_AMX_EXPORT_PushArray] = (void*)&amx_PushArray;
	m_AMXExports[PLUGIN_AMX_EXPORT_PushString] = (void*)&amx_PushString;
	m_AMXExports[PLUGIN_AMX_EXPORT_RaiseError] = (void*)&amx_RaiseError;
	m_AMXExports[PLUGIN_AMX_EXPORT_Register] = (void*)&amx_Register;
	m_AMXExports[PLUGIN_AMX_EXPORT_Release] = (void*)&amx_Release;
	m_AMXExports[PLUGIN_AMX_EXPORT_SetCallback] = (void*)&amx_SetCallback;
	m_AMXExports[PLUGIN_AMX_EXPORT_SetDebugHook] = (void*)&amx_SetDebugHook;
	m_AMXExports[PLUGIN_AMX_EXPORT_SetString] = (void*)&amx_SetString;
	m_AMXExports[PLUGIN_AMX_EXPORT_SetUserData] = (void*)&amx_SetUserData;
	m_AMXExports[PLUGIN_AMX_EXPORT_StrLen] = (void*)&amx_StrLen;
	m_AMXExports[PLUGIN_AMX_EXPORT_UTF8Check] = (void*)&amx_UTF8Check;
	m_AMXExports[PLUGIN_AMX_EXPORT_UTF8Get] = (void*)&amx_UTF8Get;
	m_AMXExports[PLUGIN_AMX_EXPORT_UTF8Len] = (void*)&amx_UTF8Len;
	m_AMXExports[PLUGIN_AMX_EXPORT_UTF8Put] = (void*)&amx_UTF8Put;

	// Set up table of Plugin exports
	m_PluginData[PLUGIN_DATA_LOGPRINTF] = (void*)&Log;
	
	m_PluginData[PLUGIN_DATA_AMX_EXPORTS] = m_AMXExports;
	//m_PluginData[PLUGIN_DATA_CALLPUBLIC_FS] = (void*)&PluginCallPublicFS;
	//m_PluginData[PLUGIN_DATA_CALLPUBLIC_GM] = (void*)&PluginCallPublicGM;

	// Internals
	//m_PluginData[PLUGIN_DATA_NETGAME] = (void*)&PluginGetNetGame;
	//m_PluginData[PLUGIN_DATA_CONSOLE] = (void*)&PluginGetConsole;
	//m_PluginData[PLUGIN_DATA_RAKSERVER] = (void*)&PluginGetRakServer;
	m_PluginData[PLUGIN_DATA_LOADFSCRIPT] = (void*)&LoadScript;
	m_PluginData[PLUGIN_DATA_UNLOADFSCRIPT] = (void*)&UnloadScript;
}

#include <fstream>
#include <sstream>
//////////////////////////////////////////
struct DLLInfo;

std::vector<DLLInfo> plugins;

struct DLLInfo
{
	HINSTANCE Instance;
	std::string Identifier;

	bool UseDestructor;
	//
	bool IsAMX;

	SUPPORTS_FLAGS dwSupportFlags;
	// Core Plugin Interface
	ServerPluginLoad_t		Load;
	ServerPluginUnload_t	Unload;
	ServerPluginSupports_t	Supports;
	ServerPluginProcessTick_t	ProcessTick;
	ServerPluginKeyHit_t	KeyboardHit;

	// AMX Plugin Interface
	ServerPluginAmxLoad_t	AmxLoad;
	ServerPluginAmxUnload_t AmxUnload;


	DLLInfo(std::string FileToLoad, bool _IsAMX_)
	{
		dbg(("[DBG]Start Constructting DLLInfo(%s,%b)",FileToLoad.c_str(),_IsAMX_));
		this->UseDestructor = false;
		this->IsAMX = _IsAMX_;
		this->Identifier = FileToLoad;
		this->Instance = NULL;
		dbg(("[DBG]End Constructting DLLInfo(%s,%b)",FileToLoad.c_str(),_IsAMX_));
	}
    // 1. copy constructor
    DLLInfo(const DLLInfo& that) : 
		Instance(that.Instance), 
		Identifier(that.Identifier), 
		IsAMX(that.IsAMX), 
		dwSupportFlags(that.dwSupportFlags), 
		Load(that.Load), 
		Unload(that.Unload), 
		Supports(that.Supports), 
		ProcessTick(that.ProcessTick), 
		AmxLoad(that.AmxLoad), 
		AmxUnload(that.AmxUnload),
		KeyboardHit(that.KeyboardHit)
    {
		dbg(("[DBG]DLLInfo(const DLLInfo& that)"));
		this->UseDestructor = true;
    }

    // 2. copy assignment operator
    DLLInfo& operator=(const DLLInfo& that)
    {
		dbg(("[DBG]Start-DLLInfo& operator=(const DLLInfo& that)"));
		if (this == &that)
		{
			return *this;
		}
		dbg(("[DBG]DLLInfo 0"));
        Instance = that.Instance;
		dbg(("[DBG]DLLInfo 1"));
		Identifier.clear();
		Identifier += that.Identifier;
		dbg(("[DBG]DLLInfo 2"));
		IsAMX = that.IsAMX;
		dbg(("[DBG]DLLInfo 3"));
		dwSupportFlags = that.dwSupportFlags;
		dbg(("[DBG]DLLInfo 4"));
		Load = that.Load;
		dbg(("[DBG]DLLInfo 5"));
		Unload = that.Unload;
		dbg(("[DBG]DLLInfo 6"));
		Supports = that.Supports;
		dbg(("[DBG]DLLInfo 7"));
		ProcessTick = that.ProcessTick;
		dbg(("[DBG]DLLInfo 8"));
		AmxLoad = that.AmxLoad;
		dbg(("[DBG]DLLInfo 9"));
		AmxUnload = that.AmxUnload;
		dbg(("[DBG]DLLInfo 10"));
		UseDestructor = that.UseDestructor;
		dbg(("[DBG]DLLInfo 11"));
		KeyboardHit = that.KeyboardHit;
		dbg(("[DBG]End-DLLInfo& operator=(const DLLInfo& that)"));
        return *this;
    }

	bool Init()
	{
		for(std::vector<DLLInfo>::iterator it = plugins.begin(); it != plugins.end(); ++it) 
		{
			if(_stricmp(it->Identifier.c_str(),this->Identifier.c_str()) == 0)
			{
				dbg(("[DBG]Plugin %s already inited, aborting.",this->Identifier.c_str()));
				this->UseDestructor = false;
				return false;
			}
		}
		dbg(("[DBG]DLLInfo.Init()"));
		std::string xxFileToLoad;
		if(this->IsAMX)
		{
			xxFileToLoad.insert(0,this->Identifier);
			xxFileToLoad.insert(0,"\\");
			//"libraries" / "pluginsa" / "plugins"
			xxFileToLoad.insert(0,(boost::filesystem::current_path() / samod2_config["AMX Plugins Relative Path"]).string());
			this->Instance = LoadLibrary(xxFileToLoad.c_str());
		}
		else
		{
			xxFileToLoad.insert(0,this->Identifier);
			xxFileToLoad.insert(0,"\\");
			//"libraries"
			xxFileToLoad.insert(0,(boost::filesystem::current_path() / samod2_config["DLL Libraries Relative Path"] ).string());
			this->Instance = LoadLibrary(xxFileToLoad.c_str());
		}
		dbg(("[DBG]Identifier(%s),Instance(%d)",this->Identifier.c_str(),this->Instance));
		if(this->Instance  == NULL)return false;
		if(this->IsAMX)
		{
			this->Load = (ServerPluginLoad_t)GetProcAddress(this->Instance, "Load");
			this->Unload = (ServerPluginUnload_t)GetProcAddress(this->Instance, "Unload");
			this->Supports = (ServerPluginSupports_t)GetProcAddress(this->Instance, "Supports");
			this->KeyboardHit = (ServerPluginKeyHit_t)GetProcAddress(this->Instance, "KeyboardHit");
			dbg(("[DBG]Load(%x) Unload(%x) Supports(%x) KeyboardHit(%x)",this->Load,this->Unload,this->Supports,this->KeyboardHit));
			if (this->Load == NULL || this->Supports == NULL)
			{
				dbg(("[DBG]Plugin has no load or supports"));
				return false;
			}
			if ((this->dwSupportFlags & SUPPORTS_VERSION_MASK) > SUPPORTS_VERSION) 
			{
				dbg(("[DBG]Plugin version (%d) does not conform to loader version (%d)",this->dwSupportFlags & SUPPORTS_VERSION_MASK,SUPPORTS_FLAGS::SUPPORTS_VERSION));
				return false;
			}

			this->AmxLoad = (ServerPluginAmxLoad_t)GetProcAddress(this->Instance, "AmxLoad");
			this->AmxUnload = (ServerPluginAmxUnload_t)GetProcAddress(this->Instance, "AmxUnload");
			dbg(("[DBG]a)AmxLoad(%x) AmxUnload(%x)",this->AmxLoad,this->AmxUnload));

			this->ProcessTick = (ServerPluginProcessTick_t)GetProcAddress(this->Instance, "ProcessTick");
			dbg(("[DBG]a)ProcessTick(%x)",this->ProcessTick));

			dbg(("[DBG]Waintin for plugin init..."));
			if (!this->Load(m_PluginData)) return false;
			dbg(("[DBG]...Plugin Loaded"));
			return true;
		}
		return true;
	}
	bool DLL_Unload()
	{
		dbg(("[DBG]~DLLInfo"));
		if(this->UseDestructor)
		{
			dbg(("[DBG]~DLLInfo::if(this->UseDestructor) passed"));
			if(this->Unload != NULL && this->IsAMX)
			{
				dbg(("[DBG]~DLLInfo::if(this->UseDestructor)::if(this->Unload != NULL && this->IsAMX) passed"));
				this->Unload();
			}
			FreeLibrary(this->Instance);
			dbg(("[DBG]~DLLInfo::end"));
		}
		dbg(("[DBG]~DLLInfo::end2"));
		return 1;
	}
	// 3. destructor
	~DLLInfo()
	{

	}
};

std::string get_file_contents(const char *filename)
{
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize((unsigned int)in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
	dbg(("[DBG]get_file_contents(%s)->%s",filename,contents.c_str()));
    return(contents);
  }
  return std::string("");
}

bool LoadPlugin(std::string FileName,bool IsAMX)
{
	dbg(("[DBG]LoadPlugin(%s,%d)",FileName.c_str(),IsAMX));
	DLLInfo temporary(FileName,IsAMX);
	Log("Loading plugin '%s' ...",FileName.c_str());
	if(temporary.Init())
	{
		dbg(("[DBG]LoadPlugin(%s,%d)->temporary.Init() true",FileName.c_str(),IsAMX));
		plugins.push_back(temporary);
		Log("Plugin '%s' loaded.",FileName.c_str());
		return true;
	}
	Log("Plugin '%s' load failed.",FileName.c_str());
	return false;
}

bool UnloadPlugin(std::string FileName)
{
	dbg(("[DBG]UnloadPlugin(%s)",FileName.c_str()));
	Log("Unloading plugin '%s' ...",FileName.c_str());
	for(std::vector<DLLInfo>::iterator it = plugins.begin(); it != plugins.end(); ++it) 
	{
		if(lstrcmp(it->Identifier.c_str(),FileName.c_str()) == 0)
		{
			it->DLL_Unload();
			plugins.erase(it);
			dbg(("[DBG]UnloadPlugin(%s)->it unloaded",FileName.c_str()));
			Log("Plugin '%s' unload succesful.",FileName.c_str());
			return true;
		}
	}
	Log("Plugin '%s' unload failed.",FileName.c_str());
	return false;
}

void UnloadPlugins()
{
	dbg(("[DBG]UnloadPlugins()"));
	Log("Unloading all plugins...");
	int count = 0;
	while(!plugins.empty())
	{
		plugins.back().DLL_Unload();
		plugins.pop_back();
		++count;
	}
	Log("All (%d) plugins unloaded.",count);
	dbg(("[DBG]UnloadPlugins()::Done"));
}

void LoadPlugins()
{
	dbg(("[DBG]LoadPlugins()"));
	std::istringstream iss(samod2_config["AMX Libraries To Load"]);
	std::string sub; 
	while (iss >> sub) 
	{
		LoadPlugin(sub,true);
	}
	std::istringstream iss2(samod2_config["DLL Libraries To Load"]);
	while (iss2 >> sub) 
	{
		LoadPlugin(sub,false);
	}
}
#endif 


