#include "../game_sa/StdInc.h"
#pragma once

struct SRwResourceStats
{
    uint uiTextures;
    uint uiRasters;
    uint uiGeometries;
};

struct SClothesCacheStats
{
    uint uiCacheHit;
    uint uiCacheMiss;
    uint uiNumTotal;
    uint uiNumUnused;
    uint uiNumRemoved;
};

typedef unsigned long AssocGroupId;
typedef unsigned long AnimationId;

typedef bool ( ExplosionHandler ) ( class CEntity* pExplodingEntity, class CEntity* pCreator, const CVector& vecPosition, enum eExplosionType ExplosionType );
typedef void ( PreContextSwitchHandler ) ( class CPlayerPed* pPlayer );
typedef void ( PostContextSwitchHandler ) ( void );
typedef bool ( PreWeaponFireHandler ) ( class CPlayerPed* pPlayer, bool bStopIfUsingBulletSync );
typedef void ( PostWeaponFireHandler ) ( void );
typedef void ( BulletImpactHandler ) ( class CPed* pInitiator, class CEntity* pVictim, const CVector* pvecStartPosition, const CVector* pvecEndPosition );
typedef void ( BulletFireHandler ) ( class CPed* pInitiator, const CVector* pvecStartPosition, const CVector* pvecEndPosition );
typedef bool ( DamageHandler ) ( class CPed* pDamagePed, class CEventDamage * pEvent );
typedef void ( FireHandler ) ( class CFire* pFire );
typedef bool ( ProjectileStopHandler ) ( class CEntity * owner, enum eWeaponType weaponType, class CVector * origin, float fForce, class CVector * target, class CEntity * targetEntity );
typedef void ( ProjectileHandler ) ( class CEntity * owner, class CProjectile* projectile, class CProjectileInfo* projectileInfo, enum eWeaponType weaponType, class CVector * origin, float fForce, class CVector * target, class CEntity * targetEntity );
typedef bool ( BreakTowLinkHandler ) ( class CVehicle * towingVehicle );
typedef bool ( ProcessCamHandler ) ( class CCam* pCam );
typedef void ( DrawRadarAreasHandler ) ( void );
typedef void ( Render3DStuffHandler ) ( void );
typedef bool ( ChokingHandler ) ( unsigned char ucWeaponType );
typedef void ( PreWorldProcessHandler ) ( void );
typedef void ( PostWorldProcessHandler ) ( void );
typedef void ( IdleHandler ) ( void );
typedef void ( PreFxRenderHandler ) ( void );
typedef void ( PreHudRenderHandler ) ( void );
typedef void ( AddAnimationHandler ) ( RpClump * pClump, AssocGroupId animGroup, AnimationId animID );
typedef void ( BlendAnimationHandler ) ( RpClump * pClump, AssocGroupId animGroup, AnimationId animID, float fBlendDelta );
typedef bool ( ProcessCollisionHandler ) ( class CEntitySAInterface* pThisInterface, class CEntitySAInterface* pOtherInterface );
typedef bool ( VehicleCollisionHandler ) ( class CVehicleSAInterface* pCollidingVehicle, class CEntitySAInterface* pCollidedVehicle, int iModelIndex, float fDamageImpulseMag, float fCollidingDamageImpulseMag, uint16 usPieceType, CVector vecCollisionPos, CVector vecCollisionVelocity );
typedef bool ( HeliKillHandler ) ( class CVehicleSAInterface* pVehicle, class CPedSAInterface* pPed );
typedef bool ( WaterCannonHitHandler ) ( class CVehicleSAInterface* pCannonVehicle, class CPedSAInterface* pHitPed );
typedef void ( GameObjectDestructHandler ) ( CEntitySAInterface* pObject );
typedef void ( GameVehicleDestructHandler ) ( CEntitySAInterface* pVehicle );
typedef void ( GamePlayerDestructHandler ) ( CEntitySAInterface* pPlayer );
typedef void ( GameModelRemoveHandler ) ( ushort usModelId );
typedef void ( GameEntityRenderHandler ) ( CEntitySAInterface* pEntity );

/**
 * This class contains information used for shot syncing, one exists per player.
 */
class CShotSyncData
{
public:
    CVector m_vecShotTarget;
    CVector m_vecShotOrigin;
    // so we can make the arm move vertically (mainly while on foot) and horizontally (mainly while in vehicles)
    float m_fArmDirectionX; 
    float m_fArmDirectionY;
    // only for in-vehicle shooting
    char m_cInVehicleAimDirection; // 0 = forwards, 1 = left, 2 = back, 3 = right
    // use origin
    bool m_bUseOrigin;

    bool m_bRemoteBulletSyncVectorsValid;
    CVector m_vecRemoteBulletSyncStart;
    CVector m_vecRemoteBulletSyncEnd;
};

