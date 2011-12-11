#include "TheFlyWin32.h"
#include "OurAction.h"

bool actorChangePose( ACTORid , ACTIONid );

bool beenHit( ACTORid attacker , ACTORid defender, OurFrame frame );

float twoObjectDis( OBJECTid , OBJECTid );

float posObjectDis( float* , OBJECTid );