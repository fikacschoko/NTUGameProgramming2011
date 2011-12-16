#include "TheFlyWin32.h"
#include "action.h"

bool actorChangePose( ACTORid , ACTIONid );

bool beenHit( ACTORid  , ACTORid , OurFrame );

float twoObjectDis( OBJECTid , OBJECTid );

float posObjectDis( float* , OBJECTid );

float twoObjectAngle(  OBJECTid , OBJECTid );

float twoObjectCross(  OBJECTid , OBJECTid );

//float twoDirAngle( float* , float* );