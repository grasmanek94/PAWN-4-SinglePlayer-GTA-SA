#include <proxydll.h>
#include <format.h>

extern "C" int amx_CoreInit(AMX* amx);
extern "C" int amx_CoreCleanup(AMX* amx);
extern "C" int amx_FloatInit(AMX* amx);
extern "C" int amx_FloatCleanup(AMX* amx);
extern "C" int amx_StringInit(AMX* amx);
extern "C" int amx_StringCleanup(AMX* amx);
extern "C" int amx_FileInit(AMX* amx);
extern "C" int amx_FileCleanup(AMX* amx);
extern "C" int amx_TimeInit(AMX* amx);
extern "C" int amx_TimeCleanup(AMX* amx);
int amx_SSFEInit(AMX *amx);

void DoAmxLoad(AMX *amx) 
{
	dbg(("[DBG]DoAmxLoad"));
	for(std::vector<DLLInfo>::iterator it = plugins.begin(); it != plugins.end(); ++it) 
	{
		if(it->AmxLoad != NULL)
		{
			dbg(("[DBG]DoAmxLoad doing for plugin: %s",it->Identifier.c_str()));
			it->AmxLoad(amx);
		}
	}
	//nativechecker
	AMX_HEADER *hdr = reinterpret_cast<AMX_HEADER*>(amx->base);
	AMX_FUNCSTUBNT *natives = reinterpret_cast<AMX_FUNCSTUBNT*>(hdr->natives + reinterpret_cast<unsigned int>(amx->base));
	AMX_FUNCSTUBNT *libraries = reinterpret_cast<AMX_FUNCSTUBNT*>(hdr->libraries + reinterpret_cast<unsigned int>(amx->base));
	
	for (AMX_FUNCSTUBNT *n = natives; n < libraries; n++) 
	{
		if (n->address == 0) 
		{
			char *name = reinterpret_cast<char*>(n->nameofs + reinterpret_cast<unsigned int>(hdr));
			Log("   Error: Function not registered: '%s'", name);
		}
	}
	//
	dbg(("[DBG]DoAmxLoad::End"));
}

//---------------------------------------

void DoAmxUnload(AMX *amx)
{
	dbg(("[DBG]DoAmxUnload"));
	for(std::vector<DLLInfo>::iterator it = plugins.begin(); it != plugins.end(); ++it) 
	{
		if(it->AmxUnload != NULL)
		{
			dbg(("[DBG]DoAmxUnload doing for plugin: %s",it->Identifier.c_str()));
			it->AmxUnload(amx);
		}
	}
	dbg(("[DBG]DoAmxUnload::End"));
}
//---------------------------------------
struct ScriptInfo;

std::vector<ScriptInfo> scripts;

int AMXAPI aux_LoadProgram(AMX* amx, const char* filename)
{
	FILE* fp;
	if ((fp = fopen(filename, "rb")) == NULL)
	{
		return AMX_ERR_NOTFOUND;
	}
	AMX_HEADER hdr;
	fread(&hdr, sizeof hdr, 1, fp);
	amx_Align16(&hdr.magic);
	amx_Align32((uint32_t *)&hdr.size);
	amx_Align32((uint32_t *)&hdr.stp);

	if (hdr.magic != AMX_MAGIC)
	{
		fclose(fp);
		return AMX_ERR_FORMAT;
	}

	void* memblock;
	if ((memblock = malloc(hdr.stp)) == NULL)
	{
		fclose(fp);
		return AMX_ERR_MEMORY;
	}

	rewind(fp);
	fread(memblock, 1, (size_t)hdr.size, fp);
	fclose(fp);

	//amx_SetDebugHook(amx, (AMX_DEBUG)amx_Debug);
	memset(amx, 0, sizeof(*amx));
	int result = amx_Init(amx, memblock);
	if (result != AMX_ERR_NONE)
	{
		free(memblock);
		amx->base = NULL;
	}
	return result;
}

int AMXAPI aux_FreeProgram(AMX *amx)
{
	if (amx->base != NULL)
	{
		amx_Cleanup(amx);
		free(amx->base);
		memset(amx, 0, sizeof(AMX));
	}
	return AMX_ERR_NONE;
}
//----------------------------------------------------------------------------------

