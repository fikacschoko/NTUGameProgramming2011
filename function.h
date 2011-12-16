#include "TheFlyWin32.h"
#include "OurActor.h"

bool actorChangePose( ACTORid , ACTIONid );

bool beenHit( ACTORid  , ACTORid , OurFrame, float );

float twoObjectDis( OBJECTid , OBJECTid );

float posObjectDis( float* , OBJECTid );

float twoObjectAngle(  OBJECTid , OBJECTid );