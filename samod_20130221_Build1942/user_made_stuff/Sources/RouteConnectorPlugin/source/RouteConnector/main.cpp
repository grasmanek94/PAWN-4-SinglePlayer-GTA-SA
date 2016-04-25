/*
		ROUTER PLUGIN
		- GPS ADDITION TO SA-MP
		- Made By Gamer_Z a.k.a. grasmanek94 , Rafal Grasman

		October-2011

		contact: gzx@live.nl

		http://gpb.googlecode.com/
*/

//Definitions
#include "defines.h"

//System
#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <algorithm>
#include <concurrent_queue.h>
#include <bitset>
#include <list>
#include <map>
#include <stdarg.h>
#include <math.h>

//SDK
#include "plugin.h"

//Pathfinder
#include "PathFinder.h"

//Variables
#include "main.h"
#include "DPoint.h"

//The plugin
#include "natives.h"
#include "functions.h"

PLUGIN_EXPORT bool PLUGIN_CALL Load( void **ppData ) 
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	for(int i = 0; i < MAX_NODES; ++i)
	{
		xNode[i].Exists = false;
		xNode[i].AddedToGraph = false;
		xNode[i].NodeID = NULL;
		for(int j = 0; j < MAX_CONNECTIONS; ++j)
		{
			xNode[i].CW[j].ID = (-1);
		}
	}
	for(int i = 0; i < MAX_PLAYERS; ++i)
	{
		CheckPlayerOCNIDC[i] = true;
	}

	Functions::ReadNodeData("scriptfiles/GPS.dat");
	for(int i = 0; i < MAX_NODES; ++i)
	{
		Thread_xNode[i].xPOS = xNode[i].xPOS;
		Thread_xNode[i].yPOS = xNode[i].yPOS;
	}
	std::cout << "---Loading---\r\n\tGamer_Z's Project Bundle: \r\n\t\tRoute Connector (a.k.a. GPS) R" << PLUGIN_VERSION << "\r\n---LOADED---";
	START_THREAD( BackgroundCalculator );
	return true;
}


#ifdef OS_WINDOWS
	void BackgroundCalculator( void *unused )
#else
	void *BackgroundCalculator( void *unused )
