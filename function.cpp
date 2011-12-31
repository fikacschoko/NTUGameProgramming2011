#include "function.h"
#include "TheFlyWin32.h"
#define M_PI 3.14159265
#define CLOSE_ENOUGH 100
#define CLOSE_START_ANGLE 310
#define CLOSE_PLUS_ANGLE 100
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

bool beenHit( ACTORid attackerID , ACTORid defenderID, OurFrame frame, float angle ){

	FnActor attacker, defender;
	attacker.Object( attackerID );
	defender.Object( defenderID );
	//float angle = twoObjectAngle( attacker.GetBaseObject(), defender.GetBaseObject() );
	float dis = twoObjectDis( attacker.GetBaseObject(), defender.GetBaseObject() );
	
	if( dis <= frame.valid_dis )
	{
		if( frame.start_angle + frame.plus_angle <= 360 )
		{
			if( dis <= CLOSE_ENOUGH && angle <= CLOSE_START_ANGLE && angle >= CLOSE_PLUS_ANGLE)
			{
				return true;
			}
			if(angle <= frame.start_angle + frame.plus_angle && angle >= frame.start_angle )
			{

				return true;
			}
			else
				return false;
		}
		else
		{
			if( dis <= CLOSE_ENOUGH && (angle >= CLOSE_START_ANGLE || angle <= (CLOSE_START_ANGLE + CLOSE_PLUS_ANGLE)-360) )
			{
				return true;
			}
			if( angle >= frame.start_angle || angle <= (frame.start_angle + frame.plus_angle)-360 )
			{

				return true;
			}
			else
				return false;
		}
		
	}
	return false;
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

float twoActorDis( ACTORid a, ACTORid b )
{
	FnActor A, B;
	float pos_a[3], pos_b[3], x, y, z;
	
	A.Object( a );
	B.Object( b );
	A.GetPosition( pos_a );
	B.GetPosition( pos_b );
	x = pos_a[0]-pos_b[0];
	y = pos_a[1]-pos_b[1];
	z = pos_a[2]-pos_b[2];

	return sqrt( x*x + y*y + z*z );
}

float twoPointDis( float *a, float *b )
{
	float x = a[0]-b[0];
	float y = a[1]-b[1];
	float z = a[2]-b[2];

	return sqrt( x*x + y*y + z*z );
}

float twoObjectAngle(  OBJECTid centerID , OBJECTid targetID ){
	FnObject center, target;
	center.Object( centerID );
	target.Object( targetID );

	float cenPos[3], tarPos[3];
	float cenDir[3] ,cen_tar[3] ,eat[3];
	float angle, absA, absB;
	float cross;
	//float dis = twoObjectDis( center, target );
	float dot;

	center.GetWorldPosition( cenPos );
	target.GetWorldPosition( tarPos );
	center.GetWorldDirection( cenDir, eat );
	
	cen_tar[0] = tarPos[0]- cenPos[0];
	cen_tar[1] = tarPos[1]- cenPos[1];
	
	//¤º¿nattDir&att_def
	dot = cenDir[0] * cen_tar[0] + cenDir[1] * cen_tar[1];
	absA = sqrt(cenDir[0] * cenDir[0] + cenDir[1]* cenDir[1]);
	absB = sqrt(cen_tar[0] * cen_tar[0] + cen_tar[1]* cen_tar[1]);
	float temp;
	if(dot/(absA*absB) > 1)
		temp = 1;
	else
		temp = dot/(absA*absB);
	angle = acos(temp)*180/M_PI;

	cross = cenDir[0] * cen_tar[1] - cenDir[1] * cen_tar[0];
	if( cross < 0 )
	{
		angle = 360 - angle;			
	}
	return angle;

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


bool actorFaceTo( ACTORid aID, float x, float y, float z ){
	FnActor actor;
	float pos[3], face[3];
	actor.Object(aID);
	
	actor.GetWorldPosition( pos );
	face[0] = x-pos[0];
	face[1] = y-pos[1];
	face[2] = z-pos[2];

	float uDir[3] = {0,0,1};
	actor.SetWorldDirection(face,uDir);

	return true;
}
