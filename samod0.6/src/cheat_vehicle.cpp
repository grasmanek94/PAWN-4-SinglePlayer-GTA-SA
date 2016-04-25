/*

	PROJECT:		mod_sa
	LICENSE:		See LICENSE in the top level directory
	COPYRIGHT:		Copyright we_sux

	mod_sa is available from http://code.google.com/p/m0d-s0beit-sa/

	mod_sa is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	mod_sa is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with mod_sa.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "main.h"

// new function to jump into vehicles without jacking (also for single player)
bool vehicleJumper ( int iVehicleID )
{
	// can't touch this
	if ( iVehicleID == VEHICLE_SELF )
		return false;

	// get vehicle_info
	struct vehicle_info *pVehicle = vehicle_info_get( iVehicleID, 0 );

	// check that the vehicle is legit
	if ( isBadPtr_GTA_pVehicle(pVehicle) )
		return false;

	traceLastFunc( "vehicleJumper()" );

	if ( pVehicle->hitpoints == 0.0f )
	{
		cheat_state_text( "Vehicle is destroyed" );
		return false;
	}

	if ( !pVehicle->base.bIsVisible )
	{
		cheat_state_text( "Vehicle is not visible." );
		return false;
	}

	struct actor_info	*self = actor_info_get( ACTOR_SELF, 0 );
	if ( self != NULL && pVehicle->base.interior_id != self->base.interior_id )
	{
		cheat_state_text( "Vehicle is in another interior." );
		return false;
	}

	int iGTAVehicleID;
	iGTAVehicleID = ScriptCarId( pVehicle );

	if ( pVehicle->passengers[0] == self )
		return false;
	
	if ( !pVehicle->base.bUsesCollision )
		pVehicle->base.bUsesCollision = 1;
	
	// put into first available seat
	if ( pVehicle->passengers[0] == NULL )
	{
		GTAfunc_PutActorInCar(GetVehicleByGtaId(iGTAVehicleID));
		pGameInterface->GetCamera()->RestoreWithJumpCut();
		return true;
	}

	const int	seat_count = gta_vehicle_get_by_id( pVehicle->base.model_alt_id )->passengers;
	if ( seat_count > 0 )
	{
		for ( int seat = 1; seat <= seat_count; seat++ )
		{
			if ( pVehicle->passengers[seat] == NULL )
			{
				GTAfunc_PutActorInCarAsPassenger(GetVehicleByGtaId((iGTAVehicleID)), seat - 1);
				pGameInterface->GetCamera()->RestoreWithJumpCut();
				return true;
			}
		}
	}

	// no seats left, oh well
	cheat_state_text( "No seats left to teleport into." );
	return false;
}

void cheat_state_text ( const char *fmt, ... )
{

}

void vehicle_detachables_teleport ( struct vehicle_info *info, const float from[3], const float to[3] )
{
	int i, n;

	/*Log("detachables for type=%d id=%d:", info->vehicle_type, info->vehicle_id);
   dump_vect("bike1[0]", info->detachable_bike1[0].position);
   for(i=0; i<4; i++)
      dump_vect("bike2[]", info->detachable_bike2[i].position);
   for(i=0; i<4; i++)
      dump_vect("car[]", info->detachable_car[i].position);
   Log("");*/
	switch ( info->vehicle_type )
	{
	case VEHICLE_TYPE_CAR:	/* + helicopters and planes */
		for ( n = 0; n < 4; n++ )
		{
			for ( i = 0; i < 3; i++ )
				info->detachable_car[n].position[i] += to[i] - from[i];
		}
		break;

	case VEHICLE_TYPE_BIKE:
		for ( i = 0; i < 3; i++ )
			info->detachable_bike1[0].position[i] += to[i] - from[i];
		for ( n = 0; n < 4; n++ )
		{
			for ( i = 0; i < 3; i++ )
				info->detachable_bike2[n].position[i] += to[i] - from[i];
		}
		break;

	case VEHICLE_TYPE_TRAIN:
		/* XXX: fixme */
		break;

	case VEHICLE_TYPE_BOAT:
		for ( n = 0; n < 2; n++ )
		{
			for ( i = 0; i < 3; i++ )
				info->detachable_bike2[n].position[i] += to[i] - from[i];
		}
		break;
	}

	/*Log("detachables after teleport for type=%d id=%d:", info->vehicle_type, info->vehicle_id);
   dump_vect("bike1[0]", info->detachable_bike1[0].position);
   for(i=0; i<4; i++)
      dump_vect("bike2[]", info->detachable_bike2[i].position);
   for(i=0; i<4; i++)
      dump_vect("car[]", info->detachable_car[i].position);
   Log("");
   Log("");
   Log("");*/
}
void cheat_vehicle_teleport ( struct vehicle_info *info, const float pos[3], int interior_id )
{
	if ( info == NULL )
		return;

	traceLastFunc( "cheat_vehicle_teleport()" );

	float				diff[3];
	float				new_pos[3];
	struct vehicle_info *temp;

	vect3_vect3_sub( pos, &info->base.matrix[4 * 3], diff );

	for ( temp = info; temp != NULL; temp = temp->trailer )
	{
		if(temp == NULL) return;

		vect3_vect3_add( &temp->base.matrix[4 * 3], diff, new_pos );

		vehicle_detachables_teleport( temp, &temp->base.matrix[4 * 3], new_pos );
		vect3_copy( new_pos, &temp->base.matrix[4 * 3] );

		vect3_zero( temp->speed );
		vect3_zero( temp->spin );

		gta_interior_id_set( interior_id );
		temp->base.interior_id = ( uint8_t ) interior_id;

	}
}

/*
CMatrix
	vRight = CVector ( 1.0f, 0.0f, 0.0f );
	vFront = CVector ( 0.0f, 1.0f, 0.0f );
	vUp    = CVector ( 0.0f, 0.0f, 1.0f );
	vPos   = CVector ( 0.0f, 0.0f, 0.0f );
MATRIX4X4
	VECTOR right;	// vRight
	VECTOR up;		// vFront
	VECTOR at;		// vUp
	VECTOR pos;		// vPos
*/
CVector cheat_vehicle_getPositionUnder ( CVehicle *cveh )
{
	traceLastFunc( "cheat_vehicle_getPositionUnder()" );

	CVector offsetVector;
	CMatrix matrix;
	cveh->GetMatrix( &matrix );

	CMatrix_Padded	matrixPad ( matrix );
	offsetVector.fX = 0 * matrixPad.vRight.fX + 0 * matrixPad.vFront.fX - 1 * matrixPad.vUp.fX;
	offsetVector.fY = 0 * matrixPad.vRight.fY + 0 * matrixPad.vFront.fY - 1 * matrixPad.vUp.fY;
	offsetVector.fZ = 0 * matrixPad.vRight.fZ + 0 * matrixPad.vFront.fZ - 1 * matrixPad.vUp.fZ;
	return offsetVector;
}