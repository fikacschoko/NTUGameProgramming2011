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
protected:
	int priority;
	ACTIONid actID;
	float play_speed;
	float frames_num;
	Action_type type;
public:
	
};
class AttackAction : public OurAction{
protected:
	int defense_pt;
	Frame frame;
};