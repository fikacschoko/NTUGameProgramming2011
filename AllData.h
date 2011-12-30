#ifndef _ALLDATA_H_
#define _ALLDATA_H_
#include "TheFlyWin32.h"
#include "FyFx.h"

class AllAudio
{
public:
	static AUDIOid s01_pose05;
	static AUDIOid s01_pose07;
	static AUDIOid s01_pose12;
	static AUDIOid s01_pose16;
	static AUDIOid s02_pose07;
	static AUDIOid s02_pose10;
	static AUDIOid s02_pose22;
	static AUDIOid s02_pose25;
	static AUDIOid s02_pose26;
	static AUDIOid s03_pose18;
	static AUDIOid s03_pose22;
	static AUDIOid s03_pose25;
	static AUDIOid eat;
	static AUDIOid se_select;
};

class AllFx
{
public:
	static WORLDid gID;
	static eF3DFX* getFX(char *filename, SCENEid sid);
	static char* Attack01;
	
private:
	
};

void loadAll( WORLDid gID );
#endif