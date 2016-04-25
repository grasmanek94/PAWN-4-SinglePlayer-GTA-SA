/*  SA-MP Functions
 *
 *  (c) Copyright 2005-2012, SA-MP Team
 *
 */

#if defined _samp_included
	#endinput
#endif
#define _samp_included
#pragma library samp

// Limits and internal constants
#define MAX_PLAYER_NAME							(24)
#define MAX_PLAYERS								(1)
#define MAX_VEHICLES							(700)
#define INVALID_PLAYER_ID						(0xFFFF)
#define INVALID_VEHICLE_ID						(0xFFFF)
#define NO_TEAM									(255)
#define MAX_OBJECTS								(1000)
#define INVALID_OBJECT_ID						(0xFFFF)
#define MAX_GANG_ZONES							(1024)
#define MAX_TEXT_DRAWS							(2048)
#define MAX_PLAYER_TEXT_DRAWS					(256)
#define MAX_MENUS								(128)
#define MAX_3DTEXT_GLOBAL						(1024)
#define MAX_3DTEXT_PLAYER						(1024)
#define MAX_PICKUPS								(4096)
#define INVALID_MENU							(0xFF)
#define INVALID_TEXT_DRAW						(0xFFFF)
#define INVALID_GANG_ZONE						(-1)
#define INVALID_3DTEXT_ID						(0xFFFF)

// --------------------------------------------------
// Natives
// --------------------------------------------------

// Util
native SendClientMessage(playerid, color, const message[]);
native SendClientMessageToAll(color, const message[]);
native SendPlayerMessageToPlayer(playerid, senderid, const message[]);
native SendPlayerMessageToAll(senderid, const message[]);
native SendDeathMessage(killer,killee,weapon);
native GameTextForAll(const string[],time,style);
native GameTextForPlayer(playerid,const string[],time,style);
native SetTimer(funcname[], interval, repeating);
native SetTimerEx(funcname[], interval, repeating, const format[], {Float,_}:...);
native KillTimer(timerid);
native GetMaxPlayers();
native CallRemoteFunction(const function[], const format[], {Float,_}:...);
native CallLocalFunction(const function[], const format[], {Float,_}:...);
native Float:asin(Float:value);
native Float:acos(Float:value);
native Float:atan(Float:value);
native Float:atan2(Float:x, Float:y);

// Game
native SetGameModeText(const string[]);
native SetTeamCount(count);
native AddPlayerClass(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo);
native AddPlayerClassEx(teamid, modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo);
native AddStaticVehicle(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, color1, color2);
native AddStaticVehicleEx(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, color1, color2, respawn_delay);
native AddStaticPickup(model, type, Float:X, Float:Y, Float:Z, virtualworld = 0);
native CreatePickup(model, type, Float:X, Float:Y, Float:Z, virtualworld = 0);
native DestroyPickup(pickup);
native ShowNameTags(show);
native ShowPlayerMarkers(mode);
native GameModeExit();
native SetWorldTime(hour);
native GetWeaponName(weaponid, const weapon[], len);
native EnableTirePopping(enable);
native AllowInteriorWeapons(allow);
native SetWeather(weatherid);
native SetGravity(Float:gravity);
native AllowAdminTeleport(allow);
native SetDeathDropAmount(amount);
native CreateExplosion(Float:X, Float:Y, Float:Z, type, Float:Radius);
native EnableZoneNames(enable);
native UsePlayerPedAnims();		// Will cause the players to use CJ running/walking animations
native DisableInteriorEnterExits();  // will disable all interior enter/exits in the game.
native SetNameTagDrawDistance(Float:distance); // Distance at which nametags will start rendering on the client.
native DisableNameTagLOS(); // Disables the nametag Line-Of-Sight checking
native LimitGlobalChatRadius(Float:chat_radius);
native LimitPlayerMarkerRadius(Float:marker_radius);

// Npc
native ConnectNPC(name[], script[]);
native IsPlayerNPC(playerid);

// Admin
native IsPlayerAdmin(playerid);
native Kick(playerid);
native Ban(playerid);
native BanEx(playerid, const reason[]);
native SendRconCommand(command[]);
native GetServerVarAsString(const varname[], buffer[], len);
native GetServerVarAsInt(const varname[]);
native GetServerVarAsBool(const varname[]);
native GetPlayerNetworkStats(playerid, retstr[], retstr_size);
native GetNetworkStats(retstr[], retstr_size);
native GetPlayerVersion(playerid, const version[], len); // Returns the SA-MP client revision as reported by the player

// Menu
native Menu:CreateMenu(const title[], columns, Float:x, Float:y, Float:col1width, Float:col2width = 0.0);
native DestroyMenu(Menu:menuid);
native AddMenuItem(Menu:menuid, column, const menutext[]);
native SetMenuColumnHeader(Menu:menuid, column, const columnheader[]);
native ShowMenuForPlayer(Menu:menuid, playerid);
native HideMenuForPlayer(Menu:menuid, playerid);
native IsValidMenu(Menu:menuid);
native DisableMenu(Menu:menuid);
native DisableMenuRow(Menu:menuid, row);
native Menu:GetPlayerMenu(playerid);

