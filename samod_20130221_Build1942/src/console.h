#include <main.h>
#include <proxydll.h>

namespace SA
{
	namespace MOD
	{
		namespace Console
		{
			std::string string_format(const std::string fmt, ...) 
			{
				int size = 512;
				std::string str;
				va_list ap;
				while (1) {
					str.resize(size);
					va_start(ap, fmt);
					int n = vsnprintf((char *)str.c_str(), size, fmt.c_str(), ap);
					va_end(ap);
					if (n > -1 && n < size) {
						str.resize(n);
						return str;
					}
					if (n > -1)
						size = n + 1;
					else
						size *= 2;
				}
				return str;
			}

			namespace Colors
			{
				unsigned int Box						=0x96000000;
				unsigned int BoxOutline					=0x96000000;
				unsigned int TextBoxText				=0xFFFFFFFF;
				unsigned int TextBox					=0x00000000;
				unsigned int TextBoxOutline				=0x9600C800;
				unsigned int DefaultText				=0xFFFFFFFF;
				unsigned int ScrollbarBackground		=0x44444444;
				unsigned int ScrollbarOutline			=0x96FF0000;
				unsigned int ScrollIndicatorBackground	=0x96999999;
				unsigned int ScrollIndicatorOutline		=0x96333333;
			};//Colors

			namespace var
			{
				float lines = 0.0f;
				float startpos = 0.0f;
				float MaxDHeight;
				float StatusW;
				float StatusH;
				float StatusP;
				bool g_Console = false;
				bool ParseColorsInCommandLine = false;
				bool HasSeenWelcomeScreen = false;
				int LastEntry = 1;
				int Scroll = 0;
				double last_backspace;
				VideoMode mode;
				CD3DFont* font;
				std::vector < std::string > buffer;
				char string[512];
				std::string LastCommand;
				std::string TypedString;
			};//var
			
			void LogFromConsole ( const char *fmt, ... )
			{
				if ( !g_szWorkingDirectory ) return;

				SYSTEMTIME	time;
				va_list		ap;

				if ( g_flLogAll == NULL )
				{
					char	filename_all[512];
					snprintf( filename_all, sizeof(filename_all), "%s\\%s", g_szWorkingDirectory, "samod2_all.log" );

					g_flLogAll = fopen( filename_all, "a" );
					if ( g_flLogAll == NULL )
						return;
				}

				GetLocalTime( &time );
				fprintf( g_flLogAll, "[%02d-%02d-%02d || %02d:%02d:%02d.%03d] ", time.wDay, time.wMonth, time.wYear, time.wHour,
						 time.wMinute, time.wSecond, time.wMilliseconds );
				va_start( ap, fmt );
				vfprintf( g_flLogAll, fmt, ap );
				va_end( ap );
				fprintf( g_flLogAll, "\n" );
				fflush( g_flLogAll );
			}

			void AddLine(const char * string,bool ToLog = true)//process commands here
			{
				if(strlen(string) == 0)
					return;
				if(ToLog)
					LogFromConsole("[Console] %s",string);
				var::buffer.push_back(string);
				if(var::buffer.size() == 512)
					var::buffer.erase (var::buffer.begin());
			}

			namespace Commands
			{
				typedef int (*func_t)(std::string);
				typedef std::map<std::string,func_t> function_map;
				// fill the map
				function_map execute;
				bool Initialised = false;
				//
				#define COMMAND(a) int a(std::string arguments)

