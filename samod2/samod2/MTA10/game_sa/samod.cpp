#include "StdInc.h"
#include <set>
#include <unordered_map>

namespace DirectXFont
{
	CD3DFont * Access(int ID);
	std::map<std::pair<std::string,std::pair<int,DWORD>>,CD3DFont*> fonts;
	std::unordered_map<unsigned int,std::pair<std::string,std::pair<int,DWORD>>> font_id;
	int FontCounter = -1;
	int Add(std::string fontname, int size, DWORD flags)
	{
		if(fonts.find(std::make_pair(fontname,std::make_pair(size,flags))) == fonts.end())
		{
			for(auto it = font_id.begin(); it != font_id.end(); ++it)
			{
				if(it->second == std::make_pair(fontname,std::make_pair(size,flags)))
				{
					return it->first;//guaranteed to happen
				}
			}
		}
		fonts.emplace(std::make_pair(fontname,std::make_pair(size,flags)),new CD3DFont(fontname.c_str(),size,flags));
		font_id[++FontCounter] = std::make_pair(fontname,std::make_pair(size,flags));
		Access(FontCounter)->Initialize(gl_pmyIDirect3DDevice9);
		return FontCounter;
	}
	bool Initialize(int ID)
	{
		if(fonts.find(font_id[ID]) == fonts.end())
			return false;
		return fonts.at(font_id[ID])->Initialize( gl_pmyIDirect3DDevice9 ) == S_OK;
	}
	bool Remove(int ID)
	{
		if(fonts.find(font_id[ID]) == fonts.end())
			return false;
		fonts.at(font_id[ID])->Invalidate();
		fonts.erase(font_id[ID]);
		return true;
	}
	void InitializeAll()
	{
		for(auto it = fonts.begin(); it != fonts.end(); ++it)
			it->second->Initialize( gl_pmyIDirect3DDevice9 );
	}
	void InvalidateAll()
	{
		for(auto it = fonts.begin(); it != fonts.end(); ++it)
			it->second->Invalidate();
	}
	CD3DFont * Access(int ID)
	{
		if(ID < 0)
			return 0;
		if(fonts.find(font_id[ID]) == fonts.end())
			return 0;
		return fonts.at(font_id[ID]);
	}
};

