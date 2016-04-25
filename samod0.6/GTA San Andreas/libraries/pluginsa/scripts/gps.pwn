#include <a_singleplayer>
#include "RouteConnector.inc"

main()
{
    gps_AddPlayer(0);
    ChangeOPCNIURate(2);
}

public OnScriptExit()
{
    gps_RemovePlayer(0);
    return 1;
}

new Size = 0;
new Float:RoutePos[2048][3];
new calculating = 0;

public GPS_WhenRouteIsCalculated(routeid,node_id_array[],amount_of_nodes,Float:distance,Float:Polygon[],Polygon_Size)//Every processed Queue will be called here
{
	for(new i = 0; i < amount_of_nodes; ++i)
	{
	    GetNodePos(node_id_array[i],RoutePos[i][0],RoutePos[i][1],RoutePos[i][2]);
	}
	Size = amount_of_nodes;
	calculating = 0;
	return 1;
}

public OnPlayerClosestNodeIDChange(playerid,old_NodeID,new_NodeID)
{
	if(calculating)
	{
		if(GetTickCount()-calculating < 1000)
		{
			return 1;
		}
		calculating = 0;
  		RemoveAllRouteIDFromQueue();
	}
	new id = new_NodeID;
	if(id == (-1))
	{
		id = NearestPlayerNode(0);
	}
	CalculatePath(id,0);
	calculating = GetTickCount();
	return 1;
}

public DirectXRenderer()
{
	if(Size > 15)
	{
	    for(new i = 0; i < 30; ++i)
	    {
	        SP_DrawLine(RoutePos[i][0],RoutePos[i][1],RoutePos[i][2],RoutePos[i+1][0],RoutePos[i+1][1],RoutePos[i+1][2],0xFF00FF00);
	    }
	}
	else
	{
	    for(new i = 0; i < (Size-1); ++i)
	    {
	        SP_DrawLine(RoutePos[i][0],RoutePos[i][1],RoutePos[i][2],RoutePos[i+1][0],RoutePos[i+1][1],RoutePos[i+1][2],0xFF00FF00);
	    }
	}
	return 1;
}
