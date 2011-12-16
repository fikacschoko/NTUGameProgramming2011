#ifndef _Donzo_H_
#define _Donzo_H_
#include "TheFlyWin32.h"
#include "OurEnemyActor.h"
#define PI 3.14159265

class Donzo : public OurEnemyActor{
	private:
		WORLDid gID;
		SCENEid sID;

	public:
		//var
		FnActor actor;
		float pos_begin[3];

		//method
		Donzo( WORLDid , SCENEid );
		void AI();

};
#endif