#endif
{
	std::vector <cell>	CalculatedPath;
	cell CalculatedPathCost;
	QuedData RecievedData;
	std::vector <cell>	PolygonTemp;
	while( true )
    {
		if(QueueVector.try_pop(RecievedData))
		{
			dgraph->findPath_r(xNode[RecievedData.start].NodeID ,xNode[RecievedData.end].NodeID,CalculatedPath,CalculatedPathCost);
			dgraph->reset();

			if(RecievedData.CreatePolygon == 1)
			{
				if(CalculatedPath.size() > 3)
				{
					//black-magic-geometry-stuff
					//if you read this you have no life.
					DPoint temp[4];
					temp[0] = DPoint(Thread_xNode[CalculatedPath.at(0)].xPOS,Thread_xNode[CalculatedPath.at(0)].yPOS);
					temp[1] = DPoint(Thread_xNode[CalculatedPath.at(1)].xPOS,Thread_xNode[CalculatedPath.at(1)].yPOS);

					temp[2] = temp[0] - (function_r(temp[1] - temp[0]) * RecievedData.Width);
					temp[3] = temp[0] + (function_r(temp[1] - temp[0]) * RecievedData.Width);

					PolygonTemp.push_back(amx_ftoc(temp[2].X));
					PolygonTemp.push_back(amx_ftoc(temp[2].Y));
					PolygonTemp.push_back(amx_ftoc(temp[3].X));
					PolygonTemp.push_back(amx_ftoc(temp[3].Y));

					for(unsigned int k = 1; k < CalculatedPath.size()-2; ++k)
					{
						temp[0] = DPoint(Thread_xNode[CalculatedPath.at(k)].xPOS,Thread_xNode[CalculatedPath.at(k)].yPOS);
						temp[1] = DPoint(Thread_xNode[CalculatedPath.at(k+1)].xPOS,Thread_xNode[CalculatedPath.at(k+1)].yPOS);
						temp[2] = DPoint(Thread_xNode[CalculatedPath.at(k-1)].xPOS,Thread_xNode[CalculatedPath.at(k-1)].yPOS);
						
						temp[3] = temp[0] + (function_r(function_u(temp[1] - temp[0]) - function_u(temp[2] - temp[0])) * RecievedData.Width);
						
						PolygonTemp.push_back(amx_ftoc(temp[3].X));
						PolygonTemp.push_back(amx_ftoc(temp[3].Y));
					}

					temp[0] = DPoint(Thread_xNode[CalculatedPath.at(CalculatedPath.size()-1)].xPOS,Thread_xNode[CalculatedPath.at(CalculatedPath.size()-1)].yPOS);
					temp[1] = DPoint(Thread_xNode[CalculatedPath.at(CalculatedPath.size()-2)].xPOS,Thread_xNode[CalculatedPath.at(CalculatedPath.size()-2)].yPOS);

					temp[2] = temp[0] - (function_r(temp[1] - temp[0]) * RecievedData.Width);
					temp[3] = temp[0] + (function_r(temp[1] - temp[0]) * RecievedData.Width);

					PolygonTemp.push_back(amx_ftoc(temp[2].X));
					PolygonTemp.push_back(amx_ftoc(temp[2].Y));
					PolygonTemp.push_back(amx_ftoc(temp[3].X));
					PolygonTemp.push_back(amx_ftoc(temp[3].Y));

					for(unsigned int k = CalculatedPath.size()-2; k > 0; --k)
					{
						temp[0] = DPoint(Thread_xNode[CalculatedPath.at(k)].xPOS,Thread_xNode[CalculatedPath.at(k)].yPOS);
						temp[1] = DPoint(Thread_xNode[CalculatedPath.at(k+1)].xPOS,Thread_xNode[CalculatedPath.at(k+1)].yPOS);
						temp[2] = DPoint(Thread_xNode[CalculatedPath.at(k-1)].xPOS,Thread_xNode[CalculatedPath.at(k-1)].yPOS);

						temp[3] = temp[0] - (function_r(function_u(temp[1] - temp[0]) - function_u(temp[2] - temp[0])) * RecievedData.Width);
					
						PolygonTemp.push_back(amx_ftoc(temp[3].X));
						PolygonTemp.push_back(amx_ftoc(temp[3].Y));
					}
					//close the polygon
					PolygonTemp.push_back(PolygonTemp.at(0));
					PolygonTemp.push_back(PolygonTemp.at(1));
					//if you finished reading - go to hell, now();
				}
				else
				{
					PolygonTemp.push_back(0);
				}
			}
			else
			{
				PolygonTemp.push_back(0);
			}
			PassVector.push(PassData(CalculatedPath.begin(),CalculatedPath.end(),RecievedData.extraid,CalculatedPathCost,RecievedData.script,PolygonTemp.begin(),PolygonTemp.end()));
			CalculatedPath.clear();
			PolygonTemp.clear();
			CalculatedPathCost = 0;
		}
		SLEEP(5);
		//-------------------------
    }
}

PLUGIN_EXPORT void PLUGIN_CALL
	ProcessTick()
{
	if(PassVector.try_pop(LocalPass))
	{
		auto it = rcp_amxinfo.find(LocalPass.script);
		if (it != rcp_amxinfo.end()) 
		{
			if(it->second.GPSRouteCalculated.PublicFound)
			{
				amx_Push(LocalPass.script, LocalPass.Polygon.size());
				amx_PushArray(LocalPass.script, &ProcessTick_amxaddr, &ProcessTick_physAddr, LocalPass.Polygon.data(), LocalPass.Polygon.size());
				amx_Push(LocalPass.script, LocalPass.MoveCost);
				amx_Push(LocalPass.script, LocalPass.Paths.size());
				amx_PushArray(LocalPass.script, &ProcessTick_amxaddr, &ProcessTick_physAddr, LocalPass.Paths.data(), LocalPass.Paths.size());
				amx_Push(LocalPass.script, LocalPass.extraid);
				amx_Exec(LocalPass.script, NULL, it->second.GPSRouteCalculated.POINTER);
				amx_Release(LocalPass.script,ProcessTick_amxaddr);
			}
		}
	}
	if(g_Ticked++ == g_TickMax)
	{
		if(OnPCNIDCenable == true)
		{
			unsigned int playerid = 0;	
			unsigned int size = PlayerLoopList.size();
			for (unsigned int index = 0; index < size; ++index)
			{
				playerid = PlayerLoopList.at(index);
				
				if(CheckPlayerOCNIDC[playerid] == false)
				{
					continue;
				}
				int Nearest = -1;
				double prevdist = 4800000000.11f;//(40000*40000)*3+0.11
				double newdist;
				
				float X;			
				float Y;
				float Z;
				
				//Functions::GetPlayerPos(playerid,&X,&Y,&Z);
				GetPlayerPos(playerid,&X,&Y,&Z);
				if(UseRealClosestDistance == true)
				{
					ChangeNode[playerid].lastID = ChangeNode[playerid].newID;
					for(int i = 0; i < MAX_NODES; ++i)
					{
						if(xNode[i].Exists == false)
							continue;
						newdist = sqrt(pow(xNode[i].xPOS-X,2.0f)+pow(xNode[i].yPOS-Y,2.0f)+pow(xNode[i].zPOS-Z,2.0f));
						if(newdist < prevdist)
						{
							prevdist = newdist;
							Nearest = i;
						}
					}
				}
				else
				{
					int Xloc = 0, Yloc = 0;
					GetArea(Xloc,Yloc,X,Y);
					ChangeNode[playerid].lastID = ChangeNode[playerid].newID;
					for(int i = 0,j = (int)Area[Xloc][Yloc].size(); i < j; ++i)
					{
						int NDX = Area[Xloc][Yloc].at(i).NodeID;
						if(xNode[NDX].Exists == false)
							continue;
						newdist = sqrt(pow(xNode[NDX].xPOS-X,2.0f)+pow(xNode[NDX].yPOS-Y,2.0f)+pow(xNode[NDX].zPOS-Z,2.0f));
						if(newdist < prevdist)
						{
							prevdist = newdist;
							Nearest = NDX;
						}
					}
				}
				ChangeNode[playerid].newID = Nearest;
				if(ChangeNode[playerid].lastID != ChangeNode[playerid].newID && ChangeNode[playerid].lastID != (-1))
				{
					Functions::OnPlayerClosestNodeIDChange(playerid, ChangeNode[playerid].lastID,ChangeNode[playerid].newID);
				}
			}
		}
		g_Ticked = 0;
	}
}

