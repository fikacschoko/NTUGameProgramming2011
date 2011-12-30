#ifndef _OurEnemyActor_H_
#define _OurEnemyActor_H_
#include "TheFlyWin32.h"
#include "OurActor.h"
#define PI 3.14159265
#define AWARE_DISTANCE 500
#define COMBAT_DISTANCE 150
#define CROWDED_DISTANCE 80
#define OVERLAP_DISTANCE 30
#define SPEED 5

class OurEnemyActor;
class EnemyTeam{
public:
	int teamNO;
	bool aware;
	int member_num;
	OurEnemyActor **members;
	//ACTORid members[5];

	EnemyTeam(int teamNO, int member_num)
	{
		this->teamNO = teamNO;
		this->aware = false;
		this->member_num = member_num;
		this->members = new OurEnemyActor*[5];
	}
};

class OurEnemyActor : public OurActor{
	public:
		//var
		EnemyTeam *team;

		bool justRun;
		//method
		OurEnemyActor();
		virtual void AI(ACTORid enemy, EnemyTeam **team, int teamCount) = 0;
		void detectEnemy(ACTORid enemy);
		void walkingAgent(ACTORid enemy, EnemyTeam **team, int teamCount);
		void attackAgent();
		OBJECTid bloodID;
		float blood_length;
		float blood_width;

		void bloodAdjust();
		
	private:
		void flockingPosition(float *newPos, float *selfPos, float *targetPos, EnemyTeam **team, int teamCount);
	
};

#endif