#include <a_singleplayer>

new _s@T[1024];

#define sprintf(%1) (format(_s@T,1024,%1),_s@T)
new id;

main()
{
	id = SP_AddFont("Lucida Console",8,FW_BOLD);
}

#define M_PI 3.14159265358979323844

public DirectXRenderer()
{
    static bool:block = false;
    if(block)return 1;
	if(SP_IsKeyDown(VK_F11))
	{
	    if(block == false)
	    {
	        block = true;
	        SP_ReloadMe();
	        return 1;
	    }
	}
	if(SP_IsKeyDown(VK_TAB))
	{
		SP_DrawText(id,50.0,170.0,0xFF00FFFF,"HOLDING");
	    SP_SetPosition(PED_PLAYER,0.0,0.0,7.0,false);
	}
	if(SP_GetVehicleModel(SP_GetPedVehicle(PED_PLAYER)) == 0)return 1;
	if(SP_IsKeyDown(VK_KEY_0))
	{
		SP_DrawText(id,50.0,170.0,0xFF00FFFF,"HOLDING");
	    SP_SetPosition(PED_PLAYER,0.0,0.0,7.0,true);
	}
	new Float:nil;
	new Float:XY,Float:ZY,Float:ZX;
	SP_GetVehicleRotation(SP_GetPedVehicle(PED_PLAYER),ROTATION_VECTOR_RIGHT,nil,nil,nil,nil,ZX,nil);
	SP_GetVehicleRotation(SP_GetPedVehicle(PED_PLAYER),ROTATION_VECTOR_FRONT,XY,nil,nil,nil,nil,nil);
	SP_GetVehicleRotation(SP_GetPedVehicle(PED_PLAYER),ROTATION_VECTOR_UP,nil,nil,ZY,nil,nil,nil);
	SP_DrawText(id,50.0,160.0,0xFF00FFFF,sprintf("R: %.0f  F: %.0f  U: %.0f",ZX,XY,ZY-90.0));

	//FRONT XY - Z angle , North = (-)180, East = 90, South = 0, West = -90
	//UP - ZY - up/down degrees - even = 180.0, to up = >180 , to down < 180, upsidedown = *-1
	//RIGHT ZX - rotation around car (-)180 - up, 0 - upside down
	return 1;
}