char * AMXAPI aux_StrError(int errnum)
{
	static char *messages[] = {
		/* AMX_ERR_NONE      */ "(none)",
		/* AMX_ERR_EXIT      */ "Forced exit",
		/* AMX_ERR_ASSERT    */ "Assertion failed",
		/* AMX_ERR_STACKERR  */ "Stack/heap collision (insufficient stack size)",
		/* AMX_ERR_BOUNDS    */ "Array index out of bounds",
		/* AMX_ERR_MEMACCESS */ "Invalid memory access",
		/* AMX_ERR_INVINSTR  */ "Invalid instruction",
		/* AMX_ERR_STACKLOW  */ "Stack underflow",
		/* AMX_ERR_HEAPLOW   */ "Heap underflow",
		/* AMX_ERR_CALLBACK  */ "No (valid) native function callback",
		/* AMX_ERR_NATIVE    */ "Native function failed",
		/* AMX_ERR_DIVIDE    */ "Divide by zero",
		/* AMX_ERR_SLEEP     */ "(sleep mode)",
		/* 13 */                "(reserved)",
		/* 14 */                "(reserved)",
		/* 15 */                "(reserved)",
		/* AMX_ERR_MEMORY    */ "Out of memory",
		/* AMX_ERR_FORMAT    */ "Invalid/unsupported P-code file format",
		/* AMX_ERR_VERSION   */ "File is for a newer version of the AMX",
		/* AMX_ERR_NOTFOUND  */ "File or function is not found",
		/* AMX_ERR_INDEX     */ "Invalid index parameter (bad entry point)",
		/* AMX_ERR_DEBUG     */ "Debugger cannot run",
		/* AMX_ERR_INIT      */ "AMX not initialized (or doubly initialized)",
		/* AMX_ERR_USERDATA  */ "Unable to set user data field (table full)",
		/* AMX_ERR_INIT_JIT  */ "Cannot initialize the JIT",
		/* AMX_ERR_PARAMS    */ "Parameter error",
		};
	if (errnum < 0 || errnum >= sizeof messages / sizeof messages[0])
		return "(unknown)";
	return messages[errnum];
}

//----------------------------------------------------------------------------------

cell* get_amxaddr(AMX *amx,cell amx_addr)
{
  return (cell *)(amx->base + (int)(((AMX_HEADER *)amx->base)->dat + amx_addr));
}

//----------------------------------------------------------------------------------

int set_amxstring(AMX *amx,cell amx_addr,const char *source,int max)
{
  cell* dest = (cell *)(amx->base + (int)(((AMX_HEADER *)amx->base)->dat + amx_addr));
  cell* start = dest;
  while (max--&&*source)
    *dest++=(cell)*source++;
  *dest = 0;
  return dest-start;
}

//----------------------------------------------------------------------------------

char* format_amxstring(AMX *amx, cell *params, int parm, int &len)
{
	static char outbuf[4096];
	cell *addr = get_amxaddr(amx, params[parm++]);
	len = atcprintf(outbuf, sizeof(outbuf)-1, addr, amx, params, &parm);
	return outbuf;
}
//----------------------------------------------------------------------------------

int amx_CustomInit(AMX *amx)
{
  return amx_Register(amx, custom_Natives, -1);
}

extern "C" int amx_sampDbInit(AMX *amx);
extern "C" int amx_sampDbCleanup(AMX *amx);

extern int PublicCall;

struct ScriptInfo
{
	AMX amx;
	std::string Identifier;
	std::string FileNameAndPath;
	bool Loaded;
	ScriptInfo(std::string FileToLoad)
	{
		dbg(("[DBG]ScriptInfo(%s)",FileToLoad.c_str()));
		this->FileNameAndPath = ((boost::filesystem::current_path() / "libraries" / "pluginsa" / "scripts" / FileToLoad).string());
		this->Identifier.append(FileToLoad);
		dbg(("[DBG]ScriptInfo(%s)->FNAP(%s)",FileToLoad.c_str(),this->FileNameAndPath.c_str()));
		this->Loaded = false;
	}

    // 1. copy constructor
    ScriptInfo(const ScriptInfo& that) : 
		amx(that.amx), 
		Identifier(that.Identifier), 
		FileNameAndPath(that.FileNameAndPath)
    {
		dbg(("[DBG]ScriptInfo(const ScriptInfo& that) %s - %s",Identifier.c_str(),that.Identifier.c_str()));
		this->Loaded = false;
    }

