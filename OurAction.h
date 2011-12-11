#ifndef _OURACTION_H_
#define _OURACTION_H_
#include "TheFlyWin32.h"

enum Action_type{ACTION_IDLE, ACTION_WALK, ACTION_DAMAGED, ACTION_ATTACK};
struct Frame{
	float start_angle;
	float valid_angle;
	float valid_dis;
	int damage_pt;
	bool combo_able;
};
class OurAction{
public:
	OurAction();
	OurAction(int priority,	float play_speed,	float frames_num,	ACTIONid actID,	Action_type type);
	ACTIONid actID;
	int priority;
	float play_speed;
	float frames_num;
	Action_type type;
};
class AttackAction : public OurAction{
protected:
	int defense_pt;
	Frame frame;
};
#endif