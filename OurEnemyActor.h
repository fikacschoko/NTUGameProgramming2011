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

		//method
		virtual void AI() = 0;

		//blood
		FnBillBoard blood;
		OBJECTid bloodID;
		float blood_length;
		float blood_width;

		void bloodAdjust();
};
#endif