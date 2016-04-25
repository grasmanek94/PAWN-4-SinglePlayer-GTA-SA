/*

	PROJECT:		mod_sa
	LICENSE:		See LICENSE in the top level directory
	COPYRIGHT:		Copyright we_sux

	mod_sa is available from http://code.google.com/p/m0d-s0beit-sa/

	mod_sa is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	mod_sa is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with mod_sa.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "main.h"

stPlayerPool					*g_Players = NULL;
stVehiclePool					*g_Vehicles = NULL;
stInputInfo						*g_Input = NULL;

// global managed support variables
stTranslateGTASAMP_vehiclePool	translateGTASAMP_vehiclePool;
stTranslateGTASAMP_pedPool		translateGTASAMP_pedPool;

stStreamedOutPlayerInfo			g_stStreamedOutInfo;



//////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// FUNCTIONS //////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

// update SAMPGTA vehicle translation structure
void update_translateGTASAMP_vehiclePool ( void )
{
	traceLastFunc( "update_translateGTASAMP_vehiclePool()" );
	if ( !g_Vehicles )
		return;

	int iGTAID;
	for ( int i = 0; i <= SAMP_VEHICLE_MAX; i++ )
	{
		if ( g_Vehicles->iIsListed[i] != 1 )
			continue;
		if ( isBadPtr_writeAny(g_Vehicles->pSAMP_Vehicle[i], sizeof(stSAMPVehicle)) )
			continue;
		iGTAID = getVehicleGTAIDFromInterface( (DWORD *)g_Vehicles->pSAMP_Vehicle[i]->pGTA_Vehicle );
		if ( iGTAID <= SAMP_VEHICLE_MAX && iGTAID >= 0 )
		{
			translateGTASAMP_vehiclePool.iSAMPID[iGTAID] = i;
		}
	}
}

// update SAMPGTA ped translation structure
void update_translateGTASAMP_pedPool ( void )
{
	traceLastFunc( "update_translateGTASAMP_pedPool()" );
	if ( !g_Players )
		return;

	int iGTAID, i;
	for ( i = 0; i < SAMP_PLAYER_MAX; i++ )
	{
		if ( i == g_Players->sLocalPlayerID )
		{
			translateGTASAMP_pedPool.iSAMPID[0] = i;
			continue;
		}

		if ( isBadPtr_writeAny(g_Players->pRemotePlayer[i], sizeof(stRemotePlayer)) )
			continue;
		if ( isBadPtr_writeAny(g_Players->pRemotePlayer[i]->pPlayerData, sizeof(stRemotePlayerData)) )
			continue;
		if ( isBadPtr_writeAny(g_Players->pRemotePlayer[i]->pPlayerData->pSAMP_Actor, sizeof(stSAMPPed)) )
			continue;

		iGTAID = getPedGTAIDFromInterface( (DWORD *)g_Players->pRemotePlayer[i]->pPlayerData->pSAMP_Actor->pGTA_Ped );
		if ( iGTAID <= SAMP_PLAYER_MAX && iGTAID >= 0 )
		{
			translateGTASAMP_pedPool.iSAMPID[iGTAID] = i;
		}
	}
}

//ClientCommands

extern int	joining_server;
void cmd_change_server ( char *param )	//127.0.0.1 7777 Username Password
{
	traceLastFunc( "cmd_change_server()" );

	char	*result;
	bool	success = false;

	char	IP[257], LocalName[MAX_PLAYER_NAME];
	int		Port;
	strcpy( IP, g_SAMP->szIP );
	Port = g_SAMP->ulPort;
	strcpy( LocalName, getPlayerName(g_Players->sLocalPlayerID) );

	result = strtok( param, " :" );
	for ( int i = 0; i <= 4; i++ )
	{
		if ( result == NULL && !success )
		{
			addMessageToChatWindow( "USAGE: /m0d_change_server <ip> <port> <Username> <Server Password>" );
			addMessageToChatWindow( "Variables that are set to \"NULL\" (capitalized) will be ignored." );
			addMessageToChatWindow( "If you set the Password to \"NULL\" it is set to <no server password>." );
			addMessageToChatWindow( "Username and password can also be left out completely." );
			strcpy( g_SAMP->szIP, IP );
			g_SAMP->ulPort = Port;
			setLocalPlayerName( LocalName );
			return;
		}
		else if ( result == NULL && success )
		{
			joining_server = 1;
			return;
		}

		switch ( i )
		{
		case 0:
			if ( strcmp(result, "NULL") != 0 )
				strcpy( g_SAMP->szIP, result );
			break;

		case 1:
			if ( strcmp(result, "NULL") != 0 )
				g_SAMP->ulPort = atoi( result );
			success = true;
			break;

		case 2:
			if ( strcmp(result, "NULL") != 0 )
			{
				if ( strlen(result) > ALLOWED_PLAYER_NAME_LENGTH )
					addMessageToChatWindow( "Username was too long - adjusted size." );
				strncpy_s( LocalName, result, ALLOWED_PLAYER_NAME_LENGTH );
				setLocalPlayerName( LocalName );
			}
			break;

		case 3:
			if ( strcmp(result, "NULL") != 0 )
				setPassword( result );
			else
				setPassword( "" );
			break;

		default:
			{
				addMessageToChatWindow( "Too many variables." );
				addMessageToChatWindow( "USAGE: /m0d_change_server <ip> <port> <Username> <Server Password>" );
				addMessageToChatWindow( "Variables that are set to \"NULL\" (capitalized) will be ignored." );
				addMessageToChatWindow( "If you set the Password to \"NULL\" it is set to <no server password>." );
				addMessageToChatWindow( "Username and password can also be left out completely." );
				strcpy( g_SAMP->szIP, IP );
				g_SAMP->ulPort = Port;
				strcpy( LocalName, getPlayerName(g_Players->sLocalPlayerID) );
				if ( i >= 3 )
				{
					addMessageToChatWindow( "Setting password to <no server password>." );
					setPassword( "" );
				}

				return;
			}
		}

		result = strtok( NULL, " :" );
	}
}

void cmd_change_server_fav ( char *param )
{
	traceLastFunc( "cmd_change_server_fav()" );

	if ( strlen(param) == 0 )
	{
		addMessageToChatWindow( "/m0d_fav_server <server name/part of server name>" );
		addMessageToChatWindow( "In order to see the favorite server list type: /m0d_fav_server list" );
		return;
	}

	if ( strncmp(param, "list", 4) == 0 )
	{
		int count = 0;
		for ( int i = 0; i < INI_SERVERS_MAX; i++ )
		{
			if ( set.server[i].server_name == NULL )
				continue;

			count++;
			addMessageToChatWindow( "%s", set.server[i].server_name );
		}
		if ( count == 0 )
			addMessageToChatWindow( "No servers in favorite server list. Edit the ini file to add some." );
		return;
	}

	char	IP[257], LocalName[29];
	int		Port;
	strcpy( IP, g_SAMP->szIP );
	Port = g_SAMP->ulPort;
	strcpy( LocalName, getPlayerName(g_Players->sLocalPlayerID) );

	for ( int i = 0; i < INI_SERVERS_MAX; i++ )
	{
		if ( set.server[i].server_name == NULL || set.server[i].ip == NULL
			|| strlen(set.server[i].ip) < 7 || set.server[i].port == 0 )
			continue;

		if ( !findstrinstr((char *)set.server[i].server_name, param) )
			continue;

		if ( !set.use_current_name )
			setLocalPlayerName( set.server[i].nickname );

		strcpy( g_SAMP->szIP, set.server[i].ip );
		g_SAMP->ulPort = set.server[i].port;
		setPassword( set.server[i].password );
		joining_server = 1;
		return;
	}

	addMessageToChatWindow( "/m0d_fav_server <server name/part of server name>" );
	return;
}

void cmd_current_server ( char *param )
{
	addMessageToChatWindow( "Server Name: %s", g_SAMP->szHostname );
	addMessageToChatWindow( "Server Address: %s:%i", g_SAMP->szIP, g_SAMP->ulPort );
	addMessageToChatWindow( "Username: %s", getPlayerName(g_Players->sLocalPlayerID) );
}

// strtokstristr?
bool findstrinstr ( char *text, char *find )
{
	char	realtext[256];
	char	subtext[256];
	char	*result;
	char	*next;
	char	temp;
	int		i = 0;

	traceLastFunc( "findstrinstr()" );

	// can't find stuff that isn't there unless you are high
	if ( text == NULL || find == NULL )
		return false;

	// lower case text ( sizeof()-2 = 1 for array + 1 for termination after while() )
	while ( text[i] != NULL && i < (sizeof(realtext)-2) )
	{
		temp = text[i];
		if ( isupper(temp) )
			temp = tolower( temp );
		realtext[i] = temp;
		i++;
	}
	realtext[i] = 0;

	// replace unwanted characters/spaces with dots
	i = 0;
	while ( find[i] != NULL && i < (sizeof(subtext)-2) )
	{
		temp = find[i];
		if ( isupper(temp) )
			temp = tolower( temp );
		if ( !isalpha(temp) )
			temp = '.';
		subtext[i] = temp;
		i++;
	}
	subtext[i] = 0;

	// use i to count the successfully found text parts
	i = 0;

	// split and find every part of subtext/find in text
	result = &subtext[0];
	while ( *result != NULL )
	{
		next = strstr( result, "." );
		if ( next != NULL )
		{
			// more than one non-alphabetic character
			if ( next == result )
			{
				do
					next++;
				while ( *next == '.' );

				if ( *next == NULL )
					return (i != 0);
				result = next;
				next = strstr( result, "." );
				if ( next != NULL )
					*next = NULL;
			}
			else
				*next = NULL;
		}

		if ( strstr(realtext, result) == NULL )
			return false;

		if ( next == NULL )
			return true;

		i++;
		result = next + 1;
	}

	return false;
}

void cmd_tele_loc ( char *param )
{
	if ( strlen(param) == 0 )
	{
		addMessageToChatWindow( "USAGE: /m0d_tele_loc <location name>" );
		addMessageToChatWindow( "Use /m0d_tele_locations to show the location names." );
		addMessageToChatWindow( "The more specific you are on location name the better the result." );
		return;
	}

	for ( int i = 0; i < STATIC_TELEPORT_MAX; i++ )
	{
		if ( strlen(set.static_teleport_name[i]) == 0 || vect3_near_zero(set.static_teleport[i].pos) )
			continue;

		if ( !findstrinstr(set.static_teleport_name[i], param) )
			continue;

		cheat_state_text( "Teleported to: %s.", set.static_teleport_name[i] );
		cheat_teleport( set.static_teleport[i].pos, set.static_teleport[i].interior_id );
		return;
	}

	addMessageToChatWindow( "USAGE: /m0d_tele_loc <location name>" );
	addMessageToChatWindow( "Use /m0d_tele_locations to show the location names." );
	addMessageToChatWindow( "The more specific you are on location name the better the result." );
}

void cmd_tele_locations ()
{
	for ( int i = 0; i < STATIC_TELEPORT_MAX; i++ )
	{
		if ( strlen(set.static_teleport_name[i]) == 0 || vect3_near_zero(set.static_teleport[i].pos) )
			continue;
		addMessageToChatWindow( "%s", set.static_teleport_name[i] );
	}

	addMessageToChatWindow( "To teleport use the menu or: /m0d_tele_loc <location name>" );
}

// new functions to check for bad pointers
int isBadPtr_SAMP_iVehicleID ( int iVehicleID )
{
	if ( g_Vehicles == NULL || iVehicleID == (uint16_t)-1)
		return 1;
	return !g_Vehicles->iIsListed[iVehicleID];

	// this hasn't been required yet
	//if (g_Vehicles->pSAMP_Vehicle[iVehicleID] == NULL) continue;
}

int isBadPtr_SAMP_iPlayerID ( int iPlayerID )
{
	if ( g_Players == NULL || iPlayerID < 0 || iPlayerID > SAMP_PLAYER_MAX)
		return 1;
	return !g_Players->iIsListed[iPlayerID];
}

void getSamp ()
{
	if ( set.basic_mode )
		return;

	uint32_t	samp_dll = getSampAddress();

	if ( samp_dll != NULL )
	{
		g_dwSAMP_Addr = ( uint32_t ) samp_dll;

		if ( g_dwSAMP_Addr != NULL )
		{
			if ( memcmp_safe((uint8_t *)g_dwSAMP_Addr + 0xBABE, hex_to_bin(SAMP_CMP_03DR1), 10) )
			{
				strcpy(g_szSAMPVer, "SA:MP 0.3d");
				Log( "%s was detected. g_dwSAMP_Addr: 0x%p", g_szSAMPVer, g_dwSAMP_Addr );
				iIsSAMPSupported = 1;
			}
			else if ( memcmp_safe((uint8_t *)g_dwSAMP_Addr + 0xBABE, hex_to_bin(SAMP_CMP_03DR2), 10) )
			{
				strcpy(g_szSAMPVer, "SA:MP 0.3d-R2");
				Log( "%s was detected. g_dwSAMP_Addr: 0x%p", g_szSAMPVer, g_dwSAMP_Addr );

				// (0.3d-R2 temp) disable AC
				if(memcmp_safe((uint32_t *)(g_dwSAMP_Addr + 0x8F210), "\xE9\x34\x91\x24\x00", 5))
					memset_safe((uint32_t *)(g_dwSAMP_Addr + 0x8F210), 0xC3, 1);

				iIsSAMPSupported = 0;
				set.basic_mode = true;
				g_dwSAMP_Addr = NULL;
			}
			else
			{
				Log( "Unknown SA:MP version. Running in basic mode." );
				iIsSAMPSupported = 0;
				set.basic_mode = true;

				g_dwSAMP_Addr = NULL;
			}
		}
	}
	else
	{
		iIsSAMPSupported = 0;
		set.basic_mode = true;
		Log( "samp.dll not found. Running in basic mode." );
	}

	return;
}

uint32_t getSampAddress ()
{
	if ( set.run_mode == RUNMODE_SINGLEPLAYER )
		return 0x0;

	uint32_t	samp_dll;

	if ( set.run_mode == RUNMODE_SAMP )
	{
		if ( set.wine_compatibility )
		{
			HMODULE temp = LoadLibrary( SAMP_DLL );
			__asm mov samp_dll, eax
		}
		else
		{
			void	*temp = dll_baseptr_get( SAMP_DLL );
			__asm mov samp_dll, eax
		}
	}

	if ( samp_dll == NULL )
		return 0x0;

	return samp_dll;
}

struct stSAMP *stGetSampInfo ( void )
{
	if ( g_dwSAMP_Addr == NULL )
		return NULL;

	uint32_t	info_ptr;
	info_ptr = ( UINT_PTR ) * ( uint32_t * ) ( (uint8_t *) (void *)((uint8_t *)g_dwSAMP_Addr + SAMP_INFO_OFFSET) );
	if ( info_ptr == NULL )
		return NULL;

	return (struct stSAMP *)info_ptr;
}

struct stChatInfo *stGetSampChatInfo ( void )
{
	if ( g_dwSAMP_Addr == NULL )
		return NULL;

	uint32_t	chat_ptr;
	chat_ptr = ( UINT_PTR ) * ( uint32_t * ) ( (uint8_t *) (void *)((uint8_t *)g_dwSAMP_Addr + SAMP_CHAT_INFO_OFFSET) );
	if ( chat_ptr == NULL )
		return NULL;

	return (struct stChatInfo *)chat_ptr;
}

struct stInputInfo *stGetInputInfo ( void )
{
	if ( g_dwSAMP_Addr == NULL )
		return NULL;

	uint32_t	input_ptr;
	input_ptr = ( UINT_PTR ) * ( uint32_t * ) ( (uint8_t *) (void *)((uint8_t *)g_dwSAMP_Addr + SAMP_CHAT_INPUT_INFO_OFFSET) );
	if ( input_ptr == NULL )
		return NULL;

	return (struct stInputInfo *)input_ptr;
}

struct stKillInfo *stGetKillInfo ( void )
{
	if ( g_dwSAMP_Addr == NULL )
		return NULL;

	uint32_t	kill_ptr;
	kill_ptr = ( UINT_PTR ) * ( uint32_t * ) ( (uint8_t *) (void *)((uint8_t *)g_dwSAMP_Addr + SAMP_KILL_INFO_OFFSET) );
	if ( kill_ptr == NULL )
		return NULL;

	return (struct stKillInfo *)kill_ptr;
}

D3DCOLOR samp_color_get ( int id, DWORD trans )
{
	if ( g_dwSAMP_Addr == NULL )
		return NULL;

	D3DCOLOR	*color_table;
	if ( id < 0 || id >= (SAMP_PLAYER_MAX + 3) )
		return D3DCOLOR_ARGB( 0xFF, 0x99, 0x99, 0x99 );

	switch ( id )
	{
	case ( SAMP_PLAYER_MAX ):
		return 0xFF888888;

	case ( SAMP_PLAYER_MAX + 1 ):
		return 0xFF0000AA;

	case ( SAMP_PLAYER_MAX + 2 ):
		return 0xFF63C0E2;
	}

	color_table = ( D3DCOLOR * ) ( (uint8_t *)g_dwSAMP_Addr + SAMP_COLOR_OFFSET );
	return ( color_table[id] >> 8 ) | trans;
}

void spectatePlayer(int iPlayerID)
{
	if ( iPlayerID == -1 )
	{
		GTAfunc_TogglePlayerControllable(0);
		GTAfunc_LockActor(0);
		pGameInterface->GetCamera()->RestoreWithJumpCut();

		g_iSpectateEnabled = 0;
		g_iSpectateLock = 0;
		g_iSpectatePlayerID = -1;
		return;
	}

	g_iSpectatePlayerID = iPlayerID;
	g_iSpectateLock = 0;
	g_iSpectateEnabled = 1;
}
void spectateHandle()
{
	if(g_iSpectateEnabled)
	{
		if(g_iSpectateLock) return;

		if(g_iSpectatePlayerID != -1)
		{
			if(g_Players->iIsListed[g_iSpectatePlayerID] != 0)
			{
				if(g_Players->pRemotePlayer[g_iSpectatePlayerID] != NULL)
				{
					int iState = getPlayerState(g_iSpectatePlayerID);

					if(iState == PLAYER_STATE_ONFOOT)
					{
						struct actor_info *pPlayer = getGTAPedFromSAMPPlayerID(g_iSpectatePlayerID);
						if(pPlayer == NULL) return;
						GTAfunc_CameraOnActor(pPlayer);
						g_iSpectateLock = 1;
					}
					else if(iState == PLAYER_STATE_DRIVER)
					{
						struct vehicle_info *pPlayerVehicleID = g_Players->pRemotePlayer[g_iSpectatePlayerID]->pPlayerData->pSAMP_Vehicle->pGTA_Vehicle;
						if(pPlayerVehicleID == NULL) return;
						GTAfunc_CameraOnVehicle(pPlayerVehicleID);
						g_iSpectateLock = 1;
					}
					else if(iState == PLAYER_STATE_PASSENGER)
					{
						struct vehicle_info *pPlayerVehicleID = g_Players->pRemotePlayer[g_iSpectatePlayerID]->pPlayerData->pSAMP_Vehicle->pGTA_Vehicle;
						if(pPlayerVehicleID == NULL) return;
						GTAfunc_CameraOnVehicle(pPlayerVehicleID);
						g_iSpectateLock = 1;
					}
				}
				else
				{
					cheat_state_text("Player is not streamed in");
					g_iSpectateEnabled = 0;
				}
			}
		}
	}
}

void sampMainCheat ()
{
	traceLastFunc( "sampMainCheat()" );

	// g_Vehicles & g_Players pointers need to be refreshed or nulled
	if ( isBadPtr_writeAny(g_SAMP->pPools, sizeof(stSAMPPools)) )
	{
		g_Vehicles = NULL;
		g_Players = NULL;
	}
	else
	{
		if ( isBadPtr_writeAny(g_SAMP->pPools->pPool_Vehicle, sizeof(stVehiclePool)) )
			g_Vehicles = NULL;
		else
			g_Vehicles = g_SAMP->pPools->pPool_Vehicle;
		if ( isBadPtr_writeAny(g_SAMP->pPools->pPool_Player, sizeof(stPlayerPool)) )
			g_Players = NULL;
		else
			g_Players = g_SAMP->pPools->pPool_Player;
	}

	// update GTA to SAMP translation structures
	update_translateGTASAMP_vehiclePool();
	update_translateGTASAMP_pedPool();

	spectateHandle();

	// start chatbox logging
	if ( set.chatbox_logging )
	{
		static int	chatbox_init;
		if ( !chatbox_init )
		{
			SYSTEMTIME	time;
			GetLocalTime( &time );
			LogChatbox( false, "Session started at %02d/%02d/%02d", time.wDay, time.wMonth, time.wYear );
			chatbox_init = 1;
		}
	}

	if ( KEY_DOWN(set.secondary_key) )
	{
		if ( KEY_PRESSED(set.key_player_info_list) )
			cheat_state->player_info_list ^= 1;

		if ( KEY_PRESSED(set.key_rejoin) )
		{
			restartGame();
			disconnect( 500 );
			cheat_state_text( "Rejoining in %d seconds...", set.rejoin_delay / 1000 );
			cheat_state->_generic.rejoinTick = GetTickCount();
		}

		if ( KEY_PRESSED(set.key_respawn) )
			playerSpawn();
	}

	if ( KEY_DOWN(set.chat_secondary_key) )
	{
		int			i, key, spam;
		const char	*msg;
		for ( i = 0; i < INI_CHATMSGS_MAX; i++ )
		{
			struct chat_msg *msg_item = &set.chat[i];
			if ( msg_item->key == NULL )
				continue;
			if ( msg_item->msg == NULL )
				continue;
			if ( msg_item->key != key_being_pressed )
				continue;
			key = msg_item->key;
			msg = msg_item->msg;
			spam = msg_item->spam;
			if ( spam )
			{
				if ( msg )
					if ( KEY_DOWN(key) )
						say( "%s", msg );
			}
			else
			{
				if ( msg )
					if ( KEY_PRESSED(key) )
						say( "%s", msg );
			}
		}
	}

	static int	iSAMPHooksInstalled;
	if ( !iSAMPHooksInstalled )
	{
		installSAMPHooks();
		iSAMPHooksInstalled = 1;
	}

	if ( cheat_state->_generic.rejoinTick && cheat_state->_generic.rejoinTick < (GetTickCount() - set.rejoin_delay) )
	{
		g_SAMP->iGameState = GAMESTATE_WAIT_CONNECT;
		cheat_state->_generic.rejoinTick = 0;
	}

	if ( joining_server == 1 )
	{
		restartGame();
		disconnect( 500 );
		cheat_state_text( "Joining server in %d seconds...", set.rejoin_delay / 1000 );
		cheat_state->_generic.join_serverTick = GetTickCount();
		joining_server = 2;
	}

	if ( joining_server == 2
	 &&	 cheat_state->_generic.join_serverTick
	 &&	 cheat_state->_generic.join_serverTick < (GetTickCount() - set.rejoin_delay) )
	{
		g_SAMP->iGameState = GAMESTATE_WAIT_CONNECT;
		joining_server = 0;
		cheat_state->_generic.join_serverTick = 0;
	}
}

int getNthPlayerID ( int n )
{
	if ( g_Players == NULL )
		return -1;

	int thisplayer = 0;
	for ( int i = 0; i < SAMP_PLAYER_MAX; i++ )
	{
		if ( g_Players->iIsListed[i] != 1 )
			continue;
		if ( g_Players->sLocalPlayerID == i )
			continue;
		if ( thisplayer < n )
		{
			thisplayer++;
			continue;
		}

		return i;
	}

	//shouldnt happen
	return -1;
}

int getPlayerCount ( void )
{
	if ( g_Players == NULL )
		return NULL;

	int iCount = 0;
	int i;

	for ( i = 0; i < SAMP_PLAYER_MAX; i++ )
	{
		if ( g_Players->iIsListed[i] != 1 )
			continue;
		iCount++;
	}

	return iCount + 1;
}

#define SAMP_FUNC_NAMECHANGE	0x9800
int setLocalPlayerName ( const char *name )
{
	if ( g_Players == NULL || g_Players->pLocalPlayer == NULL )
		return 0;

	int strlen_name = strlen( name );
	if ( strlen_name == 0 || strlen_name > ALLOWED_PLAYER_NAME_LENGTH )
		return 0;

	traceLastFunc( "setLocalPlayerName()" );

	//strcpy(g_Players->szLocalPlayerName, name);
	//g_Players->iStrlen_LocalPlayerName = strlen_name;

	DWORD	vtbl_nameHandler = ((DWORD)&g_Players->pVTBL_txtHandler);
	DWORD	func = g_dwSAMP_Addr + SAMP_FUNC_NAMECHANGE;
	__asm push strlen_name
	__asm push name
	__asm mov ecx, vtbl_nameHandler
	__asm call func
	return 1;
}

int getVehicleCount ( void )
{
	if ( g_Vehicles == NULL )
		return NULL;

	int iCount = 0;
	int i;

	for ( i = 0; i < SAMP_VEHICLE_MAX; i++ )
	{
		if ( g_Vehicles->iIsListed[i] != 1 )
			continue;
		iCount++;
	}

	return iCount;
}

int getPlayerPos ( int iPlayerID, float fPos[3] )
{
	traceLastFunc( "getPlayerPos()" );

	struct actor_info	*pActor = NULL;
	struct vehicle_info *pVehicle = NULL;

	struct actor_info	*pSelfActor = actor_info_get( ACTOR_SELF, 0 );

	if ( g_Players == NULL )
		return 0;
	if ( g_Players->iIsListed[iPlayerID] != 1 )
		return 0;
	if ( g_Players->pRemotePlayer[iPlayerID] == NULL )
		return 0;
	if ( g_Players->pRemotePlayer[iPlayerID]->pPlayerData == NULL )
		return 0;

	if ( g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor == NULL )
		return 0;	// not streamed
	else
	{
		pActor = g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor->pGTA_Ped;

		if ( g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Vehicle != NULL )
			pVehicle = g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Vehicle->pGTA_Vehicle;

		if ( pVehicle != NULL && pActor->vehicle == pVehicle && pVehicle->passengers[0] == pActor )
		{
			// driver of a vehicle
			vect3_copy( &pActor->vehicle->base.matrix[4 * 3], fPos );

			//vect3_copy(g_Players->pRemotePlayer[iPlayerID]->fVehiclePosition, fPos);
		}
		else if ( pVehicle != NULL )
		{
			// passenger of a vehicle
			vect3_copy( &pActor->base.matrix[4 * 3], fPos );

			//vect3_copy(g_Players->pRemotePlayer[iPlayerID]->fActorPosition, fPos);
		}
		else
		{
			// on foot
			vect3_copy( &pActor->base.matrix[4 * 3], fPos );

			//vect3_copy(g_Players->pRemotePlayer[iPlayerID]->fActorPosition, fPos);
		}
	}

	if ( pSelfActor != NULL )
	{
		if ( vect3_dist(&pSelfActor->base.matrix[4 * 3], fPos) < 100.0f )
			vect3_copy( &pActor->base.matrix[4 * 3], fPos );
	}

	// detect zombies
	if ( vect3_near_zero(fPos) )
		vect3_copy( &pActor->base.matrix[4 * 3], fPos );

	return !vect3_near_zero( fPos );
}

const char *getPlayerName ( int iPlayerID )
{
	if ( g_Players == NULL || iPlayerID < 0 || iPlayerID > SAMP_PLAYER_MAX )
		return NULL;

	if ( iPlayerID == g_Players->sLocalPlayerID )
	{
		if ( g_Players->iStrlen_LocalPlayerName <= 0xF )
			return g_Players->szLocalPlayerName;
		return g_Players->pszLocalPlayerName;
	}

	if ( g_Players->pRemotePlayer[iPlayerID] == NULL )
		return NULL;

	if ( g_Players->pRemotePlayer[iPlayerID]->iStrlenName <= 0xF )
		return g_Players->pRemotePlayer[iPlayerID]->szPlayerName;

	return g_Players->pRemotePlayer[iPlayerID]->pszPlayerName;
}

int getPlayerState ( int iPlayerID )
{
	if ( g_Players == NULL || iPlayerID < 0 || iPlayerID > SAMP_PLAYER_MAX )
		return NULL;
	if ( iPlayerID == g_Players->sLocalPlayerID )
		return NULL;
	if ( g_Players->iIsListed[iPlayerID] != 1 )
		return NULL;
	if ( g_Players->pRemotePlayer[iPlayerID]->pPlayerData == NULL )
		return NULL;

	return g_Players->pRemotePlayer[iPlayerID]->pPlayerData->bytePlayerState;
}

int getPlayerVehicleGTAScriptingID ( int iPlayerID )
{
	if ( g_Players == NULL )
		return 0;

	// fix to always return our own vehicle always if that's what's being asked for
	if ( iPlayerID == ACTOR_SELF )
	{
		if(g_Players->pLocalPlayer->sCurrentVehicleID == (uint16_t)-1) return 0;

		stSAMPVehicle	*sampveh = g_Vehicles->pSAMP_Vehicle[g_Players->pLocalPlayer->sCurrentVehicleID];
		if ( sampveh )
		{
			return ScriptCarId( sampveh->pGTA_Vehicle );
			//return (int)( ((DWORD) sampveh->pGTA_Vehicle) - (DWORD) pool_vehicle->start ) / 2584;
		}
		else
			return 0;
	}

	// make sure remote player is legit
	if ( g_Players->pRemotePlayer[iPlayerID] == NULL || g_Players->pRemotePlayer[iPlayerID]->pPlayerData == NULL ||
		g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Vehicle == NULL ||
		g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Vehicle->pGTA_Vehicle == NULL)
		return 0;

	// make sure samp knows the vehicle exists
	if ( g_Vehicles->pSAMP_Vehicle[g_Players->pRemotePlayer[iPlayerID]->pPlayerData->sVehicleID] == NULL )
		return 0;

	// return the remote player's vehicle
	return ScriptCarId( g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Vehicle->pGTA_Vehicle );
}

int getPlayerSAMPVehicleID(int iPlayerID)
{
	if(g_Players == NULL && g_Vehicles == NULL) return 0;
	if(g_Players->pRemotePlayer[iPlayerID] == NULL) return 0;
	if(g_Vehicles->pSAMP_Vehicle[g_Players->pRemotePlayer[iPlayerID]->pPlayerData->sVehicleID] == NULL) return 0;
	return g_Players->pRemotePlayer[iPlayerID]->pPlayerData->sVehicleID;
}

struct actor_info *getGTAPedFromSAMPPlayerID ( int iPlayerID )
{
	if ( g_Players == NULL || iPlayerID < 0 || iPlayerID > SAMP_PLAYER_MAX )
		return NULL;
	if ( iPlayerID == g_Players->sLocalPlayerID )
		return actor_info_get( ACTOR_SELF, 0 );
	if ( g_Players->iIsListed[iPlayerID] != 1 )
		return NULL;
	if ( g_Players->pRemotePlayer[iPlayerID] == NULL )
		return NULL;
	if ( g_Players->pRemotePlayer[iPlayerID]->pPlayerData == NULL )
		return NULL;
	if ( g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor == NULL )
		return NULL;
	if ( g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor->pGTA_Ped == NULL )
		return NULL;
		
	// return actor_info, null or otherwise
	return g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor->pGTA_Ped;
}

struct vehicle_info *getGTAVehicleFromSAMPVehicleID ( int iVehicleID )
{
	if ( g_Vehicles == NULL || iVehicleID < 0 || iVehicleID >= SAMP_VEHICLE_MAX )
		return NULL;
	if ( iVehicleID == g_Players->pLocalPlayer->sCurrentVehicleID )
		return vehicle_info_get( VEHICLE_SELF, 0 );
	if ( g_Vehicles->iIsListed[iVehicleID] != 1 )
		return NULL;

	// return vehicle_info, null or otherwise
	return g_Vehicles->pGTA_Vehicle[iVehicleID];
}

int getSAMPPlayerIDFromGTAPed ( struct actor_info *pGTAPed )
{
	if ( g_Players == NULL )
		return 0;
	if ( actor_info_get(ACTOR_SELF, 0) == pGTAPed )
		return g_Players->sLocalPlayerID;

	int i;
	for ( i = 0; i < SAMP_PLAYER_MAX; i++ )
	{
		if ( g_Players->iIsListed[i] != 1 )
			continue;
		if ( g_Players->pRemotePlayer[i] == NULL )
			continue;
		if ( g_Players->pRemotePlayer[i]->pPlayerData == NULL )
			continue;
		if ( g_Players->pRemotePlayer[i]->pPlayerData->pSAMP_Actor == NULL )
			continue;
		if ( g_Players->pRemotePlayer[i]->pPlayerData->pSAMP_Actor->pGTA_Ped == NULL )
			continue;
		if ( g_Players->pRemotePlayer[i]->pPlayerData->pSAMP_Actor->pGTA_Ped == pGTAPed )
			return i;
	}

	return ACTOR_SELF;
}

int getSAMPVehicleIDFromGTAVehicle ( struct vehicle_info *pVehicle )
{
	if ( g_Vehicles == NULL )
		return NULL;
	if ( vehicle_info_get(VEHICLE_SELF, 0) == pVehicle && g_Players != NULL )
		return g_Players->pLocalPlayer->sCurrentVehicleID;

	int i, iReturn = 0;
	for ( i = 0; i < SAMP_VEHICLE_MAX; i++ )
	{
		if ( g_Vehicles->iIsListed[i] != 1 )
			continue;
		if ( g_Vehicles->pGTA_Vehicle[i] == pVehicle )
			return i;
	}

	return VEHICLE_SELF;
}

uint32_t getPedGTAScriptingIDFromPlayerID ( int iPlayerID )
{
	if ( g_Players == NULL )
		return NULL;

	if ( g_Players->iIsListed[iPlayerID] != 1 )
		return NULL;
	if ( g_Players->pRemotePlayer[iPlayerID] == NULL )
		return NULL;
	if ( g_Players->pRemotePlayer[iPlayerID]->pPlayerData == NULL )
		return NULL;
	if ( g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor == NULL )
		return NULL;

	return g_Players->pRemotePlayer[iPlayerID]->pPlayerData->pSAMP_Actor->ulGTA_Ped_ID;
}

uint32_t getVehicleGTAScriptingIDFromVehicleID ( int iVehicleID )
{
	if ( g_Vehicles == NULL )
		return NULL;

	if ( g_Vehicles->iIsListed[iVehicleID] != 1 )
		return NULL;
	if ( g_Vehicles->pSAMP_Vehicle[iVehicleID] == NULL )
		return NULL;

	return g_Vehicles->pSAMP_Vehicle[iVehicleID]->ulGTA_Vehicle_ID;
}

struct m0dClientCMD
{
#pragma pack( 1 )
	char	cmd_name[30];

	//char cmd_description[128];
} m0d_cmd_list[( MAX_CLIENTCMDS - 22 )];
int m0d_cmd_num = 0;

void cmd_showCMDS ()
{
	int i = 0;
	for ( ; i < m0d_cmd_num; i++ )
	{
		addMessageToChatWindow( "%s", m0d_cmd_list[i].cmd_name );
	}
}

#define FUNC_ADDCLIENTCMD	0x60780
void addClientCommand ( char *name, int function )
{
	if ( name == NULL || function == NULL || g_Input == NULL )
		return;

	if ( g_Input->iCMDCount == (MAX_CLIENTCMDS-1) )
	{
		Log( "Error: couldn't initialize '%s'. Maximum command amount reached.", name );
		return;
	}

	if ( strlen(name) > 30 )
	{
		Log( "Error: command name '%s' was too long.", name );
		return;
	}

	if ( m0d_cmd_num < (MAX_CLIENTCMDS - 22) )
	{
		strncpy_s( m0d_cmd_list[m0d_cmd_num].cmd_name, name, sizeof(m0d_cmd_list[m0d_cmd_num].cmd_name)-1 );
		m0d_cmd_num++;
	}
	else
		Log( "m0d_cmd_list[] too short." );

	uint32_t	data = g_dwSAMP_Addr + SAMP_CHAT_INPUT_INFO_OFFSET;
	uint32_t	func = g_dwSAMP_Addr + FUNC_ADDCLIENTCMD;
	__asm mov eax, data
	__asm mov ecx, [eax]
	__asm push function
	__asm push name
	__asm call func
}

bool	modcommands = false;
bool get_isModCommandsActive ()
{
	return modcommands;
}

void init_samp_chat_cmds ()
{
	if ( modcommands == true )
	{
		return;
	}
	else
	{
		cheat_state_text( "initiated modcommands" );
		modcommands = true;
	}

	addClientCommand( "m0d_show_cmds", (int)cmd_showCMDS );
	addClientCommand( "m0d_change_server", (int)cmd_change_server );
	addClientCommand( "m0d_fav_server", (int)cmd_change_server_fav );
	addClientCommand( "m0d_current_server", (int)cmd_current_server );
	addClientCommand( "m0d_tele_loc", (int)cmd_tele_loc );
	addClientCommand( "m0d_teleport_location", (int)cmd_tele_loc );
	addClientCommand( "m0d_tele_locations", (int)cmd_tele_locations );
	addClientCommand( "m0d_teleport_locations", (int)cmd_tele_locations );
}

struct gui	*gui_samp_cheat_state_text = &set.guiset[1];
void addMessageToChatWindow ( const char *text, ... )
{
	if ( g_SAMP != NULL )
	{
		va_list ap;
		if ( text == NULL )
			return;

		char	tmp[512];
		memset( tmp, 0, 512 );

		va_start( ap, text );
		vsnprintf( tmp, sizeof(tmp)-1, text, ap );
		va_end( ap );

		addToChatWindow( tmp, D3DCOLOR_XRGB(gui_samp_cheat_state_text->red, gui_samp_cheat_state_text->green,
						 gui_samp_cheat_state_text->blue) );
	}
	else
	{
		va_list ap;
		if ( text == NULL )
			return;

		char	tmp[512];
		memset( tmp, 0, 512 );

		va_start( ap, text );
		vsnprintf( tmp, sizeof(tmp)-1, text, ap );
		va_end( ap );

		cheat_state_text( tmp, D3DCOLOR_ARGB(255, 0, 200, 200) );
	}
}

void addMessageToChatWindowSS ( const char *text, ... )
{
	if ( g_SAMP != NULL )
	{
		va_list ap;
		if ( text == NULL )
			return;

		char	tmp[512];
		memset( tmp, 0, 512 );

		va_start( ap, text );
		vsprintf( tmp, text, ap );
		va_end( ap );

		addMessageToChatWindow( tmp, D3DCOLOR_ARGB(255, 0, 200, 200) );
	}
	else
	{
		va_list ap;
		if ( text == NULL )
			return;

		char	tmp[512];
		memset( tmp, 0, 512 );

		va_start( ap, text );
		vsprintf( tmp, text, ap );
		va_end( ap );

		cheat_state_text( tmp, D3DCOLOR_ARGB(255, 0, 200, 200) );
	}
}

#define FUNC_ADDTOCHATWND	0x5F060
void addToChatWindow ( char *text, D3DCOLOR textColor, int playerID )
{
	if ( g_SAMP == NULL || g_Chat == NULL )
		return;

	if ( text == NULL )
		return;

	if ( playerID < -1 )
		playerID = -1;

	uint32_t	chatinfo = g_dwSAMP_Addr + SAMP_CHAT_INFO_OFFSET;
	uint32_t	func = g_dwSAMP_Addr + FUNC_ADDTOCHATWND;

	if ( playerID != -1 )
	{
		// getPlayerName does the needed validity checks, no need for doubles
		char *playerName = (char*)getPlayerName(playerID);
		if ( playerName == NULL )
			return;

		D3DCOLOR playerColor = samp_color_get(playerID);

		__asm mov eax, dword ptr[chatinfo]
		__asm mov ecx, dword ptr[eax]
		__asm push playerColor
		__asm push textColor
		__asm push playerName
		__asm push text
		__asm push 10
		__asm call func
//		__asm pop eax
//		__asm pop ecx
		return;
	}

	__asm mov eax, dword ptr[chatinfo]
	__asm mov ecx, dword ptr[eax]
	__asm push 0
	__asm push textColor
	__asm push 0
	__asm push text
	__asm push 12
	__asm call func
//	__asm pop eax
//	__asm pop ecx
	return;
}

#define FUNC_RESTARTGAME	0x8720
void restartGame ()
{
	if ( g_SAMP == NULL )
		return;

	uint32_t	samp_info = g_dwSAMP_Addr + SAMP_INFO_OFFSET;
	uint32_t	func = g_dwSAMP_Addr + FUNC_RESTARTGAME;
	__asm mov eax, dword ptr[samp_info]
	__asm mov ecx, dword ptr[eax]
	__asm call func
	__asm pop eax
	__asm pop ecx
}

void say ( char *text, ... )
{
	if ( g_SAMP == NULL )
		return;

	if ( text == NULL )
		return;
	if ( isBadPtr_readAny(text, 128) )
		return;
	traceLastFunc( "say()" );

	va_list ap;
	char	tmp[128];
	memset( tmp, 0, 128 );

	va_start( ap, text );
	vsprintf( tmp, text, ap );
	va_end( ap );

	addSayToChatWindow( tmp );
}

#define FUNC_SAY		0x49F0
#define FUNC_SENDCMD	0x60910
void addSayToChatWindow ( char *msg )
{
	if ( g_SAMP == NULL )
		return;

	if ( msg == NULL )
		return;
	if ( isBadPtr_readAny(msg, 128) )
		return;
	traceLastFunc( "addSayToChatWindow()" );

	if ( msg[0] == '/' )
	{
		uint32_t	func = g_dwSAMP_Addr + FUNC_SENDCMD;
		__asm push msg
		__asm call func
	}
	else
	{
		uint32_t	func = g_dwSAMP_Addr + FUNC_SAY;
		void		*lpPtr = g_Players->pLocalPlayer;
		__asm mov ebx, dword ptr[lpPtr]
		__asm push msg
		__asm call func
		__asm pop ebx
	}
}

#define FUNC_GAMETEXT	0x91A40
void showGameText ( char *text, int time, int textsize )
{
	if ( g_SAMP == NULL )
		return;

	uint32_t	func = g_dwSAMP_Addr + FUNC_GAMETEXT;
	__asm push textsize
	__asm push time
	__asm push text
	__asm call func
}

#define FUNC_SPAWN	0x34B0
void playerSpawn ( void )
{
	if ( g_SAMP == NULL )
		return;

	uint32_t	func = g_dwSAMP_Addr + FUNC_SPAWN;
	void		*lpPtr = g_Players->pLocalPlayer;
	__asm mov ecx, dword ptr[lpPtr]
	__asm push ecx
	__asm call func
	__asm pop ecx
}

void disconnect ( int reason /*0=timeout, 500=quit*/ )
{
	if ( g_SAMP == NULL )
		return;

	void	*rakptr = g_SAMP->pRakClientInterface;
	__asm mov ecx, dword ptr[rakptr]
	__asm mov eax, dword ptr[ecx]
	__asm push 0
	__asm push reason
	__asm call dword ptr[eax + 8]
	__asm pop ecx
	__asm pop eax
}

