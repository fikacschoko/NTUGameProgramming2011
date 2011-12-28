#include "TheFlyWin32.h"
#include "OurActor.h"

bool actorChangePose( ACTORid , ACTIONid );

bool beenHit( ACTORid  , ACTORid , OurFrame, float );

float twoObjectDis( OBJECTid , OBJECTid );

float twoPointDis( float*, float* );

float posObjectDis( float* , OBJECTid );

float twoObjectAngle(  OBJECTid , OBJECTid );

bool actorFaceTo( ACTORid aID, float x, float y, float z ); //world position