				COMMAND(quit)
				{
					ExitProcess(0);
					return 1;
				}
				COMMAND(help)
				{
					AddLine("--------[Available Commands]--------");	
					for(auto it = execute.begin(); it != execute.end(); ++it)
						AddLine(it->first.c_str());
					AddLine("------------------------------------");
					return 1;
				}
				COMMAND(text)
				{
					AddLine(arguments.c_str());
					return 1;
				}
				COMMAND(loadscript)
				{
					return LoadScript(arguments);
				}
				COMMAND(unloadscript)
				{
					return UnloadScript(arguments);
				}
				COMMAND(unloadscripts)
				{
					UnloadScripts();
					return 1;
				}
				COMMAND(reloadscript)
				{
					return ReLoadScript(arguments);
				}
				COMMAND(loadamxplugin)
				{
					return LoadPlugin(arguments,true);
				}
				COMMAND(loaddll)
				{
					return LoadPlugin(arguments,false);
				}
				COMMAND(unloadplugin)
				{
					return UnloadPlugin(arguments);
				}
				COMMAND(unloadplugins)
				{
					UnloadPlugins();
					return 1;
				}
				COMMAND(unloadall)
				{
					UnloadScripts();
					UnloadPlugins();
					return 1;
				}
				COMMAND(loadall)
				{
					LoadPlugins();
					LoadScripts();
					return 1;
				}
				COMMAND(reloadall)
				{
					UnloadScripts();
					UnloadPlugins();
					LoadPlugins();
					LoadScripts();
					return 1;
				}
				COMMAND(listscripts)
				{
					AddLine(string_format("------------[Loaded Scripts (%d)]------------",scripts.size()).c_str());
					AddLine("{FF00FFFF}[IDENTIFIER]{FFFFFFFF} | [LOADED]{FFFFFFFF} | {FFFFFF00}[FILENAME]");
					for(std::vector<ScriptInfo>::iterator it = scripts.begin(); it != scripts.end(); ++it) 
					{
						AddLine(string_format("{FF00FFFF}%s{FFFFFFFF} | {%s}%s{FFFFFFFF} | {FFFFFF00}%s",it->Identifier.c_str(),((it->Loaded) ? "FF00FF00" : "FFFF0000"),((it->Loaded) ? "Loaded" : "ERROR: Not Loaded"),it->FileNameAndPath.c_str()).c_str());
					}
					AddLine("---------------------------------------------");
					return 1;
				}
				COMMAND(listplugins)
				{
					AddLine(string_format("------------[Loaded Plugins (%d)]------------",plugins.size()).c_str());
					AddLine("{FF00FFFF}[IDENTIFIER]{FFFFFFFF} | [Is AMX plugin or DLL]{FFFFFFFF} | {FFFFFF00}[INSTANCE]");
					for(std::vector<DLLInfo>::iterator it = plugins.begin(); it != plugins.end(); ++it) 
					{
						AddLine(string_format("{FF00FFFF}%s{FFFFFFFF} | {%s}%s{FFFFFFFF} | {FFFFFF00}%x",it->Identifier.c_str(),((it->IsAMX) ? "FF00FF00" : "FFFF0000"),((it->IsAMX) ? "AMX Plugin" : "DLL"),it->Instance).c_str());
					}
					AddLine("---------------------------------------------");
					return 1;
				}
				COMMAND(gameinfo)
				{
					AddLine("------------[Game Info]------------");	
					AddLine(string_format("Peds: %d/%d",pPools->GetPedCount(),pool_actor->size).c_str());
					AddLine(string_format("Vehicles: %d/%d",pPools->GetVehicleCount(),pool_vehicle->size).c_str());
					AddLine(string_format("Objects: %d/%d",pPools->GetObjectCount(),pool_object->size).c_str());
					AddLine("-----------------------------------");
					return 1;
				}
				COMMAND(pawncmd)
				{
					cell
						retval,
						amx_addr,
						* amx_physAddr;

					for(std::vector<ScriptInfo>::iterator it = scripts.begin(); it != scripts.end(); ++it) 
					{
						if (!amx_FindPublic(&it->amx, "OnConsoleCommand", &PublicCall))
						{
							amx_PushString(&it->amx, &amx_addr, &amx_physAddr, arguments.c_str(), 0, 0);
							amx_Exec(&it->amx, &retval, PublicCall);
							amx_Release(&it->amx, amx_addr);
							if(retval != 0)
								return retval;
						}
					}				
					return 0;
				}
				COMMAND(addped)
				{
					if(arguments.size() > 0)
					{
						int x = atoi(arguments.c_str());
						pGameInterface->GetModelInfo ( x )->AddRef ( true );
						gsPed = pGameInterface->GetPools()->AddPed(static_cast < ePedModel > (x));
						if(gsPed != NULL)
						{
							gsPed->SetPosition(pPedSelf->GetPosition()->fX+2.0f,pPedSelf->GetPosition()->fY+2.0f,pPedSelf->GetPosition()->fZ+2.0f);
							return gsPed->GetArrayID();
						}					
						return -1;
					}
					return -2;	
				}
				COMMAND(addcped)
				{
					if(arguments.size() > 0)
					{
						int x = atoi(arguments.c_str());
						pGameInterface->GetModelInfo ( x )->AddRef ( true );
						gsPed = pGameInterface->GetPools()->AddCivilianPed(static_cast < ePedModel > (x));
						if(gsPed != NULL)
						{
							gsPed->SetPosition(pPedSelf->GetPosition()->fX+2.0f,pPedSelf->GetPosition()->fY+2.0f,pPedSelf->GetPosition()->fZ+2.0f);
							return gsPed->GetArrayID();
						}					
						return -1;
					}
					return -2;
				}
				COMMAND(lockme)
				{
					GTAfunc_LockActor(true);
					GTAfunc_TogglePlayerControllable(true);
					return 0;
				}
				COMMAND(unlockme)
				{
					GTAfunc_LockActor(false);
					GTAfunc_TogglePlayerControllable(false);
					return 0;
				}

