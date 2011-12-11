#include "lyubu.h"
#include "function.h"

extern float debug[4];
Lyubu::Lyubu( WORLDid gID, SCENEid sID )
{
	FnWorld gw;
	FnScene scene;

	pos_begin[0]=3569.0;
	pos_begin[1]=-3208.0;
	pos_begin[2]=1000.0f;
	MOVE_SPEED=10.0;
	MOVE_ANGLE=5.0;
	this->gID = gID;
	this->sID = sID;

	gw.Object(gID);
	scene.Object(sID);

	gw.SetObjectPath("Data\\NTU4\\Characters");
	gw.SetTexturePath("Data\\NTU4\\Characters");
	gw.SetCharacterPath("Data\\NTU4\\Characters");
	gw.SetShaderPath( "Data\\NTU4\\Shaders" );

	aID = scene.LoadActor("Lyubu");
	actor.Object(aID);

	waiting_pose = actor.GetBodyAction(NULL, "Idle");
	running_pose = actor.GetBodyAction(NULL, "Run");
	actor.MakeCurrentAction(0, NULL, waiting_pose);
}

void Lyubu::movement(Direction direction)
{
	if (direction == FORWARD) {
		int result = actor.MoveForward( MOVE_SPEED, TRUE, FALSE, 0.0f, TRUE);
		
		::actorChangePose( aID, running_pose );
	}
	else if(direction == LEFT){	
		::actorChangePose( aID, running_pose );
		actor.TurnRight(-90);
	}
	else if (direction == RIGHT){
		::actorChangePose( aID, running_pose );
		actor.TurnRight(90);
	}
	else if (direction == BACK) {
		int result = actor.MoveForward( -MOVE_SPEED, TRUE, FALSE, 0.0f, TRUE);
		::actorChangePose( aID, running_pose );
	}
	else{
		::actorChangePose( aID, waiting_pose );
	}
}
void Lyubu::Rotate(int degree, float cameraPos[])
{
	float rad = degree*2*PI/360;
	float distanceToOrigin[3];
	float actPos[3];
	actor.GetPosition(actPos);
	distanceToOrigin[0] = cameraPos[0];
	distanceToOrigin[1] = cameraPos[1];
	distanceToOrigin[2] = cameraPos[2];

	float M[12] = {	cos(rad), sin(rad), 0,
					-sin(rad), cos(rad), 0,
					0,	0,	1,
					-distanceToOrigin[0]*cos(rad)+distanceToOrigin[1]*sin(rad)+distanceToOrigin[0],-distanceToOrigin[0]*sin(rad)-distanceToOrigin[1]*cos(rad)+distanceToOrigin[1], 0
	};
	FnObject actObj;
	actObj.Object(actor.GetBaseObject());
	actObj.SetMatrix(M, GLOBAL);
	actor.GetPosition(actPos);
	debug[0] = actPos[0];
	debug[1] = actPos[1];
	debug[2] = actPos[2];
}