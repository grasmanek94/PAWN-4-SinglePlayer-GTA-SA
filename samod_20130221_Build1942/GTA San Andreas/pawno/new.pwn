#include <a_singleplayer>//our SP include

public OScriptInit()//Inits when player spawns in game
{
	return 1;
}

public OnScriptExit()//When game exits
{
	return 1;
}

public ProcTick()//executed every 30 ms after ScriptInit
{
	return 1;
}

public OnPlayerHealthChange(Float:oldHealth,Float:newHealth)//...
{
	return 1;
}

public KeyBoardStateChange(wparam,lparam)//see keyboardhookproc from Microsoft for more details about WM_KEYBOARD SetHookEx lParam and wParam
{
	return 1;
}

main()//the main lol
{

}
