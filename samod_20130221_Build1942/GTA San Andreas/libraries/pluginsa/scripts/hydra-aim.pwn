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

public DirectXRenderer()
{
	if(SP_IsKeyDown(VK_F11))SP_ReloadMe();
	//if(SP_IsKeyDown(VK_KEY_0))
	//{
	//    SP_SetPosition(0.0,0.0,5.0,true);
	//}
    if(SP_GetVehicleModel(SP_GetPedVehicle(PED_PLAYER)) == 520)
    {
	    new Float:pos[2][3];
	    new Float:SPEED;
		SP_GetPosition(PED_PLAYER,pos[0][0],pos[0][1],pos[0][2],true);
        SPEED = floatsqroot(pos[1][0]*pos[1][0]+pos[1][1]*pos[1][1]+pos[1][2]*pos[1][2]);
		new Float:Dist = (10.0)*((SPEED+1.0)*25.0);
		getPositionFromElementOffset(SP_GetPedVehicle(PED_PLAYER),0.0,Dist,0.0,pos[1][0],pos[1][1],pos[1][2]);
	    SP_DrawLine(pos[0][0],pos[0][1],pos[0][2],pos[1][0],pos[1][1],pos[1][2],0xFFFF0000);
		getPositionFromElementOffset(SP_GetPedVehicle(PED_PLAYER),4.0,-0.5,-0.5,pos[0][0],pos[0][1],pos[0][2]);
		getPositionFromElementOffset(SP_GetPedVehicle(PED_PLAYER),4.0,Dist,-0.5,pos[1][0],pos[1][1],pos[1][2]);
	    SP_DrawLine(pos[0][0],pos[0][1],pos[0][2],pos[1][0],pos[1][1],pos[1][2],0xFF00FF00);
		getPositionFromElementOffset(SP_GetPedVehicle(PED_PLAYER),-4.0,-0.5,-0.5,pos[0][0],pos[0][1],pos[0][2]);
		getPositionFromElementOffset(SP_GetPedVehicle(PED_PLAYER),-4.0,Dist,-0.5,pos[1][0],pos[1][1],pos[1][2]);
	    SP_DrawLine(pos[0][0],pos[0][1],pos[0][2],pos[1][0],pos[1][1],pos[1][2],0xFF00FF00);
	}
	return 1;
}
