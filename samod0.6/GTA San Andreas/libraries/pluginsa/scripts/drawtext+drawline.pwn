#include <a_singleplayer>

new text[5][256];
new _s@T[1024];

#define sprintf(%1) (format(_s@T,1024,%1),_s@T)
#define AddTextF(%1) AddText(sprintf(%1))

new alive = 0;

stock AddText(Text[])
{
	format(text[4],256,"%s",text[3]);
	format(text[3],256,"%s",text[2]);
	format(text[2],256,"%s",text[1]);
	format(text[1],256,"%s",text[0]);
	format(text[0],256,"%s",Text);
}

#define MAX_DRAWID (8)
#define MAX_DRAW_TEXTSIZE (256)

enum DrawInfo
{
	DI_ms_delay,
	DI_ms_howlong,
	DI_FontID,
	Float:DI_X,
	Float:DI_Y,
	DI_color,
	DI_text[MAX_DRAW_TEXTSIZE],
	DI_Taken
};

new Drawer[MAX_DRAWID][DrawInfo];

stock DrawTextTimed(FontID,Float:X,Float:Y,color,Text[],ms_howlong,ms_delay=0,DrawID=0)
{
	if(MAX_DRAWID <= DrawID)return -1;
	if(DrawID < 0)return -1;
	Drawer[DrawID][DI_ms_delay] = GetTickCount()+ms_delay;
	Drawer[DrawID][DI_ms_howlong] = GetTickCount()+ms_delay+ms_howlong;
	Drawer[DrawID][DI_X] = X;
	Drawer[DrawID][DI_Y] = Y;
	Drawer[DrawID][DI_color] = color;
	Drawer[DrawID][DI_FontID] = FontID;
	Drawer[DrawID][DI_Taken] = 1;
	format(Drawer[DrawID][DI_text],MAX_DRAW_TEXTSIZE,"%s",Text);
	return DrawID;
}

stock DrawTextTimedEx(FontID,Float:X,Float:Y,color,Text[],ms_howlong,ms_delay=0)
{
	for(new i = 0; i < MAX_DRAWID; ++i)
	{
	    if(Drawer[i][DI_Taken] == 0)
	    {
			return DrawTextTimed(FontID,X,Y,color,Text,ms_howlong,ms_delay,DrawID);
		}
	}
	return -1;
}

forward DrawTimedTexts();
public DrawTimedTexts()
{
	for(new i= 0,time = GetTickCount(); i < MAX_DRAWID; ++i)
	{
	    if(Drawer[i][DI_Taken] == 0)continue;
	    if(Drawer[i][DI_ms_delay] < time)
	    {
	        if(time < Drawer[i][DI_ms_howlong])
	        {
	        	SP_DrawText(Drawer[i][DI_FontID],Drawer[i][DI_X],Drawer[i][DI_Y],Drawer[i][DI_color],Drawer[i][DI_text]);
			}
			else
			{
				Drawer[i][DI_Taken] = 0;
			}
		}
	}
}
public OnScriptInit()
{
	AddTextF("OnScriptInit");
	return 1;
}

public OnScriptExit()
{
	AddTextF("OnScriptExit");
	return 1;
}

//#define PLAYER_STATE_NONE (0)
//#define PLAYER_STATE_ONFOOT (1)
//#define PLAYER_STATE_VEHICLE (2)
public OnPedHealthChange(Float:oldHealth,Float:newHealth)
{
	AddTextF("Your health changed from %.1f to %.1f",oldHealth,newHealth);
	if(newHealth > 0.0)
	{
	    alive=1;
	}
	return 1;
}

public OnPedDead()
{
	AddTextF("You died");
	return 1;
}

public OnPedStateChange(oldstate,newstate)
{
	AddTextF("Your state changed from %d to %.d",oldstate,newstate);
	if(newstate == 0)alive = 0;
	else alive = 1;
}

new id;

public OnPedVehicleEnter()
{
	AddTextF("Entered Vehicle");
	DrawTextTimed(id,50.0,100.0,0xFFFF0000,"3",1000,500,0);
	DrawTextTimed(id,50.0,110.0,0xFF00FF00,"2",1000,1500,1);
	DrawTextTimed(id,50.0,120.0,0xFF0000FF,"1",1000,2500,2);
	return 1;
}

public OnPedVehicleExit()
{
	AddTextF("Left Vehicle");
	return 1;
}

main()
{
	AddText("--");
	AddText("--");
	AddText("--");
	AddText("--");
	AddText("--");
	
	id = SP_AddFont("Lucida Console",8,FW_BOLD);
	
}

new depth,rf,width,height;

