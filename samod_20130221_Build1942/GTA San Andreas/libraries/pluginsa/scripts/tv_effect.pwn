#include <a_singleplayer>

new D3DMAT:Matrix;
new VECT2:TV_Pos;
new VECT3:Vector;

new TextureHolder:tTV_Flash;
new TextureHolder:tTV_Frame;

new SpriteHolder:sTV_Flash;
new SpriteHolder:sTV_Frame;

main()
{
	//////////////////////
	Matrix = SP_CreateD3DMatrixHolder();
	Vector = SP_CreateVector3Holder();
	TV_Pos = SP_CreateVector2Holder();
	
	tTV_Flash 	= SP_CreateTextureHolder();
	tTV_Frame 	= SP_CreateTextureHolder();
	
	sTV_Flash 	= SP_CreateSpriteHolder ();
	sTV_Frame 	= SP_CreateSpriteHolder ();

	SP_CreateTexture("Z:/Games/GTASA/libraries/tv_flash.png",tTV_Flash);
	SP_CreateTexture("Z:/Games/GTASA/libraries/tv_frame.png",tTV_Frame);
	
	SP_CreateSprite(sTV_Flash);
	SP_CreateSprite(sTV_Frame);
}

public InitDXRenderer()
{
	if(SP_IsTextureNull(tTV_Frame))
	{
		SP_CreateTexture("Z:/Games/GTASA/libraries/tv_frame.png",tTV_Frame);
	}
	if(SP_IsTextureNull(tTV_Flash))
	{
		SP_CreateTexture("Z:/Games/GTASA/libraries/tv_flash.png",tTV_Flash);
	}
	if(SP_IsSpriteNull(sTV_Frame))
	{
        SP_CreateSprite(sTV_Frame);
	}
	if(SP_IsSpriteNull(sTV_Flash))
	{
		SP_CreateSprite(sTV_Flash);
	}
}

public OnConsoleCommand(command[])
{
	return 0;
}

public DirectXRenderer(Float:TimeDiff)
{
	if(KeyHook_PRESSED(VK_F6))
	{
	    SomeCameraFunction();
	}
	if(SP_IsTextureNull(tTV_Frame))
	{
        print("tTV_Frame == NULL");
	}else if(SP_IsTextureNull(tTV_Flash))
	{
        print("tTV_Flash == NULL");
	}else if(SP_IsSpriteNull(sTV_Frame))
	{
        print("sTV_Frame == NULL");
	}else if(SP_IsSpriteNull(sTV_Flash))
	{
        print("sTV_Flash == NULL");
	}
	else
	{
	    static Width,Height,BackBufferH,BackBufferW,Float:y,Float:u;
	    
	    SP_GetScreenInfo(BackBufferH,BackBufferH,Height,Width);
		BackBufferH = SP_GetBackBufferHeight();
		BackBufferW = SP_GetBackBufferWidth();

		SP_Vector2Set(TV_Pos,floatdiv(BackBufferW,512),floatdiv(BackBufferH,1024));
        SP_D3DMatrixTransformation2D(Matrix,VECT2:-1,0.0,TV_Pos,VECT2:-1,0.0,VECT2:-1);
        
		SP_Sprite_Begin(sTV_Flash, (1<<4) );
		SP_Sprite_SetTransform(sTV_Flash,Matrix);

		SP_Vector3Get(Vector,u,y,u);
		y -= floatdiv(Height,1.0/3.0)*TimeDiff;
		if(y < 0)
		{
   			y = (Height+150);
		}
		SP_Vector3Set(Vector,0.0,y,0.0);

		SP_Sprite_Draw(sTV_Flash, tTV_Flash, RECT:-1, VECT3:-1, Vector, 0x44FFFFFF );
		SP_Sprite_End(sTV_Flash);
		
		SP_Vector2Set(TV_Pos,floatdiv(BackBufferW,1024),floatdiv(BackBufferH,512));
        SP_D3DMatrixTransformation2D(Matrix,VECT2:-1,0.0,TV_Pos,VECT2:-1,0.0,VECT2:-1);

		SP_Sprite_Begin(sTV_Frame, (1<<4) );
		SP_Sprite_SetTransform(sTV_Frame,Matrix);
		SP_Sprite_Draw(sTV_Frame, tTV_Frame, RECT:-1, VECT3:-1, VECT3:-1, 0xFFFFFFFF );
		SP_Sprite_End(sTV_Frame);
	}
	return 1;
}