void setPassword ( char *password )
{
	if ( g_SAMP == NULL )
		return;

	void	*rakptr = g_SAMP->pRakClientInterface;
	__asm mov ecx, dword ptr[rakptr]
	__asm mov eax, dword ptr[ecx]
	__asm push password
	__asm call dword ptr[eax + 16]
	__asm pop ecx
	__asm pop eax
}

#define FUNC_SENDINTERIOR	0x4940
void sendSetInterior ( uint8_t interiorID )
{
	if ( g_SAMP == NULL )
		return;

	uint32_t	func = g_dwSAMP_Addr + FUNC_SENDINTERIOR;
	void		*lpPtr = g_Players->pLocalPlayer;
	__asm mov ecx, dword ptr[interiorID]
	__asm push ecx
	__asm mov ecx, dword ptr[lpPtr]
	__asm call func
	__asm pop ecx
}

#define FUNC_SETSPECIALACTION	0x2AE0
void setSpecialAction ( uint8_t byteSpecialAction )
{
	if ( g_SAMP == NULL )
		return;

	if ( g_Players->pLocalPlayer == NULL )
		return;

	g_Players->pLocalPlayer->onFootData.byteSpecialAction = byteSpecialAction;

	uint32_t	func = g_dwSAMP_Addr + FUNC_SETSPECIALACTION;
	void		*lpPtr = g_Players->pLocalPlayer;
	__asm mov ecx, dword ptr[byteSpecialAction]
	__asm push ecx
	__asm mov ecx, dword ptr[lpPtr]
	__asm call func
	__asm pop ecx
}