namespace internal__samod
{
	std::set<samod *> * mods = nullptr;
	void RenderStuffInit()
	{
		DirectXFont::InitializeAll();
		for (auto& mod: (*mods))
			mod->RenderStuffInit();
	}
	void RenderStuffUnInit()
	{
		DirectXFont::InvalidateAll();
		for (auto& mod: (*mods))
			mod->RenderStuffUnInit();
	}
	void OnPluginLoaded()
	{
		for (auto& mod: (*mods))
			mod->OnPluginLoaded();
	}
	void OnBeginScene_Before()
	{
		for (auto& mod: (*mods))
			mod->OnBeginScene_Before();
	}
	void OnBeginScene_After(HRESULT result)
	{
		for (auto& mod: (*mods))
			mod->OnBeginScene_After(result);
	}
	void OnEndScene_Before()
	{
		for (auto& mod: (*mods))
			mod->OnEndScene_Before();
	}
	void OnEndScene_After(HRESULT result)
	{
		for (auto& mod: (*mods))
			mod->OnEndScene_After(result);
	}
	void OnPresent_Before(CONST RECT* &pSourceRect,CONST RECT* &pDestRect,HWND &hDestWindowOverride,CONST RGNDATA* &pDirtyRegion)
	{
		for (auto& mod: (*mods))
			mod->OnPresent_Before(pSourceRect,pDestRect,hDestWindowOverride,pDirtyRegion);
	}
	void OnPresent_After(CONST RECT* &pSourceRect,CONST RECT* &pDestRect,HWND &hDestWindowOverride,CONST RGNDATA* &pDirtyRegion, HRESULT &result)
	{
		for (auto& mod: (*mods))
			mod->OnPresent_After(pSourceRect,pDestRect,hDestWindowOverride,pDirtyRegion,result);
	}
	void OnUnload()
	{
		for (auto& mod: (*mods))
			mod->OnUnload();
	}
	void OnRelease_Before()
	{
		for (auto& mod: (*mods))
			mod->OnRelease_Before();
	}
	void OnRelease_After(ULONG count)
	{
		for (auto& mod: (*mods))
			mod->OnRelease_After(count);
	}
	void OnReset_Before(D3DPRESENT_PARAMETERS* &pPresentationParameters)
	{
		for (auto& mod: (*mods))
			mod->OnReset_Before(pPresentationParameters);
	}
	void OnReset_After(D3DPRESENT_PARAMETERS* &pPresentationParameters, HRESULT result)
	{
		for (auto& mod: (*mods))
			mod->OnReset_After(pPresentationParameters,result);
	}
	void DrawRadarGangOverlay()
	{
		for (auto& mod: (*mods))
			mod->DrawRadarGangOverlay();
	}
	bool OnBreakTowLink(CVehicle * vehicle)
	{
		bool ret = true;
		for (auto& mod: (*mods))
		{
			if(!mod->OnBreakTowLink(vehicle))
				ret = false;
		}
		return ret;
	}
	void VehicleCollisionHandler(CVehicleSAInterface* pCollidingVehicle, CEntitySAInterface* pCollidedVehicle, int iModelIndex, float fDamageImpulseMag, float fCollidingDamageImpulseMag, uint16 usPieceType, CVector &vecCollisionPos, CVector &vecCollisionVelocity)
	{
		for (auto& mod: (*mods))
			mod->VehicleCollisionHandler(pCollidingVehicle,pCollidedVehicle,iModelIndex,fDamageImpulseMag,fCollidingDamageImpulseMag,usPieceType,vecCollisionPos,vecCollisionVelocity);
	}
	void OnFireWeapon(CPed* ped, CVector* origin)
	{
		for (auto& mod: (*mods))
			mod->OnFireWeapon(ped,origin);
	}
	void OnPostFireWeapon(CPed* ped)
	{
		for (auto& mod: (*mods))
			mod->OnPostFireWeapon(ped);
	}
	void BulletImpactHandler(CPed* pInitiator, CEntity* pVictim, const CVector* pvecStartPosition, const CVector* pvecEndPosition)
	{
		for (auto& mod: (*mods))
			mod->BulletImpactHandler(pInitiator,pVictim, pvecStartPosition,pvecEndPosition);
	}
	bool IsKeyPressed[256];
	bool IsMousePressed[3];
	void ResetStrokes()
	{
		for(int i = 0; i < 256; ++i)
			IsKeyPressed[i] = false;
		for(int i = 0; i < 3; ++i)
			IsMousePressed[i] = false;
	}
	void OnMouseDown(unsigned char c)
	{
		if(IsMousePressed[c])
			return;
		IsMousePressed[c] = true;
		for (auto& mod: (*mods))
			mod->OnMouseDown(c);
	}
	void OnMouseUp(unsigned char c)
	{
		if(!IsMousePressed[c])
			return;
		IsMousePressed[c] = false;
		for (auto& mod: (*mods))
			mod->OnMouseUp(c);
	}
	void OnKeyDown(unsigned char c)
	{
		if(IsKeyPressed[c])
			return;
		IsKeyPressed[c] = true;
		for (auto& mod: (*mods))
			mod->OnKeyDown(c);
	}
	void OnKeyUp(unsigned char c)
	{
		if(!IsKeyPressed[c])
			return;
		IsKeyPressed[c] = false;
		for (auto& mod: (*mods))
			mod->OnKeyUp(c);
	}
}

using namespace internal__samod;

samod::samod()
{
	if(mods == nullptr)
		mods = new std::set<samod *>;
	mods->insert(this);
}

samod::~samod()
{
	this->OnUnload();
	mods->erase(this);
}

bool samod::GetMouseState(unsigned char c)
{
	return IsMousePressed[c];
}
bool samod::GetKeyboardState(unsigned char c)
{
	return IsKeyPressed[c];
}

int samod::AddFont(std::string fontname, int size, DWORD flags)
{
	return DirectXFont::Add(fontname,size,flags);
}

int samod::RemoveFont(int ID)
{
	return DirectXFont::Remove(ID);
}

CD3DFont* samod::GetFont(int ID)
{
	return DirectXFont::Access(ID);
}