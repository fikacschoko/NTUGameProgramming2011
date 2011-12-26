#include"AllData.h"

AUDIOid AllAudio::s01_pose05 = 0;
AUDIOid AllAudio::s01_pose07 = 0;
AUDIOid AllAudio::s01_pose12 = 0;
AUDIOid AllAudio::s01_pose16 = 0;
AUDIOid AllAudio::s02_pose07 = 0;
AUDIOid AllAudio::s02_pose10 = 0;
AUDIOid AllAudio::s02_pose22 = 0;
AUDIOid AllAudio::s02_pose25 = 0;
AUDIOid AllAudio::s02_pose26 = 0;
AUDIOid AllAudio::s03_pose18 = 0;
AUDIOid AllAudio::s03_pose22 = 0;
AUDIOid AllAudio::s03_pose25 = 0;
AUDIOid AllAudio::eat = 0;
AUDIOid AllAudio::se_select = 0;


void loadAll( WORLDid gID )
{
	FnWorld gw;
	FnAudio audio;
	gw.Object(gID);
	gw.SetAudioPath("Data\\Audio");

	AllAudio::s01_pose05 = gw.CreateAudio();
	audio.Object(AllAudio::s01_pose05);
	audio.Load("01_pose05");

	AllAudio::s01_pose07 = gw.CreateAudio();
	audio.Object(AllAudio::s01_pose07);
	audio.Load("01_pose07");

	AllAudio::s01_pose12 = gw.CreateAudio();
	audio.Object(AllAudio::s01_pose12);
	audio.Load("01_pose12");

	AllAudio::s01_pose16 = gw.CreateAudio();
	audio.Object(AllAudio::s01_pose16);
	audio.Load("01_pose16");

	AllAudio::s02_pose07 = gw.CreateAudio();
	audio.Object(AllAudio::s02_pose07);
	audio.Load("02_pose07");

	AllAudio::s02_pose10 = gw.CreateAudio();
	audio.Object(AllAudio::s02_pose10);
	audio.Load("02_pose10");

	AllAudio::s02_pose22 = gw.CreateAudio();
	audio.Object(AllAudio::s02_pose22);
	audio.Load("02_pose22");

	AllAudio::s02_pose25 = gw.CreateAudio();
	audio.Object(AllAudio::s02_pose25);
	audio.Load("02_pose25");

	AllAudio::s02_pose26 = gw.CreateAudio();
	audio.Object(AllAudio::s02_pose26);
	audio.Load("02_pose26");

	AllAudio::s03_pose18 = gw.CreateAudio();
	audio.Object(AllAudio::s03_pose18);
	audio.Load("03_pose18");

	AllAudio::s03_pose22 = gw.CreateAudio();
	audio.Object(AllAudio::s03_pose22);
	audio.Load("03_pose22");

	AllAudio::s03_pose25 = gw.CreateAudio();
	audio.Object(AllAudio::s03_pose25);
	audio.Load("03_pose25");

	AllAudio::eat = gw.CreateAudio();
	audio.Object(AllAudio::eat);
	audio.Load("eat");

	AllAudio::se_select = gw.CreateAudio();
	audio.Object(AllAudio::se_select);
	audio.Load("se_select");
}