class CStatsData
{
public:
    float       StatTypesFloat[MAX_FLOAT_STATS];
    int         StatTypesInt[MAX_INT_STATS];
    float       StatReactionValue[MAX_REACTION_STATS];
};

class CRemoteDataStorage
{
public:
    virtual class CControllerState *    CurrentControllerState ( void ) = 0;
    virtual class CControllerState *    LastControllerState ( void ) = 0;
    virtual class CShotSyncData *       ShotSyncData ( void ) = 0;
    virtual class CStatsData *          Stats ( void ) = 0;
    virtual float                       GetCameraRotation ( void ) = 0;
    virtual void                        SetCameraRotation ( float fCameraRotation ) = 0;
    virtual void                        SetGravity ( float fGravity ) = 0;
    virtual void                        SetProcessPlayerWeapon ( bool bProcess ) = 0;
    virtual CVector&                    GetAkimboTarget     ( void ) = 0;
    virtual bool                        GetAkimboTargetUp   ( void ) = 0;
    virtual void                        SetAkimboTarget     ( const CVector& vecTarget ) = 0;
    virtual void                        SetAkimboTargetUp   ( bool bUp ) = 0;
};

class CRemoteDataStorageSA : public CRemoteDataStorage
{
public:
    inline CRemoteDataStorageSA () 
    { 
        memset ( &m_pad, 0, sizeof ( CPadSAInterface ) );
        m_fCameraRotation = 0.0f;
        memset ( &m_stats, 0, sizeof ( CStatsData ) );
        m_fGravity = 0.008f;
        m_bAkimboTargetUp = false;
        m_bProcessPlayerWeapon = false;
        m_shotSyncData.m_bRemoteBulletSyncVectorsValid = false;
    }

    inline CControllerState *   CurrentControllerState () { return &m_pad.NewState; }
    inline CControllerState *   LastControllerState () { return &m_pad.OldState; }
    inline CShotSyncData *      ShotSyncData        () { return &m_shotSyncData; }
    inline CStatsData *         Stats               () { return &m_stats; }
    inline void                 SetCameraRotation   ( float fCameraRotation ) { m_fCameraRotation = fCameraRotation; }
    inline float                GetCameraRotation   () { return m_fCameraRotation; }
    inline void                 SetGravity          ( float fGravity ) { m_fGravity = fGravity; }
    inline bool                 ProcessPlayerWeapon () { return m_bProcessPlayerWeapon; }
    inline void                 SetProcessPlayerWeapon ( bool bProcess )    { m_bProcessPlayerWeapon = bProcess; }

    CVector&                    GetAkimboTarget     () { return m_vecAkimboTarget; }
    bool                        GetAkimboTargetUp   () { return m_bAkimboTargetUp; }

    void                        SetAkimboTarget     ( const CVector& vecTarget ) { m_vecAkimboTarget = vecTarget; }
    void                        SetAkimboTargetUp   ( bool bUp ) { m_bAkimboTargetUp = bUp; }

    // The player's pad
    CPadSAInterface             m_pad;  
    float                       m_fCameraRotation;
    CShotSyncData               m_shotSyncData;
    CVector                     m_vecAkimboTarget;
    bool                        m_bAkimboTargetUp;
    CStatsData                  m_stats;
    float                       m_fGravity;
    bool                        m_bProcessPlayerWeapon;
};


class CRemoteDataSA
{
public:
    static void                     Init                    ();

    // Static accessors
    static CRemoteDataStorageSA *   GetRemoteDataStorage    ( CPlayerPed* pPed );
    static CRemoteDataStorageSA *   GetRemoteDataStorage    ( CPedSAInterface* pPed );
    static void                     AddRemoteDataStorage    ( CPlayerPed* pPed, CRemoteDataStorage* pData );
    static void                     RemoveRemoteDataStorage ( CPlayerPed* pPed );

private:
    static CPools*                  m_pPools;
    static std::map < CPlayerPed*, CRemoteDataStorageSA* > m_RemoteData;
};

class CLimits
{
public:
    virtual         ~CLimits            () {}

    virtual int     GetIPLFiles         () const = 0;
    virtual bool    SetIPLFiles         ( int ) = 0;

    virtual int     GetCollisionFiles   () const = 0;
    virtual bool    SetCollisionFiles   ( int ) = 0;

    virtual int     GetQuadtreeNodes    () const = 0;
    virtual bool    SetQuadtreeNodes    ( int ) = 0;

    virtual int     GetVehicleStructs   () const = 0;
    virtual bool    SetVehicleStructs   ( int ) = 0;

    virtual int     GetPolygons         () const = 0;
    virtual bool    SetPolygons         ( int ) = 0;

    virtual int     GetStreamingMemory  () const = 0;
    virtual bool    SetStreamingMemory  ( int ) = 0;

    virtual int     GetStreamingVehicles () const = 0;
    virtual bool    SetStreamingVehicles ( int ) = 0;
};


