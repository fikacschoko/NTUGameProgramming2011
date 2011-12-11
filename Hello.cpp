/*==============================================================
  This is a demo program for TheFly System

  Hello !

  (C)2004 Chuan-Chang Wang, All Rights Reserved
  Created : 0303, 2004, C. Wang

  Last Updated : 1010, 2004, C.Wang
 ===============================================================*/

#define MAX_LINES 1

#include <time.h>
#include "TheFlyWin32.h"

int oldX, oldY, oldXM, oldYM, oldXMM, oldYMM;

WORLDid gID = FAILED_ID;
VIEWPORTid vID, vID2;
SCENEid sID, sID2;
OBJECTid axisGuideID,
		 cID, cID2,
		 lID, lID2,
		 terrainID,
		 planeID,
		 linesID[MAX_LINES];
MATERIALid redID, greenID, blueID, whiteID;
SHUFAid shufaID;
ACTORid actorID;

void QuitGame(WORLDid, BYTE, BOOL);
void GameAI(int);

void InitPivot(WORLDid, int, int);
void PivotCam(WORLDid, int, int);
void InitMove(WORLDid, int, int);
void MoveCam(WORLDid, int, int);
void InitZoom(WORLDid, int, int);
void ZoomCam(WORLDid, int, int);
void setWorld();
void setCamera();
void setMaterial();
void setLight();
void setObject();
void setActor();
void setInteraction();

/*------------------
  the main program
  C.Wang 0308, 2004
 -------------------*/
void main(int argc, char **argv)
{
	setWorld();
	setCamera();
	setMaterial();
	setLight();
	setObject();
	//setActor();
	setInteraction();

   /* bind a timer, frame rate = 30 fps */
   FyBindTimer(0, 30.0f, GameAI, TRUE);

   // invoke the system
   FyInvokeTheFly(TRUE);
}

/*------------------
  set All the things
 -------------------*/

