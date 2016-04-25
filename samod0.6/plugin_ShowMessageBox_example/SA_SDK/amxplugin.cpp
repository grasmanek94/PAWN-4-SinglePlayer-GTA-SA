//
// This provides an interface to call amx library functions
// within samp-server.
//
// To use:
//    Define the extern in your main source file:
//        extern void *pAMXFunctions;
//    And, in your Initialize function, assign:
//        pAMXFunctions = data[PLUGIN_DATA_AMX];
//
//
// WIN32: To regenerate thunks for calls from prototypes in amx.h
// Run a regex with:
//   S: ^(.*)(AMXAPI amx_)([^(]*)([^\;]*);$
//   R: NUDE \1\2\3\4\n{\n\t_asm mov eax, pAMXFunctions;\n\t_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_\3*4];\n}\n
//
// LINUX/BSD: To regenerate thunks for calls from prototypes in amx.h
// Run a regex with:
//   S: ^(.*)(AMXAPI amx_)([^(]*)([^\;]*);$
//   R: typedef \1 AMXAPI (*amx_\3_t)\4;\n\1\2\3\4\n{\n\tamx_\3_t fn = ((amx_\3_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_\3];\n\treturn fn\4;\n}\n
// Modify internal function calls as nessesary
//

//----------------------------------------------------------

#include "plugin.h"

//----------------------------------------------------------

void *pAMXFunctions;

//----------------------------------------------------------

#if (defined(WIN32) || defined(_WIN32)) && defined(_MSC_VER)

// Optimized Inline Assembly Thunks for MS VC++

#define NUDE _declspec(naked) 

NUDE uint16_t * AMXAPI amx_Align16(uint16_t *v)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_Align16*4];
}

NUDE uint32_t * AMXAPI amx_Align32(uint32_t *v)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_Align32*4];
}

#if defined _I64_MAX || defined HAVE_I64
NUDE   uint64_t * AMXAPI amx_Align64(uint64_t *v)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_Align64*4];
}

#endif
NUDE int AMXAPI amx_Allot(AMX *amx, int cells, cell *amx_addr, cell **phys_addr)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_Allot*4];
}

NUDE int AMXAPI amx_Callback(AMX *amx, cell index, cell *result, cell *params)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_Callback*4];
}

NUDE int AMXAPI amx_Cleanup(AMX *amx)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_Cleanup*4];
}

NUDE int AMXAPI amx_Clone(AMX *amxClone, AMX *amxSource, void *data)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_Clone*4];
}

NUDE int AMXAPI amx_Exec(AMX *amx, cell *retval, int index)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_Exec*4];
}

NUDE int AMXAPI amx_FindNative(AMX *amx, const char *name, int *index)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_FindNative*4];
}

NUDE int AMXAPI amx_FindPublic(AMX *amx, const char *funcname, int *index)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_FindPublic*4];
}

NUDE int AMXAPI amx_FindPubVar(AMX *amx, const char *varname, cell *amx_addr)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_FindPubVar*4];
}

NUDE int AMXAPI amx_FindTagId(AMX *amx, cell tag_id, char *tagname)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_FindTagId*4];
}

NUDE int AMXAPI amx_Flags(AMX *amx,uint16_t *flags)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_Flags*4];
}

NUDE int AMXAPI amx_GetAddr(AMX *amx,cell amx_addr,cell **phys_addr)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_GetAddr*4];
}

NUDE int AMXAPI amx_GetNative(AMX *amx, int index, char *funcname)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_GetNative*4];
}

NUDE int AMXAPI amx_GetPublic(AMX *amx, int index, char *funcname)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_GetPublic*4];
}

NUDE int AMXAPI amx_GetPubVar(AMX *amx, int index, char *varname, cell *amx_addr)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_GetPubVar*4];
}

NUDE int AMXAPI amx_GetString(char *dest,const cell *source, int use_wchar, size_t size)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_GetString*4];
}

NUDE int AMXAPI amx_GetTag(AMX *amx, int index, char *tagname, cell *tag_id)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_GetTag*4];
}

NUDE int AMXAPI amx_GetUserData(AMX *amx, long tag, void **ptr)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_GetUserData*4];
}

NUDE int AMXAPI amx_Init(AMX *amx, void *program)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_Init*4];
}

NUDE int AMXAPI amx_InitJIT(AMX *amx, void *reloc_table, void *native_code)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_InitJIT*4];
}

NUDE int AMXAPI amx_MemInfo(AMX *amx, long *codesize, long *datasize, long *stackheap)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_MemInfo*4];
}

NUDE int AMXAPI amx_NameLength(AMX *amx, int *length)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_NameLength*4];
}

NUDE AMX_NATIVE_INFO * AMXAPI amx_NativeInfo(const char *name, AMX_NATIVE func)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_NativeInfo*4];
}

NUDE int AMXAPI amx_NumNatives(AMX *amx, int *number)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_NumNatives*4];
}

NUDE int AMXAPI amx_NumPublics(AMX *amx, int *number)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_NumPublics*4];
}

NUDE int AMXAPI amx_NumPubVars(AMX *amx, int *number)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_NumPubVars*4];
}

NUDE int AMXAPI amx_NumTags(AMX *amx, int *number)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_NumTags*4];
}

NUDE int AMXAPI amx_Push(AMX *amx, cell value)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_Push*4];
}

NUDE int AMXAPI amx_PushArray(AMX *amx, cell *amx_addr, cell **phys_addr, const cell array[], int numcells)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_PushArray*4];
}

NUDE int AMXAPI amx_PushString(AMX *amx, cell *amx_addr, cell **phys_addr, const char *string, int pack, int use_wchar)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_PushString*4];
}

NUDE int AMXAPI amx_RaiseError(AMX *amx, int error)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_RaiseError*4];
}

NUDE int AMXAPI amx_Register(AMX *amx, const AMX_NATIVE_INFO *nativelist, int number)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_Register*4];
}

NUDE int AMXAPI amx_Release(AMX *amx, cell amx_addr)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_Release*4];
}

NUDE int AMXAPI amx_SetCallback(AMX *amx, AMX_CALLBACK callback)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_SetCallback*4];
}

NUDE int AMXAPI amx_SetDebugHook(AMX *amx, AMX_DEBUG debug)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_SetDebugHook*4];
}

NUDE int AMXAPI amx_SetString(cell *dest, const char *source, int pack, int use_wchar, size_t size)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_SetString*4];
}

NUDE int AMXAPI amx_SetUserData(AMX *amx, long tag, void *ptr)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_SetUserData*4];
}

NUDE int AMXAPI amx_StrLen(const cell *cstring, int *length)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_StrLen*4];
}

NUDE int AMXAPI amx_UTF8Check(const char *string, int *length)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_UTF8Check*4];
}

NUDE int AMXAPI amx_UTF8Get(const char *string, const char **endptr, cell *value)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_UTF8Get*4];
}

NUDE int AMXAPI amx_UTF8Len(const cell *cstr, int *length)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_UTF8Len*4];
}

NUDE int AMXAPI amx_UTF8Put(char *string, char **endptr, int maxchars, cell value)
{
	_asm mov eax, pAMXFunctions;
	_asm jmp dword ptr [eax+PLUGIN_AMX_EXPORT_UTF8Put*4];
}
#endif

//----------------------------------------------------------
// EOF