// Text Draw
native Text:TextDrawCreate(Float:x, Float:y, text[]);
native TextDrawDestroy(Text:text);
native TextDrawLetterSize(Text:text, Float:x, Float:y);
native TextDrawTextSize(Text:text, Float:x, Float:y);
native TextDrawAlignment(Text:text, alignment);
native TextDrawColor(Text:text, color);
native TextDrawUseBox(Text:text, use);
native TextDrawBoxColor(Text:text, color);
native TextDrawSetShadow(Text:text, size);
native TextDrawSetOutline(Text:text, size);
native TextDrawBackgroundColor(Text:text, color);
native TextDrawFont(Text:text, font);
native TextDrawSetProportional(Text:text, set);
native TextDrawSetSelectable(Text:text, set);
native TextDrawShowForPlayer(playerid, Text:text);
native TextDrawHideForPlayer(playerid, Text:text);
native TextDrawShowForAll(Text:text);
native TextDrawHideForAll(Text:text);
native TextDrawSetString(Text:text, string[]);

// Gang Zones
native GangZoneCreate(Float:minx, Float:miny, Float:maxx, Float:maxy);
native GangZoneDestroy(zone);
native GangZoneShowForPlayer(playerid, zone, color);
native GangZoneShowForAll(zone, color);
native GangZoneHideForPlayer(playerid, zone);
native GangZoneHideForAll(zone);
native GangZoneFlashForPlayer(playerid, zone, flashcolor);
native GangZoneFlashForAll(zone, flashcolor);
native GangZoneStopFlashForPlayer(playerid, zone);
native GangZoneStopFlashForAll(zone);

// Global 3D Text Labels
native Text3D:Create3DTextLabel(text[], color, Float:X, Float:Y, Float:Z, Float:DrawDistance, virtualworld, testLOS=0);
native Delete3DTextLabel(Text3D:id);
native Attach3DTextLabelToPlayer(Text3D:id, playerid, Float:OffsetX, Float:OffsetY, Float:OffsetZ);
native Attach3DTextLabelToVehicle(Text3D:id, vehicleid, Float:OffsetX, Float:OffsetY, Float:OffsetZ);
native Update3DTextLabelText(Text3D:id, color, text[]);

// Per-player 3D Text Labels
native PlayerText3D:CreatePlayer3DTextLabel(playerid, text[], color, Float:X, Float:Y, Float:Z, Float:DrawDistance, attachedplayer=INVALID_PLAYER_ID, attachedvehicle=INVALID_VEHICLE_ID, testLOS=0);
native DeletePlayer3DTextLabel(playerid, PlayerText3D:id);
native UpdatePlayer3DTextLabelText(playerid, PlayerText3D:id, color, text[]);

// Player GUI Dialog
#define DIALOG_STYLE_MSGBOX		0
#define DIALOG_STYLE_INPUT		1
#define DIALOG_STYLE_LIST		2
#define DIALOG_STYLE_PASSWORD	3

native ShowPlayerDialog(playerid, dialogid, style, caption[], info[], button1[], button2[]);

// --------------------------------------------------
// Defines
// --------------------------------------------------

// States
#define PLAYER_STATE_NONE						(0)
#define PLAYER_STATE_ONFOOT						(1)
#define PLAYER_STATE_DRIVER						(2)
#define PLAYER_STATE_PASSENGER					(3)
#define PLAYER_STATE_EXIT_VEHICLE				(4) // (used internally)
#define PLAYER_STATE_ENTER_VEHICLE_DRIVER		(5) // (used internally)
#define PLAYER_STATE_ENTER_VEHICLE_PASSENGER	(6) // (used internally)
#define PLAYER_STATE_WASTED						(7)
#define PLAYER_STATE_SPAWNED					(8)
#define PLAYER_STATE_SPECTATING					(9)

// Marker modes used by ShowPlayerMarkers()
#define PLAYER_MARKERS_MODE_OFF			(0)
#define PLAYER_MARKERS_MODE_GLOBAL		(1)
#define PLAYER_MARKERS_MODE_STREAMED	(2)

// Weapons
#define WEAPON_BRASSKNUCKLE				(1)
#define WEAPON_GOLFCLUB					(2)
#define WEAPON_NITESTICK				(3)
#define WEAPON_KNIFE					(4)
#define WEAPON_BAT						(5)
#define WEAPON_SHOVEL					(6)
#define WEAPON_POOLSTICK				(7)
#define WEAPON_KATANA					(8)
#define WEAPON_CHAINSAW					(9)
#define WEAPON_DILDO					(10)
#define WEAPON_DILDO2					(11)
#define WEAPON_VIBRATOR					(12)
#define WEAPON_VIBRATOR2				(13)
#define WEAPON_FLOWER					(14)
#define WEAPON_CANE						(15)
#define WEAPON_GRENADE					(16)
#define WEAPON_TEARGAS					(17)
#define WEAPON_MOLTOV					(18)
#define WEAPON_COLT45					(22)
#define WEAPON_SILENCED					(23)
#define WEAPON_DEAGLE					(24)
#define WEAPON_SHOTGUN					(25)
#define WEAPON_SAWEDOFF					(26)
#define WEAPON_SHOTGSPA					(27)
#define WEAPON_UZI						(28)
#define WEAPON_MP5						(29)
#define WEAPON_AK47						(30)
#define WEAPON_M4						(31)
#define WEAPON_TEC9						(32)
#define WEAPON_RIFLE					(33)
#define WEAPON_SNIPER					(34)
#define WEAPON_ROCKETLAUNCHER			(35)
#define WEAPON_HEATSEEKER				(36)
#define WEAPON_FLAMETHROWER				(37)
#define WEAPON_MINIGUN					(38)
#define WEAPON_SATCHEL					(39)
#define WEAPON_BOMB						(40)
#define WEAPON_SPRAYCAN					(41)
#define WEAPON_FIREEXTINGUISHER			(42)
#define WEAPON_CAMERA					(43)
#define WEAPON_PARACHUTE				(46)
#define WEAPON_VEHICLE					(49)
#define WEAPON_DROWN					(53)
#define WEAPON_COLLISION				(54)

