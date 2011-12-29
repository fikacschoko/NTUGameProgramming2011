#include "OurEnemyActor.h"
void OurEnemyActor::bloodAdjust()
{
	float size[2];
	size[0] = blood_length*HP/HP_MAX;
	size[1] = blood_width;
	blood.SetSize(size);
}