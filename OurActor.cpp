#include "OurAction.h"
#include "OurActor.h"
#include "function.h"
int current_frame;
//OurActControl
OurActor::OurActor()
{
}
void OurActor::ourPlayAction()
{
	FnActor actor;
	actor.Object( aID );
	
	::actorChangePose( aID, current_OurAction->actID );
	//it's a loop action?
	if( current_OurAction->type == Action_type::ACTION_IDLE() ||
		current_OurAction->type == Action_type::ACTION_WALK() )
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

bool OurActor::sendAction( OurAction* action )
{
	if( action->priority < current_OurAction->priority )
		return false;
	current_OurAction = action;
	return true;
}

OurAction* OurActor::getCurrentAction()
{
	return current_OurAction;
}

OurFrame* OurActor::getKeyFrame()
{
	int i;
	
	if( current_OurAction->type != Action_type::ACTION_ATTACK() )
		return NULL;
		
	AttackAction *attackAction = (AttackAction*)current_OurAction;
	for( i=0 ; i < attackAction->numOfKeyFrames ; i++ )
	{
		if( attackAction->keyFrames[i]->frameNO <= current_frame && attackAction->keyFrames[i]->frameNO > current_frame - attackAction->play_speed )
			return attackAction->keyFrames[i];
	}
	return NULL;
}

float OurActor::getCurrentFrame()
{
	return current_frame;
}