#define FUNC_SENDSCMEVENT	0x1890
void sendSCMEvent ( int iEvent, int iVehicleID, int iParam1, int iParam2 )
{
	if ( g_SAMP == NULL )
		return;

	uint32_t	func = g_dwSAMP_Addr + FUNC_SENDSCMEVENT;
	__asm push iParam2
	__asm push iParam1
	__asm push iVehicleID
	__asm push iEvent
	__asm call func
}

/*
// this doesn't work when wrapped around the toggle below, samp sux
CMatrix toggleSAMPCursor_Camera = CMatrix();
void _cdecl toggleSAMPCursor_SaveCamera ( void )
{
	pGame->GetCamera()->GetMatrix(&toggleSAMPCursor_Camera);
}

void _cdecl toggleSAMPCursor_RestoreCamera ( void )
{
	pGame->GetCamera()->SetMatrix(&toggleSAMPCursor_Camera);
}
*/

#define FUNC_TOGGLECURSOR			0x914E0
#define FUNC_CURSORUNLOCKACTORCAM	0x913C0
void toggleSAMPCursor(int iToggle)
{
	if(g_Input->iInputEnabled) return;

	uint32_t	func = g_dwSAMP_Addr + FUNC_TOGGLECURSOR;

	if(iToggle)
	{
		_asm
		{
			//call toggleSAMPCursor_SaveCamera;
			mov ecx, g_Input;
			push 0;
			push 2;
			call func;
			//call toggleSAMPCursor_RestoreCamera;
		}
		g_iCursorEnabled = 1;
	}
	else
	{
		_asm
		{
			mov ecx, g_Input;
			push 1;
			push 0;
			call func;
		}
		uint32_t funcunlock = g_dwSAMP_Addr + FUNC_CURSORUNLOCKACTORCAM;
		_asm
		{
			mov ecx, g_Input;
			call funcunlock;
		}
		g_iCursorEnabled = 0;
	}
}


