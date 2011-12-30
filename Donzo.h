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
		//var
		//FnActor actor;
		float pos_begin[3];
		OurAction *ourDamageLAction, 
			*ourDamageHAction,
			*ourDieAction;

		//method
		Donzo( WORLDid , SCENEid );
		void AI(ACTORid enemy, ACTORid *friends, int friends_num, bool leader);
		void damaged(int, ACTORid, float);
};
#endif