class CLimitsSA : public CLimits
{
public:
                ~CLimitsSA () {}

    int         GetIPLFiles         () const;
    bool        SetIPLFiles         ( int );

    int         GetCollisionFiles   () const;
    bool        SetCollisionFiles   ( int );

    int         GetQuadtreeNodes    () const;
    bool        SetQuadtreeNodes    ( int );

    int         GetVehicleStructs   () const;
    bool        SetVehicleStructs   ( int );

    int         GetPolygons         () const;
    bool        SetPolygons         ( int );

    int         GetStreamingMemory  () const;
    bool        SetStreamingMemory  ( int );

    int         GetStreamingVehicles () const;
    bool        SetStreamingVehicles ( int );
};

class CMultiplayer
{
public:
    enum EFastClothesLoading
    {
        FAST_CLOTHES_OFF = 0,
        FAST_CLOTHES_AUTO = 1,
        FAST_CLOTHES_ON = 2,
    };

    virtual CRemoteDataStorage *        CreateRemoteDataStorage     () = 0;
    virtual void                        DestroyRemoteDataStorage    ( CRemoteDataStorage* pData ) = 0;
    virtual void                        AddRemoteDataStorage        ( CPlayerPed* pPed, CRemoteDataStorage* pData ) = 0;
    virtual CRemoteDataStorage*         GetRemoteDataStorage        ( CPlayerPed* pPed ) = 0;
    virtual void                        RemoveRemoteDataStorage     ( CPlayerPed* pPed ) = 0;

    virtual class CPed *                GetContextSwitchedPed       ( void ) = 0;

    virtual class CPopulationMP         * GetPopulationMP           () = 0;
    virtual void                        PreventLeavingVehicles      () = 0;
    virtual void                        HideRadar                   ( bool bHide ) = 0;
    virtual void                        SetCenterOfWorld            ( class CEntity * entity, class CVector * vecPosition, FLOAT fHeading ) = 0;
    virtual void                        DisablePadHandler           ( bool bDisabled ) = 0;
    virtual void                        DisableAllVehicleWeapons    ( bool bDisable ) = 0;
    virtual void                        DisableBirds                ( bool bDisabled ) = 0;
    virtual void                        DisableQuickReload          ( bool bDisable ) = 0;
    virtual void                        DisableCloseRangeDamage     ( bool bDisable ) = 0;

    virtual bool                        GetExplosionsDisabled       () = 0;
    virtual void                        DisableExplosions           ( bool bDisabled ) = 0;
    virtual void                        SetExplosionHandler         ( ExplosionHandler * pExplosionHandler ) = 0;
    virtual void                        SetBreakTowLinkHandler      ( BreakTowLinkHandler * pBreakTowLinkHandler ) = 0;
    virtual void                        SetDamageHandler            ( DamageHandler * pDamageHandler ) = 0;
    virtual void                        SetFireHandler              ( FireHandler * pFireHandler ) = 0;
    virtual void                        SetProcessCamHandler        ( ProcessCamHandler* pProcessCamHandler ) = 0;
    virtual void                        SetChokingHandler           ( ChokingHandler* pChokingHandler ) = 0;
    virtual void                        SetProjectileHandler        ( ProjectileHandler * pProjectileHandler ) = 0;
    virtual void                        SetProjectileStopHandler    ( ProjectileStopHandler * pProjectileHandler ) = 0;
    virtual void                        SetPreWorldProcessHandler   ( PreWorldProcessHandler * pHandler ) = 0;
    virtual void                        SetPostWorldProcessHandler  ( PostWorldProcessHandler * pHandler ) = 0;
    virtual void                        SetIdleHandler              ( IdleHandler * pHandler ) = 0;
    virtual void                        SetPreFxRenderHandler       ( PreFxRenderHandler * pHandler ) = 0;
    virtual void                        SetPreHudRenderHandler      ( PreHudRenderHandler * pHandler ) = 0;
    virtual void                        SetAddAnimationHandler      ( AddAnimationHandler * pHandler ) = 0;
    virtual void                        SetBlendAnimationHandler    ( BlendAnimationHandler * pHandler ) = 0;
    virtual void                        SetProcessCollisionHandler  ( ProcessCollisionHandler * pHandler ) = 0;
    virtual void                        SetVehicleCollisionHandler  ( VehicleCollisionHandler * pHandler ) = 0;
    virtual void                        SetHeliKillHandler          ( HeliKillHandler * pHandler ) = 0;
    virtual void                        SetWaterCannonHitHandler    ( WaterCannonHitHandler * pHandler ) = 0;
    virtual void                        SetGameObjectDestructHandler    ( GameObjectDestructHandler * pHandler ) = 0;
    virtual void                        SetGameVehicleDestructHandler   ( GameVehicleDestructHandler * pHandler ) = 0;
    virtual void                        SetGamePlayerDestructHandler    ( GamePlayerDestructHandler * pHandler ) = 0;
    virtual void                        SetGameModelRemoveHandler       ( GameModelRemoveHandler * pHandler ) = 0;
    virtual void                        SetGameEntityRenderHandler      ( GameEntityRenderHandler * pHandler ) = 0;