#define HOOK_EXIT_ANTICARJACKED_HOOK	0xFB87
uint16_t	anticarjacked_vehid;
DWORD		anticarjacked_ebx_backup;
DWORD		anticarjacked_jmp;
uint8_t _declspec ( naked ) carjacked_hook ( void )
{
	__asm mov anticarjacked_ebx_backup, ebx
	__asm mov ebx, [ebx + 7]
	__asm mov anticarjacked_vehid, bx
	__asm pushad
	cheat_state->_generic.anti_carjackTick = GetTickCount();
	cheat_state->_generic.car_jacked = true;

	if ( g_Vehicles != NULL && g_Vehicles->pGTA_Vehicle[anticarjacked_vehid] != NULL )
		vect3_copy( &g_Vehicles->pGTA_Vehicle[anticarjacked_vehid]->base.matrix[4 * 3],
					cheat_state->_generic.car_jacked_lastPos );

	__asm popad
	__asm mov ebx, g_dwSAMP_Addr
	__asm add ebx, HOOK_EXIT_ANTICARJACKED_HOOK
	__asm mov anticarjacked_jmp, ebx
	__asm xor ebx, ebx
	__asm mov ebx, anticarjacked_ebx_backup
	__asm jmp anticarjacked_jmp
}

#define HOOK_EXIT_SERVERMESSAGE_HOOK	0x5F641
int		g_iNumPlayersMuted = 0;
bool	g_bPlayerMuted[SAMP_PLAYER_MAX];
uint8_t _declspec ( naked ) server_message_hook ( void )
{
	int		thismsg;
	DWORD	thiscolor;

	__asm mov thismsg, esi
	__asm mov thiscolor, eax
	thiscolor = ( thiscolor >> 8 ) | 0xFF000000;

	static char		last_servermsg[256];
	static DWORD	allow_show_again;
	if ( !set.anti_spam || cheat_state->_generic.cheat_panic_enabled
	 || (strcmp(last_servermsg, (char *)thismsg) != NULL || GetTickCount() > allow_show_again) )
	{
		// might be a personal message by muted player - look for name in server message
		// ignore message, if name was found
		if ( set.anti_spam && g_iNumPlayersMuted > 0 )
		{
			int i, j;
			char *playerName = NULL;
			for ( i = 0, j = 0; i < SAMP_PLAYER_MAX && j < g_iNumPlayersMuted; i++ )
			{
				if ( g_bPlayerMuted[i] )
				{
					playerName = (char*)getPlayerName(i);

					if ( playerName == NULL )
					{
						// Player not connected anymore - remove player from muted list
						g_bPlayerMuted[i] = false;
						g_iNumPlayersMuted--;
						continue;
					}
					else if ( strstr((char*)thismsg, playerName) != NULL )
						goto ignoreThisServChatMsg;
					j++;
				}
			}
		}
		strncpy_s( last_servermsg, (char *)thismsg, sizeof(last_servermsg)-1 );
		addToChatWindow( (char *)thismsg, thiscolor );
		allow_show_again = GetTickCount() + 5000;
		
		if( set.chatbox_logging )
			LogChatbox( false, "%s", thismsg );
	}

ignoreThisServChatMsg:
	__asm mov ebx, g_dwSAMP_Addr
	__asm add ebx, HOOK_EXIT_SERVERMESSAGE_HOOK
	__asm jmp ebx
}

