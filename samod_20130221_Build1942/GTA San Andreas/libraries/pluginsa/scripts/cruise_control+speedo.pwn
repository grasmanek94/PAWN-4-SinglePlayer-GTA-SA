#include <a_singleplayer>

new _s@T[1024];

#define sprintf(%1) (format(_s@T,1024,%1),_s@T)

new alive = 0;

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

new id;

new D3DMAT:mat;
new VECT2:NeedlePos;
new VECT2:SpeedoPos;
new VECT2:axisSpeedo;
new VECT2:axisNeedle;
new TextureHolder:tSpeedo;
new TextureHolder:tNeedle;
new SpriteHolder:sSpeedo;
new SpriteHolder:sNeedle;

main()
{
	id = SP_AddFont("Lucida Console",8,FW_BOLD);
	
	//////////////////////
	mat = SP_CreateD3DMatrixHolder();
	
	SpeedoPos  	= SP_CreateVector2Holder();
	NeedlePos  	= SP_CreateVector2Holder();
	axisSpeedo 	= SP_CreateVector2Holder();
	axisNeedle 	= SP_CreateVector2Holder();
	tSpeedo 	= SP_CreateTextureHolder();
	tNeedle 	= SP_CreateTextureHolder();
	sSpeedo 	= SP_CreateSpriteHolder ();
	sNeedle 	= SP_CreateSpriteHolder ();

	SP_CreateTexture("Z:\\Games\\GTA San Andreas\\speedo.png",tSpeedo);
	SP_CreateTexture("Z:\\Games\\GTA San Andreas\\needle.png",tNeedle);
	SP_CreateSprite(sSpeedo);//speedo
	SP_CreateSprite(sNeedle);//needle

	new BackBufferH = SP_GetBackBufferHeight();
	new BackBufferW = SP_GetBackBufferWidth();
	
	SP_Vector2Set(NeedlePos,floatdiv(BackBufferW,1024),floatdiv(BackBufferH,768));
	SP_Vector2Set(SpeedoPos,750.0*floatdiv(BackBufferW,1024),float(BackBufferH)-(292.0*floatdiv(BackBufferH,768)));
}

public InitDXRenderer()
{
	if(SP_IsTextureNull(tNeedle))
	{
		SP_CreateTexture("Z:\\Games\\GTA San Andreas\\needle.png",tNeedle);
	}
	if(SP_IsTextureNull(tSpeedo))
	{
		SP_CreateTexture("Z:\\Games\\GTA San Andreas\\speedo.png",tSpeedo);
	}
	if(SP_IsSpriteNull(sNeedle))
	{
        SP_CreateSprite(sNeedle);//needle
	}
	if(SP_IsSpriteNull(sSpeedo))
	{
		SP_CreateSprite(sSpeedo);//speedo
	}
}

public UnInitDXRenderer()
{
	SP_Texture_Release(tSpeedo);
	SP_Texture_Release(tNeedle);
	SP_Sprite_Release(sSpeedo);
	SP_Sprite_Release(sNeedle);
}

#define M_PI 3.14159265358979323844