    virtual void                        AllowMouseMovement          ( bool bAllow ) = 0;
    virtual void                        DoSoundHacksOnLostFocus     ( bool bLostFocus ) = 0;
    virtual bool                        HasSkyColor                 () = 0;
    virtual void                        GetSkyColor                 ( unsigned char& TopRed, unsigned char& TopGreen, unsigned char& TopBlue, unsigned char& BottomRed, unsigned char& BottomGreen, unsigned char& BottomBlue ) = 0;
    virtual void                        SetSkyColor                 ( unsigned char TopRed, unsigned char TopGreen, unsigned char TopBlue, unsigned char BottomRed, unsigned char BottomGreen, unsigned char BottomBlue ) = 0;
    virtual void                        ResetSky                    () = 0;
    virtual void                        SetHeatHaze                 ( const SHeatHazeSettings& settings ) = 0;
    virtual void                        GetHeatHaze                 ( SHeatHazeSettings& settings ) = 0;
    virtual void                        ResetHeatHaze               ( void ) = 0;
    virtual void                        SetHeatHazeEnabled          ( bool bEnabled ) = 0;
    virtual bool                        HasWaterColor               () = 0;
    virtual void                        GetWaterColor               ( float& fWaterRed, float& fWaterGreen, float& fWaterBlue, float& fWaterAlpha ) = 0;
    virtual void                        SetWaterColor               ( float fWaterRed, float fWaterGreen, float fWaterBlue, float fWaterAlpha ) = 0;
    virtual void                        ResetWater                  ( void ) = 0;
    virtual void                        SetCloudsEnabled            ( bool bDisabled ) = 0;
    virtual bool                        GetInteriorSoundsEnabled    () = 0;
    virtual void                        SetInteriorSoundsEnabled    ( bool bEnabled ) = 0;
    virtual void                        SetWindVelocity             ( float fX, float fY, float fZ ) = 0;
    virtual void                        GetWindVelocity             ( float& fX, float& fY, float& fZ ) = 0;
    virtual void                        RestoreWindVelocity         ( void ) = 0;
    virtual void                        SetFarClipDistance          ( float fDistance ) = 0;
    virtual float                       GetFarClipDistance          ( void ) = 0;
    virtual void                        RestoreFarClipDistance      ( void ) = 0;
    virtual void                        SetFogDistance              ( float fDistance ) = 0;
    virtual float                       GetFogDistance              ( void ) = 0;
    virtual void                        RestoreFogDistance          ( void ) = 0;
    virtual void                        GetSunColor                 ( unsigned char& ucCoreRed, unsigned char& ucCoreGreen, unsigned char& ucCoreBlue, unsigned char& ucCoronaRed, unsigned char& ucCoronaGreen, unsigned char& ucCoronaBlue ) = 0;
    virtual void                        SetSunColor                 ( unsigned char ucCoreRed, unsigned char ucCoreGreen, unsigned char ucCoreBlue, unsigned char ucCoronaRed, unsigned char ucCoronaGreen, unsigned char ucCoronaBlue ) = 0;
    virtual void                        ResetSunColor               ( ) = 0;
    virtual float                       GetSunSize                  ( ) = 0;
    virtual void                        SetSunSize                  ( float fSize ) = 0;
    virtual void                        ResetSunSize                ( ) = 0;

    virtual void                        DisableEnterExitVehicleKey  ( bool bDisabled ) = 0;

    virtual void                        SetNightVisionEnabled       ( bool bEnabled ) = 0;
    virtual void                        SetThermalVisionEnabled     ( bool bEnabled ) = 0;
    virtual bool                        IsNightVisionEnabled        ( ) = 0;
    virtual bool                        IsThermalVisionEnabled      ( ) = 0;

    virtual void                        RebuildMultiplayerPlayer    ( class CPed * player ) = 0;

    virtual void                        AllowWindowsCursorShowing   ( bool bAllow ) = 0;

    virtual class CShotSyncData *       GetLocalShotSyncData        () = 0;

    virtual void                        SetPreContextSwitchHandler  ( PreContextSwitchHandler* pHandler ) = 0;
    virtual void                        SetPostContextSwitchHandler ( PostContextSwitchHandler* pHandler ) = 0;
    virtual void                        SetPreWeaponFireHandler     ( PreWeaponFireHandler* pHandler ) = 0;
    virtual void                        SetPostWeaponFireHandler    ( PostWeaponFireHandler* pHandler ) = 0;
    virtual void                        SetBulletImpactHandler      ( BulletImpactHandler* pHandler ) = 0;
    virtual void                        SetBulletFireHandler        ( BulletFireHandler* pHandler ) = 0;
    virtual void                        SetDrawRadarAreasHandler    ( DrawRadarAreasHandler * pRadarAreasHandler ) = 0;
    virtual void                        SetRender3DStuffHandler     ( Render3DStuffHandler * pHandler ) = 0;

