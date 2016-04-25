#pragma once

#define MAX_SCRIPT_VARS	16	// Size of our variable saving array
#define MAX_SCRIPT_SIZE	255	// Size of ScriptBuf - Max is really (2+(13*5))

struct GAME_SCRIPT_THREAD // 0xE0 bytes total.
{       //
	void* pNext;   // 0x00
	void* pPrev;   // 0x04
	char strName[8];  // 0x08
	DWORD dwBaseIP;   // 0x10
	DWORD dwScriptIP;  // 0x14
	DWORD dwReturnStack[8]; // 0x18
	WORD dwStackPointer; // 0x38
	DWORD dwLocalVar[34]; // 0x3C
	BYTE bStartNewScript; // 0xC4
	BYTE bJumpFlag;   // 0xC5
	BYTE bIsMissionThread;// 0xC6
	BYTE bIsExternalScript;// 0xC7
	BYTE bInMenu;   // 0xC8
	BYTE bUnknown; // 0xC9
	DWORD dwWakeTime;  // 0xCC
	WORD wIfParam;   // 0xD0
	BYTE bNotFlag;   // 0xD2
	BYTE bWastedBustedCheck;// 0xD3
	BYTE bWastedBustedFlag; // 0xD4
	DWORD dwSceneSkipIP;  // 0xD8
	BYTE bMissionThread; // 0xDC
};

struct SCRIPT_COMMAND		//	Params
{							//		i = integer
	WORD OpCode;			//		f = float
	char * Params;		//		v = variable
};

int ScriptCommand(const SCRIPT_COMMAND* ScriptCommand, AMX* amx, cell * params);

struct Vector 
{
	float x;
	float y;
	float z;
	float a;
};

extern void __cdecl ScriptTick();