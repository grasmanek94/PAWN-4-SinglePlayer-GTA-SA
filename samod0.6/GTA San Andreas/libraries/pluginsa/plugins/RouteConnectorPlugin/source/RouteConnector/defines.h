#pragma once

#define PLUGIN_VERSION (182)

#define AREA_SIZE 75.0f//Define your Area size here lol.
/*
	remember to change 
vector			<NodesInfoScanner>				Area[x][x];
x = round_up(40000.0/AREA_SIZE)+1
*/

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#	include <process.h>
#	include <windows.h>
#	define OS_WINDOWS
#	define EXIT_THREAD() { _endthread(); }
#	define START_THREAD(a) { _beginthread( a, 0, (void *)( 0 ) ); }
#	define SLEEP(x) { Sleep(x); }
	static void						BackgroundCalculator		( void *unused );
#else
#	include <pthread.h>
#	define sscanf_s sscanf
#	define sprintf_s sprintf
#	define EXIT_THREAD() { pthread_exit( NULL ); }
#	define START_THREAD(a) {	pthread_t thread; pthread_create( &thread, NULL, a, (void *)( 0 ) ); }
#	define SLEEP(x) { usleep(x * 1000); }
	static void	*					BackgroundCalculator		( void *unused );
#endif
	
#define GetArea(a,b,c,d);  if(c > -20000.0 && c < 20.000){a = (int)floor((c+20000.0f)/AREA_SIZE); if(a < 0 || a > 534){a = 0;}}else{a = 0;}if(d > -20000.0 && d < 20.000){b=(int)floor((d+20000.0f)/AREA_SIZE); if(b < 0 || b > 534){b = 0;}}else{b = 0;}

#define PLUGIN_PRIVATE_UPDATE_AC (55)

#define MAX_NODES (32768)
#define MAX_CONNECTIONS (5)

#define BOTH (0)
#define One_To_Two (1)

#define MAX_PLAYERS (1)