#include <main.h>
#include <amx.h>
#include "scm.h"

#pragma warning(disable:4312)

const FARPROC ProcessOneCommand	= (FARPROC)0x469F00;

DWORD SCRIPT_BASE = 0xA49960;
GAME_SCRIPT_THREAD gst;				
BYTE ScriptBuf[MAX_SCRIPT_SIZE];
DWORD ScriptVars[MAX_SCRIPT_VARS];

int __declspec(naked) ExecuteScriptBuf()
{
	__asm
	{
		mov ecx, OFFSET ScriptBuf
			mov gst.dwScriptIP, ecx 
			mov ecx, OFFSET gst  
			call ProcessOneCommand
			xor eax, eax   
			mov al, gst.bJumpFlag
			ret
	}
}

int ScriptCommand(const SCRIPT_COMMAND* ScriptCommand, AMX* amx, cell * params)
{
	const char* p = ScriptCommand->Params;							
	memcpy(&ScriptBuf, &ScriptCommand->OpCode, 2);
	int buf_pos = 2;	
	int var_pos = 0;
	int currpos = 2;
	while(*p)			
	{
		switch(*p)		
		{
		case 'i':	
			{
				long i = params[currpos++];	
				if ((i >= -128) && (i <= 127))	
				{
					char ch = (char)i;			
					ScriptBuf[buf_pos] = 0x04;	
					buf_pos++;					
					ScriptBuf[buf_pos] = ch;	
					buf_pos++;	
				}
				else if ((i >= -32768) && (i <= 32767))	
				{
					short sh = (short)i;				
					ScriptBuf[buf_pos] = 0x05;			
					buf_pos++;							
					memcpy(&ScriptBuf[buf_pos], &sh, 2);
					buf_pos += 2;						
				}
				else									
				{
					ScriptBuf[buf_pos] = 0x01;			
					buf_pos++;;							
					memcpy(&ScriptBuf[buf_pos], &i, 4);	
					buf_pos += 4;						
				}
				break;
			}
		case 'f':	
			{
				float f = amx_ctof(params[currpos++]);	
				ScriptBuf[buf_pos] = 0x06;			
				buf_pos++;								
				memcpy(&ScriptBuf[buf_pos], &f, 4);		
				buf_pos += 4;							
				break;
			}
		case 'v':
			{
				DWORD* v = (DWORD *)params[currpos++];					
				ScriptBuf[buf_pos] = 0x02;								
				buf_pos++;												
				WORD var_offs = var_pos * 4;							
				DWORD* ScriptVar = (DWORD*)(SCRIPT_BASE + var_offs);	
				ScriptVars[var_pos] = *ScriptVar;						
				*ScriptVar = *v;										
				memcpy(&ScriptBuf[buf_pos], &var_offs, 2);				
				buf_pos += 2;											
				var_pos++;												
				break;
			}
		case 's':	// If string... Updated 13th Jan 06.. (kyeman) SA string support
			{
				char* sz;
				amx_StrParam(amx,params[currpos++],sz);	
				unsigned char aLen = (int)strlen(sz);
				ScriptBuf[buf_pos] = 0x0E;
				buf_pos++;
				ScriptBuf[buf_pos] = aLen;
				buf_pos++;
				memcpy(&ScriptBuf[buf_pos],sz,aLen);				
				buf_pos += aLen;
				break;
			}
		}
		++p;		
	}	

	BYTE WaitBuf[4] = {0x01, 0x00, 0x04, 0x00};
	memcpy((void*)&ScriptBuf[buf_pos], (void*)&WaitBuf[0], 4);

	if (var_pos)	
	{
		int result = ExecuteScriptBuf();	

		p = ScriptCommand->Params;	
		currpos = 2;
		var_pos = 0;	

		while(*p)						
		{
			switch(*p)					
			{
			case 'i':				
				{
					params[currpos++];	
					break;
				}
			case 'f':				
				{
					params[currpos++];
					break;
				}
			case 'v':				
				{
					DWORD* v = (DWORD *)params[currpos++];
					DWORD* ScriptVar = (DWORD*)(SCRIPT_BASE + (var_pos*4));	
					*v = *ScriptVar;										
					*ScriptVar = ScriptVars[var_pos];						
					var_pos++;												
					break;
				}
			case 's':				
				{
					params[currpos++];
					break;
				}
			}
			++p;		
		}	

		return result;	
	} else {
		return ExecuteScriptBuf();
	}
}

void __cdecl ScriptTick()
{
	__asm mov eax, 0x590D00
	__asm call eax
	
	__asm mov gst, ecx
}