	bool Init()
	{
		//dbg(("[DBG]ScriptInfo(const ScriptInfo& that) %s - %s",Identifier.c_str(),that.Identifier.c_str()));
		for(std::vector<ScriptInfo>::iterator it = scripts.begin(); it != scripts.end(); ++it) 
		{
			dbg(("[DBG]ScriptInfo::script |%s| - |%s|",it->Identifier.c_str(),this->Identifier.c_str()));
			if(_stricmp(it->Identifier.c_str(),this->Identifier.c_str()) == 0 && it->Loaded == true)
			{
				dbg(("[DBG]ScriptInfo::script already loaded |%s| - |%s|",it->Identifier.c_str(),this->Identifier.c_str()));
				return false;
			}
		}
		memset((void*)&this->amx, 0, sizeof(AMX));
		int err = aux_LoadProgram(&amx,FileNameAndPath.c_str());

		if (err != AMX_ERR_NONE)
		{
			dbg(("[DBG]ScriptInfo::ERRORa"));
			Log("Script[%s]: Run time error %d: \"%s\"",// on line %ld\n",
				this->Identifier.c_str(), err, aux_StrError(err));//, (long)amx->curline);
			return false;
		}
		dbg(("[DBG]ScriptInfo::Init2"));
		amx_CoreInit(&this->amx);
		amx_FloatInit(&this->amx);
		amx_StringInit(&this->amx);
		amx_FileInit(&this->amx);
		amx_TimeInit(&this->amx);
		amx_CustomInit(&this->amx);
		amx_SSFEInit(&this->amx);
		amx_sampDbInit(&this->amx);
		DoAmxLoad(&this->amx);
		dbg(("[DBG]ScriptInfo::Init3"));
		cell ret = 0;
		dbg(("[DBG]ScriptInfo::Init3a"));
		if (!amx_FindPublic(&this->amx, "OnScriptInit", &PublicCall))
		{
			dbg(("[DBG]ScriptInfo::Init3b"));
			try
			{
				amx_Exec(&this->amx, (cell*)&ret, PublicCall);
			}
			#ifdef __DEBUGx
			catch( char * str )
			{
				dbg(("[DBG]ScriptInfo::AMX EXEC Failed: %s",str));
			}
			#else
			catch( ... )
			{

			}
			#endif
		}
		dbg(("[DBG]ScriptInfo::Init5"));
		err = amx_Exec(&this->amx, &ret, AMX_EXEC_MAIN);
		dbg(("[DBG]ScriptInfo::PublicsExecuted"));
		if (err != AMX_ERR_NONE)
		{
			dbg(("[DBG]ScriptInfo::ERRORb"));
			Log("Script[%s]: Run time error %d: \"%s\"",// on line %ld\n",
				this->Identifier.c_str(), err, aux_StrError(err));//, (long)amx->curline);
			dbg(("[DBG]ScriptInfo::ERRORENDb"));
			return false;
		}
		dbg(("[DBG]ScriptInfo::Init()::End()"));
		this->Loaded = true;
		return true;
	}
	// 3. destructor
	~ScriptInfo()
	{

	}
	bool Unload()
	{
		dbg(("[DBG]ScriptInfo::~ScriptInfo (%s)",this->Identifier.c_str()));
		if(this->Loaded)
		{
			dbg(("[DBG]ScriptInfo::~ScriptInfo->UseDestructor"));
			dbg(("[DBG]ScriptInfo::~ScriptInfo->Doing amx_FindPublic..."));
			if (!amx_FindPublic(&this->amx, "OnScriptExit", &PublicCall))
			{
				amx_Exec(&this->amx, NULL, PublicCall);
			}
			dbg(("[DBG]ScriptInfo::~ScriptInfo->Free"));
			aux_FreeProgram(&this->amx);
			DoAmxUnload(&this->amx);
			amx_TimeCleanup(&this->amx);
			amx_FileCleanup(&this->amx);
			amx_StringCleanup(&this->amx);
			amx_FloatCleanup(&this->amx);
			amx_CoreCleanup(&this->amx);
			amx_sampDbCleanup(&this->amx);
			this->Loaded=false;
			dbg(("[DBG]ScriptInfo::~ScriptInfo->Free->Done"));
		}
		dbg(("[DBG]ScriptInfo::~ScriptInfo->End"));
		return true;
	}
	bool Reload()
	{
		__try
		{
			dbg(("[DBG]ScriptInfo::Reload->UseDestructor"));
			dbg(("[DBG]ScriptInfo::Reload->Doing amx_FindPublic..."));
			if (!amx_FindPublic(&this->amx, "OnScriptExit", &PublicCall))
			{
				amx_Exec(&this->amx, NULL, PublicCall);
			}
			dbg(("[DBG]ScriptInfo::Reload->Free"));
			aux_FreeProgram(&this->amx);
			DoAmxUnload(&this->amx);
			amx_TimeCleanup(&this->amx);
			amx_FileCleanup(&this->amx);
			amx_StringCleanup(&this->amx);
			amx_FloatCleanup(&this->amx);
			amx_CoreCleanup(&this->amx);
			amx_sampDbCleanup(&this->amx);
			dbg(("[DBG]ScriptInfo::Reload->Free->Done"));
			this->Loaded=false;
			dbg(("[DBG]ScriptInfo::Reload()"));
			memset((void*)&this->amx, 0, sizeof(AMX));
			int err = aux_LoadProgram(&amx,FileNameAndPath.c_str());
			if (err != AMX_ERR_NONE)
			{
				dbg(("[DBG]ScriptInfo::Reload->ERRORa"));
				Log("Script[%s]: Run time error %d: \"%s\"",// on line %ld\n",
					this->Identifier.c_str(), err, aux_StrError(err));//, (long)amx->curline);
				return false;
			}
			dbg(("[DBG]ScriptInfo::Reload->Init2"));
			amx_CoreInit(&this->amx);
			amx_FloatInit(&this->amx);
			amx_StringInit(&this->amx);
			amx_FileInit(&this->amx);
			amx_TimeInit(&this->amx);
			amx_CustomInit(&this->amx);
			amx_SSFEInit(&this->amx);
			amx_sampDbInit(&this->amx);
			DoAmxLoad(&this->amx);
			dbg(("[DBG]ScriptInfo::Reload->Init3"));
			cell ret = 0;
			dbg(("[DBG]ScriptInfo::Reload->Init3a"));
			if (!amx_FindPublic(&this->amx, "OnScriptInit", &PublicCall))
			{
				dbg(("[DBG]ScriptInfo::Reload->Init3b"));
				amx_Exec(&this->amx, (cell*)&ret, PublicCall);
			}
			dbg(("[DBG]ScriptInfo::Reload->Init5"));
			err = amx_Exec(&this->amx, &ret, AMX_EXEC_MAIN);
			dbg(("[DBG]ScriptInfo::Reload->PublicsExecuted"));
			if (err != AMX_ERR_NONE)
			{
				dbg(("[DBG]ScriptInfo::Reload->ERRORb"));
				Log("Script[%s]: Run time error %d: \"%s\"",// on line %ld\n",
					this->Identifier.c_str(), err, aux_StrError(err));//, (long)amx->curline);
				dbg(("[DBG]ScriptInfo::Reload->ERRORENDb"));
				return false;
			}
			dbg(("[DBG]ScriptInfo::Init()::Reload->End()"));
			this->Loaded = true;
			return true;
		}
		#ifdef __DEBUGx
		__except( EXCEPTION_EXECUTE_HANDLER )
		{
			dbg(("[DBG]ScriptInfo::RELOAD FAILED: %d",GetLastError()));
			return false;
		}
		#else
		__except( EXCEPTION_EXECUTE_HANDLER )
		{
			return false;
		}
		#endif
	}
};