#define HOOK_EXIT_CLIENTMESSAGE_HOOK	0xCA9A
uint8_t _declspec ( naked ) client_message_hook ( void )
{
	static char last_clientmsg[SAMP_PLAYER_MAX][256];
	int			thismsg;
	uint16_t	id;

	__asm mov id, dx
	__asm lea edx, [esp+0x128]
	__asm mov thismsg, edx

	if ( id >= 0 && id <= SAMP_PLAYER_MAX )
	{
		if( id == g_Players->sLocalPlayerID )
		{
			addToChatWindow( (char*)thismsg, g_Chat->clTextColor, id );

			if( set.chatbox_logging )
				LogChatbox( false, "%s: %s", getPlayerName(id), thismsg );
			goto client_message_hook_jump_out;
		}

		static DWORD	allow_show_again = GetTickCount();
		if ( !set.anti_spam
		 ||  (strcmp(last_clientmsg[id], (char *)thismsg) != NULL || GetTickCount() > allow_show_again)
		 ||	 cheat_state->_generic.cheat_panic_enabled )
		{
			// ignore chat from muted players
			if ( set.anti_spam && g_iNumPlayersMuted > 0 && g_bPlayerMuted[id] )
				goto client_message_hook_jump_out;

			// nothing to copy anymore, after chatbox_logging, so copy this before
			strncpy_s( last_clientmsg[id], (char *)thismsg, sizeof(last_clientmsg[id])-1 );

			if( set.chatbox_logging )
				LogChatbox( false, "%s: %s", getPlayerName(id), thismsg );

			addToChatWindow( (char*)thismsg, g_Chat->clTextColor, id );
			allow_show_again = GetTickCount() + 5000;
		}
	}

client_message_hook_jump_out:;
	__asm mov ebx, g_dwSAMP_Addr
	__asm add ebx, HOOK_EXIT_CLIENTMESSAGE_HOOK
	__asm jmp ebx
}

