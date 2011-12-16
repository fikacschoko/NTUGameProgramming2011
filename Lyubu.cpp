#include "Lyubu.h"
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

	//�f���S���ʧ@
	FnActor actor;
	actor.Object( aID );
	//IDLE
	ourIdleAction = new OurAction();
	ourIdleAction->actID = actor.GetBodyAction(NULL, "Idle");
	ourIdleAction->frames_num = 0;
	ourIdleAction->play_speed = 1;
	ourIdleAction->priority = 0;
	ourIdleAction->type.value = LyubuAction::ACTION_IDLE();

	current_OurAction = ourIdleAction;
	//RUN
	/*
	ourRunAction = new OurAction::Builder()	->actID(actor.GetBodyAction(NULL, "Run"))
											->defense_pt(0)
											->frames_num(0)
											->play_speed(1)
											->priority(0)
											->type(LyubuAction::ACTION_WALK())
											->build();
	*/
	ourRunAction = new OurAction();
	ourRunAction->actID = actor.GetBodyAction(NULL, "Run");
	ourRunAction->frames_num = 0;
	ourRunAction->play_speed = 1;
	ourRunAction->priority = 0;
	ourRunAction->type.value = LyubuAction::ACTION_WALK();
	//ATTACKS
	ourAttack1Action = new AttackAction();
	ourAttack1Action->actID = actor.GetBodyAction(NULL, "NormalAttack1");
	ourAttack1Action->numOfKeyFrames = 1;
	ourAttack1Action->frames_num = 0;
	ourAttack1Action->play_speed = 1;
	ourAttack1Action->priority = 5;
	ourAttack1Action->type.value = LyubuAction::ACTION_NORMAL_ATTACK1();
	ourAttack1Action->keyFrames = new OurFrame*[1];
	ourAttack1Action->keyFrames[0] = new OurFrame;
	ourAttack1Action->keyFrames[0]->frameNO = 7;
	ourAttack1Action->keyFrames[0]->start_angle = 340;
	ourAttack1Action->keyFrames[0]->valid_angle = 40;
	ourAttack1Action->keyFrames[0]->valid_dis = 80;
	ourAttack1Action->keyFrames[0]->damage_pt = 50;

	ourAttack1Action = new AttackAction();
	ourAttack1Action->actID = actor.GetBodyAction(NULL, "NormalAttack2");
	ourAttack1Action->frames_num = 0;
	ourAttack1Action->play_speed = 1;
	ourAttack1Action->priority = 5;
	ourAttack1Action->type.value = LyubuAction::ACTION_NORMAL_ATTACK2();

	ourAttack1Action = new AttackAction();
	ourAttack1Action->actID = actor.GetBodyAction(NULL, "NormalAttack3");
	ourAttack1Action->frames_num = 0;
	ourAttack1Action->play_speed = 1;
	ourAttack1Action->priority = 5;
	ourAttack1Action->type.value = LyubuAction::ACTION_NORMAL_ATTACK3();

	ourAttack1Action = new AttackAction();
	ourAttack1Action->actID = actor.GetBodyAction(NULL, "NormalAttack4");
	ourAttack1Action->frames_num = 0;
	ourAttack1Action->play_speed = 1;
	ourAttack1Action->priority = 5;
	ourAttack1Action->type.value = LyubuAction::ACTION_NORMAL_ATTACK4();

	actor.MakeCurrentAction(0, NULL, ourIdleAction->actID);
}
void Lyubu::dealKey()
{
	static bool Zpressed=false;
	if (FyCheckHotKeyStatus(FY_Z) ) {
		OurFrame *keyFrame = getKeyFrame();
		if(keyFrame != NULL)
		{
			if(	current_OurAction->type == LyubuAction::ACTION_NORMAL_ATTACK1())
			{
				//sendAction(ourAttack2Action);
			}
		}
		if(!Zpressed)
			//sendAction(ourAttack1Action);
		Zpressed=true;
	}
	else
	{
		Zpressed = false;
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