    virtual void                        Reset                       () = 0;

    virtual void                        ConvertEulerAnglesToMatrix  ( CMatrix& Matrix, float fX, float fY, float fZ ) = 0;
    virtual void                        ConvertMatrixToEulerAngles  ( const CMatrix& Matrix, float& fX, float& fY, float& fZ ) = 0;

    virtual void                        ConvertEulerAnglesToMatrix  ( CMatrix& Matrix, CVector & vecAngles ) = 0;
    virtual void                        ConvertMatrixToEulerAngles  ( const CMatrix& Matrix, CVector & vecAngles ) = 0;

    virtual float                       GetGlobalGravity            () = 0;
    virtual void                        SetGlobalGravity            ( float fGravity ) = 0;

    virtual float                       GetLocalPlayerGravity       () = 0;
    virtual void                        SetLocalPlayerGravity       ( float fGravity ) = 0;

    virtual unsigned char               GetTrafficLightState        () = 0;
    virtual void                        SetTrafficLightState        ( unsigned char ucState ) = 0;

    virtual bool                        GetTrafficLightsLocked      () = 0;
    virtual void                        SetTrafficLightsLocked      ( bool bLocked ) = 0;

    virtual void                        SetLocalStatValue           ( unsigned short usStat, float fValue ) = 0;
    virtual void                        SetLocalStatsStatic         ( bool bStatic ) = 0;

    virtual void                        SetLocalCameraRotation      ( float fRotation ) = 0;
    virtual bool                        IsCustomCameraRotationEnabled () = 0;
    virtual void                        SetCustomCameraRotationEnabled ( bool bEnabled ) = 0;

    virtual void                        SetDebugVars                ( float f1, float f2, float f3 ) = 0;

    virtual CVector&                    GetAkimboTarget             () = 0;
    virtual bool                        GetAkimboTargetUp           () = 0;

    virtual void                        SetAkimboTarget             ( const CVector& vecTarget ) = 0;
    virtual void                        SetAkimboTargetUp           ( bool bUp ) = 0;

    virtual void                        AllowCreatedObjectsInVerticalLineTest ( bool bOn ) = 0;
    virtual void                        DeleteAndDisableGangTags    () = 0;

    virtual CLimits*                    GetLimits                   () = 0;

    virtual bool                        IsSuspensionEnabled         ( ) = 0;
    virtual void                        SetSuspensionEnabled        ( bool bEnabled ) = 0;

    virtual void                        SetFastClothesLoading       ( EFastClothesLoading fastClothesLoading ) = 0;
    virtual void                        SetLODSystemEnabled         ( bool bEnable ) = 0;
    virtual void                        SetAltWaterOrderEnabled     ( bool bEnable ) = 0;

    virtual float                       GetAircraftMaxHeight        ( void ) = 0;
    virtual void                        SetAircraftMaxHeight        ( float fHeight ) = 0;

    virtual void                        SetAutomaticVehicleStartupOnPedEnter    ( bool bSet ) = 0;

    virtual void                        SetPedTargetingMarkerEnabled ( bool bEnabled ) = 0;
    virtual bool                        IsPedTargetingMarkerEnabled ( void ) = 0;

    virtual void                        GetRwResourceStats          ( SRwResourceStats& outStats ) = 0;
    virtual void                        GetClothesCacheStats        ( SClothesCacheStats& outStats ) = 0;
    virtual void                        SetIsMinimizedAndNotConnected ( bool bIsMinimizedAndNotConnected ) = 0;
};

class CMultiplayerSA : public CMultiplayer
{
    friend class COffsetsMP;

private:
    CRemoteDataSA               * RemoteData;
    CPopulationSA               * Population;
public:
    ZERO_ON_NEW

    CMultiplayerSA();
    void                        InitHooks();
    void                        InitHooks_CrashFixHacks();
    void                        Init_13 ( void );
    void                        InitHooks_13 ( void );
    void                        InitMemoryCopies_13 ( void );
    void                        InitHooks_ClothesSpeedUp ( void );
    void                        InitHooks_FixBadAnimId ( void );
    void                        InitHooks_HookDestructors ( void );
    void                        InitHooks_RwResources ( void );
    void                        InitHooks_ClothesCache ( void );
    void                        InitHooks_Files ( void );
    void                        InitHooks_Weapons ( void );
    void                        InitHooks_Rendering ( void );
    CRemoteDataStorage *        CreateRemoteDataStorage     ();
    void                        DestroyRemoteDataStorage    ( CRemoteDataStorage* pData );
    void                        AddRemoteDataStorage        ( CPlayerPed* pPed, CRemoteDataStorage* pData );
    CRemoteDataStorage*         GetRemoteDataStorage        ( CPlayerPed* pPed );
    void                        RemoveRemoteDataStorage     ( CPlayerPed* pPed );
    void                        EnableHooks_ClothesMemFix   ( bool bEnable );

