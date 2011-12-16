#include "OurAction.h"
#include "OurActor.h"
#include "function.h"
#include <exception>
//OurActControl
OurActor::OurActor()
{
}
void OurActor::ourPlayAction()
{
	FnActor actor;
	actor.Object( aID );
	
	if(::actorChangePose( aID, current_OurAction->actID ))
	{
		current_frame = 0;
	}
	//it's a loop action?
	if( current_OurAction->type == Action_type::ACTION_IDLE() ||
		current_OurAction->type == Action_type::ACTION_WALK() )
	{
		actor.Play(0, LOOP, current_OurAction->play_speed, false, true);
		current_frame += current_OurAction->play_speed;
	}
	else
	{
		bool notOver;
		notOver = actor.Play(0, ONCE, current_OurAction->play_speed, false, true);
		if( !notOver )
			current_OurAction = ourIdleAction;
		else
			current_frame += current_OurAction->play_speed;
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
	if(current_OurAction->keyFrames > 0)
	{
		for( i=0 ; i < current_OurAction->numOfKeyFrames ; i++ )
		{
			if( current_OurAction->keyFrames[i]->frameNO <= current_frame && current_OurAction->keyFrames[i]->frameNO > current_frame - current_OurAction->play_speed )
				return current_OurAction->keyFrames[i];
		}
	}
	return NULL;
}

float OurActor::getCurrentFrame()
{
	return current_frame;
}