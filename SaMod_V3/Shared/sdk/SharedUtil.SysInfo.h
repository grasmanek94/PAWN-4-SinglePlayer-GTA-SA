/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        SharedUtil.SysInfo.h
*  PURPOSE:
*  DEVELOPERS:  
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#ifdef MTA_CLIENT

#include "WinVer.h"

namespace SharedUtil
{
    struct SQueryWMIResult : public std::vector < std::vector < SString > >
    {
    };

    struct SLibVersionInfo : VS_FIXEDFILEINFO
    {
        SString strCompanyName;
        SString strProductName;
    };

    bool        QueryWMI                        ( SQueryWMIResult& outResult, const SString& strQuery, const SString& strKeys, const SString& strNamespace = "CIMV2" );
    SString     GetWMIOSVersion                 ( void );
    long long   GetWMIVideoAdapterMemorySize    ( const SString& strDisplay );
    long long   GetWMITotalPhysicalMemory       ( void );
    void        GetWMIAntiVirusStatus           ( std::vector < SString >& outEnabledList, std::vector < SString >& outDisabledList );
    bool        GetLibVersionInfo               ( const SString& strLibName, SLibVersionInfo* pOutLibVersionInfo );
}

#endif
