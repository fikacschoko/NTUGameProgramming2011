#ifndef _Robber02_H_
	#define _Robber02_H_
	#include "TheFlyWin32.h"
	#include "OurEnemyActor.h"
	#define PI 3.14159265

	class Robber02Action : public Action_type
	{
		public:

	};	


	class Robber02 : public OurEnemyActor{
		private:
			WORLDid gID;
			SCENEid sID;
	
		public:
			//var
			float pos_begin[3];
			OurAction *ourDamage1Action, 
				*ourDamage2Action,
				*ourDieAction;

			//method
			Robber02( WORLDid , SCENEid );
			void AI(ACTORid enemy, ACTORid *friends, int friends_num, bool leader);
			void damaged(int, ACTORid, float);
	};
#endif