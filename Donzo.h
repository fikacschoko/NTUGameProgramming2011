#ifndef _Donzo_H_
#define _Donzo_H_
#include "TheFlyWin32.h"
#include "OurEnemyActor.h"
#define PI 3.14159265

class Donzo : public OurEnemyActor{
	public:
		Donzo();
		void AI();
};
#endif