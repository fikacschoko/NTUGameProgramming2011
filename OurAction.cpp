#include "OurAction.h"
OurAction::OurAction()
{
	this->combo_able_frame_end = 0;
	this->combo_able_frame_start = 0;
	this->defense_pt = 0;
	this->keyFrames = 0;
	this->isAttack = false;
	this->audioFrames = 0;
	//this->type = NULL;
}

OurAction::OurAction(int priority,	float play_speed,	float frames_num,	ACTIONid actID,	char *type){
	//OurAction();
	this->priority = priority;
	this->play_speed = play_speed;
	this->frames_num = frames_num;
	this->actID = actID;
	this->type.value = type;
};