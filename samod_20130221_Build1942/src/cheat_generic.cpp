/*

	PROJECT:		 samod 2
	LICENSE:		See LICENSE in the top level directory
	COPYRIGHT:		Copyright Gamer_Z

	 samod 2 is available from http://gpb.googlecode.com/

	 samod 2 is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	 samod 2 is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with  samod 2.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "main.h"

/* XXX move to cheat_funcs.cpp? */
extern bool GTAfunc_RemoveActorFromCarAndPutAt_requestingTeleport;
void cheat_teleport ( const float pos[3], int interior_id )
{
	if ( cheat_state->state == CHEAT_STATE_ACTOR
		|| GTAfunc_RemoveActorFromCarAndPutAt_requestingTeleport )
	{
		GTAfunc_RemoveActorFromCarAndPutAt_requestingTeleport = false;
		cheat_actor_teleport( actor_info_get(ACTOR_SELF, 0), pos, interior_id );
	}
	else if ( cheat_state->state == CHEAT_STATE_VEHICLE ) //&& !set.teleport_slow )
		cheat_vehicle_teleport( vehicle_info_get(VEHICLE_SELF, 0), pos, interior_id );
	/*else if ( cheat_state->state == CHEAT_STATE_VEHICLE && set.teleport_slow )
	{
		vect3_copy(pos, cheat_state->vehicle.tele_coords);
		cheat_state->vehicle.tele_on = 1;
	}*/
}

void cheat_teleport_nearest_car ( void )
{
	int					id = vehicle_find_nearest( VEHICLE_ALIVE | VEHICLE_EMPTY );
	struct vehicle_info *info = vehicle_info_get( id, VEHICLE_ALIVE | VEHICLE_EMPTY );

	if ( id == -1 || info == NULL )
		return;

	cheat_teleport( &info->base.matrix[4 * 3], info->base.interior_id );
}