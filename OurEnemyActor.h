#ifndef _OurEnemyActor_H_
#define _OurEnemyActor_H_
#include "TheFlyWin32.h"
#include "OurActor.h"
#define PI 3.14159265

struct EnemyTeam{
	int teamNO;
	bool aware;
};

class OurEnemyActor : public OurActor{
	public:
		//var
		EnemyTeam *team;
		bool justRun;
		//method
		OurEnemyActor();
		virtual void AI(ACTORid enemy, ACTORid *friends, int friends_num, bool leader) = 0;
		void walkingAgent(ACTORid enemy, ACTORid *friends, int friends_num, bool leader);
	private:
		void flockingPosition(float *newPos, float *selfPos, float *targetPos, ACTORid *groupMember, float groupMember_num);
};

#endif