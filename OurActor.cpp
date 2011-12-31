#include "OurAction.h"
#include "OurActor.h"
#include "function.h"
#include "AllData.h"

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
		bool notOver;
		notOver = actor.Play(0, ONCE, current_OurAction->play_speed, false, true);
		if( notOver )
			current_frame += current_OurAction->play_speed;
		else
		{
			current_frame = 0;
			actor.MakeCurrentAction(0, NULL, current_OurAction->actID);
		}
	}
	else
	{
		bool notOver;
		notOver = actor.Play(0, ONCE, current_OurAction->play_speed, FALSE, TRUE);
		if( !notOver )
		{
			if( current_OurAction->type.value != Action_type::ACTION_DIE() )
				current_OurAction = ourIdleAction;
		}
		else
			current_frame += current_OurAction->play_speed;
	}

	playActionAudio();
	playActionFx();
}

bool OurActor::sendAction( OurAction* action )
{
	if( action->priority < current_OurAction->priority )
		return false;
	current_OurAction = action;
	if( action->type == Action_type::ACTION_DAMAGED() )
	{
		current_frame = 0;
		actor.MakeCurrentAction(0, NULL, current_OurAction->actID);
	}
	return true;
}

OurAction* OurActor::getCurrentAction()
{
	return current_OurAction;
}

OurFrame* OurActor::getKeyFrame()
{
	if(current_OurAction->keyFrames > 0)
	{
		for( int i=0 ; i < current_OurAction->numOfKeyFrames ; i++ )
		{
			if( current_OurAction->keyFrames[i]->frameNO <= current_frame && 
				current_OurAction->keyFrames[i]->frameNO >= current_frame - current_OurAction->play_speed )
				return current_OurAction->keyFrames[i];
		}
	}
	return NULL;
}

float OurActor::getCurrentFrame()
{
	return current_frame;
}

bool OurActor::playActionAudio()
{
	if( current_OurAction->numOfAudioFrames > 0 )
	{
		FnAudio audio;
		for( int i=0 ; i<current_OurAction->numOfAudioFrames ; i++ )
		{
			if( current_OurAction->audioFrames[i]->frameNO <= current_frame && 
				current_OurAction->audioFrames[i]->frameNO >= current_frame - current_OurAction->play_speed ){
					audio.Object(current_OurAction->audioFrames[i]->audioID);
					audio.Play(ONCE);
					return true;
			}
		}
	}
	return false;
}

bool OurActor::playActionFx()
{
	if( current_OurAction->numOfFxFrames > 0 )
	{
		for( int i=0 ; i<current_OurAction->numOfFxFrames ; i++ )
		{
			if( current_OurAction->fxFrames[i]->frameNO <= current_frame && 
				current_OurAction->fxFrames[i]->frameNO >= current_frame - current_OurAction->play_speed ){
				
					current_OurAction->fxFrames[i]->fx = AllFx::getFX(current_OurAction->fxFrames[i]->fxName, sID);


					eF3DBaseFX *fx_sub;
					float pos[3];
					actor.GetWorldPosition(pos);
					pos[2] = 0;
					int numFX = current_OurAction->fxFrames[i]->fx->NumberFXs();
					for (int j = 0; j < numFX; j++) {
						fx_sub = current_OurAction->fxFrames[i]->fx->GetFX(j);
						if( fx_sub == NULL )
							continue;
						char *parent_name = fx_sub->GetParentName();
						OBJECTid oid = actor.GetBoneObject( parent_name );
						if( oid != FAILED_ID )
						{
							fx_sub->SetParent( oid );
							//pos[0] = pos[1] = pos[2] = 0;
							//fx_sub->InitPosition(pos);
						}
						else
						{
							fx_sub->InitPosition(pos);
						}
					}

					FXcenter::playFX( current_OurAction->fxFrames[i]->fx );
			}
		}
	}
	return false;
}