#ifndef _OURACTION_H_
#define _OURACTION_H_
#include "TheFlyWin32.h"
#include "FyFx.h"

class Action_type
{
public:
	Action_type()
	{
	}
	Action_type(char *type)
	{
		this->value = type;
	}
	char *value;
	static char*	ACTION_IDLE(){	return "GENERAL_IDLE"; }
	static char*	ACTION_WALK(){	return "GENERAL_WALK"; }
	static char*	ACTION_DAMAGED(){	return "GENERAL_DAMAGED"; }
	static char*	ACTION_ATTACK(){	return "GENERAL_ATTACK"; }
	static char*	ACTION_DIE(){	return "GENERAL_DIE"; }

	bool operator==(const char *value)
	{
		return strcmp(this->value, value) == 0;
	}
	bool operator!=(const char *value)
	{
		return strcmp(this->value, value) != 0;
	}
};

struct OurFrame{
	float frameNO;
	float start_angle;
	float plus_angle;
	float valid_dis;
	int damage_pt;
};

struct OurAudioFrame{
	float frameNO;
	AUDIOid audioID;
};

struct OurFxFrame{
	float frameNO;
	char* fxName;
	eF3DFX *fx;
};

class OurAction{
public:
	ACTIONid actID;
	int priority;
	float play_speed;
	float frames_num;
	int defense_pt;
	bool isAttack;
	Action_type type;
	float combo_able_frame_start, combo_able_frame_end;
	//key frame for attack detect
	int numOfKeyFrames;
	OurFrame **keyFrames;
	//audio frame
	int numOfAudioFrames;
	OurAudioFrame **audioFrames;
	//FX frame
	int numOfFxFrames;
	OurFxFrame **fxFrames;

	OurAction();	
	OurAction(int priority,	float play_speed,	float frames_num,	ACTIONid actID,	char *type);
	/*
	static class Builder{
	public:
		ACTIONid temp_actID;
		int temp_priority;
		float temp_play_speed;
		float temp_frames_num;
		int temp_defense_pt;
		Action_type *temp_type;

		Builder* actID(ACTIONid actID){	this->temp_actID = actID; return this; }
		Builder* priority(int priority){	this->temp_priority = priority; return this; }
		Builder* play_speed(float play_speed){	this->temp_play_speed = play_speed; return this; }
		Builder* frames_num(float frames_num){	this->temp_frames_num = frames_num; return this; }
		Builder* defense_pt(int defense_pt){	this->temp_defense_pt = defense_pt; return this; }
		Builder* type(char *type)
		{
			this->temp_type = new Action_type(type);
			return this;
		}
		OurAction* build()
		{
			return new OurAction(this);
		}
	};
	OurAction(Builder *builder)
	{
		actID = builder->temp_actID;
		priority = builder->temp_priority;
		play_speed = builder->temp_play_speed;
		frames_num = builder->temp_frames_num;
		defense_pt = builder->temp_defense_pt;
		type = builder->temp_type;
	}
	*/
};
#endif