//including world, scene, terrain
void generateLine(OBJECTid lineID, float *corner)
{
	FnObject line;
	line.Object(lineID);

	float vertices[6] = {corner[0]-10, corner[1], corner[2],
					 corner[0]+10, corner[1], corner[2]};
	line.Lines(XYZ, redID, vertices, 2, NULL);

	srand(time(NULL));
	//line.Rotate(Z_AXIS, (double)(rand()%90), LOCAL);
}
void setWorld()
{
   // create a new world
   gID = FyWin32CreateWorld("Hello TheFly !", 0, 0, 800, 600, 32, FALSE);
   //gID = FyWin32CreateWorld("Hello TheFly !", 0, 0, 100, 100, 32, FALSE);

   FnWorld gw;
   gw.Object(gID);
   gw.SetObjectPath("Data\\NTU4\\stage4\\Scenes");
   gw.SetTexturePath("Data\\NTU4\\stage4\\Textures");
   gw.SetScenePath("Data\\NTU4\\stage4\\Scenes");
   
   // create a 3D scene & the 3D entities
   sID = gw.CreateScene(10);
   sID2 = gw.CreateScene(11);

   FnScene scene;
   scene.Object(sID);
   scene.Load("scene2");
   cID = scene.CreateCamera(ROOT);
   lID = scene.CreateLight(ROOT);

   //
   scene.Object(sID2);
   cID2 = scene.CreateCamera(ROOT);
   lID2 = scene.CreateLight(ROOT);

   // set text output
   shufaID = gw.CreateShuFa("·s²Ó©úÅé", 12);

   // set terrain
   terrainID = scene.CreateTerrain(ROOT);
   FnTerrain terrain;
   terrain.Object(terrainID);
   terrain.Load("ground");
   terrain.GenerateTerrainData();

}
void setMaterial()
{
	FnWorld gw;
	gw.Object(gID);

	float red[3] = {255.0f, 0.0f, 0.0f};
	float green[3] = {0.0f, 255.0f, 0.0f};
	float blue[3] = {0.0f, 0.0, 255.0f};
	float white[3] = {255.0f, 255.0f, 255.0f};
	redID = gw.CreateMaterial(red, red, red, 1, red, 1, FALSE);
	greenID = gw.CreateMaterial(green, green, green, 1, green, 1, FALSE);
	blueID = gw.CreateMaterial(blue, blue, blue, 1, blue, 1, FALSE);
	whiteID = gw.CreateMaterial(white, white, white, 1, white, 1, FALSE);
	FnMaterial m;
	m.Object(whiteID);
	m.AddTexture(1,1,"trans");
}
void setObject()
{
	// create a axisGuide
	FnScene scene;
	scene.Object(sID2);
	axisGuideID = scene.CreateObject(ROOT);

	FnObject axisGuide;
	axisGuide.Object(axisGuideID);
	float pos[18] = {0.0f, 0.0f, 0.0f,
					 2.0f, 0.0f, 0.0f,
					 0.0f, 0.0f, 0.0f,
					 0.0f, 2.0f, 0.0f,
					 0.0f, 0.0f, 0.0f,
					 0.0f, 0.0f, 2.0f};
	axisGuide.Lines(XYZ, redID, pos, 2, FALSE);
	axisGuide.Lines(XYZ, greenID, pos+6, 2, FALSE);
	axisGuide.Lines(XYZ, blueID, pos+12, 2, FALSE);

	// create a plane
	scene.Object(sID);
	planeID = scene.CreateObject(ROOT);

	FnObject plane;
	plane.Object(planeID);
	float pos2[18] = {-20.0f, -20.0f, -0.5f,
					 20.0f, -20.0f, -0.5f,
					 -20.0f, 20.0f, -0.5f,
					 20.0f, -20.0f, -0.5f,
					 20.0f, 20.0f, -0.5f,
					-20.0f, 20.0f, -0.5f};
	int indices[6] = {0, 1, 2,3,4,5};
	int texLen[6] = {0};
	plane.IndexedTriangle(XYZ, blueID, 1, 6, 1, texLen, pos2, 2, indices, FALSE);

	// create lines
	scene.Object(sID);
	for(int i = 0; i < MAX_LINES; i++)
	{
		linesID[i] = scene.CreateObject(ROOT);

		FnObject line;
		line.Object(linesID[i]);
		line.Load("laser_in");
		//line.SetOpacity(0.3f);

	}
}
void setCamera()
{
   FnWorld gw;
   gw.Object(gID);
   // create a viewport
   vID = gw.CreateViewport(0, 0, 800, 600);
   vID2 = gw.CreateViewport(750, 550, 50, 50);
   FnViewport vp;
   vp.Object(vID);
   vp.SetBackgroundColor(0.3f, 0.4f, 0.5f);
   vp.Object(vID2);
   vp.SetBackgroundColor(0.3f, 0.4f, 0.5f);

   // translate the camera
   FnCamera camera;
   camera.Object(cID);
   camera.Rotate(X_AXIS, 60.0f, LOCAL);
   camera.Translate(0.0f, 0.0f, 100.0f, LOCAL);
   camera.Object(cID2);
   camera.Rotate(X_AXIS, 60.0f, LOCAL);
   camera.Translate(0.0f, 0.0f, 100.0f, LOCAL);
   camera.SetFov(1.0f);
}
void setActor()
{
   FnWorld gw;
   gw.Object(gID);
   FnScene scene;
   scene.Object(sID);

   // set actor
   gw.SetObjectPath("Data\\NTU4\\Characters");
   gw.SetTexturePath("Data\\NTU4\\Characters");
   gw.SetCharacterPath("Data\\NTU4\\Characters");
   gw.SetShaderPath("Data\\NTU4\\Shaders");

   actorID = scene.LoadActor("Lyubu");
   FnActor actor;
   actor.Object(actorID);
   float a[3] = {0.0f, 0.0f, 1000.0f};
   actor.SetPosition(a);
   if(!actor.PutOnTerrain(terrainID, FALSE, 0.0f))
	   printf("FAILED");

   // get Idle action
   ACTIONid  idleID = actor.GetBodyAction(NULL, "Idle");
   // make idle action as the current action
   actor.MakeCurrentAction(0, NULL, idleID);
}
void setLight()
{
   // translate the light
   FnLight light;
   light.Object(lID);
   light.SetName("MainLight");
   light.Translate(-50.0f, -50.0f, 50.0f, GLOBAL);
   light.Object(lID2);
   light.SetName("MainLight2");
   light.Translate(-50.0f, -50.0f, 50.0f, GLOBAL);
}
void setInteraction()
{
   // set Hotkeys
   FyDefineHotKey(FY_ESCAPE, QuitGame, FALSE);

   // define some mouse functions
   FyBindMouseFunction(LEFT_MOUSE, InitPivot, PivotCam, NULL, NULL);
   FyBindMouseFunction(MIDDLE_MOUSE, InitZoom, ZoomCam, NULL, NULL);
   FyBindMouseFunction(RIGHT_MOUSE, InitMove, MoveCam, NULL, NULL);
}

