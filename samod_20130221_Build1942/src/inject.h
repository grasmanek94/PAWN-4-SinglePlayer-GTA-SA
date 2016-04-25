#pragma once
#include <Windows.h>
#include <type_traits>
#include <cstdint>

union memory_pointer
{
	void*	 p;
	uintptr_t a;

	memory_pointer()						{ p = nullptr; }
	memory_pointer(void* x) : p(x)			{}
	memory_pointer(uint32_t x) : a(x)		{}

	operator void*()		{ return p; }
	operator uintptr_t()	{ return a; }

	memory_pointer& operator=(void* x)		{ return p = x, *this; }
	memory_pointer& operator=(uintptr_t x)	{ return a = x, *this; }
};

union memory_pointer_a	// used to hack the compiler, don't use for general purposes
{
	void*	 p;
	uintptr_t a;

	memory_pointer_a()						{ p = nullptr; }
	memory_pointer_a(void* x) : p(x)		{}
	memory_pointer_a(uint32_t x) : a(x)		{}

	template<class T>
	operator T*() { return reinterpret_cast<T*>(p); }
};

#define DeclareFunc(address, func)	func = memory_pointer_a(address)
#define DeclareArray(address, xtype, name) std::decay<xtype>::type name = memory_pointer_a(address);
#define DeclareVar(address, xtype, name) xtype *name = reinterpret_cast<xtype*>(address)
#define DeclareMethod(address, func) func { _asm mov eax, address _asm jmp eax }

struct RwRGBA
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;
};

struct RwRect
{
	float x;
	float y; 
	float w; 
	float h;
};

inline RwRGBA __cdecl RwRGBASet(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	RwRGBA col;
	col.red = r;
	col.green = g;
	col.blue = b;
	col.alpha = a;
	return col;
}

// decclare adresses 
DeclareFunc(0x71A700, void (__cdecl *C2DText__draw)(float, float, char *));
DeclareFunc(0x7195C0, void (__cdecl *C2DText__setBackground)(bool, bool));
DeclareFunc(0x719380, void (__cdecl *C2DText__setSize)(float, float));
DeclareFunc(0x719610, void (__cdecl *C2DText__setAlignment)(BYTE));
DeclareFunc(0x719570, void (__cdecl *C2DText__setShadowSize)(BYTE));
DeclareFunc(0x719590, void (__cdecl *C2DText__setOutlineSize)(BYTE));
DeclareFunc(0x719510, void (__cdecl *C2DText__setShadowRGBA)(RwRGBA));
DeclareFunc(0x719490, void (__cdecl *C2DText__setFont)(BYTE));
DeclareFunc(0x719430, void (__cdecl *C2DText__setRGBA)(RwRGBA));
DeclareFunc(0x719420, void (__cdecl *C2DText__setSlant)(DWORD));
DeclareFunc(0x71A820, void (__cdecl *C2DText__drawWithSlant)(float, float, char *));
DeclareFunc(0x7194E0, void (__cdecl *C2DText__setBackgroundWidth)(float));

DeclareVar(0xC17044, DWORD*, RsGlobalMaximumWidth);
DeclareVar(0xC17048, DWORD*, RsGlobalMaximumHeight);

DeclareFunc(0x727B60, void (__cdecl *DrawRect)(RwRect *rect, RwRGBA *color));
DeclareFunc(0x7170C0, RwRGBA* (__fastcall *RGBA__set)(void *_this, BYTE _unused, unsigned char r, unsigned char g, unsigned char b, unsigned char a));

DeclareFunc(0x43A0B0, void (__cdecl *SpawnCarAtPlayerLocation)(int));