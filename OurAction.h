#ifndef _OURACTION_H_
#define _OURACTION_H_
#include "TheFlyWin32.h"

enum Action_type{ACTION_IDLE, ACTION_WALK, ACTION_DAMAGED, ACTION_ATTACK};
struct OurFrame{
	float frameNO;
	float start_angle;
	float valid_angle;
	float valid_dis;
	int damage_pt;
};

class OurAction{
public:
	OurAction();
	OurAction(int priority,	float play_speed,	float frames_num,	ACTIONid actID,	Action_type type);
	ACTIONid actID;
	int priority;
	float play_speed;
	float frames_num;
	int defense_pt;
	Action_type type;
};
class AttackAction : public OurAction{
public:
	int numOfKeyFrames;
	float combo_able_frame_start, combo_able_frame_end;
	OurFrame **keyFrames;
};
#endif