// Keys
#define KEY_ACTION				(1)
#define KEY_CROUCH				(2)
#define KEY_FIRE				(4)
#define KEY_SPRINT				(8)
#define KEY_SECONDARY_ATTACK	(16)
#define KEY_JUMP				(32)
#define KEY_LOOK_RIGHT			(64)
#define KEY_HANDBRAKE			(128)
#define KEY_LOOK_LEFT			(256)
#define KEY_SUBMISSION			(512)
#define KEY_LOOK_BEHIND			(512)
#define KEY_WALK				(1024)
#define KEY_ANALOG_UP			(2048)
#define KEY_ANALOG_DOWN			(4096)
#define KEY_ANALOG_LEFT			(8192)
#define KEY_ANALOG_RIGHT		(16384)
#define KEY_YES					(65536)
#define KEY_NO					(131072)
#define KEY_CTRL_BACK			(262144)

#define KEY_UP					(-128)
#define KEY_DOWN				(128)
#define KEY_LEFT				(-128)
#define KEY_RIGHT				(128)

// --------------------------------------------------
// Forwards (Callback declarations)
// --------------------------------------------------

forward OnGameModeInit();
forward OnGameModeExit();
forward OnFilterScriptInit();
forward OnFilterScriptExit();
forward OnPlayerConnect(playerid);
forward OnPlayerDisconnect(playerid, reason);
forward OnPlayerSpawn(playerid);
forward OnPlayerDeath(playerid, killerid, reason);
forward OnVehicleSpawn(vehicleid);
forward OnVehicleDeath(vehicleid, killerid);
forward OnPlayerText(playerid, text[]);
forward OnPlayerCommandText(playerid, cmdtext[]);
forward OnPlayerRequestClass(playerid, classid);
forward OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
forward OnPlayerExitVehicle(playerid, vehicleid);
forward OnPlayerStateChange(playerid, newstate, oldstate);
forward OnPlayerEnterCheckpoint(playerid);
forward OnPlayerLeaveCheckpoint(playerid);
forward OnPlayerEnterRaceCheckpoint(playerid);
forward OnPlayerLeaveRaceCheckpoint(playerid);
forward OnRconCommand(cmd[]);
forward OnPlayerRequestSpawn(playerid);
forward OnObjectMoved(objectid);
forward OnPlayerObjectMoved(playerid, objectid);
forward OnPlayerPickUpPickup(playerid, pickupid);
forward OnVehicleMod(playerid, vehicleid, componentid);
forward OnEnterExitModShop(playerid, enterexit, interiorid);
forward OnVehiclePaintjob(playerid, vehicleid, paintjobid);
forward OnVehicleRespray(playerid, vehicleid, color1, color2);
forward OnVehicleDamageStatusUpdate(vehicleid, playerid);
forward OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat);
forward OnPlayerSelectedMenuRow(playerid, row);
forward OnPlayerExitedMenu(playerid);
forward OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
forward OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
forward OnRconLoginAttempt( ip[], password[], success );
forward OnPlayerUpdate(playerid);
forward OnPlayerStreamIn(playerid, forplayerid);
forward OnPlayerStreamOut(playerid, forplayerid);
forward OnVehicleStreamIn(vehicleid, forplayerid);
forward OnVehicleStreamOut(vehicleid, forplayerid);
forward OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
forward OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid);
forward OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid);
forward OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ);
forward OnPlayerClickTextDraw(playerid, Text:clickedid);
forward OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid);

#define CLICK_SOURCE_SCOREBOARD		0
forward OnPlayerClickPlayer(playerid, clickedplayerid, source);

#define EDIT_RESPONSE_CANCEL		0
#define EDIT_RESPONSE_FINAL			1
#define EDIT_RESPONSE_UPDATE		2

forward OnPlayerEditObject( playerid, playerobject, objectid, response,
Float:fX, Float:fY, Float:fZ, Float:fRotX, Float:fRotY, Float:fRotZ );

forward OnPlayerEditAttachedObject( playerid, response, index, modelid, boneid,
Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ,
Float:fRotX, Float:fRotY, Float:fRotZ,
Float:fScaleX, Float:fScaleY, Float:fScaleZ );