#define HOOK_CALL_STREAMEDOUTINFO	0x91AC0
DWORD dwStreamedOutInfoOrigFunc;
float fStreamedOutInfoPosX, fStreamedOutInfoPosY, fStreamedOutInfoPosZ;
uint16_t wStreamedOutInfoPlayerID;
uint8_t _declspec ( naked ) StreamedOutInfo ( void )
{
	_asm
	{
		push eax
		mov eax, dword ptr [esp+12]
		mov fStreamedOutInfoPosX, eax
		mov eax, dword ptr [esp+16]
		mov fStreamedOutInfoPosY, eax
		mov eax, dword ptr [esp+20]
		mov fStreamedOutInfoPosZ, eax
		mov ax, word ptr [esp+24]
		mov wStreamedOutInfoPlayerID, ax
		pop eax
	}

	_asm pushad
	g_stStreamedOutInfo.iPlayerID[wStreamedOutInfoPlayerID] = (int)wStreamedOutInfoPlayerID;
	g_stStreamedOutInfo.fPlayerPos[wStreamedOutInfoPlayerID][0] = fStreamedOutInfoPosX;
	g_stStreamedOutInfo.fPlayerPos[wStreamedOutInfoPlayerID][1] = fStreamedOutInfoPosY;
	g_stStreamedOutInfo.fPlayerPos[wStreamedOutInfoPlayerID][2] = fStreamedOutInfoPosZ;
	_asm popad

	_asm
	{
		push eax
		mov eax, g_dwSAMP_Addr
		add eax, HOOK_CALL_STREAMEDOUTINFO
		mov dwStreamedOutInfoOrigFunc, eax
		pop eax

		jmp dwStreamedOutInfoOrigFunc
	}
}

