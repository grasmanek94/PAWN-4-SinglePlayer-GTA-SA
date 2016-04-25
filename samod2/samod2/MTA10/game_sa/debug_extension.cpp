#include "StdInc.h"

class debug: public samod
{
	static const int messages = 20;
	std::string debug_messages[messages];
	void AddDebugMessage(const std::string fmt, ...) 
	{
		int size = 512;
		std::string str;
		va_list ap;
		while (1) {
			str.resize(size);
			va_start(ap, fmt);
			int n = vsnprintf((char *)str.c_str(), size, fmt.c_str(), ap);
			va_end(ap);
			if (n > -1 && n < size) {
				str.resize(n);
				for(char i = 0; i < (messages-1); ++i)
					debug_messages[i] = debug_messages[i+1];
				debug_messages[messages-1] = str;
				return;
			}
			if (n > -1)
				size = n + 1;
			else
				size *= 2;
		}
		for(char i = 0; i < (messages-1); ++i)
			debug_messages[i] = debug_messages[i+1];
		debug_messages[messages-1] = str;
		return;
	}
	int FontID;
public:
	debug()
	{
		AddDebugMessage("Debug Extension Initialized");
	}
	~debug(){}
	void OnPluginLoaded()
	{
		FontID = this->AddFont("Consolas",8,FW_BOLD);
		AddDebugMessage("Plugin Initialized");
	}
	void RenderStuffInit()
	{
		AddDebugMessage("debug::RenderStuffInit()");
	}
	void RenderStuffUnInit()
	{
		AddDebugMessage("debug::RenderStuffUnInit()");
	}
	void OnBeginScene_Before(){}
	void OnBeginScene_After(HRESULT result){}
	void OnEndScene_Before(){}
	void OnEndScene_After(HRESULT result){}
	void OnPresent_Before(CONST RECT* &pSourceRect,CONST RECT* &pDestRect,HWND &hDestWindowOverride,CONST RGNDATA* &pDirtyRegion)
	{
		float currentmessage = 10.0f;
		for(char i = 0; i < messages; ++i)
		{
			this->GetFont(FontID)->Print(10.0,currentmessage,-1,debug_messages[i].c_str());
			currentmessage += 9.0f;
		}
	}
	void OnPresent_After(CONST RECT* &pSourceRect,CONST RECT* &pDestRect,HWND &hDestWindowOverride,CONST RGNDATA* &pDirtyRegion, HRESULT &result){}
	void OnUnload(){}
	void OnRelease_Before(){}
	void OnRelease_After(ULONG count){}
	void OnReset_Before(D3DPRESENT_PARAMETERS* &pPresentationParameters){}
	void OnReset_After(D3DPRESENT_PARAMETERS* &pPresentationParameters, HRESULT result){}
	void DrawRadarGangOverlay(){}
	bool OnBreakTowLink(CVehicle * vehicle)
	{
		AddDebugMessage("debug::OnBreakTowLink(%08x)",vehicle);
		return true;
	}
	void VehicleCollisionHandler(CVehicleSAInterface* pCollidingVehicle, CEntitySAInterface* pCollidedVehicle, int iModelIndex, float fDamageImpulseMag, float fCollidingDamageImpulseMag, uint16 usPieceType, CVector& vecCollisionPos, CVector& vecCollisionVelocity)
	{
		AddDebugMessage("debug::VehicleCollisionHandler(%08x;%08x;%d;%.2f;%.2f;%d;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f;)",pCollidingVehicle,pCollidedVehicle,iModelIndex,fDamageImpulseMag,fDamageImpulseMag,usPieceType,vecCollisionPos.fX,vecCollisionPos.fY,vecCollisionPos.fZ,vecCollisionVelocity.fX,vecCollisionVelocity.fY,vecCollisionVelocity.fZ);
	}
	void OnFireWeapon(CPed* ped, CVector* origin)
	{
		AddDebugMessage("debug::OnFireWeapon(%08x;%.2f;%.2f;%.2f)",ped,origin->fX,origin->fY,origin->fZ);
	}
	void OnPostFireWeapon(CPed* ped)
	{
		AddDebugMessage("debug::OnPostFireWeapon(%08x)",ped);
	}
	void BulletImpactHandler(CPed* pInitiator, CEntity* pVictim, const CVector* pvecStartPosition, const CVector* pvecEndPosition)
	{
		AddDebugMessage("debug::BulletImpactHandler(%08x;%08x;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f)",pInitiator,pVictim,pvecStartPosition->fX,pvecStartPosition->fY,pvecStartPosition->fZ,pvecEndPosition->fX,pvecEndPosition->fY,pvecEndPosition->fZ);
	}
	void OnKeyDown(unsigned char c)
	{
		AddDebugMessage("debug::OnKeyDown(%03d)",c);
	}
	void OnKeyUp(unsigned char c)
	{
		AddDebugMessage("debug::OnKeyUp(%03d)",c);
	}
	void OnMouseUp(unsigned char c)
	{
		AddDebugMessage("debug::OnMouseUp(%d)",c);
	}
	void OnMouseDown(unsigned char c)
	{
		AddDebugMessage("debug::OnMouseDown(%d)",c);
	}
} _debug;