				COMMAND(addvehicle)
				{
					if(arguments.size() > 0)
					{
						int x = atoi(arguments.c_str());
						pGameInterface->GetModelInfo ( x )->AddRef ( true );
						gsVehicle = pGameInterface->GetPools()->AddVehicle(static_cast < eVehicleTypes > (x));
						if(gsVehicle != NULL)
						{
							gsVehicle->SetPosition(pPedSelf->GetPosition()->fX+2.0f,pPedSelf->GetPosition()->fY+2.0f,pPedSelf->GetPosition()->fZ+2.0f);
							return gsVehicle->GetArrayID();
						}     
						return -1;
					}
					return -2; 
				}
				COMMAND(reloadconfiguration)
				{
					LoadSAModConfig();
					return 1;
				}
				COMMAND(displayconfiguration)
				{
					AddLine("--------[Configuration]--------");	
					for(auto it = samod2_config.begin(); it != samod2_config.end(); ++it)
						AddLine(string_format("\"{FF00FF00}%s{FFFFFFFF}\"=\"{FFFF0000}%s{FFFFFFFF}\"",it->first.c_str(),it->second.c_str()).c_str());
					AddLine("------------------------------------");
					return 1;
				}
				//
				void Init()
				{
					execute["quit"] = &quit;
					execute["text"] = &text;
					execute["load_script"] = &loadscript;
					execute["unload_script"] = &unloadscript;
					execute["reload_script"] = &reloadscript;
					execute["unload_all_scripts"] = &unloadscripts;
					execute["load_amxplugin"] = &loadamxplugin;
					execute["load_dll"] = &loaddll;
					execute["unload_plugin"] = &unloadplugin;
					execute["unload_all_plugins"] = &unloadplugins;
					execute["unload_all"] = &unloadall;
					execute["load_all"] = &loadall;
					execute["reload_all"] = &reloadall;
					execute["script_list"] = &listscripts;
					execute["plugin_list"] = &listplugins;
					execute["help"] = &help;
					execute["gameinfo"] = &gameinfo;
					execute["addped"] = &addped;
					execute["addcped"] = &addcped;
					execute["lockme"] = &lockme;
					execute["unlockme"] = &unlockme;
					execute["addvehicle"] = &addvehicle;
					execute["reload_config"] = &reloadconfiguration;
					execute["display_config"] = &displayconfiguration;
					Initialised = true;
				}
			};//commands