public DirectXRenderer()
{
	SP_DrawText(id,50.0,40.0,0x7700FFFF,text[4]);
	SP_DrawText(id,50.0,50.0,0x9900FFFF,text[3]);
	SP_DrawText(id,50.0,60.0,0xBB00FFFF,text[2]);
	SP_DrawText(id,50.0,70.0,0xDD00FFFF,text[1]);
	SP_DrawText(id,50.0,80.0,0xFF00FFFF,text[0]);
	SP_GetScreenInfo(depth,rf,height,width);
	SP_DrawText( id,50.0,90.0,0xFFFFFFFF,sprintf( "Depth: %d RF: %d RES: %dx%d FPS:%03.1f",depth,rf,width,height,SP_GetFPS() ) );
	DrawTimedTexts();
	if(alive)//create a cube around the player
	{
	    new Float:pos[5][3];
	    new Float:pA = SP_GetCurrentRotation(PED_PLAYER);
		SP_GetMovementSpeed(PED_PLAYER,pos[0][0],pos[0][1],pos[0][2],false);
		SP_DrawText( id,50.0,100.0,0xFFFFFFFF,sprintf( "X: %04.1f Y: %04.1f Z: %04.1f",pos[0][0]*100.0,pos[0][1]*100.0,pos[0][2]*100.0) );
        SP_GetMovementSpeed(PED_PLAYER,pos[0][0],pos[0][1],pos[0][2],true);
		SP_DrawText( id,50.0,110.0,0xFFFFFFFF,sprintf( "X: %04.1f Y: %04.1f Z: %04.1f",pos[0][0]*100.0,pos[0][1]*100.0,pos[0][2]*100.0) );
		SP_GetPosition(PED_PLAYER,pos[0][0],pos[0][1],pos[0][2],false);
		SP_DrawText( id,50.0,120.0,0xFFFFFFFF,sprintf( "X: %04.1f Y: %04.1f Z: %04.1f A: %03.1f V: %d",pos[0][0],pos[0][1],pos[0][2],pA, SP_GetVehicleCount() ) );
        SP_GetPosition(PED_PLAYER,pos[0][0],pos[0][1],pos[0][2],true);
		SP_DrawText( id,50.0,130.0,0xFFFFFFFF,sprintf( "X: %04.1f Y: %04.1f Z: %04.1f A: %03.1f V: %d",pos[0][0],pos[0][1],pos[0][2],pA, SP_GetVehicleCount() ) );
		SP_DrawText( id,50.0,140.0,0xFFFFFFFF,sprintf( "IsPlayerInVehicle: %d, GetVehicleModel: %d, GetCurrentGear: %d IsKeyDown(VK_TAB): %d GetVehicleGasPedal: %1.20f",SP_IsPlayerInVehicle(), SP_GetVehicleModel(SP_GetPedVehicle(PED_PLAYER)), SP_GetCurrentGear(SP_GetPedVehicle(PED_PLAYER)), SP_IsKeyDown(VK_TAB), SP_GetVehicleGasPedal(SP_GetPedVehicle(PED_PLAYER))) );

		GetXYAtAngleFromXY(pos[0][0],pos[0][1],5.0,pA-45.0,pos[1][0],pos[1][1]);
		GetXYAtAngleFromXY(pos[0][0],pos[0][1],5.0,pA+45.0,pos[2][0],pos[2][1]);
		GetXYAtAngleFromXY(pos[0][0],pos[0][1],5.0,pA+135.0,pos[3][0],pos[3][1]);
		GetXYAtAngleFromXY(pos[0][0],pos[0][1],5.0,pA-135.0,pos[4][0],pos[4][1]);

        pos[0][2] -= 2.0;
	    SP_DrawLine(pos[1][0],pos[1][1],pos[0][2],pos[2][0],pos[2][1],pos[0][2],0xFFFF0000);
	    SP_DrawLine(pos[2][0],pos[2][1],pos[0][2],pos[3][0],pos[3][1],pos[0][2],0xFFFF0000);
	    SP_DrawLine(pos[3][0],pos[3][1],pos[0][2],pos[4][0],pos[4][1],pos[0][2],0xFFFF0000);
	    SP_DrawLine(pos[4][0],pos[4][1],pos[0][2],pos[1][0],pos[1][1],pos[0][2],0xFFFF0000);
		pos[0][2] += 4.0;
	    SP_DrawLine(pos[1][0],pos[1][1],pos[0][2],pos[2][0],pos[2][1],pos[0][2],0xFFFF0000);
	    SP_DrawLine(pos[2][0],pos[2][1],pos[0][2],pos[3][0],pos[3][1],pos[0][2],0xFFFF0000);
	    SP_DrawLine(pos[3][0],pos[3][1],pos[0][2],pos[4][0],pos[4][1],pos[0][2],0xFFFF0000);
	    SP_DrawLine(pos[4][0],pos[4][1],pos[0][2],pos[1][0],pos[1][1],pos[0][2],0xFFFF0000);
        pos[0][2] -= 2.0;
	    SP_DrawLine(pos[1][0],pos[1][1],pos[0][2]-2.0,pos[1][0],pos[1][1],pos[0][2]+2.0,0xFFFF0000);
	    SP_DrawLine(pos[2][0],pos[2][1],pos[0][2]-2.0,pos[2][0],pos[2][1],pos[0][2]+2.0,0xFFFF0000);
	    SP_DrawLine(pos[3][0],pos[3][1],pos[0][2]-2.0,pos[3][0],pos[3][1],pos[0][2]+2.0,0xFFFF0000);
	    SP_DrawLine(pos[4][0],pos[4][1],pos[0][2]-2.0,pos[4][0],pos[4][1],pos[0][2]+2.0,0xFFFF0000);
	}
	return 1;
}

stock GetXYAtAngleFromXY(Float:x, Float:y, Float:distance, Float:angle, &Float:Rx, &Float:Ry)
{
	Rx = x + (distance * floatsin(-angle, degrees));
	Ry = y + (distance * floatcos(-angle, degrees));
}