#define SAMP_HOOKPOS_ServerMessage	0x5F62A
#define SAMP_HOOKPOS_ClientMessage	0xCA3D
#define SAMP_HOOK_STATECHANGE		0xFB76
#define SAMP_HOOK_StreamedOutInfo	0xDF4E
void installSAMPHooks ()
{
	if( g_SAMP == NULL )
		return;

	CDetour api;

	if ( set.anti_spam || set.chatbox_logging )
	{
		if ( memcmp_safe((uint8_t *)g_dwSAMP_Addr + SAMP_HOOKPOS_ServerMessage, hex_to_bin("6A00C1E8"), 4) )
		{
			if ( api.Create((uint8_t *) ((uint32_t) g_dwSAMP_Addr) + SAMP_HOOKPOS_ServerMessage,
							 (uint8_t *)server_message_hook, DETOUR_TYPE_JMP, 5) == 0 )
				Log( "Failed to hook ServerMessage." );
		}
		else
			Log( "Failed to hook ServerMessage (memcmp)" );

		if ( memcmp_safe((uint8_t *)g_dwSAMP_Addr + SAMP_HOOKPOS_ClientMessage, hex_to_bin("663BD175"), 4) )
		{
			if ( api.Create((uint8_t *) ((uint32_t) g_dwSAMP_Addr) + SAMP_HOOKPOS_ClientMessage,
							 (uint8_t *)client_message_hook, DETOUR_TYPE_JMP, 5) == 0 )
				Log( "Failed to hook ClientMessage." );
		}
		else
			Log( "Failed to hook ClientMessage (memcmp)" );
	}

	if ( set.anti_carjacking )
	{
		if ( memcmp_safe((uint8_t *)g_dwSAMP_Addr + SAMP_HOOK_STATECHANGE, hex_to_bin("6A0568E8"), 4) )
		{
			if ( api.Create((uint8_t *) ((uint32_t) g_dwSAMP_Addr) + SAMP_HOOK_STATECHANGE, (uint8_t *)carjacked_hook,
							 DETOUR_TYPE_JMP, 5) == 0 )
				Log( "Failed to hook StateChange." );
		}
		else
			Log( "Failed to hook StateChange (memcmp)" );
	}

	if ( memcmp_safe((uint8_t *)g_dwSAMP_Addr + SAMP_HOOK_StreamedOutInfo, hex_to_bin("E8"), 1) )
	{
		if ( api.Create((uint8_t *) ((uint32_t) g_dwSAMP_Addr) + SAMP_HOOK_StreamedOutInfo,
						 (uint8_t *)StreamedOutInfo, DETOUR_TYPE_CALL_FUNC, 5) == 0 )
			Log( "Failed to hook StreamedOutInfo." );
	}
	else
		Log( "Failed to hook StreamedOutInfo (memcmp)" );
}

