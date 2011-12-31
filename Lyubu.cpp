#include "Lyubu.h"
#include "function.h"
#include "AllData.h"

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

	//呂布特有動作
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
	ourAttack1Action = new OurAction();
	ourAttack1Action->actID = actor.GetBodyAction(NULL, "NormalAttack1");
	ourAttack1Action->isAttack = true;
	ourAttack1Action->frames_num = 0;
	ourAttack1Action->play_speed = 1;
	ourAttack1Action->priority = 5;
	ourAttack1Action->type.value = LyubuAction::ACTION_NORMAL_ATTACK1();
	ourAttack1Action->combo_able_frame_start = 10;
	ourAttack1Action->combo_able_frame_end = 20;
	ourAttack1Action->numOfKeyFrames = 1;
	ourAttack1Action->keyFrames = new OurFrame*[1];
	ourAttack1Action->keyFrames[0] = new OurFrame;
	ourAttack1Action->keyFrames[0]->frameNO = 7;
	ourAttack1Action->keyFrames[0]->start_angle = 340;
	ourAttack1Action->keyFrames[0]->plus_angle = 40;
	ourAttack1Action->keyFrames[0]->valid_dis = 160;
	ourAttack1Action->keyFrames[0]->damage_pt = 50;
	//sound
	ourAttack1Action->numOfAudioFrames = 2;
	ourAttack1Action->audioFrames = new OurAudioFrame*[2];
	ourAttack1Action->audioFrames[0] = new OurAudioFrame;
	ourAttack1Action->audioFrames[0]->frameNO = 0;
	ourAttack1Action->audioFrames[0]->audioID = AllAudio::s02_pose10;
	ourAttack1Action->audioFrames[1] = new OurAudioFrame;
	ourAttack1Action->audioFrames[1]->frameNO = 10;
	ourAttack1Action->audioFrames[1]->audioID = AllAudio::eat;
	//fx
	ourAttack1Action->numOfFxFrames = 1;
	ourAttack1Action->fxFrames = new OurFxFrame*[1];
	ourAttack1Action->fxFrames[0] = new OurFxFrame;
	ourAttack1Action->fxFrames[0]->frameNO = 5;
	ourAttack1Action->fxFrames[0]->fxName = AllFx::Attack01;

	ourAttack2Action = new OurAction();
	ourAttack2Action->actID = actor.GetBodyAction(NULL, "NormalAttack2");
	ourAttack2Action->isAttack = true;
	ourAttack2Action->frames_num = 0;
	ourAttack2Action->play_speed = 0.8;
	ourAttack2Action->priority = 6;
	ourAttack2Action->type.value = LyubuAction::ACTION_NORMAL_ATTACK2();
	ourAttack2Action->combo_able_frame_start = 15;
	ourAttack2Action->combo_able_frame_end = 35;
	ourAttack2Action->numOfKeyFrames = 2;
	ourAttack2Action->keyFrames = new OurFrame*[2];
	ourAttack2Action->keyFrames[0] = new OurFrame;
	ourAttack2Action->keyFrames[0]->frameNO = 22;
	ourAttack2Action->keyFrames[0]->start_angle = 180;
	ourAttack2Action->keyFrames[0]->plus_angle = 180;
	ourAttack2Action->keyFrames[0]->valid_dis = 150;
	ourAttack2Action->keyFrames[0]->damage_pt = 30;
	ourAttack2Action->keyFrames[1] = new OurFrame;
	ourAttack2Action->keyFrames[1]->frameNO = 25;
	ourAttack2Action->keyFrames[1]->start_angle = 0;
	ourAttack2Action->keyFrames[1]->plus_angle = 90;
	ourAttack2Action->keyFrames[1]->valid_dis = 170;
	ourAttack2Action->keyFrames[1]->damage_pt = 30;
	//sound
	ourAttack2Action->numOfAudioFrames = 1;
	ourAttack2Action->audioFrames = new OurAudioFrame*[1];
	ourAttack2Action->audioFrames[0] = new OurAudioFrame;
	ourAttack2Action->audioFrames[0]->frameNO = 5;
	ourAttack2Action->audioFrames[0]->audioID = AllAudio::s02_pose10;

	ourAttack3Action = new OurAction();
	ourAttack3Action->actID = actor.GetBodyAction(NULL, "NormalAttack3");
	ourAttack3Action->isAttack = true;
	ourAttack3Action->frames_num = 0;
	ourAttack3Action->play_speed = 0.8;
	ourAttack3Action->priority = 7;
	ourAttack3Action->type.value = LyubuAction::ACTION_NORMAL_ATTACK3();
	ourAttack3Action->combo_able_frame_start = 25;
	ourAttack3Action->combo_able_frame_end = 45;
	ourAttack3Action->numOfKeyFrames = 1;
	ourAttack3Action->keyFrames = new OurFrame*[1];
	ourAttack3Action->keyFrames[0] = new OurFrame;
	ourAttack3Action->keyFrames[0]->frameNO = 23;
	ourAttack3Action->keyFrames[0]->start_angle = 90;
	ourAttack3Action->keyFrames[0]->plus_angle = 180;
	ourAttack3Action->keyFrames[0]->valid_dis = 80;
	ourAttack3Action->keyFrames[0]->damage_pt = 30;

	ourAttack4Action = new OurAction();
	ourAttack4Action->actID = actor.GetBodyAction(NULL, "NormalAttack4");
	ourAttack4Action->isAttack = true;
	ourAttack4Action->frames_num = 0;
	ourAttack4Action->play_speed = 0.8;
	ourAttack4Action->priority = 8;
	ourAttack4Action->type.value = LyubuAction::ACTION_NORMAL_ATTACK4();
	ourAttack4Action->combo_able_frame_start = 25;
	ourAttack4Action->combo_able_frame_end = 48;
	ourAttack4Action->numOfKeyFrames = 1;
	ourAttack4Action->keyFrames = new OurFrame*[1];
	ourAttack4Action->keyFrames[0] = new OurFrame;
	ourAttack4Action->keyFrames[0]->frameNO = 25;
	ourAttack4Action->keyFrames[0]->start_angle = 340;
	ourAttack4Action->keyFrames[0]->plus_angle = 40;
	ourAttack4Action->keyFrames[0]->valid_dis = 100;
	ourAttack4Action->keyFrames[0]->damage_pt = 50;

	ourHeavyAttack1Action = new OurAction();
	ourHeavyAttack1Action->actID = actor.GetBodyAction(NULL, "HeavyAttack1");
	ourHeavyAttack1Action->isAttack = true;
	ourHeavyAttack1Action->frames_num = 72;
	ourHeavyAttack1Action->play_speed = 1;
	ourHeavyAttack1Action->priority = ourAttack1Action->priority + 10;
	ourHeavyAttack1Action->type.value = LyubuAction::ACTION_HEAVY_ATTACK1();
	ourHeavyAttack1Action->combo_able_frame_start = 40;
	ourHeavyAttack1Action->combo_able_frame_end = 60;
	ourHeavyAttack1Action->numOfKeyFrames = 2;
	ourHeavyAttack1Action->keyFrames = new OurFrame*[2];
	ourHeavyAttack1Action->keyFrames[0] = new OurFrame;
	ourHeavyAttack1Action->keyFrames[0]->frameNO = 21;
	ourHeavyAttack1Action->keyFrames[0]->start_angle = 0;
	ourHeavyAttack1Action->keyFrames[0]->plus_angle = 100;
	ourHeavyAttack1Action->keyFrames[0]->valid_dis = 100;
	ourHeavyAttack1Action->keyFrames[0]->damage_pt = 70;
	ourHeavyAttack1Action->keyFrames[1] = new OurFrame;
	ourHeavyAttack1Action->keyFrames[1]->frameNO = 40;
	ourHeavyAttack1Action->keyFrames[1]->start_angle = 320;
	ourHeavyAttack1Action->keyFrames[1]->plus_angle = 80;
	ourHeavyAttack1Action->keyFrames[1]->valid_dis = 160;
	ourHeavyAttack1Action->keyFrames[1]->damage_pt = 80;

	ourHeavyAttack2Action = new OurAction();
	ourHeavyAttack2Action->actID = actor.GetBodyAction(NULL, "HeavyAttack2");
	ourHeavyAttack2Action->isAttack = true;
	ourHeavyAttack2Action->frames_num = 72;
	ourHeavyAttack2Action->play_speed = 1;
	ourHeavyAttack2Action->priority = ourAttack2Action->priority + 10;
	ourHeavyAttack2Action->type.value = LyubuAction::ACTION_HEAVY_ATTACK2();
	ourHeavyAttack2Action->combo_able_frame_start = 22;
	ourHeavyAttack2Action->combo_able_frame_end = 40;
	ourHeavyAttack2Action->numOfKeyFrames = 1;
	ourHeavyAttack2Action->keyFrames = new OurFrame*[1];
	ourHeavyAttack2Action->keyFrames[0] = new OurFrame;
	ourHeavyAttack2Action->keyFrames[0]->frameNO = 22;
	ourHeavyAttack2Action->keyFrames[0]->start_angle = 0;
	ourHeavyAttack2Action->keyFrames[0]->plus_angle = 180;
	ourHeavyAttack2Action->keyFrames[0]->valid_dis = 120;
	ourHeavyAttack2Action->keyFrames[0]->damage_pt = 60;

	ourHeavyAttack3Action = new OurAction();
	ourHeavyAttack3Action->actID = actor.GetBodyAction(NULL, "HeavyAttack3");
	ourHeavyAttack3Action->isAttack = true;
	ourHeavyAttack3Action->frames_num = 72;
	ourHeavyAttack3Action->play_speed = 1;
	ourHeavyAttack3Action->priority = ourAttack3Action->priority + 10;
	ourHeavyAttack3Action->type.value = LyubuAction::ACTION_HEAVY_ATTACK3();
	ourHeavyAttack3Action->combo_able_frame_start = 24;
	ourHeavyAttack3Action->combo_able_frame_end = 40;
	ourHeavyAttack3Action->numOfKeyFrames = 1;
	ourHeavyAttack3Action->keyFrames = new OurFrame*[1];
	ourHeavyAttack3Action->keyFrames[0] = new OurFrame;
	ourHeavyAttack3Action->keyFrames[0]->frameNO = 24;
	ourHeavyAttack3Action->keyFrames[0]->start_angle = 30;
	ourHeavyAttack3Action->keyFrames[0]->plus_angle = 180;
	ourHeavyAttack3Action->keyFrames[0]->valid_dis = 90;
	ourHeavyAttack3Action->keyFrames[0]->damage_pt = 60;
	//Damaged
	ourHeavyDamagedAction = new OurAction();
	ourHeavyDamagedAction->actID = actor.GetBodyAction(NULL, "HeavyDamaged");
	ourHeavyDamagedAction->frames_num = 0;
	ourHeavyDamagedAction->play_speed = 1;
	ourHeavyDamagedAction->priority = 100;
	ourHeavyDamagedAction->type.value = LyubuAction::ACTION_DAMAGED();

	ourLeftDamagedAction = new OurAction();
	ourLeftDamagedAction->actID = actor.GetBodyAction(NULL, "LeftDamaged");
	ourLeftDamagedAction->frames_num = 0;
	ourLeftDamagedAction->play_speed = 1;
	ourLeftDamagedAction->priority = 100;
	ourLeftDamagedAction->type.value = LyubuAction::ACTION_DAMAGED();

	ourRightDamagedAction = new OurAction();
	ourRightDamagedAction->actID = actor.GetBodyAction(NULL, "RightDamaged");
	ourRightDamagedAction->frames_num = 0;
	ourRightDamagedAction->play_speed = 1;
	ourRightDamagedAction->priority = 100;
	ourRightDamagedAction->type.value = LyubuAction::ACTION_DAMAGED();

	ourDieAction = new OurAction();
	ourDieAction->actID = actor.GetBodyAction(NULL, "Die");
	ourDieAction->frames_num = 0;
	ourDieAction->play_speed = 1;
	ourDieAction->priority = 1000;
	ourDieAction->type.value = LyubuAction::ACTION_DIE();

	actor.MakeCurrentAction(0, NULL, ourIdleAction->actID);
}
void Lyubu::dealKey()
{
	static bool Zpressed=false, Xpressed=false;
	if (FyCheckHotKeyStatus(FY_Z) ) {
		if(!Zpressed)
		{
			//表示是攻擊動作，且在接招範圍
			if(current_OurAction
				&&	current_frame > current_OurAction->combo_able_frame_start && current_frame <= current_OurAction->combo_able_frame_end)
			{
				if(	current_OurAction->type == LyubuAction::ACTION_NORMAL_ATTACK1())
					sendAction(ourAttack2Action);
				else if(current_OurAction->type == LyubuAction::ACTION_NORMAL_ATTACK2())
					sendAction(ourAttack3Action);
				else if(current_OurAction->type == LyubuAction::ACTION_NORMAL_ATTACK3())
					sendAction(ourAttack4Action);
			}
			sendAction(ourAttack1Action);
		}
		Zpressed=true;
		Xpressed=false;
	}
	else if (FyCheckHotKeyStatus(FY_X) )
	{
		if(!Xpressed)
		{
			//表示是攻擊動作，且在接招範圍
			if(current_OurAction
				&&	current_frame > current_OurAction->combo_able_frame_start && current_frame <= current_OurAction->combo_able_frame_end)
			{
				if(	current_OurAction->type == LyubuAction::ACTION_NORMAL_ATTACK1())
					sendAction(ourHeavyAttack1Action);
				else if(current_OurAction->type == LyubuAction::ACTION_NORMAL_ATTACK2())
					sendAction(ourHeavyAttack2Action);
				else if(current_OurAction->type == LyubuAction::ACTION_NORMAL_ATTACK3())
					sendAction(ourHeavyAttack3Action);
			}
		}
		Xpressed=true;
		Zpressed=false;
	}
	else
	{
		Xpressed=false;
		Zpressed=false;
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

void Lyubu::damaged( int attack_pt, ACTORid attacker, float angle )
{
	HP -= attack_pt;
	if( HP <= 0 )
	{
		sendAction(ourDieAction);
	}
	else if(attack_pt > 100)
	{
		sendAction(ourHeavyDamagedAction);
	}
	else
	{
		if( angle < 180 )
			sendAction(ourLeftDamagedAction);
		else
			sendAction(ourRightDamagedAction);
	}
}