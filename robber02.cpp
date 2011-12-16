#include "robber02.h"
#include "function.h"
#include "OurActor.h"
#include "OurAction.h" 

Robber02::Robber02( WORLDid gID, SCENEid sID )
{
	FnWorld gw;
	FnScene scene;

	pos_begin[0]=3550.0;
	pos_begin[1]=-3216.0;
	pos_begin[2]=1000.0f;
	this->gID = gID;
	this->sID = sID;

	gw.Object(gID);
	scene.Object(sID);

	gw.SetObjectPath("Data\\NTU4\\Characters");
	gw.SetTexturePath("Data\\NTU4\\Characters");
	gw.SetCharacterPath("Data\\NTU4\\Characters");
	gw.SetShaderPath( "Data\\NTU4\\Shaders" );

	aID = scene.LoadActor("Robber02");
	actor.Object(aID);

	//-------action---------
	//IDLE
	ourIdleAction = new OurAction();
	ourIdleAction->actID = actor.GetBodyAction(NULL, "CombatIdle");
	ourIdleAction->frames_num = 0;
	ourIdleAction->play_speed = 1;
	ourIdleAction->priority = 0;
	ourIdleAction->type.value = Action_type::ACTION_IDLE();

	current_OurAction = ourIdleAction;
	actor.MakeCurrentAction(0, NULL, ourIdleAction->actID);
	//Damage1
	ourDamage1Action = new OurAction();
	ourDamage1Action->actID = actor.GetBodyAction(NULL, "Damage1");
	ourDamage1Action->frames_num = 0;
	ourDamage1Action->play_speed = 1;
	ourDamage1Action->priority = 100;
	ourDamage1Action->type.value = Action_type::ACTION_DAMAGED();
	//Damage2
	ourDamage2Action = new OurAction();
	ourDamage2Action->actID = actor.GetBodyAction(NULL, "Damage2");
	ourDamage2Action->frames_num = 0;
	ourDamage2Action->play_speed = 1;
	ourDamage2Action->priority = 100;
	ourDamage2Action->type.value = Action_type::ACTION_DAMAGED();

}

void Robber02::AI()
{

}

void Robber02::damaged( int attack_pt, ACTORid attacker, float angle )
{
	if( angle < 180 )
		sendAction(ourDamage1Action);
	else
		sendAction(ourDamage2Action);
}