#define SELECT_OBJECT_GLOBAL_OBJECT	1
#define SELECT_OBJECT_PLAYER_OBJECT 2

forward OnPlayerSelectObject(playerid, type, objectid, modelid, Float:fX, Float:fY, Float:fZ);

// --------------------------------------------------
 /*  SA-MP threaded HTTP/1.0 client for pawn
 *
 *  (c) Copyright 2010, SA-MP Team
 *
 */

// HTTP requests
#define HTTP_GET						1
#define HTTP_POST						2
#define HTTP_HEAD						3

// HTTP error response codes
// These codes compliment ordinary HTTP response codes returned in 'response_code'
// (10x) (20x OK) (30x Moved) (40x Unauthorised) (50x Server Error)  
#define HTTP_ERROR_BAD_HOST				1
#define HTTP_ERROR_NO_SOCKET			2
#define HTTP_ERROR_CANT_CONNECT			3
#define HTTP_ERROR_CANT_WRITE			4
#define HTTP_ERROR_CONTENT_TOO_BIG		5
#define HTTP_ERROR_MALFORMED_RESPONSE	6

native HTTP(index, type, url[], data[], callback[]);

// example HTTP callback: public MyHttpResponse(index, response_code, data[]) { ... }

/*  SA-MP Object Functions
 *
 *  (c) Copyright 2005-2012, SA-MP Team
 *
 */

#if defined _objects_included
	#endinput
#endif
#define _objects_included
#pragma library objects

// Objects

native CreateObject(modelid, Float:X, Float:Y, Float:Z, Float:rX, Float:rY, Float:rZ, Float:DrawDistance = 0.0);
native AttachObjectToVehicle(objectid, vehicleid, Float:OffsetX, Float:OffsetY, Float:OffsetZ, Float:RotX, Float:RotY, Float:RotZ);
native AttachObjectToObject(objectid, attachtoid, Float:OffsetX, Float:OffsetY, Float:OffsetZ, Float:RotX, Float:RotY, Float:RotZ, SyncRotation = 1);
native AttachObjectToPlayer(objectid, playerid, Float:OffsetX, Float:OffsetY, Float:OffsetZ, Float:RotX, Float:RotY, Float:RotZ);
native SetObjectPos(objectid, Float:X, Float:Y, Float:Z);
native GetObjectPos(objectid, &Float:X, &Float:Y, &Float:Z);
native SetObjectRot(objectid, Float:RotX, Float:RotY, Float:RotZ);
native GetObjectRot(objectid, &Float:RotX, &Float:RotY, &Float:RotZ);
native IsValidObject(objectid);
native DestroyObject(objectid);
native MoveObject(objectid, Float:X, Float:Y, Float:Z, Float:Speed, Float:RotX = -1000.0, Float:RotY = -1000.0, Float:RotZ = -1000.0);
native StopObject(objectid);
native IsObjectMoving(objectid);
native EditObject(playerid, objectid);
native EditPlayerObject(playerid, objectid);
native SelectObject(playerid);
native CancelEdit(playerid);
native CreatePlayerObject(playerid, modelid, Float:X, Float:Y, Float:Z, Float:rX, Float:rY, Float:rZ, Float:DrawDistance = 0.0);
native AttachPlayerObjectToVehicle(playerid, objectid, vehicleid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:RotZ);
native SetPlayerObjectPos(playerid, objectid, Float:X, Float:Y, Float:Z);
native GetPlayerObjectPos(playerid, objectid, &Float:X, &Float:Y, &Float:Z);
native SetPlayerObjectRot(playerid, objectid, Float:RotX, Float:RotY, Float:RotZ);
native GetPlayerObjectRot(playerid, objectid, &Float:RotX, &Float:RotY, &Float:RotZ);
native IsValidPlayerObject(playerid, objectid);
native DestroyPlayerObject(playerid, objectid);
native MovePlayerObject(playerid, objectid, Float:X, Float:Y, Float:Z, Float:Speed, Float:RotX = -1000.0, Float:RotY = -1000.0, Float:RotZ = -1000.0);
native StopPlayerObject(playerid, objectid);
native IsPlayerObjectMoving(playerid, objectid);
native AttachPlayerObjectToPlayer(objectplayer, objectid, attachplayer, Float:OffsetX, Float:OffsetY, Float:OffsetZ, Float:rX, Float:rY, Float:rZ);

#define OBJECT_MATERIAL_SIZE_32x32		10
#define OBJECT_MATERIAL_SIZE_64x32		20
#define OBJECT_MATERIAL_SIZE_64x64		30
#define OBJECT_MATERIAL_SIZE_128x32		40
#define OBJECT_MATERIAL_SIZE_128x64		50
#define OBJECT_MATERIAL_SIZE_128x128	60
#define OBJECT_MATERIAL_SIZE_256x32		70
#define OBJECT_MATERIAL_SIZE_256x64		80
#define OBJECT_MATERIAL_SIZE_256x128	90
#define OBJECT_MATERIAL_SIZE_256x256	100
#define OBJECT_MATERIAL_SIZE_512x64		110
#define OBJECT_MATERIAL_SIZE_512x128	120
#define OBJECT_MATERIAL_SIZE_512x256	130
#define OBJECT_MATERIAL_SIZE_512x512	140

