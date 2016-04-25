#include <a_singleplayer>

main()
{

}

stock getPositionFromElementOffset(vehicle,Float:offX,Float:offY,Float:offZ,&Float:x,&Float:y,&Float:z)
{
	new
		Float:vRightX,
		Float:vRightY,
		Float:vRightZ,
		Float:vFrontX,
		Float:vFrontY,
		Float:vFrontZ,
		Float:vUpX,
		Float:vUpY,
		Float:vUpZ,
		Float:vPosX,
		Float:vPosY,
		Float:vPosZ;
		
    SP_GetVehicleMatrix(vehicle,vRightX,vRightY,vRightZ,vFrontX,vFrontY,vFrontZ,vUpX,vUpY,vUpZ,vPosX,vPosY,vPosZ);

	x = offX * vRightX + offY * vFrontX + offZ * vUpX + vPosX;
	y = offX * vRightY + offY * vFrontY + offZ * vUpY + vPosY;
	z = offX * vRightZ + offY * vFrontZ + offZ * vUpZ + vPosZ;
	return 1;
}

//new id = -1;
//new _s@T[1024];
//#define sprintf(%1) (format(_s@T,1024,%1),_s@T)

public DirectXRenderer()
{
	//if(id == -1)
	//{
	//	id = SP_AddFont("Arial",10,FW_BOLD);
	//}
	if(SP_IsKeyDown(VK_F11))SP_ReloadMe();
	if(SP_IsKeyDown(VK_KEY_0) && SP_IsKeyDown(VK_LSHIFT))
	{
	    SP_SetPosition(PED_PLAYER,0.0,0.0,5.0,true);
	}
	if(SP_IsKeyDown(VK_RSHIFT) && SP_IsKeyDown(VK_F4))
	{
		SP_TERMINATE_GTA();
	}
    if(SP_GetVehicleModel(SP_GetPedVehicle(PED_PLAYER)) != 0)
    {
		new Float:pos[2][3];
		static color = 0xFF00FF00;
		SP_GetPosition(PED_PLAYER,pos[0][0],pos[0][1],pos[0][2],true);
		getPositionFromElementOffset(SP_GetPedVehicle(PED_PLAYER),0.0,2.0,0.5,pos[0][0],pos[0][1],pos[0][2]);
		getPositionFromElementOffset(SP_GetPedVehicle(PED_PLAYER),0.0,50.0,0.5,pos[1][0],pos[1][1],pos[1][2]);
		//SP_DrawText( id,50.0,130.0,0xFFFFFFFF,sprintf( "X: %04.1f Y: %04.1f Z: %04.1f A: %3.1f",pos[0][0],pos[0][1],pos[0][2],SP_GetCurrentRotation() ) );
		if(SP_IsKeyDown(VK_KEY_H))SP_ProcessLineOfSight(pos[0][0],pos[0][1],pos[0][2],pos[1][0],pos[1][1],pos[1][2],pos[1][0],pos[1][1],pos[1][2]);
	    SP_DrawLine(pos[0][0],pos[0][1],pos[0][2],pos[1][0],pos[1][1],pos[1][2],color);

		//getPositionFromElementOffset(4.0,0.0,-0.5,pos[0][0],pos[0][1],pos[0][2]);
		//getPositionFromElementOffset(4.0,300.0,-0.5,pos[1][0],pos[1][1],pos[1][2]);
		//SP_ProcessLineOfSight(pos[0][0],pos[0][1],pos[0][2],pos[1][0],pos[1][1],pos[1][2],pos[1][0],pos[1][1],pos[1][2]);
	    //SP_DrawLine(pos[0][0],pos[0][1],pos[0][2],pos[1][0],pos[1][1],pos[1][2],0xFF00FF00);

				//getPositionFromElementOffset(-4.0,0.0,-0.5,pos[0][0],pos[0][1],pos[0][2]);
				//getPositionFromElementOffset(-4.0,300.0,-0.5,pos[1][0],pos[1][1],pos[1][2]);
				//SP_ProcessLineOfSight(pos[0][0],pos[0][1],pos[0][2],pos[1][0],pos[1][1],pos[1][2],pos[1][0],pos[1][1],pos[1][2]);
			    //SP_DrawLine(pos[0][0],pos[0][1],pos[0][2],pos[1][0],pos[1][1],pos[1][2],0xFF00FF00);
	}
	return 1;
}
