#include "TheFlyWin32.h"
#include "OurAction.h"

class OurActor{
public:
	ACTORid aid;
	int current_frame;

	void ourPlayAction();
	bool sendAction(OurAction);
	OurAction getCurrentAction();
	
	void damaged(int attack_pt);
};