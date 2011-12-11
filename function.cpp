#include "function.h"
#include "TheFlyWin32.h"

void actorChangePose( ACTORid man, ACTIONid act )
{
	FnActor actor;
	actor.Object(man);
	if( actor.GetCurrentAction(NULL, 0) != act )
		actor.MakeCurrentAction(0, NULL, act);
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