//------------------------------------------------------------------------------------
// timer callback function which will be invoked by TheFly3D System every 1/30 second
// C.Wang 0308, 2004
//------------------------------------------------------------------------------------
void GameAI(int skip)
{
   FnActor actor;
   actor.Object(actorID);

   // play the 1 st frame of idle action
   actor.Play(0, LOOP, (float)skip, FALSE, TRUE);

   // move line
   /*
   FnObject line;
   line.Object(linesID[0]);
   float pos[3];
   line.GetPosition(pos);
   if(pos[1] < -10.0f)
   {
	   float corner[3] = {0.0f, 0.0f, 0.0f};
	   generateLine(linesID[0], corner);
   }
   else
   {
	   line.MoveForward(0.1f);
   }
   */
   // render the scene
   FnViewport vp;
   vp.Object(vID);
   vp.Render(cID, TRUE, TRUE);
   vp.Object(vID2);
   vp.Render(cID2, TRUE, TRUE);

   // show text "after" rendering
   FnCamera camera;
   camera.Object(cID);
   float fDir[3], uDir[3];
   camera.GetDirection(fDir, uDir);

   FnShuFa writer;
   writer.Object(shufaID);
   writer.Begin(vID);
   char str[100];
   sprintf(str, "Camera fDir (%.1f, %.1f, %.1f)", fDir[0], fDir[1], fDir[2]);
   writer.Write(str, 10, 600-12-20, 0xFF, 0xFF, 0xFF);
   sprintf(str, "Camera uDir (%.1f, %.1f, %.1f)", uDir[0], uDir[1], uDir[2]);
   writer.Write(str, 10, 600-12-10, 0xFF, 0xFF, 0xFF);
   writer.End();

   FnWorld gw;
   gw.Object(gID);
   gw.SwapBuffers();
}


//-------------------
// quit the demo
// C.Wang 0304, 2004
//-------------------
void QuitGame(WORLDid gID, BYTE code, BOOL value)
{
   if (code == FY_ESCAPE) {
      if (value) {
         FyWin32EndWorld(gID);
      }
   }
}


/*-----------------------------------
  initialize the pivot of the camera
  C.Wang 0329, 2004
 ------------------------------------*/
void InitPivot(WORLDid g, int x, int y)
{
   oldX = x;
   oldY = y;
}


/*------------------
  pivot the camera
  C.Wang 0329, 2004
 -------------------*/
void PivotCam(WORLDid g, int x, int y)
{
   FnObject model;

   if (x != oldX) {
      model.Object(cID);
      model.Rotate(Z_AXIS, (float) (x - oldX), GLOBAL);
	  // rotate axisGuide
	  model.Object(cID2);
      model.Rotate(Z_AXIS, (float) (x - oldX), GLOBAL);
      oldX = x;
   }

   if (y != oldY) {
      model.Object(cID);
      model.Rotate(X_AXIS, (float) (y - oldY), GLOBAL);
      // rotate axisGuide
	  model.Object(cID2);
      model.Rotate(X_AXIS, (float) (y - oldY), GLOBAL);
      oldY = y;
   }
}


/*----------------------------------
  initialize the move of the camera
  C.Wang 0329, 2004
 -----------------------------------*/
void InitMove(WORLDid g, int x, int y)
{
   oldXM = x;
   oldYM = y;
}


/*------------------
  move the camera
  C.Wang 0329, 2004
 -------------------*/
void MoveCam(WORLDid g, int x, int y)
{
   if (x != oldXM) {
      FnObject model;

      model.Object(cID);
      model.Translate((float)(x - oldXM)*0.1f, 0.0f, 0.0f, LOCAL);
      oldXM = x;
   }
   if (y != oldYM) {
      FnObject model;

      model.Object(cID);
      model.Translate(0.0f, (float)(oldYM - y)*0.1f, 0.0f, LOCAL);
      oldYM = y;
   }
}


/*----------------------------------
  initialize the zoom of the camera
  C.Wang 0329, 2004
 -----------------------------------*/
void InitZoom(WORLDid g, int x, int y)
{
   oldXMM = x;
   oldYMM = y;
}


/*------------------
  zoom the camera
  C.Wang 0329, 2004
 -------------------*/
void ZoomCam(WORLDid g, int x, int y)
{
   if (x != oldXMM || y != oldYMM) {
      FnObject model;

      model.Object(cID);
      model.Translate(0.0f, 0.0f, (float)(x - oldXMM), LOCAL);
      oldXMM = x;
      oldYMM = y;
   }
}