#define SAMP_ONFOOTSENDRATE		0xDC098
#define SAMP_INCARSENDRATE		0xDC09C
#define SAMP_AIMSENDRATE		0xDC0A0
void setSAMPCustomSendRates ( int iOnFoot, int iInCar, int iAim, int iHeadSync )
{
	if ( !set.samp_custom_sendrates_enable )
		return;
	if ( g_dwSAMP_Addr == NULL )
		return;
	if ( g_SAMP == NULL )
		return;

	memcpy_safe( (void *)(g_dwSAMP_Addr + SAMP_ONFOOTSENDRATE), &iOnFoot, sizeof(int) );
	memcpy_safe( (void *)(g_dwSAMP_Addr + SAMP_INCARSENDRATE), &iInCar, sizeof(int) );
	memcpy_safe( (void *)(g_dwSAMP_Addr + SAMP_AIMSENDRATE), &iAim, sizeof(int) );
}

#define SAMP_DISABLE_NAMETAGS		0x671B0
#define SAMP_DISABLE_NAMETAGS_HP	0x661A0
int sampPatchDisableNameTags ( int iEnabled )
{
	static struct patch_set sampPatchEnableNameTags_patch =
	{
		"Remove player status",
		0,
		0,
		{
			{ 1, (void *)( (uint8_t *)g_dwSAMP_Addr + SAMP_DISABLE_NAMETAGS ), NULL, (uint8_t *)"\xC3", NULL },
			{ 1, (void *)( (uint8_t *)g_dwSAMP_Addr + SAMP_DISABLE_NAMETAGS_HP ), NULL, (uint8_t *)"\xC3", NULL }
		}
	};
	if ( iEnabled && !sampPatchEnableNameTags_patch.installed )
		return patcher_install( &sampPatchEnableNameTags_patch );
	else if ( !iEnabled && sampPatchEnableNameTags_patch.installed )
		return patcher_remove( &sampPatchEnableNameTags_patch );
	return NULL;
}

#define SAMP_SKIPSENDINTERIOR 0x63D3
int sampPatchDisableInteriorUpdate ( int iEnabled )
{
	static struct patch_set sampPatchDisableInteriorUpdate_patch =
	{
		"NOP sendinterior",
		0,
		0,
		{
			{ 1, (void *)( (uint8_t *)g_dwSAMP_Addr + SAMP_SKIPSENDINTERIOR ), NULL, (uint8_t *)"\xEB", NULL }
		}
	};

	if ( iEnabled && !sampPatchDisableInteriorUpdate_patch.installed )
		return patcher_install( &sampPatchDisableInteriorUpdate_patch );
	else if ( !iEnabled && sampPatchDisableInteriorUpdate_patch.installed )
		return patcher_remove( &sampPatchDisableInteriorUpdate_patch );
	return NULL;
}


// not used anymore
// fix the damn laggy screenshot function Kye
#define SAMP_NOPSCREENSHOT	0x67400


#define SAMP_NOPSCOREBOARDTOGGLEON			0x64819
#define SAMP_NOPSCOREBOARDTOGGLEONKEYLOCK	0x64822
int sampPatchDisableScoreboardToggleOn ( int iEnabled )
{
	static struct patch_set sampPatchDisableScoreboard_patch =
	{
		"NOP Scoreboard Functions",
		0,
		0,
		{
			{ 1, (void *)( (uint8_t *)g_dwSAMP_Addr + SAMP_NOPSCOREBOARDTOGGLEON ), NULL, (uint8_t *)"\x00", NULL },
			{ 1, (void *)( (uint8_t *)g_dwSAMP_Addr + SAMP_NOPSCOREBOARDTOGGLEONKEYLOCK ), NULL, (uint8_t *)"\x00", NULL }
		}
	};
	if ( iEnabled && !sampPatchDisableScoreboard_patch.installed )
		return patcher_install( &sampPatchDisableScoreboard_patch );
	else if ( !iEnabled && sampPatchDisableScoreboard_patch.installed )
		return patcher_remove( &sampPatchDisableScoreboard_patch );
	return NULL;
}

#define SAMP_CHATINPUTADJUST_Y				0x5F034
#define SAMP_CHATINPUTADJUST_X				0x60525
int sampPatchDisableChatInputAdjust ( int iEnabled )
{
	static struct patch_set sampPatchDisableChatInputAdj_patch =
	{
		"NOP Adjust Chat input box",
		0,
		0,
		{
			{ 6, (void *)( (uint8_t *)g_dwSAMP_Addr + SAMP_CHATINPUTADJUST_Y ), NULL, (uint8_t *)"\x90\x90\x90\x90\x90\x90", NULL },
			{ 7, (void *)( (uint8_t *)g_dwSAMP_Addr + SAMP_CHATINPUTADJUST_X ), NULL, (uint8_t *)"\x90\x90\x90\x90\x90\x90\x90", NULL }
		}
	};
	if ( iEnabled && !sampPatchDisableChatInputAdj_patch.installed )
		return patcher_install( &sampPatchDisableChatInputAdj_patch );
	else if ( !iEnabled && sampPatchDisableChatInputAdj_patch.installed )
		return patcher_remove( &sampPatchDisableChatInputAdj_patch );
	return NULL;
}