    CPed *                      GetContextSwitchedPed       ( void );

    CPopulationMP               * GetPopulationMP           () { return (CPopulationMP*)Population; }
    void                        PreventLeavingVehicles      ();
    void                        HideRadar                   ( bool bHide );
    void                        SetCenterOfWorld            ( CEntity * entity, CVector * vecPosition, FLOAT fHeading );
    void                        DisablePadHandler           ( bool bDisabled );
    void                        DisableEnterExitVehicleKey  ( bool bDisabled );
    void                        DisableAllVehicleWeapons    ( bool bDisable );
    void                        DisableBirds                ( bool bDisabled );
    void                        DisableQuickReload          ( bool bDisable );
    void                        DisableCloseRangeDamage     ( bool bDisable );

    bool                        GetExplosionsDisabled       ();
    void                        DisableExplosions           ( bool bDisabled );
    void                        SetExplosionHandler         ( ExplosionHandler * pExplosionHandler );
    void                        SetDamageHandler            ( DamageHandler * pDamageHandler );
    void                        SetProjectileHandler        ( ProjectileHandler * pProjectileHandler );
    void                        SetProjectileStopHandler    ( ProjectileStopHandler * pProjectileHandler );
    void                        SetFireHandler              ( FireHandler * pFireHandler );
    void                        SetBreakTowLinkHandler      ( BreakTowLinkHandler * pBreakTowLinkHandler );
    void                        SetProcessCamHandler        ( ProcessCamHandler* pProcessCamHandler );
    void                        SetChokingHandler           ( ChokingHandler* pChokingHandler );
    void                        SetPreWorldProcessHandler   ( PreWorldProcessHandler * pHandler );
    void                        SetPostWorldProcessHandler  ( PostWorldProcessHandler * pHandler );
    void                        SetIdleHandler              ( IdleHandler * pHandler );
    void                        SetPreFxRenderHandler       ( PreFxRenderHandler * pHandler );
    void                        SetPreHudRenderHandler      ( PreHudRenderHandler * pHandler );
    void                        SetAddAnimationHandler      ( AddAnimationHandler * pHandler );
    void                        SetBlendAnimationHandler    ( BlendAnimationHandler * pHandler );
    void                        SetProcessCollisionHandler  ( ProcessCollisionHandler * pHandler );
    void                        SetVehicleCollisionHandler  ( VehicleCollisionHandler * pHandler );
    void                        SetHeliKillHandler          ( HeliKillHandler * pHandler );
    void                        SetWaterCannonHitHandler    ( WaterCannonHitHandler * pHandler );
    void                        SetGameObjectDestructHandler    ( GameObjectDestructHandler * pHandler );
    void                        SetGameVehicleDestructHandler   ( GameVehicleDestructHandler * pHandler );
    void                        SetGamePlayerDestructHandler    ( GamePlayerDestructHandler * pHandler );
    void                        SetGameModelRemoveHandler       ( GameModelRemoveHandler * pHandler );
    void                        SetGameEntityRenderHandler  ( GameEntityRenderHandler * pHandler );

    void                        AllowMouseMovement          ( bool bAllow );
    void                        DoSoundHacksOnLostFocus     ( bool bLostFocus );
    bool                        HasSkyColor                 ();
    void                        GetSkyColor                 ( unsigned char& TopRed, unsigned char& TopGreen, unsigned char& TopBlue, unsigned char& BottomRed, unsigned char& BottomGreen, unsigned char& BottomBlue );
    void                        SetSkyColor                 ( unsigned char TopRed, unsigned char TopGreen, unsigned char TopBlue, unsigned char BottomRed, unsigned char BottomGreen, unsigned char BottomBlue );
    void                        SetHeatHaze                 ( const SHeatHazeSettings& settings );
    void                        GetHeatHaze                 ( SHeatHazeSettings& settings );
    void                        ResetHeatHaze               ( void );
    void                        SetHeatHazeEnabled          ( bool bEnabled );
    void                        ApplyHeatHazeEnabled        ( void );
    void                        ResetSky                    ();
    bool                        HasWaterColor               ();
    void                        GetWaterColor               ( float& fWaterRed, float& fWaterGreen, float& fWaterBlue, float& fWaterAlpha );
    void                        SetWaterColor               ( float fWaterRed, float fWaterGreen, float fWaterBlue, float fWaterAlpha );
    void                        ResetWater                  ();
    void                        SetCloudsEnabled            ( bool bDisabled );
    void                        RebuildMultiplayerPlayer    ( CPed * player );
    bool                        GetInteriorSoundsEnabled    ();
    void                        SetInteriorSoundsEnabled    ( bool bEnabled );
    void                        SetWindVelocity             ( float fX, float fY, float fZ );
    void                        GetWindVelocity             ( float& fX, float& fY, float& fZ );
    void                        RestoreWindVelocity         ( void );
    float                       GetFarClipDistance          ( void );
    void                        SetFarClipDistance          ( float fDistance );
    void                        RestoreFarClipDistance      ( void );
    float                       GetFogDistance              ( void );
    void                        SetFogDistance              ( float fDistance );
    void                        RestoreFogDistance          ( void );
    void                        GetSunColor                 ( unsigned char& ucCoreRed, unsigned char& ucCoreGreen, unsigned char& ucCoreBlue, unsigned char& ucCoronaRed, unsigned char& ucCoronaGreen, unsigned char& ucCoronaBlue );
    void                        SetSunColor                 ( unsigned char ucCoreRed, unsigned char ucCoreGreen, unsigned char ucCoreBlue, unsigned char ucCoronaRed, unsigned char ucCoronaGreen, unsigned char ucCoronaBlue );
    void                        ResetSunColor               ( );
    float                       GetSunSize                  ( );
    void                        SetSunSize                  ( float fSize );
    void                        ResetSunSize                ( );