//void AmxLoad_delay( AMX *amx );

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad( AMX *amx ) 
{
	//Code from sscanf2.5, [PAWN]SSCANF Author: Y_Less
	int
		num,
		idx;
	// Operate on the raw AMX file, don't use the amx_ functions to avoid issues
	// with the fact that we've not actually finished initialisation yet.  Based
	// VERY heavilly on code from "amx.c" in the PAWN runtime library.
	AMX_HEADER *
		hdr = (AMX_HEADER *)amx->base;
	AMX_FUNCSTUB *
		func;
	num = NUMENTRIES(hdr, natives, libraries);
	for (idx = 0; idx != num; ++idx)
	{
		func = GETENTRY(hdr, natives, idx);
		if (!strcmp("GetPlayerPos", GETENTRYNAME(hdr, func)))
		{
			GetPlayerPosAddress = func->address;
			FakeAMX = *amx;
			FakeAMX.data = ((unsigned char*)&GetPlayerPosPositionData) + (2*sizeof(cell));
			std::cout << "ADDR:: " << (int)FakeAMX.data << std::endl;
			func_GetPlayerPos = (n_GetPlayerPos*)GetPlayerPosAddress;
			break;
		}
	}
	//end sscanf cut
	Callbacks temp;
	if (!amx_FindPublic(amx, "OnPlayerClosestNodeIDChange", &temp.ClosestNodeIDChange.POINTER))
	{
		temp.ClosestNodeIDChange.PublicFound = true;
	}
	if (!amx_FindPublic(amx, "GPS_WhenRouteIsCalculated", &temp.GPSRouteCalculated.POINTER))
	{
		temp.GPSRouteCalculated.PublicFound = true;
	}
	rcp_amxinfo.insert(std::pair<AMX*,Callbacks>(amx,temp));
	//g_Invoke->amx_list.push_back(amx);
	//std::shared_ptr<void*> ptr(nullptr, [amx](void*){AmxLoad_delay(amx); });
	return amx_Register( amx, Natives::AMXNatives, -1 );
}
/*
void AmxLoad_delay( AMX *amx ) 
{
	Callbacks temp;
	if (!amx_FindPublic(amx, "OnPlayerClosestNodeIDChange", &temp.ClosestNodeIDChange.POINTER))
	{
		temp.ClosestNodeIDChange.PublicFound = true;
	}
	if (!amx_FindPublic(amx, "GPS_WhenRouteIsCalculated", &temp.GPSRouteCalculated.POINTER))
	{
		temp.GPSRouteCalculated.PublicFound = true;
	}
	rcp_amxinfo.insert(std::pair<AMX*,Callbacks>(amx,temp));
}
*/
PLUGIN_EXPORT int PLUGIN_CALL AmxUnload( AMX *amx ) 
{
	//g_Invoke->amx_list.remove(amx);
	rcp_amxinfo.erase(amx); 
	return AMX_ERR_NONE;
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() 
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload( )
{
	std::cout << "---Unloading---\r\n\tGamer_Z's Project Bundle: \r\n\t\tRoute Connector (a.k.a. GPS)\r\n---UNLOADED---";
}
