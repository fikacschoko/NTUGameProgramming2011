#ifndef _OURACTOR_H_
#define _OURACTOR_H_
#include "TheFlyWin32.h"
#include "OurAction.h"

class OurActor{
public:
	OurActor();
	ACTORid aID;
	int current_frame;

	OurAction* current_OurAction;
	OurAction* ourIdleAction;
	void ourPlayAction();
	bool sendAction(OurAction*);
	OurAction* getCurrentAction();
	OurFrame* getKeyFrame();
	float getCurrentFrame();
	void damaged(int attack_pt);
};
#endif