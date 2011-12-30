#ifndef _OURACTOR_H_
#define _OURACTOR_H_
#include "TheFlyWin32.h"
#include "OurAction.h"
class OurActor{
public:
	//var
	WORLDid gID;
	SCENEid sID;
	FnActor actor;
	ACTORid aID;
	int HP_MAX;
	int HP;
	
	//method
	OurActor();
	virtual void damaged( int attack_pt, ACTORid attacker, float angle ) = 0;

	//action center
	float current_frame;
	OurAction* current_OurAction;
	OurAction* ourIdleAction;
	OurAction* ourCombatIdleAction;
	OurAction* ourRunAction;

	void ourPlayAction();
	bool sendAction(OurAction*);
	OurAction* getCurrentAction();
	OurFrame* getKeyFrame();
	float getCurrentFrame();

	bool playActionAudio();
	bool playActionFx();
};
#endif