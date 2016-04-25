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

// new function to help converting from actor_info->base to CEntitySAInterface
CEntitySAInterface *cheat_actor_GetCEntitySAInterface ( actor_info *ainfo )
{
	return (CEntitySAInterface *)ainfo;
}

//*p_CEntitySAInterface = (CEntitySAInterface*)ainfo->base;
void cheat_actor_teleport ( struct actor_info *info, const float pos[3], int interior_id )
{
	if ( info == NULL )
		return;
	vect3_zero( info->speed );
	vect3_copy( pos, &info->base.matrix[4 * 3] );
	gta_interior_id_set( interior_id );
}

CVector cheat_actor_getPositionUnder ( actor_info *ainfo )
{
	traceLastFunc( "cheat_vehicle_getPositionUnder()" );

	CVector offsetVector;
	float	*matrix = ainfo->base.matrix;
	offsetVector.fX = 0 * matrix[0] + 0 * matrix[4] - 1 * matrix[8];
	offsetVector.fY = 0 * matrix[1] + 0 * matrix[5] - 1 * matrix[9];
	offsetVector.fZ = 0 * matrix[2] + 0 * matrix[6] - 1 * matrix[10];
	return offsetVector;
}

/*
static CMatrix_Padded * mat_SpiderFeetCollisionTransform = new CMatrix_Padded();
static CMatrix_Padded * mat_SpiderFeetCollisionTransform_Original = (CMatrix_Padded*)0x968988;
uint8_t mat_SpiderFeetCollisionTransform_Offset[4] = {
	LOBYTE(LOWORD(&mat_SpiderFeetCollisionTransform)),
	HIBYTE(LOWORD(&mat_SpiderFeetCollisionTransform)),
	LOBYTE(HIWORD(&mat_SpiderFeetCollisionTransform)),
	HIBYTE(HIWORD(&mat_SpiderFeetCollisionTransform))
};
static struct patch_set patch_actor_SpiderFeetCollisionTransform =
{
	"SpiderFeet Collision Transform",
	0,
	0,
	{
		// bottom
		{ 4, (void *)0x004196D0, (uint8_t *)"\x88\x89\x96\x00", (uint8_t *)mat_SpiderFeetCollisionTransform_Offset, (uint8_t *)"\x88\x89\x96\x00" },
		{ 4, (void *)0x00419700, (uint8_t *)"\x88\x89\x96\x00", (uint8_t *)mat_SpiderFeetCollisionTransform_Offset, (uint8_t *)"\x88\x89\x96\x00" },
		{ 4, (void *)0x00418FB8, (uint8_t *)"\x88\x89\x96\x00", (uint8_t *)mat_SpiderFeetCollisionTransform_Offset, (uint8_t *)"\x88\x89\x96\x00" },
		// up 1
		{ 4, (void *)0x0041874E, (uint8_t *)"\x88\x89\x96\x00", (uint8_t *)mat_SpiderFeetCollisionTransform_Offset, (uint8_t *)"\x88\x89\x96\x00" },
		// up 2
		{ 4, (void *)0x004186AB, (uint8_t *)"\x88\x89\x96\x00", (uint8_t *)mat_SpiderFeetCollisionTransform_Offset, (uint8_t *)"\x88\x89\x96\x00" }
		//crash { 4, (void *)0x00418693 , (uint8_t *)"\x88\x89\x96\x00", (uint8_t *)mat_SpiderFeetCollisionTransform_Offset, (uint8_t *)"\x88\x89\x96\x00" },
		//crash { 4, (void *)0x00418681, (uint8_t *)"\x88\x89\x96\x00", (uint8_t *)mat_SpiderFeetCollisionTransform_Offset, (uint8_t *)"\x88\x89\x96\x00" },
		//crash { 4, (void *)0x00418672, (uint8_t *)"\x88\x89\x96\x00", (uint8_t *)mat_SpiderFeetCollisionTransform_Offset, (uint8_t *)"\x88\x89\x96\x00" }
	}
};
//
void cheat_handle_SpiderFeet ( struct actor_info *ainfo, double time_diff )
{
	traceLastFunc( "cheat_handle_SpiderFeet()" );

	if ( KEY_PRESSED(set.key_spiderfeet) )
	{
		// toggle the d-dang Ninjas
		if ( !cheat_state->actor.SpiderFeet_on )
		{
			//patcher_install( &patch_actor_SpiderFeetCollisionTransform );
		}
		cheat_state->actor.SpiderFeet_on ^= 1;
	}

	if ( cheat_state->actor.SpiderFeet_on )	//cheat_state->actor.NinjaMode_on)
	{
		// set SpiderFeet status
		cheat_state->actor.SpiderFeet_Enabled = true;

		// set NinjaMode enabler to on
		//ainfo->base.nImmunities = 0x0B;

		// get "down" facing vector
		CVector				offsetVector = cheat_actor_getPositionUnder( ainfo );

		// setup variables
		CVector				vecOrigin, vecTarget;
		CColPoint			*pCollision = NULL;
		CEntitySAInterface	*pCollisionEntity = NULL;
		int					checkDistanceMeters = 20;

		// get CEntitySAInterface pointer
		CEntitySAInterface	*p_CEntitySAInterface = cheat_actor_GetCEntitySAInterface( ainfo );

		// origin = our actor
		vecOrigin = p_CEntitySAInterface->Placeable.matrix->vPos;

		// target = vecOrigin + offsetVector * checkDistanceMeters
		vecTarget = offsetVector * checkDistanceMeters;
		vecTarget = vecTarget + vecOrigin;

		// for time/fps purposes
		float	fTimeStep = *(float *)0xB7CB5C;

		// check for collision
		bool	bCollision = GTAfunc_ProcessLineOfSight( &vecOrigin, &vecTarget, &pCollision, &pCollisionEntity, 1, 0,
														 0, 1, 1, 0, 0, 0 );

		if ( bCollision )
		{
			// set altered gravity vector
			float	fTimeStep = *(float *)0xB7CB5C;
			CVector colGravTemp = -pCollision->GetInterface()->Normal;
			CVector vehGravTemp = cheat_state->actor.gravityVector;
			CVector newRotVector;
			newRotVector = colGravTemp - vehGravTemp;
			newRotVector *= 0.05f * fTimeStep;
			offsetVector = vehGravTemp + newRotVector;

			// for collision on ground
			CMatrix colTransformer;
			//CVector colPosOriginal;
			mat_SpiderFeetCollisionTransform_Original->ConvertToMatrix( colTransformer );
			//colPosOriginal = colTransformer.vPos;
			CVector rotationAxis = colTransformer.vUp;
			rotationAxis.CrossProduct( &-colGravTemp );
			float theta = colTransformer.vUp.DotProduct( &-colGravTemp );
// add check here for theta nearzero
			colTransformer = colTransformer.Rotate( &rotationAxis, -cos(theta) );
			//colTransformer.vPos = colPosOriginal;
			mat_SpiderFeetCollisionTransform->SetFromMatrix( colTransformer );

			pCollision->Destroy();
		}
		else
		{
			// set normal gravity vector
			CVector colGravTemp;
			colGravTemp.fX = 0.0f;
			colGravTemp.fY = 0.0f;
			colGravTemp.fZ = -1.0f;

			CVector vehGravTemp = cheat_state->actor.gravityVector;
			CVector newRotVector;
			newRotVector = colGravTemp - vehGravTemp;
			newRotVector *= 0.05f * fTimeStep;
			offsetVector = vehGravTemp + newRotVector;
		}

		// set the gravity/camera
		cheat_actor_setGravity( ainfo, offsetVector );
		//pPed->SetOrientation( offsetVector.fX, offsetVector.fY, offsetVector.fZ );
		traceLastFunc( "cheat_handle_SpiderFeet()" );

		// set up vector, can make it very easy to scale walls n such
		//pPed->SetWas( &-offsetVector );

		// Ninjas know how to do awesome flips n shit
		if ( KEY_DOWN(set.key_ninjaflipfront) )
		{
			//Log("1");
			//CVector vecVelocity( pPedSelfSA->vecVelocity->fX, pPedSelfSA->vecVelocity->fY, pPedSelfSA->vecVelocity->fZ );
			//Log("2");

			//if ( vecVelocity.IsNearZero() )
			//{
			//	//ds
			//}
			//else
			//{
			//	// prepare directional vector

			//}
				//vecVelocity.Normalize();
				//vecVelocity *= 100.0f;
				//vecVelocity.ZeroNearZero();

				if ( !isBadPtr_writeAny(pPedSelfSA, sizeof(CPedSAInterface)) )
				{
					Log("good pPedSelfSA");
					if ( !isBadPtr_writeAny(pPedSelfSA->vecSpinCollision, sizeof(CVector)) )
					{
						Log("good pPedSelfSA->vecSpinCollision");
						//if ( !isBadPtr_writeAny(&pPedSelfSA->vecSpinCollision->fX, sizeof(float)) )
						//{
							//ds
						//}
						//else
						//	Log("bad pPedSelfSA->vecSpinCollision->fX");
					}
					else
						Log("bad pPedSelfSA->vecSpinCollision");
				}
				else
					Log("bad pPedSelfSA");

				//pPedSelfSA->vecSpinCollision->fX = vecVelocity.fX;

				//memcpy_safe( pPedSelfSA->vecSpinCollision, &vecVelocity, sizeof(float[3]) );
*/

			/*
			// get matrix, backup original front vector for comparison
			CMatrix matPed;
			pPed->GetMatrix( &matPed );
			CVector vecSpinOriginal;
			vecSpinOriginal.fX = matPed.vFront.fZ;
			vecSpinOriginal.fY = matPed.vRight.fZ;
			vecSpinOriginal.fZ = matPed.vUp.fZ;

			// rotate matrix on right axis
			float rotation_theta = M_PI / 2.0f;
// add check for theta nearzero
			matPed = matPed.Rotate( &matPed.vRight, rotation_theta );

			// compare
			CVector vecSpinCompare;
			vecSpinCompare.fX = matPed.vFront.fZ;
			vecSpinCompare.fY = matPed.vRight.fZ;
			vecSpinCompare.fZ = matPed.vUp.fZ;
			vecSpinCompare = (vecSpinOriginal - vecSpinCompare) * 10;

			// spin mother fucker, spin
			//pPedSA->vecSpinCollision = &(CVector)( vecSpinCompare );
			pPedSA->vecSpin = &(CVector)( vecSpinCompare );

			//pPed->SetDirection( vecSpinCompare );
			//pPed->SetWas( 
			//CVehicle blah;
			//blah.SetWas( vecSpinCompare );
			*/
/*

		}
		//key_ninjaflipfront
		//key_ninjaflipback
		//key_ninjajumpboost

		// if we're standing, rotate the CPed to match
		// TODO

	}
	else if ( cheat_state->actor.SpiderFeet_Enabled )
	{

		//patcher_remove( &patch_actor_SpiderFeetCollisionTransform );

		// set NinjaMode enabler to off
		//ainfo->base.nImmunities = 0x12;

		CVector offsetVector;

		// disable NinjaMode with normal gravity vector
		offsetVector.fX = 0.0f;
		offsetVector.fY = 0.0f;
		offsetVector.fZ = -1.0f;

		// set the gravity/camera
		cheat_actor_setGravity( ainfo, offsetVector );

		// set NinjaMode disabled
		cheat_state->actor.SpiderFeet_Enabled = false;
	}
}
*/