//---------------------------------------

void DoProcessTick()
{
	for(std::vector<DLLInfo>::iterator it = plugins.begin(); it != plugins.end(); ++it) 
	{
		if(it->ProcessTick != NULL)
		{
			it->ProcessTick();
		}
	}
}

bool ReLoadScript(std::string script)
{
	dbg(("[DBG]ReLoadScript(%s)",script.c_str()));
	for(std::vector<ScriptInfo>::iterator it = scripts.begin(); it != scripts.end(); ++it) 
	{
		if(lstrcmp(it->Identifier.c_str(),script.c_str()) == 0)
		{
			it->Reload();
			dbg(("[DBG]ReLoadScript(%s)->it.Reload",script.c_str()));
			return true;
		}
	}
	dbg(("[DBG]ReLoadScript(%s)->it.NOT.Reload",script.c_str()));	
	return false;
}

bool LoadScript(std::string script)
{
	dbg(("[DBG]LoadScript(%s)",script.c_str()));
	scripts.push_back(ScriptInfo(script));
	dbg(("[DBG]LoadScript 2"));
	if(scripts.back().Init() == false)
	{
		dbg(("[DBG]LoadScript 3"));
		scripts.pop_back();
		return false;
	}
	dbg(("[DBG]LoadScript 4"));
	return true;
}

bool UnloadScript(std::string script)
{
	dbg(("[DBG]UnloadScript(%s)",script.c_str()));
	for(std::vector<ScriptInfo>::iterator it = scripts.begin(); it != scripts.end(); ++it) 
	{
		if(lstrcmp(it->Identifier.c_str(),script.c_str()) == 0)
		{
			it->Unload();
			scripts.erase(it);
			dbg(("[DBG]UnloadScript(%s)->it.erased",script.c_str()));
			return true;
		}
	}
	dbg(("[DBG]UnloadScript(%s)->it.NOT.erased",script.c_str()));
	return false;
}

void UnloadScripts()
{
	dbg(("[DBG]UnloadScripts"));
	while(!scripts.empty())
	{
		scripts.back().Unload();
		scripts.pop_back();
	}
	dbg(("[DBG]UnloadScripts->End"));
}

void LoadScripts()
{
	dbg(("[DBG]LoadScripts()"));
	std::stringstream stream;
	stream << (boost::filesystem::current_path() / "libraries" / "pluginsa" / "scripts" / "amx_scripts.ini").string().c_str();
	std::istringstream iss(get_file_contents(stream.str().c_str()));
	dbg(("[DBG]get_file_contents(%s)->%s",stream.str().c_str(),iss.str().c_str()));
	std::string sub; 
	while (iss >> sub) 
	{
		LoadScript(sub);
	}
}
//