#include"Donzo.h"
Donzo::Donzo( WORLDid gID, SCENEid sID )
{
	FnWorld gw;
	FnScene scene;

	HP = 120;

	pos_begin[0]=3469.0;
	pos_begin[1]=-3208.0;
	pos_begin[2]=1000.0f;

	this->gID = gID;
	this->sID = sID;

	gw.Object(gID);
	scene.Object(sID);

	gw.SetObjectPath("Data\\NTU4\\Characters");
	gw.SetTexturePath("Data\\NTU4\\Characters");
	gw.SetCharacterPath("Data\\NTU4\\Characters");
	gw.SetShaderPath( "Data\\NTU4\\Shaders" );

	aID = scene.LoadActor("Donzo");
	actor.Object(aID);


	//-------action---------
	//IDLE
	ourIdleAction = new OurAction();
	ourIdleAction->actID = actor.GetBodyAction(NULL, "CombatIdle");
	ourIdleAction->frames_num = 0;
	ourIdleAction->play_speed = 1;
	ourIdleAction->priority = 0;
	ourIdleAction->type.value = DonzoAction::ACTION_IDLE();

	current_OurAction = ourIdleAction;
	actor.MakeCurrentAction(0, NULL, ourIdleAction->actID);

	//DamageL
	ourDamageLAction = new OurAction();
	ourDamageLAction->actID = actor.GetBodyAction(NULL, "DamageL");
	ourDamageLAction->frames_num = 0;
	ourDamageLAction->play_speed = 1;
	ourDamageLAction->priority = 100;
	ourDamageLAction->type.value = DonzoAction::ACTION_DAMAGED();

	//DamageH
	ourDamageHAction = new OurAction();
	ourDamageHAction->actID = actor.GetBodyAction(NULL, "DamageH");
	ourDamageHAction->frames_num = 0;
	ourDamageHAction->play_speed = 1;
	ourDamageHAction->priority = 100;
	ourDamageHAction->type.value = DonzoAction::ACTION_DAMAGED();

	//Die
	ourDieAction = new OurAction();
	ourDieAction->actID = actor.GetBodyAction(NULL, "Die");
	ourDieAction->frames_num = 0;
	ourDieAction->play_speed = 1;
	ourDieAction->priority = 1000;
	ourDieAction->type.value = DonzoAction::ACTION_DIE();
}

void Donzo::AI()
{

}

void Donzo::damaged( int attack_pt, ACTORid attacker, float angle )
{
	HP -= attack_pt;
	if( HP < 0 )
		sendAction(ourDieAction);
	else{
		if( angle < 180 )
			sendAction(ourDamageHAction);
		else
			sendAction(ourDamageLAction);
	}
}