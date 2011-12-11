#include "function.h"
#include "TheFlyWin32.h"


bool actorChangePose( ACTORid man, ACTIONid act )
{
	FnActor actor;
	actor.Object(man);
	if( actor.GetCurrentAction(NULL, 0) != act ){
		actor.MakeCurrentAction(0, NULL, act);
		return true;
	}
	return false;
}

bool beenHit( ACTORid attackerID , ACTORid defenderID, OurFrame frame ){
	FnActor attacker, defender;
	float attPos[3], defPos[3];
	float attDir[3], defDir[3];
	float bytan, bycos, bysin;
	float dis = twoObjectDis( attacker.GetBaseObject(), defender.GetBaseObject() );

	attacker.Object( attackerID );
	defender.Object( defenderID );

	attacker.GetWorldPosition( attPos );
	defender.GetWorldPosition( defPos );

	attacker.GetWorldDirection( attDir );
	defender.GetWorldDirection( defDir );
	
	//count angle
	bytan = atan( (attDir[1]-defDir[1])/(attDir[0]-defDir[0]) ) * M_PI/180;
	bycos = acos( (attDir[0]-defDir[0])/dis ) * M_PI/180;
	bysin = asin( (attDir[1]-defDir[1])/dis ) * M_PI/180;

	if( bycos>0 && bysin>0 ){
		return bysin;
	}
	if( bycos<0 && bysin>0 ){
		return 180-bysin;
	}
	if( bytan>0 && bysin<0 ){
		return 180+bytan;
	}
	if( bycos>0 && bysin<0 ){
		return 360-bycos;
	}
	//count dis

	
}

float twoObjectDis( OBJECTid a, OBJECTid b )
{
	FnObject A, B;
	float pos_a[3], pos_b[3], x, y, z;
	
	A.Object( a );
	B.Object( b );
	A.GetWorldPosition( pos_a );
	B.GetWorldPosition( pos_b );
	x = pos_a[0]-pos_b[0];
	y = pos_a[1]-pos_b[1];
	z = pos_a[2]-pos_b[2];

	return sqrt( x*x + y*y + z*z );
}

float posObjectDis( float *pos, OBJECTid a )
{
	FnObject A;
	float pos_a[3];

	A.Object( a );
	A.GetWorldPosition( pos_a );
	float x, y, z;
	x = pos_a[0]-pos[0];
	y = pos_a[1]-pos[1];
	z = pos_a[2]-pos[2];
	return sqrt( x*x + y*y + z*z );
}