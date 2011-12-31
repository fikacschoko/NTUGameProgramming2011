#ifndef _Robber02_H_
#define _Robber02_H_
#include "TheFlyWin32.h"
#include "OurEnemyActor.h"
#define PI 3.14159265

#define ATTACK_RATE 10.0

class Robber02Action : public Action_type
{
	public:

};	


class Robber02 : public OurEnemyActor{
	public:
		//var
		float pos_begin[3];
		OurAction	*ourAttack1Action,
					*ourAttack2Action,
					*ourDamage1Action, 
					*ourDamage2Action,
					*ourHeavyAttackAction,
					*ourDieAction;

		//method
		Robber02( WORLDid , SCENEid );
		bool attackAgent(ACTORid enemyID);
		void AI(ACTORid enemyID, EnemyTeam **team,  int teamCount);
		void damaged(int, ACTORid, float);
};
#endif