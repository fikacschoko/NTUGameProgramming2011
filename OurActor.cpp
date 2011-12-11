#include "OurAction.h"
#include "OurActor.h"
int current_frame;
OurAction* ourIdleAction;
OurAction* current_OurAction;

//OurActControl
void OurActor::ourPlayAction()
{
	FnActor actor;
	actor.Object( aID );
	
	::actorChangePose( aID, current_OurAction->actID );
	//it's a loop action?
	if( current_OurAction.type == ACTION_IDLE ||
		current_OurAction.type == ACTION_WALK )
	{
		actor.Play(0, LOOP, current_OurAction->play_speed, false, true);
	}
	else
	{
		bool notOver;
		notOver = actor.Play(0, ONCE, current_OurAction->play_speed, false, true);
		if( !notOver )
			current_OurAction = ourIdleAction;
	}
}

bool sendAction( OutAction* action )
{
	if( action->priority <= current_OurAction->priority )
		return false;
	current_OurAction = action;
	return true;
}

OurAction* getCurrentAction()
{
	return current_OurAction;
}