			void ProcessCommand(std::string str)
			{
				if(Commands::Initialised == false)
					Commands::Init();
				if(str.at(0) == '/')
				{
					int ret = Commands::pawncmd(str);
					if(!ret)
						AddLine(string_format("Unknown command: '%s'",str.c_str()).c_str());
					else
						AddLine(string_format("The command returned: %d",ret).c_str());
				}
				else
				{
					std::istringstream x(str);
					std::string command;
					std::getline(x, command, ' ');
					Commands::function_map::const_iterator it = Commands::execute.find(command);
					if( it == Commands::execute.end() )
					{
						AddLine(string_format("Unknown command: '%s'",command.c_str()).c_str());
						return;
					}
					std::string arguments;
					std::getline(x, arguments, '\0');
					int ret = (int)(*it->second)(arguments);
					if(ret != 1)
						AddLine(string_format("The command returned: %d",ret).c_str());
				}
				return;
			}

			void AddChar(char c)
			{
				char szInsert[2] = {c,0};
				var::TypedString.append(szInsert);
			}

			void MemSet ( void* dwDest, int cValue, unsigned int uiAmount )
			{
				memset ( dwDest, cValue, uiAmount );
			}

			void MemCpy ( void* dwDest, const void* dwSrc, unsigned int uiAmount )
			{
				memcpy ( dwDest, dwSrc, uiAmount );
			}
			void AllowWindowsCursorShowing ( bool bAllow )
			{
				/*
				0074821D   6A 00            PUSH 0
				0074821F   FF15 EC828500    CALL DWORD PTR DS:[<&USER32.ShowCursor>] ; USER32.ShowCursor
				00748225   6A 00            PUSH 0
				00748227   FF15 9C828500    CALL DWORD PTR DS:[<&USER32.SetCursor>]  ; USER32.SetCursor
				*/
				BYTE originalCode[16] = {0x6A, 0x00, 0xFF, 0x15, 0xEC, 0x82, 0x85, 0x00, 0x6A, 0x00, 0xFF, 0x15, 0x9C, 0x82, 0x85, 0x00};
				
				eGameVersion version = pGameInterface->GetGameVersion();

				DWORD ADDR_CursorHiding = 0;

				if(version == VERSION_EU_10)
					ADDR_CursorHiding = 0x74821D;
				else if(version == VERSION_US_10)
					ADDR_CursorHiding = 0x7481CD;

				if(ADDR_CursorHiding)
				{
					if ( bAllow )
					{
					
						MemSet ( (LPVOID)ADDR_CursorHiding, 0x90, 16 );
					}
					else
					{
						MemCpy ( (LPVOID)ADDR_CursorHiding, &originalCode, 16 );
					}
				}
			}
			int EmulateReleaseAllKeys()
			{
				// This structure will be used to create the keyboard
				// input event.
				INPUT ip;
				int sends = 0;
				// Set up a generic keyboard event.
				for(int i = 0; i < 256; ++i)
				{
					ip.type = INPUT_KEYBOARD;
					ip.ki.wScan = 0; // hardware scan code for key
					ip.ki.wVk = i; // virtual-key code for the  key
					ip.ki.time = 0;
					ip.ki.dwExtraInfo = 0xE234A6E8;
					ip.ki.dwFlags = KEYEVENTF_KEYUP;
					sends += SendInput(1, &ip, sizeof(INPUT));
				}
				for(int i = 0; i < 256; ++i)
				{
					ip.type = INPUT_KEYBOARD;
					ip.ki.wScan = 0; // hardware scan code for key
					ip.ki.wVk = i; // virtual-key code for the  key
					ip.ki.time = 0;
					ip.ki.dwExtraInfo = 0xE234A6E8;
					ip.ki.dwFlags = KEYEVENTF_KEYUP;
					ip.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
					sends += SendInput(1, &ip, sizeof(INPUT));
				}
				for(int i = 0; i < 256; ++i)
				{
					ip.type = INPUT_KEYBOARD;
					ip.ki.wScan = i; // hardware scan code for key
					ip.ki.wVk = 0; // virtual-key code for the  key
					ip.ki.time = 0;
					ip.ki.dwExtraInfo = 0xE234A6E8;
					ip.ki.dwFlags = KEYEVENTF_KEYUP;
					ip.ki.dwFlags |= KEYEVENTF_SCANCODE;
					sends += SendInput(1, &ip, sizeof(INPUT));
				}
				return sends;
			}
			bool ProcessConsole()
			{
				bool ALT = KEY_DOWN(VK_LMENU) || KEY_DOWN(VK_RMENU);
				bool CTRL = KEY_DOWN(VK_LCONTROL) || KEY_DOWN(VK_RCONTROL);
				if(!var::g_Console)
				{
					if(ALT && CTRL && KEY_PRESSED(VK_F10))
					{
						KEY_CONSUME(VK_F10);
						pGameInterface->GetSettings()->GetVideoModeInfo(&var::mode,pGameInterface->GetSettings()->GetCurrentVideoMode());
						var::mode.height /= 3;
						//GTAfunc_LockActor(true);
						GTAfunc_TogglePlayerControllable(true);
						var::lines = var::mode.height / 30;
						var::startpos = var::lines * 26.0f;//29
						var::font = new CD3DFont("Lucida Console",var::lines,FW_BOLD);
						var::lines *= 1.5f;//1.7f
						var::Scroll = 0;
						var::MaxDHeight = var::mode.height-(var::lines)-16.0f;
						var::font->Initialize(origIDirect3DDevice9);
						var::StatusW = 10.0;
						var::StatusP = 100.0;
						if(!var::HasSeenWelcomeScreen)
						{
							AddLine("Welcome to the {FFFF0000}SA MOD{FFFFFFFF} {33FFFFFF}<<BASIC>>{FFFFFFFF} console");
							#ifdef __DEBUGx
							AddLine("{FFFFFF00}Version 20130221 {FF00FFFF}Build 1942 {FFFFFFFF}[DEBUG VERSION]");
							#else
							AddLine("{FFFFFF00}Version 20130221 {FF00FFFF}Build 1942 {FFFFFFFF}[RELEASE]");
							#endif
							AddLine("-------------------------[KEYS]-------------------------");
							AddLine("{FF00FF00}CTRL + ALT + F10{FFFFFFFF} : Close console");
							AddLine("{FF00FF00}ARROW UP / ARROW DOWN{FFFFFFFF} : Set console text input to previous text / next text");
							AddLine("{FF00FF00}PAGE UP / PAGE DOWN{FFFFFFFF} : Scroll console log up / down");
							AddLine("{FF00FF00}TAB{FFFFFFFF} : To re-enter last used command");
							AddLine("-----------------------[COMMANDS]-----------------------");
							AddLine("Use '{FF00FF00}help{FFFFFFFF}' to see a list of available commands");
							AddLine("Use '{FF00FF00}/{FFFFFFFF}' to call pawn commands");
							AddLine("--------------------------------------------------------");
							var::HasSeenWelcomeScreen = true;
						}
						EmulateReleaseAllKeys();
						var::g_Console = true;
						var::LastEntry = 1;	
						
						//AllowWindowsCursorShowing ( true );
						//origIDirect3DDevice9->SetCursorPosition(var::mode.height,50,0);
						//origIDirect3DDevice9->ShowCursor( true );
						//ShowCursor(TRUE);
						//SetCursor(LoadCursor(NULL,IDC_ARROW));
						//ShowCursor(TRUE);
					}
				}
				else
				{
					if(CTRL && ALT && KEY_PRESSED(VK_F10))
					{
						KEY_CONSUME(VK_F10);
						var::g_Console = false;
						GTAfunc_TogglePlayerControllable(false);
						//AllowWindowsCursorShowing ( false );
						//origIDirect3DDevice9->ShowCursor( false );
						//ShowCursor(FALSE);
					}
					else
					{
						render->D3DBoxBorder(0.0f, 0.0f, (float)var::mode.width, (float)var::mode.height-14.0f, Colors::BoxOutline, Colors::Box); // edited
						render->D3DBoxBorder(0.0f, var::MaxDHeight, (float)var::mode.width-1.5f, var::lines+2.0f, Colors::TextBoxOutline, Colors::TextBox); // edited

						if(KEY_PRESSED(VK_PRIOR))//page up
						{
							var::Scroll+=3;
							if((unsigned int)(var::Scroll+18) >= var::buffer.size())
								var::Scroll = var::buffer.size()-18;
						}

						if(KEY_PRESSED(VK_NEXT))//page down
						{
							var::Scroll-=3;
							if(var::Scroll < 0)
								var::Scroll = 0;
						}

						if(var::buffer.size() > 18)
							var::StatusP = ((float)var::Scroll/((float)var::buffer.size()-18.0f));
						else
							var::StatusP = 1.0f;

						for(unsigned int i = (var::buffer.size()-1)-var::Scroll,j = i-18,x=0; j != i; --i)
						{
							if(i < 0)
								break;
							if(!var::buffer[i].empty())
							{
								var::font->Print(0.0,(var::startpos-((var::lines)*(x))),Colors::DefaultText,var::buffer[i].c_str(),true);
								++x;
							}
							else
								--j;
						}

						render->D3DBoxBorder((float)(var::mode.width-12), 0.0, (float)12, var::MaxDHeight, Colors::ScrollbarOutline, Colors::ScrollbarBackground);

						var::StatusH = ((var::MaxDHeight-1.0f)-((var::MaxDHeight-1.0f)*var::StatusP))-var::StatusW;
						if(var::StatusH < 0.0f)
							var::StatusH = 0.0f;
						render->D3DBoxBorder((float)(var::mode.width-11), var::StatusH , 10.0, var::StatusW, Colors::ScrollIndicatorOutline, Colors::ScrollIndicatorBackground);

						if(var::TypedString.size() > 0)
						{
							if(KEY_PRESSED(VK_BACK))
							{
								var::last_backspace = GetCounter();
								var::TypedString.erase(var::TypedString.end()-1,var::TypedString.end());
							}else
							if(KEY_DOWN(VK_BACK))
							{
								if(GetCounter()-var::last_backspace > 125)
								{
									var::last_backspace = GetCounter();
									var::TypedString.erase(var::TypedString.end()-1,var::TypedString.end());
								}
							}
							if(KEY_PRESSED(VK_RETURN) || KEY_PRESSED(VK_SEPARATOR))
							{
								ProcessCommand(var::TypedString);
								var::LastCommand.assign(var::TypedString);
								var::TypedString.clear();
								var::LastEntry = 1;
							}
						}
						if(var::TypedString.size() < 512)
						{							
							if(GetAsyncKeyState(VK_SHIFT))
							{
								if(KEY_PRESSED(49))AddChar('!');
								if(KEY_PRESSED(50))AddChar('@');
								if(KEY_PRESSED(51))AddChar('#');
								if(KEY_PRESSED(52))AddChar('$');
								if(KEY_PRESSED(53))AddChar('%');
								if(KEY_PRESSED(54))AddChar('^');
								if(KEY_PRESSED(55))AddChar('&');
								if(KEY_PRESSED(56))AddChar('*');
								if(KEY_PRESSED(57))AddChar('(');
								if(KEY_PRESSED(48))AddChar(')');

								for(int i = 65; i < 91; ++i)
									if(KEY_PRESSED(i))AddChar(i);

								if(KEY_PRESSED(VK_OEM_1))AddChar(':');
								if(KEY_PRESSED(VK_OEM_2))AddChar('?');
								if(KEY_PRESSED(VK_OEM_3))AddChar('~');
								if(KEY_PRESSED(VK_OEM_4))AddChar('{');
								if(KEY_PRESSED(VK_OEM_5))AddChar('|');
								if(KEY_PRESSED(VK_OEM_6))AddChar('}');
								if(KEY_PRESSED(VK_OEM_7))AddChar('"');
								if(KEY_PRESSED(VK_OEM_PLUS))AddChar('+');
								if(KEY_PRESSED(VK_OEM_COMMA))AddChar('<');
								if(KEY_PRESSED(VK_OEM_MINUS))AddChar('_');
								if(KEY_PRESSED(VK_OEM_PERIOD))AddChar('>');
							}
							else
							{
								for(int i = 48; i < 58; ++i)
									if(KEY_PRESSED(i))AddChar(i);
								for(int i = 65; i < 91; ++i)
									if(KEY_PRESSED(i))AddChar(i+32);

								if(KEY_PRESSED(VK_OEM_1))AddChar(';');
								if(KEY_PRESSED(VK_OEM_2))AddChar('/');
								if(KEY_PRESSED(VK_OEM_3))AddChar('`');
								if(KEY_PRESSED(VK_OEM_4))AddChar('[');
								if(KEY_PRESSED(VK_OEM_5))AddChar('\\');
								if(KEY_PRESSED(VK_OEM_6))AddChar(']');
								if(KEY_PRESSED(VK_OEM_7))AddChar('\'');
								if(KEY_PRESSED(VK_OEM_PLUS))AddChar('=');
								if(KEY_PRESSED(VK_OEM_COMMA))AddChar(',');
								if(KEY_PRESSED(VK_OEM_MINUS))AddChar('-');
								if(KEY_PRESSED(VK_OEM_PERIOD))AddChar('.');
							}

							if(KEY_PRESSED(VK_TAB))AddChar('\t');
							if(KEY_PRESSED(VK_SPACE))AddChar(' ');
							if(KEY_PRESSED(VK_MULTIPLY))AddChar('*');
							if(KEY_PRESSED(VK_ADD))AddChar('+');
							if(KEY_PRESSED(VK_SUBTRACT))AddChar('-');
							if(KEY_PRESSED(VK_DECIMAL))AddChar('.');
							if(KEY_PRESSED(VK_DIVIDE))AddChar('/');

							for(int i = 96; i < 106; ++i)
									if(KEY_PRESSED(i))AddChar(i-48);

						}
						if(KEY_PRESSED(VK_UP))
						{
							--var::LastEntry;
							if((unsigned int)(var::LastEntry*-1) > var::buffer.size()-1) 
								var::LastEntry = ((var::buffer.size()-1)*-1);
							var::TypedString = var::buffer.at(var::buffer.size()+(var::LastEntry-1));
						}

						if(KEY_PRESSED(VK_DOWN))
						{
							++var::LastEntry;
							if(var::LastEntry >= 1)
							{
								var::LastEntry = 1;
								var::TypedString.clear();
							}
							else
							{
								var::TypedString = var::buffer.at(var::buffer.size()+(var::LastEntry-1));
							}
						}

						if(KEY_PRESSED(VK_TAB))
							var::TypedString.assign(var::LastCommand);

						/*if (KEY_PRESSED (VK_LBUTTON ))
							AddLine(string_format("MOUSE L PRESSED").c_str(),false);
						if (KEY_PRESSED (VK_RBUTTON))
							AddLine(string_format("MOUSE R PRESSED").c_str(),false);
						if (KEY_PRESSED (VK_MBUTTON))
							AddLine(string_format("MOUSE M PRESSED").c_str(),false);
						if (KEY_RELEASED (VK_LBUTTON))
							AddLine(string_format("MOUSE L RELEASED").c_str(),false);
						if (KEY_RELEASED (VK_RBUTTON))
							AddLine(string_format("MOUSE R RELEASED").c_str(),false);
						if (KEY_RELEASED (VK_MBUTTON))
							AddLine(string_format("MOUSE M RELEASED").c_str(),false);
						if (KEY_DOWN (VK_LBUTTON))
							AddLine(string_format("MOUSE L DOWN").c_str(),false);
						if (KEY_DOWN (VK_RBUTTON))
							AddLine(string_format("MOUSE R DOWN").c_str(),false);
						if (KEY_DOWN (VK_MBUTTON))
							AddLine(string_format("MOUSE M DOWN").c_str(),false);*/

						if(var::TypedString.size() > 0)
							var::font->Print(0.0,((float)var::mode.height-12.0f)-var::lines,Colors::TextBoxText,var::TypedString.c_str(),var::ParseColorsInCommandLine);
					}
				}
				return var::g_Console;
			}
		};//console
	};//mod
};//sa