#define OBJECT_MATERIAL_TEXT_ALIGN_LEFT		0
#define OBJECT_MATERIAL_TEXT_ALIGN_CENTER	1
#define OBJECT_MATERIAL_TEXT_ALIGN_RIGHT	2

native SetObjectMaterial(objectid, materialindex, modelid, txdname[], texturename[], materialcolor=0);
native SetPlayerObjectMaterial(playerid, objectid, materialindex, modelid, txdname[], texturename[], materialcolor=0);

native SetObjectMaterialText(objectid, text[], materialindex = 0, materialsize = OBJECT_MATERIAL_SIZE_256x128, fontface[] = "Arial", fontsize = 24, bold = 1, fontcolor = 0xFFFFFFFF, backcolor = 0, textalignment = 0);
native SetPlayerObjectMaterialText(playerid, objectid, text[], materialindex = 0, materialsize = OBJECT_MATERIAL_SIZE_256x128, fontface[] = "Arial", fontsize = 24, bold = 1, fontcolor = 0xFFFFFFFF, backcolor = 0, textalignment = 0);
/*  SA-MP Player Functions
 *
 *  (c) Copyright 2005-2012, SA-MP Team
 *
 */

#if defined _players_included
	#endinput
#endif
#define _players_included
#pragma library players

#define SPECIAL_ACTION_NONE				0
#define SPECIAL_ACTION_DUCK				1
#define SPECIAL_ACTION_USEJETPACK		2
#define SPECIAL_ACTION_ENTER_VEHICLE	3
#define SPECIAL_ACTION_EXIT_VEHICLE     4
#define SPECIAL_ACTION_DANCE1			5
#define SPECIAL_ACTION_DANCE2			6
#define SPECIAL_ACTION_DANCE3			7
#define SPECIAL_ACTION_DANCE4			8
#define SPECIAL_ACTION_HANDSUP			10
#define SPECIAL_ACTION_USECELLPHONE		11
#define SPECIAL_ACTION_SITTING			12
#define SPECIAL_ACTION_STOPUSECELLPHONE 13
#define SPECIAL_ACTION_DRINK_BEER		20
#define SPECIAL_ACTION_SMOKE_CIGGY		21
#define SPECIAL_ACTION_DRINK_WINE		22
#define SPECIAL_ACTION_DRINK_SPRUNK		23
#define SPECIAL_ACTION_CUFFED			24

#define FIGHT_STYLE_NORMAL				4
#define FIGHT_STYLE_BOXING				5
#define FIGHT_STYLE_KUNGFU				6
#define FIGHT_STYLE_KNEEHEAD			7
#define FIGHT_STYLE_GRABKICK			15
#define FIGHT_STYLE_ELBOW				16

#define WEAPONSKILL_PISTOL 				0
#define WEAPONSKILL_PISTOL_SILENCED		1
#define WEAPONSKILL_DESERT_EAGLE		2
#define WEAPONSKILL_SHOTGUN				3
#define WEAPONSKILL_SAWNOFF_SHOTGUN		4
#define WEAPONSKILL_SPAS12_SHOTGUN		5
#define WEAPONSKILL_MICRO_UZI			6
#define WEAPONSKILL_MP5					7
#define WEAPONSKILL_AK47				8
#define WEAPONSKILL_M4					9
#define WEAPONSKILL_SNIPERRIFLE			10

#define WEAPONSTATE_UNKNOWN				-1
#define WEAPONSTATE_NO_BULLETS			0
#define WEAPONSTATE_LAST_BULLET			1
#define WEAPONSTATE_MORE_BULLETS		2
#define WEAPONSTATE_RELOADING			3

// Player
native SetSpawnInfo(playerid, team, skin, Float:x, Float:y, Float:z, Float:rotation, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo);
native SpawnPlayer(playerid);

