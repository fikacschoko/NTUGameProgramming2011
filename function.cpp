#include "function.h"
#include "TheFlyWin32.h"
#define M_PI 3.14159265

extern float debug[6];
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
	attacker.Object( attackerID );
	defender.Object( defenderID );

	//float attPos[3], defPos[3];
	//float attDir[3] ,att_def[3] ,eat[3];
	float angle = twoObjectAngle( attacker.GetBaseObject(), defender.GetBaseObject() );
	float dis = twoObjectDis( attacker.GetBaseObject(), defender.GetBaseObject() );
	float cross = twoObjectCross( attacker.GetBaseObject(), defender.GetBaseObject() );
	debug[0] = angle;
	debug[1] = dis;
	debug[2] = cross;
	//float dot , absA, absB;

	/*attacker.GetWorldPosition( attPos );
	defender.GetWorldPosition( defPos );
	attacker.GetWorldDirection( attDir, eat );
	defender.GetWorldDirection( defDir, eat );
	
	att_def[0] = attPos[0]- defPos[0];
	att_def[1] = attPos[1]- defPos[1];
	
	//¤º¿nattDir&att_def
	dot = attDir[0] * att_def[0] + attDir[1] * att_def[1];
	absA = sqrt(attDir[0] * attDir[0] + attDir[1]* attDir[1]);
	absB = sqrt(att_def[0] * att_def[0] + att_def[1]* att_def[1]);
	angle = acos( dot/(absA*absB) )*180/M_PI;

	cross = attDir[0] * att_def[1] - attDir[1] * att_def[0];
	*/
	
	if( dis <= frame.valid_dis )
	{
		if( cross < 0 )
		{
			angle = 360 - angle;			
		}
		if( frame.start_angle + frame.plus_angle <= 360 )
		{
			if(angle <= frame.start_angle + frame.plus_angle && angle >= frame.start_angle )
				return true;
			else
				return false;
		}
		else
		{
			if( angle >= frame.start_angle || angle <= (frame.start_angle + frame.plus_angle)-360 )
				return true;
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

float twoObjectCross(  OBJECTid centerID , OBJECTid targetID ){
	FnObject center, target;
	center.Object( centerID );
	target.Object( targetID );

	float cenPos[3], tarPos[3];
	float cenDir[3] ,cen_tar[3] ,eat[3];
	float angle, absA, absB;
	//float dis = twoObjectDis( center, target );
	float cross;

	center.GetWorldPosition( cenPos );
	target.GetWorldPosition( tarPos );
	center.GetWorldDirection( cenDir, eat );
	
	cen_tar[0] = tarPos[0]- cenPos[0];
	cen_tar[1] = tarPos[1]- cenPos[1];
	
	cross = cenDir[0] * cen_tar[1] - cenDir[1] * cen_tar[0];

	return cross;

}

float twoObjectAngle(  OBJECTid centerID , OBJECTid targetID ){
	FnObject center, target;
	center.Object( centerID );
	target.Object( targetID );

	float cenPos[3], tarPos[3];
	float cenDir[3] ,cen_tar[3] ,eat[3];
	float angle, absA, absB;
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
	angle = acos( dot/(absA*absB) )*180/M_PI;

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