    void                        SetNightVisionEnabled       ( bool bEnabled );
    void                        SetThermalVisionEnabled     ( bool bEnabled );
    bool                        IsNightVisionEnabled        ( );
    bool                        IsThermalVisionEnabled      ( );

    void                        AllowWindowsCursorShowing   ( bool bAllow );

    CShotSyncData *             GetLocalShotSyncData        ();

    void                        SetPreContextSwitchHandler  ( PreContextSwitchHandler* pHandler );
    void                        SetPostContextSwitchHandler ( PostContextSwitchHandler* pHandler );
    void                        SetPreWeaponFireHandler     ( PreWeaponFireHandler* pHandler );
    void                        SetPostWeaponFireHandler    ( PostWeaponFireHandler* pHandler );
    void                        SetBulletImpactHandler      ( BulletImpactHandler* pHandler );
    void                        SetBulletFireHandler        ( BulletFireHandler* pHandler );
    void                        SetDrawRadarAreasHandler    ( DrawRadarAreasHandler * pRadarAreasHandler );
    void                        SetRender3DStuffHandler     ( Render3DStuffHandler * pHandler );

    void                        Reset                       ();

    void                        ConvertEulerAnglesToMatrix  ( CMatrix& Matrix, float fX, float fY, float fZ );
    void                        ConvertMatrixToEulerAngles  ( const CMatrix& Matrix, float& fX, float& fY, float& fZ );
    
    inline void                 ConvertEulerAnglesToMatrix  ( CMatrix& Matrix, CVector & vecAngles )            { ConvertEulerAnglesToMatrix ( Matrix, vecAngles.fX, vecAngles.fY, vecAngles.fZ ); }
    inline void                 ConvertMatrixToEulerAngles  ( const CMatrix& Matrix, CVector & vecAngles )      { ConvertMatrixToEulerAngles ( Matrix, vecAngles.fX, vecAngles.fY, vecAngles.fZ ); }

    float                       GetGlobalGravity            ();
    void                        SetGlobalGravity            ( float fGravity );

    float                       GetLocalPlayerGravity       ();
    void                        SetLocalPlayerGravity       ( float fGravity );

    unsigned char               GetTrafficLightState        ();
    void                        SetTrafficLightState        ( unsigned char ucState );

    bool                        GetTrafficLightsLocked      ();
    void                        SetTrafficLightsLocked      ( bool bLocked );

    void                        SetLocalStatValue           ( unsigned short usStat, float fValue );
    void                        SetLocalStatsStatic         ( bool bStatic );

    void                        SetLocalCameraRotation      ( float fRotation );
    bool                        IsCustomCameraRotationEnabled   ();
    void                        SetCustomCameraRotationEnabled  ( bool bEnabled );

    void                        SetDebugVars                ( float f1, float f2, float f3 );

    CVector&                    GetAkimboTarget             () { return m_vecAkimboTarget; };
    bool                        GetAkimboTargetUp           () { return m_bAkimboTargetUp; };

    void                        SetAkimboTarget             ( const CVector& vecTarget ) { m_vecAkimboTarget = vecTarget; };
    void                        SetAkimboTargetUp           ( bool bUp ) { m_bAkimboTargetUp = bUp; };

    void                        AllowCreatedObjectsInVerticalLineTest ( bool bOn );
    void                        DeleteAndDisableGangTags    ();
    
    CLimits*                    GetLimits () { return &m_limits; }