// Player info
native SetPlayerPos(playerid, Float:x, Float:y, Float:z);
native SetPlayerPosFindZ(playerid, Float:x, Float:y, Float:z);
native GetPlayerPos(playerid, &Float:x, &Float:y, &Float:z);
native SetPlayerFacingAngle(playerid,Float:ang);
native GetPlayerFacingAngle(playerid,&Float:ang);
native IsPlayerInRangeOfPoint(playerid, Float:range, Float:x, Float:y, Float:z);
native Float:GetPlayerDistanceFromPoint(playerid, Float:X, Float:Y, Float:Z);
native IsPlayerStreamedIn(playerid, forplayerid);
native SetPlayerInterior(playerid,interiorid);
native GetPlayerInterior(playerid);
native SetPlayerHealth(playerid, Float:health);
native GetPlayerHealth(playerid, &Float:health);
native SetPlayerArmour(playerid, Float:armour);
native GetPlayerArmour(playerid, &Float:armour);
native SetPlayerAmmo(playerid, weaponslot, ammo);
native GetPlayerAmmo(playerid);
native GetPlayerWeaponState(playerid);
native GetPlayerTargetPlayer(playerid);
native SetPlayerTeam(playerid, teamid);
native GetPlayerTeam(playerid);
native SetPlayerScore(playerid,score);
native GetPlayerScore(playerid);
native GetPlayerDrunkLevel(playerid);
native SetPlayerDrunkLevel(playerid, level);
native SetPlayerColor(playerid,color);
native GetPlayerColor(playerid);
native SetPlayerSkin(playerid, skinid);
native GetPlayerSkin(playerid);
native GivePlayerWeapon(playerid, weaponid, ammo);
native ResetPlayerWeapons(playerid);
native SetPlayerArmedWeapon(playerid, weaponid);
native GetPlayerWeaponData(playerid, slot, &weapons, &ammo);
native GivePlayerMoney(playerid,money);
native ResetPlayerMoney(playerid);
native SetPlayerName(playerid, const name[]);
native GetPlayerMoney(playerid);
native GetPlayerState(playerid);
native GetPlayerIp(playerid, name[], len);
native GetPlayerPing(playerid);
native GetPlayerWeapon(playerid);
native GetPlayerKeys(playerid, &keys, &updown, &leftright);
native GetPlayerName(playerid, const name[], len);
native SetPlayerTime(playerid, hour, minute);
native GetPlayerTime(playerid, &hour, &minute);
native TogglePlayerClock(playerid, toggle);
native SetPlayerWeather(playerid, weather);
native ForceClassSelection(playerid);
native SetPlayerWantedLevel(playerid, level);
native GetPlayerWantedLevel(playerid);
native SetPlayerFightingStyle(playerid, style);
native GetPlayerFightingStyle(playerid);
native SetPlayerVelocity(playerid, Float:X, Float:Y, Float:Z);
native GetPlayerVelocity( playerid, &Float:X, &Float:Y, &Float:Z );
native PlayCrimeReportForPlayer(playerid, suspectid, crime);
native PlayAudioStreamForPlayer(playerid, url[], Float:posX = 0.0, Float:posY = 0.0, Float:posZ = 0.0, Float:distance = 50.0, usepos = 0);
native StopAudioStreamForPlayer(playerid);
native SetPlayerShopName(playerid, shopname[]);
native SetPlayerSkillLevel(playerid, skill, level);
native GetPlayerSurfingVehicleID(playerid);
native GetPlayerSurfingObjectID(playerid);
native RemoveBuildingForPlayer(playerid, modelid, Float:fX, Float:fY, Float:fZ, Float:fRadius);

// Attached to bone objects

#define MAX_PLAYER_ATTACHED_OBJECTS 10 // This is the number of attached indexes available ie 10 = 0-9

native SetPlayerAttachedObject(playerid, index, modelid, bone, Float:fOffsetX = 0.0, Float:fOffsetY = 0.0, Float:fOffsetZ = 0.0, Float:fRotX = 0.0, Float:fRotY = 0.0, Float:fRotZ = 0.0, Float:fScaleX = 1.0, Float:fScaleY = 1.0, Float:fScaleZ = 1.0, materialcolor1 = 0, materialcolor2 = 0);
native RemovePlayerAttachedObject(playerid, index);
native IsPlayerAttachedObjectSlotUsed(playerid, index);
native EditAttachedObject(playerid, index);

// Per-player TextDraws
native PlayerText:CreatePlayerTextDraw(playerid, Float:x, Float:y, text[]);
native PlayerTextDrawDestroy(playerid, PlayerText:text);
native PlayerTextDrawLetterSize(playerid, PlayerText:text, Float:x, Float:y);
native PlayerTextDrawTextSize(playerid, PlayerText:text, Float:x, Float:y);
native PlayerTextDrawAlignment(playerid, PlayerText:text, alignment);
native PlayerTextDrawColor(playerid, PlayerText:text, color);
native PlayerTextDrawUseBox(playerid, PlayerText:text, use);
native PlayerTextDrawBoxColor(playerid, PlayerText:text, color);
native PlayerTextDrawSetShadow(playerid, PlayerText:text, size);
native PlayerTextDrawSetOutline(playerid, PlayerText:text, size);
native PlayerTextDrawBackgroundColor(playerid, PlayerText:text, color);
native PlayerTextDrawFont(playerid, PlayerText:text, font);
native PlayerTextDrawSetProportional(playerid, PlayerText:text, set);
native PlayerTextDrawSetSelectable(playerid, PlayerText:text, set);
native PlayerTextDrawShow(playerid, PlayerText:text);
native PlayerTextDrawHide(playerid, PlayerText:text);
native PlayerTextDrawSetString(playerid, PlayerText:text, string[]);

// Per-player variable system (PVars)
native SetPVarInt(playerid, varname[], int_value);
native GetPVarInt(playerid, varname[]);
native SetPVarString(playerid, varname[], string_value[]);
native GetPVarString(playerid, varname[], string_return[], len);
native SetPVarFloat(playerid, varname[], Float:float_value);
native Float:GetPVarFloat(playerid, varname[]);
native DeletePVar(playerid, varname[]);

// PVar enumeration
#define PLAYER_VARTYPE_NONE			0
#define PLAYER_VARTYPE_INT			1
#define PLAYER_VARTYPE_STRING		2
#define PLAYER_VARTYPE_FLOAT		3

native GetPVarsUpperIndex(playerid);
native GetPVarNameAtIndex(playerid, index, ret_varname[], ret_len);
native GetPVarType(playerid, varname[]);

