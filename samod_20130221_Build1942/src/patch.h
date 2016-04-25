#pragma once
#include <Windows.h>
#define patch(a, v, s) _patch((void*)(a), (DWORD)(v), (s))
void _patch(void*,DWORD,DWORD);

inline void MakeJMP(DWORD adress, DWORD asmcode)
{
	patch(adress, 0xE9, 1);
	patch(adress + 1, asmcode - (adress + 5), 4);
}

inline void MakeCALL(DWORD adress, DWORD asmcode)
{
	patch(adress, 0xE8, 1);
	patch(adress + 1, asmcode - (adress + 5), 4);
}

inline void _patch(void* pAddress, DWORD data, DWORD iSize)
{
    unsigned long dwProtect[2];                            
    VirtualProtect(pAddress, iSize, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
    switch(iSize)
    {
            case 1: *(BYTE*)pAddress = (BYTE)data;
                break;
            case 2: *(WORD*)pAddress = (WORD)data;
                break;
            case 4: *(DWORD*)pAddress = (DWORD)data;
                break;
            default: memset(pAddress, data, iSize);
                break;
    }
    VirtualProtect(pAddress, iSize, dwProtect[0], &dwProtect[1]);
}
