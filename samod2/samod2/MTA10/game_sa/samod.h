#include "StdInc.h"
/*example usage
class MyExtension: public samod
{
public:
	void OnPluginLoaded(){}
	void RenderStuffInit(){}
	void RenderStuffUnInit(){}
	void OnBeginScene_Before(){}
	void OnBeginScene_After(HRESULT result){}
	void OnEndScene_Before(){}
	void OnEndScene_After(HRESULT result){}
	void OnPresent_Before(CONST RECT* &pSourceRect,CONST RECT* &pDestRect,HWND &hDestWindowOverride,CONST RGNDATA* &pDirtyRegion){}
	void OnPresent_After(CONST RECT* &pSourceRect,CONST RECT* &pDestRect,HWND &hDestWindowOverride,CONST RGNDATA* &pDirtyRegion, HRESULT &result){}
	void OnUnload(){}
	void OnRelease_Before(){}
	void OnRelease_After(ULONG count){}
	void OnReset_Before(D3DPRESENT_PARAMETERS* &pPresentationParameters){}
	void OnReset_After(D3DPRESENT_PARAMETERS* &pPresentationParameters, HRESULT result){}
	void DrawRadarGangOverlay(){}
	bool OnBreakTowLink(CVehicle * vehicle){return true;}
	void VehicleCollisionHandler(CVehicleSAInterface* pCollidingVehicle, CEntitySAInterface* pCollidedVehicle, int iModelIndex, float fDamageImpulseMag, float fCollidingDamageImpulseMag, uint16 usPieceType, CVector& vecCollisionPos, CVector& vecCollisionVelocity){}
	void OnFireWeapon(CPed* ped, CVector* origin){}
	void OnPostFireWeapon(CPed* ped){}
	void BulletImpactHandler(CPed* pInitiator, CEntity* pVictim, const CVector* pvecStartPosition, const CVector* pvecEndPosition){}
	void OnKeyDown(unsigned char c){}
	void OnKeyUp(unsigned char c){}
	void OnMouseUp(unsigned char c){}
	void OnMouseDown(unsigned char c){}
} _MyExtension;
Functions available: 
bool GetMouseState(unsigned char c); 
bool GetKeyboardState(unsigned char c);
int AddFont(std::string fontname, int size, DWORD flags);
int RemoveFont(int ID);
CD3DFont* GetFont(int ID);
*/
class samod
{
public:
	samod();
	~samod();

	bool GetMouseState(unsigned char c);
	bool GetKeyboardState(unsigned char c);

	int AddFont(std::string fontname, int size, DWORD flags);
	int RemoveFont(int ID);
	CD3DFont* GetFont(int ID);

	virtual void OnKeyDown(unsigned char c){}
	virtual void OnKeyUp(unsigned char c){}
	virtual void OnMouseUp(unsigned char c){}
	virtual void OnMouseDown(unsigned char c){}

	virtual void RenderStuffInit(){}
	virtual void RenderStuffUnInit(){}

	virtual void OnPluginLoaded(){}

	virtual void OnBeginScene_Before(){}
	virtual void OnBeginScene_After(HRESULT result){}
	virtual void OnEndScene_Before(){}
	virtual void OnEndScene_After(HRESULT result){}
	virtual void OnPresent_Before(CONST RECT* &pSourceRect,CONST RECT* &pDestRect,HWND &hDestWindowOverride,CONST RGNDATA* &pDirtyRegion){}
	virtual void OnPresent_After(CONST RECT* &pSourceRect,CONST RECT* &pDestRect,HWND &hDestWindowOverride,CONST RGNDATA* &pDirtyRegion, HRESULT &result){}
	virtual void OnUnload(){}
	virtual void OnRelease_Before(){}
	virtual void OnRelease_After(ULONG count){}
	virtual void OnReset_Before(D3DPRESENT_PARAMETERS* &pPresentationParameters){}
	virtual void OnReset_After(D3DPRESENT_PARAMETERS* &pPresentationParameters, HRESULT result){}

	virtual void DrawRadarGangOverlay(){}
	virtual bool OnBreakTowLink(CVehicle * vehicle){return true;}
	virtual void VehicleCollisionHandler(CVehicleSAInterface* pCollidingVehicle, CEntitySAInterface* pCollidedVehicle, int iModelIndex, float fDamageImpulseMag, float fCollidingDamageImpulseMag, uint16 usPieceType, CVector& vecCollisionPos, CVector& vecCollisionVelocity){}
	virtual void OnFireWeapon(CPed* ped, CVector* origin){}
	virtual void OnPostFireWeapon(CPed* ped){}
	virtual void BulletImpactHandler(CPed* pInitiator, CEntity* pVictim, const CVector* pvecStartPosition, const CVector* pvecEndPosition){}
};