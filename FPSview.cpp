#include "FPSview.h"
#include "TheFlyWin32.h"
extern float debug[6];
FPSview* FPSview::class_now=NULL;

FPSview::FPSview( WORLDid gID, SCENEid sID,OBJECTid terrainID, int view_x_pos, int view_y_pos, int width, int height )
{
	FnWorld gw;
	FnScene scene;
	FnViewport vp;
	FnCamera camera;
	FnObject camStand;
	FnTerrain terrain;
	//adr_turn = this->turn;

	pos_begin[0]=3569.0;
	pos_begin[1]=-3208.0;
	pos_begin[2]=1000.0f;

	gw.Object(gID);
	scene.Object(sID);

	cFPS = scene.CreateCamera(ROOT);
	vID = gw.CreateViewport(view_x_pos, view_y_pos, width, height);
	vp.Object(vID);
	vp.SetBackgroundColor(0.3f, 0.4f, 0.5f);
	
	FPSstandID = scene.CreateObject(ROOT);
	camStand.Object(FPSstandID);
	//camStand.SetWorldPosition(pos_begin);
	camStand.Translate( pos_begin[0], pos_begin[1], pos_begin[2], GLOBAL );
	camStand.PutOnTerrain(terrainID, false, 100);

	camera.Object(cFPS);
	camera.SetParent(FPSstandID);
	float pos[]={0,0,0};
	camera.Translate( 0, 0, 0, GLOBAL );
	camera.Rotate(X_AXIS, 90.0f, REPLACE);
	camera.Rotate(Y_AXIS, 180.0f, LOCAL);
	camera.SetFov(100);
}
void FPSview::move()
{
	FnObject model;
	model.Object(FPSstandID);
	//model.Object(tdStandID);
	if (GetAsyncKeyState(FY_W)) {
	   //model.Translate(0.0f, 0.0f, -20, LOCAL);'
	   model.MoveForward(20,TRUE,false,100);
	}
	if(GetAsyncKeyState(FY_A)){
		//model.Translate(-20, 0.0, 0.0, LOCAL);
		model.MoveRight(-20,TRUE,false,100);
	}
	if (GetAsyncKeyState(FY_S) ){
		//model.Translate(0, 0, +20, LOCAL);
		model.MoveForward(-20,TRUE,false,100);
	}
	if (GetAsyncKeyState(FY_D) ){
		//model.Translate(+20, 0, 0, LOCAL);
		model.MoveRight(+20,TRUE,false,100);
	}
	float pos[3];
	model.GetPosition(pos);
	debug[0] = pos[0];
	debug[1] = pos[1];
	debug[2] = pos[2];
}


void FPSview::turn( WORLDid gID, int x, int y )
{
	
	FnObject model;
   if (x != class_now->oldX) {
	  model.Object(class_now->FPSstandID);
	  model.TurnRight( (float)(x - class_now->oldX) );
	  class_now->oldX = x;
   }

   if (y != class_now->oldY) {
	  model.Object(class_now->cFPS);
	  model.Rotate(X_AXIS, (float)- (y - class_now->oldY), LOCAL);
	  class_now->oldY = y;
   }
}


void FPSview::render()
{
	FnViewport vp;
	vp.Object(vID);
	vp.Render(cFPS, TRUE, TRUE);
}