    void                        SetSuspensionEnabled        ( bool bEnabled );
    bool                        IsSuspensionEnabled         ( void )                    { return m_bSuspensionEnabled; };

    virtual void                SetFastClothesLoading       ( EFastClothesLoading fastClothesLoading );
    virtual void                SetLODSystemEnabled         ( bool bEnable );
    virtual void                SetAltWaterOrderEnabled     ( bool bEnable );

    float                       GetAircraftMaxHeight        ( void )                    { return m_fAircraftMaxHeight; };
    void                        SetAircraftMaxHeight        ( float fHeight )           { m_fAircraftMaxHeight = fHeight; };

    void                        SetAutomaticVehicleStartupOnPedEnter    ( bool bSet );

    void                        SetPedTargetingMarkerEnabled ( bool bEnable );
    bool                        IsPedTargetingMarkerEnabled ( void );
    bool                        IsConnected                 ( void );

    virtual void                GetRwResourceStats          ( SRwResourceStats& outStats );
    virtual void                GetClothesCacheStats        ( SClothesCacheStats& outStats );
    virtual void                SetIsMinimizedAndNotConnected ( bool bIsMinimizedAndNotConnected );

    CVector                     m_vecAkimboTarget;
    bool                        m_bAkimboTargetUp;
    static char*                ms_PlayerImgCachePtr;
private:
    bool                        m_bSuspensionEnabled;
    std::vector < char >        m_PlayerImgCache;
    EFastClothesLoading         m_FastClothesLoading;
    CLimitsSA                   m_limits;
    bool                        m_bEnabledLODSystem;
    bool                        m_bEnabledAltWaterOrder;
    bool                        m_bEnabledClothesMemFix;
    float                       m_fAircraftMaxHeight;
    bool                        m_bHeatHazeEnabled;
    bool                        m_bHeatHazeCustomized;

/*  VOID                        SetPlayerShotVectors(CPlayerPed* player, Vector3D * vecTarget, Vector3D * vecStart);
    VOID                        SetPlayerCameraVectors(CPlayerPed* player, Vector3D * vecSource, Vector3D * vecFront);
    Vector3D                    * GetLocalShotOriginVector();*/
    //Vector3D                  * GetLocalTargetVector();
    //Vector3D                  * GetLocalCrossProductVector();
    /*VOID                      SetOnFootPlayerKeyStates(CPlayerPed * player, DWORD keys);
    DWORD                       GetOnFootLocalPlayerKeyStates();
    VOID                        SetInVehiclePlayerKeyStates(CPlayerPed * player, WORD keys);
    WORD                        GetInVehicleLocalPlayerKeyStates();*/
//  Vector3D                    * GetLocalStartVector();
//  VOID                        SetPlayerStartVector(CPlayerPed* player, Vector3D * vecStart);

private:
    static unsigned long        HOOKPOS_FindPlayerCoors;
    static unsigned long        HOOKPOS_FindPlayerCentreOfWorld;
    static unsigned long        HOOKPOS_FindPlayerHeading;
    static unsigned long        HOOKPOS_CStreaming_Update_Caller;
    static unsigned long        HOOKPOS_CHud_Draw_Caller;
    static unsigned long        HOOKPOS_CRunningScript_Process;
    static unsigned long        HOOKPOS_CExplosion_AddExplosion;
    static unsigned long        HOOKPOS_CRealTimeShadowManager__ReturnRealTimeShadow;
    static unsigned long        HOOKPOS_CCustomRoadsignMgr__RenderRoadsignAtomic;
    static unsigned long        HOOKPOS_Trailer_BreakTowLink;
    static unsigned long        HOOKPOS_CRadar__DrawRadarGangOverlay;
    static unsigned long        HOOKPOS_CTaskComplexJump__CreateSubTask;
    static unsigned long        HOOKPOS_CTrain_ProcessControl_Derail;
    static unsigned long        HOOKPOS_CVehicle_SetupRender;
    static unsigned long        HOOKPOS_CVehicle_ResetAfterRender;
    static unsigned long        HOOKPOS_CObject_Render;
    static unsigned long        HOOKPOS_EndWorldColors;
    static unsigned long        HOOKPOS_CWorld_ProcessVerticalLineSectorList;
    static unsigned long        HOOKPOS_ComputeDamageResponse_StartChoking;
    static unsigned long        HOOKPOS_CAutomobile__ProcessSwingingDoor;

    static unsigned long        FUNC_CStreaming_Update;
    static unsigned long        FUNC_CAudioEngine__DisplayRadioStationName;
    static unsigned long        FUNC_CHud_Draw;

    static unsigned long        ADDR_CursorHiding;
    static unsigned long        ADDR_GotFocus;

    static unsigned long        FUNC_CPlayerInfoBase;
};

//has nothing actually to do with multiplayer, just HL HACKS
extern CMultiplayerSA* pMultiplayer;

CMultiplayer* InitMultiplayerInterface();