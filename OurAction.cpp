#include "OurAction.h"
OurAction::OurAction()
{

}

OurAction::OurAction(int priority,	float play_speed,	float frames_num,	ACTIONid actID,	char *type){
	this->priority = priority;
	this->play_speed = play_speed;
	this->frames_num = frames_num;
	this->actID = actID;
	this->type.value = type;
};