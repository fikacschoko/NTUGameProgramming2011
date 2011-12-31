#ifndef _Donzo_H_
#define _Donzo_H_
#include "TheFlyWin32.h"
#include "OurEnemyActor.h"
#include "OurActor.h"
#include "OurAction.h"
#define PI 3.14159265

class DonzoAction : public Action_type
{
	public:

};

class Donzo : public OurEnemyActor{
	public:
		static const float ATTACK_RATE;
		//var
		//FnActor actor;
		float pos_begin[3];
		OurAction	*ourAttack1Action,
					*ourHeavyAttack1Action,
					*ourHeavyAttack2Action,
					*ourDamageLAction, 
					*ourDamageHAction,
					*ourDieAction;

		//method
		Donzo( WORLDid , SCENEid );
		bool attackAgent(ACTORid enemyID);
		void AI(ACTORid enemy, EnemyTeam **team,  int teamCount);
		void damaged(int, ACTORid, float);
};
#endif