#define MAX_CHATBUBBLE_LENGTH 144
native SetPlayerChatBubble(playerid, text[], color, Float:drawdistance, expiretime);

// Player controls
native PutPlayerInVehicle(playerid, vehicleid, seatid);
native GetPlayerVehicleID(playerid);
native GetPlayerVehicleSeat(playerid);
native RemovePlayerFromVehicle(playerid);
native TogglePlayerControllable(playerid, toggle);
native PlayerPlaySound(playerid, soundid, Float:x, Float:y, Float:z);
native ApplyAnimation(playerid, animlib[], animname[], Float:fDelta, loop, lockx, locky, freeze, time, forcesync = 0);
native ClearAnimations(playerid, forcesync = 0);
native GetPlayerAnimationIndex(playerid); // return the index of any running applied animations (0 if none are running)
native GetAnimationName(index, animlib[], len1, animname[], len2); // get the animation lib/name for the index
native GetPlayerSpecialAction(playerid);
native SetPlayerSpecialAction(playerid,actionid);

// Player world/map related
native SetPlayerCheckpoint(playerid, Float:x, Float:y, Float:z, Float:size);
native DisablePlayerCheckpoint(playerid);
native SetPlayerRaceCheckpoint(playerid, type, Float:x, Float:y, Float:z, Float:nextx, Float:nexty, Float:nextz, Float:size);
native DisablePlayerRaceCheckpoint(playerid);
native SetPlayerWorldBounds(playerid,Float:x_max,Float:x_min,Float:y_max,Float:y_min);
native SetPlayerMarkerForPlayer(playerid, showplayerid, color);
native ShowPlayerNameTagForPlayer(playerid, showplayerid, show);

#define MAPICON_LOCAL			  0 // displays in the player's local are
#define MAPICON_GLOBAL			  1 // displays always
#define MAPICON_LOCAL_CHECKPOINT  2 // displays in the player's local area and has a checkpoint marker
#define MAPICON_GLOBAL_CHECKPOINT 3 // displays always and has a checkpoint marker

native SetPlayerMapIcon(playerid, iconid, Float:x, Float:y, Float:z, markertype, color, style = MAPICON_LOCAL);
native RemovePlayerMapIcon(playerid, iconid);

native AllowPlayerTeleport(playerid, allow);

// Player camera
native SetPlayerCameraPos(playerid,Float:x, Float:y, Float:z);

#define CAMERA_CUT	2
#define CAMERA_MOVE 1

native SetPlayerCameraLookAt(playerid, Float:x, Float:y, Float:z, cut = CAMERA_CUT);
native SetCameraBehindPlayer(playerid);
native GetPlayerCameraPos(playerid, &Float:x, &Float:y, &Float:z);
native GetPlayerCameraFrontVector(playerid, &Float:x, &Float:y, &Float:z);
native GetPlayerCameraMode(playerid);
native AttachCameraToObject(playerid, objectid);
native AttachCameraToPlayerObject(playerid, playerobjectid);
native InterpolateCameraPos(playerid, Float:FromX, Float:FromY, Float:FromZ, Float:ToX, Float:ToY, Float:ToZ, time, cut = CAMERA_CUT);
native InterpolateCameraLookAt(playerid, Float:FromX, Float:FromY, Float:FromZ, Float:ToX, Float:ToY, Float:ToZ, time, cut = CAMERA_CUT);

// Player conditionals
native IsPlayerConnected(playerid);
native IsPlayerInVehicle(playerid, vehicleid);
native IsPlayerInAnyVehicle(playerid);
native IsPlayerInCheckpoint(playerid);
native IsPlayerInRaceCheckpoint(playerid);

// Virtual Worlds
native SetPlayerVirtualWorld(playerid, worldid);
native GetPlayerVirtualWorld(playerid);

// Insane Stunts
native EnableStuntBonusForPlayer(playerid, enable);
native EnableStuntBonusForAll(enable);

// Spectating
#define SPECTATE_MODE_NORMAL	1
#define SPECTATE_MODE_FIXED		2
#define SPECTATE_MODE_SIDE		3

native TogglePlayerSpectating(playerid, toggle);
native PlayerSpectatePlayer(playerid, targetplayerid, mode = SPECTATE_MODE_NORMAL);
native PlayerSpectateVehicle(playerid, targetvehicleid, mode = SPECTATE_MODE_NORMAL);

// Recording for NPC playback
#define PLAYER_RECORDING_TYPE_NONE		0
#define PLAYER_RECORDING_TYPE_DRIVER	1
#define PLAYER_RECORDING_TYPE_ONFOOT	2

native StartRecordingPlayerData(playerid, recordtype, recordname[]);
native StopRecordingPlayerData(playerid);

native SelectTextDraw(playerid, hovercolor); // enables the mouse so the player can select a textdraw
native CancelSelectTextDraw(playerid);	// cancel textdraw selection with the mouse

 /*  SA-MP Native SQLite Database Functions
 *
 *  (c) Copyright 2007, SA-MP Team
 *
 */

#if defined _sampdb_included
	#endinput
#endif
#define _sampdb_included
#pragma library sampdb