public DirectXRenderer()
{
	if(SP_GetHealth(0))
	    alive = 1;
	else
	    alive = 0;
	DrawTimedTexts();
	if(alive)//create a cube around the player
	{
	    new Float:vecspeed[3];

		if(SP_IsTextureNull(tNeedle))
		{
            SP_DrawText( id,50.0,100.0,0xFFFFFFFF, "tNeedle==NULL" );
		}else if(SP_IsTextureNull(tSpeedo))
		{
            SP_DrawText( id,50.0,100.0,0xFFFFFFFF, "tSpeedo==NULL" );
		}else if(SP_IsSpriteNull(sNeedle))
		{
            SP_DrawText( id,50.0,100.0,0xFFFFFFFF, "sNeedle==NULL" );
		}else if(SP_IsSpriteNull(sSpeedo))
		{
            SP_DrawText( id,50.0,100.0,0xFFFFFFFF, "sSpeedo==NULL" );
		}
		else if(SP_GetVehicleModel(SP_GetPedVehicle(PED_PLAYER)) != 0)
		{
			SP_GetMovementSpeed(PED_PLAYER,vecspeed[0],vecspeed[1],vecspeed[2],true);
			new Float:speed = floatsqroot(vecspeed[0]*vecspeed[0]+vecspeed[1]*vecspeed[1]+vecspeed[2]*vecspeed[2])*174.0;
			if ( speed > 275.0 )
				speed = 275.0;

			static bool:cc_enabled = false;
			static bool:press = false;
			static Float:mspeed = 10.0;
			static sent = 0;
			static status = 0;
			static vKEY_UP = VK_UP;
			static vKEY_DOWN = VK_DOWN;
			static bool:extended = true;
			static bool:hardware = false;
			
			if(SP_IsKeyDown(VK_F1))
			{
			    if(press == false)
			    {
			        cc_enabled = (!cc_enabled) ? true : false;
			        press = true;
			    }
			}
			else
			{
			    press = false;
			}
			
			if(SP_IsKeyDown(VK_OEM_PLUS))
			{
			    if(SP_IsKeyDown(VK_RSHIFT) || SP_IsKeyDown(VK_LSHIFT))
			    {
			        mspeed += 0.75;
			    }
			    else
			    mspeed += 0.075;
				if(mspeed > -2.0 && mspeed < 2.0)mspeed = 2.0;
			    if(mspeed > 350.0)mspeed = 350.0;
			}
			if(SP_IsKeyDown(VK_OEM_MINUS))
			{
			    if(SP_IsKeyDown(VK_RSHIFT) || SP_IsKeyDown(VK_LSHIFT))
			    {
			        mspeed -= 0.75;
			    }
			    else
			    mspeed -= 0.075;
			    if(mspeed > -2.0 && mspeed < 2.0)mspeed = -2.0;
			    if(mspeed < -100.0)mspeed = -100.0;
			}

			if(cc_enabled)
			{
		        if((speed < (mspeed-2.0) && mspeed > 0.0) || (speed > (mspeed+2.0) && mspeed < 0.0))
		        {
		            if(status == 0)
					{
			        	SP_EmulateKeyPress(KEYPRESS_UP,vKEY_DOWN,extended,hardware);
			        	SP_EmulateKeyPress(KEYPRESS_DOWN,vKEY_UP,extended,hardware);
						sent = 1;
			        	status = 1;
		        	}
		        }
		        else if((speed > (mspeed+2.0) && mspeed > 0.0) || (speed < (mspeed-2.0) && mspeed < 0.0))
		        {
		            if(status == 0)
		            {
			        	SP_EmulateKeyPress(KEYPRESS_UP,vKEY_UP,extended,hardware);
			        	SP_EmulateKeyPress(KEYPRESS_DOWN,vKEY_DOWN,extended,hardware);
			            sent = 2;
			            status = 1;
		            }
		        }
		        else
		        {
					status = 0;
					SP_EmulateKeyPress(KEYPRESS_UP,vKEY_UP,extended,hardware);
					SP_EmulateKeyPress(KEYPRESS_UP,vKEY_DOWN,extended,hardware);
		        }
			    SP_DrawText( id,25.0,25.0,0xFFFFFFFF, sprintf("Cruise Control ENABLED - F1 to switch. SPEED: %.2f CURRENT: %.2f | use (shift+) - + to change",mspeed, speed) );
			}
	        else if(sent == 1)
	        {
	            sent = 0;
	            status = 0;
	        }
	        else if(sent == 2)
	        {
	            sent = 0;
	            status = 0;
	        }
			else
			{
			    SP_DrawText( id,25.0,25.0,0xFFFFFFFF, sprintf("Cruise Control DISABLED - F1 to switch. SPEED: %.2f CURRENT: %.2f | use (shift+) - + to change",mspeed, speed) );
			}

			static Float:mult = 10.0;
			static Float:rotationNeedle = 0.0;

			rotationNeedle = ( (0.09* ( (speed*mult)*1.55 ) * M_PI) / 180.0 );
			rotationNeedle = rotationNeedle / 2.0;
			if ( rotationNeedle > 3.29 )
				rotationNeedle = 3.29;

			static Float:pos[2];
			SP_Vector2Get(SpeedoPos,pos[0],pos[1]);
			SP_Vector2Set(axisSpeedo,pos[0],pos[1]);
			SP_Vector2Get(NeedlePos,pos[0],pos[1]);
			SP_Vector2Set(axisNeedle, 130.00 * pos[0], 152.00 * pos[1] );

			SP_D3DMatrixTransformation2D( mat, VECT2:-1, 0.0, NeedlePos, axisNeedle, 0.0, axisSpeedo );
			SP_Sprite_Begin(sSpeedo, (1<<4) );
			SP_Sprite_SetTransform( sSpeedo, mat );
			SP_Sprite_Draw(sSpeedo, tSpeedo, RECT:-1, VECT3:-1, VECT3:-1, 0xCCFFFFFF );
			SP_Sprite_End(sSpeedo);

			SP_D3DMatrixTransformation2D( mat, VECT2:-1, 0.0, NeedlePos, axisNeedle, rotationNeedle, axisSpeedo );
			SP_Sprite_Begin(sNeedle, (1<<4) );
			SP_Sprite_SetTransform( sNeedle, mat );
			SP_Sprite_Draw(sNeedle, tNeedle, RECT:-1, VECT3:-1, VECT3:-1, 0xCCFFFFFF );
			SP_Sprite_End(sNeedle);
		}
	}
	return 1;
}

stock GetXYAtAngleFromXY(Float:x, Float:y, Float:distance, Float:angle, &Float:Rx, &Float:Ry)
{
	Rx = x + (distance * floatsin(-angle, degrees));
	Ry = y + (distance * floatcos(-angle, degrees));
}