native DB:db_open(name[]);
native db_close(DB:db);
native DBResult:db_query(DB:db,query[]);
native db_free_result(DBResult:dbresult);
native db_num_rows(DBResult:dbresult);
native db_next_row(DBResult:dbresult);
native db_num_fields(DBResult:dbresult);
native db_field_name(DBResult:dbresult, field, result[], maxlength);
native db_get_field(DBResult:dbresult, field, result[], maxlength);
native db_get_field_assoc(DBResult:dbresult, const field[], result[], maxlength);

/*  SA-MP Vehicle Functions
 *
 *  (c) Copyright 2005-2012, SA-MP Team
 *
 */

#if defined _vehicles_included
	#endinput
#endif
#define _vehicles_included
#pragma library vehicles

#define CARMODTYPE_SPOILER		0
#define CARMODTYPE_HOOD			1
#define CARMODTYPE_ROOF			2
#define CARMODTYPE_SIDESKIRT	3
#define CARMODTYPE_LAMPS		4
#define CARMODTYPE_NITRO		5
#define CARMODTYPE_EXHAUST		6
#define CARMODTYPE_WHEELS		7
#define CARMODTYPE_STEREO		8
#define CARMODTYPE_HYDRAULICS	9
#define CARMODTYPE_FRONT_BUMPER	10
#define CARMODTYPE_REAR_BUMPER	11
#define CARMODTYPE_VENT_RIGHT	12
#define CARMODTYPE_VENT_LEFT	13

#define VEHICLE_PARAMS_UNSET	-1
#define VEHICLE_PARAMS_OFF		0
#define VEHICLE_PARAMS_ON		1

// Vehicle
native CreateVehicle(vehicletype, Float:x, Float:y, Float:z, Float:rotation, color1, color2, respawn_delay);
native DestroyVehicle(vehicleid);
native IsVehicleStreamedIn(vehicleid, forplayerid);
native GetVehiclePos(vehicleid, &Float:x, &Float:y, &Float:z);
native SetVehiclePos(vehicleid, Float:x, Float:y, Float:z);
native GetVehicleZAngle(vehicleid, &Float:z_angle);
native GetVehicleRotationQuat(vehicleid, &Float:w, &Float:x, &Float:y, &Float:z);
native Float:GetVehicleDistanceFromPoint(vehicleid, Float:X, Float:Y, Float:Z);
native SetVehicleZAngle(vehicleid, Float:z_angle);
native SetVehicleParamsForPlayer(vehicleid,playerid,objective,doorslocked);
native ManualVehicleEngineAndLights();
native SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
native GetVehicleParamsEx(vehicleid, &engine, &lights, &alarm, &doors, &bonnet, &boot, &objective);
native SetVehicleToRespawn(vehicleid);
native LinkVehicleToInterior(vehicleid, interiorid);
native AddVehicleComponent(vehicleid, componentid);
native RemoveVehicleComponent(vehicleid, componentid);
native ChangeVehicleColor(vehicleid, color1, color2);
native ChangeVehiclePaintjob(vehicleid, paintjobid);
native SetVehicleHealth(vehicleid, Float:health);
native GetVehicleHealth(vehicleid, &Float:health);
native AttachTrailerToVehicle(trailerid, vehicleid);
native DetachTrailerFromVehicle(vehicleid);
native IsTrailerAttachedToVehicle(vehicleid);
native GetVehicleTrailer(vehicleid);
native SetVehicleNumberPlate(vehicleid, numberplate[]);
native GetVehicleModel(vehicleid);
native GetVehicleComponentInSlot(vehicleid, slot); // There is 1 slot for each CARMODTYPE_*
native GetVehicleComponentType(component); // Find CARMODTYPE_* for component id
native RepairVehicle(vehicleid); // Repairs the damage model and resets the health
native GetVehicleVelocity(vehicleid, &Float:X, &Float:Y, &Float:Z);
native SetVehicleVelocity(vehicleid, Float:X, Float:Y, Float:Z);
native SetVehicleAngularVelocity(vehicleid, Float:X, Float:Y, Float:Z);
native GetVehicleDamageStatus(vehicleid, &panels, &doors, &lights, &tires);
native UpdateVehicleDamageStatus(vehicleid, panels, doors, lights, tires);

#define VEHICLE_MODEL_INFO_SIZE				1
#define VEHICLE_MODEL_INFO_FRONTSEAT		2
#define VEHICLE_MODEL_INFO_REARSEAT			3
#define VEHICLE_MODEL_INFO_PETROLCAP		4
#define VEHICLE_MODEL_INFO_WHEELSFRONT		5
#define VEHICLE_MODEL_INFO_WHEELSREAR		6
#define VEHICLE_MODEL_INFO_WHEELSMID		7
#define VEHICLE_MODEL_INFO_FRONT_BUMPER_Z	8
#define VEHICLE_MODEL_INFO_REAR_BUMPER_Z	9

native GetVehicleModelInfo(vehiclemodel, infotype, &Float:X, &Float:Y, &Float:Z);

// Virtual Worlds
native SetVehicleVirtualWorld(vehicleid, worldid);
native GetVehicleVirtualWorld(vehicleid);
