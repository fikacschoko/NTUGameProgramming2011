/*==============================================================
  This is a demo program for TheFly System

  Hello !

  (C)2004 Chuan-Chang Wang, All Rights Reserved
  Created : 0303, 2004, C. Wang

  Last Updated : 1010, 2004, C.Wang
 ===============================================================*/
#include "TheFlyWin32.h"

int oldX, oldY, oldXM, oldYM, oldXMM, oldYMM;

WORLDid gID = FAILED_ID;
VIEWPORTid vID;
SCENEid sID;
OBJECTid nID, cID, lID;

void QuitGame(WORLDid, BYTE, BOOL);
void GameAI(int);

void InitPivot(WORLDid, int, int);
void PivotCam(WORLDid, int, int);
void InitMove(WORLDid, int, int);
void MoveCam(WORLDid, int, int);
void InitZoom(WORLDid, int, int);
void ZoomCam(WORLDid, int, int);

/*------------------
  the main program
  C.Wang 0308, 2004
 -------------------*/
void main(int argc, char **argv)
{
   // create a new world
   gID = FyWin32CreateWorld("Hello TheFly !", 0, 0, 800, 600, 32, FALSE);

   FnWorld gw;
   gw.Object(gID);
   gw.SetObjectPath("Data\\Models");
   gw.SetTexturePath("Data\\textures");
   gw.SetShaderPath("Data\\shaders");

   // create a viewport
   vID = gw.CreateViewport(0, 0, 800, 600);
   FnViewport vp;
   vp.Object(vID);
   vp.SetBackgroundColor(0.3f, 0.4f, 0.5f);

   // create a 3D scene & the 3D entities
   sID = gw.CreateScene(1);

   FnScene scene;
   scene.Object(sID);
   nID = scene.CreateObject(ROOT);
   cID = scene.CreateCamera(ROOT);
   lID = scene.CreateLight(ROOT);

   // load the teapot model
   FnObject model;
   model.Object(nID);
   model.Load("teapot");

   // translate the camera
   FnCamera camera;
   camera.Object(cID);
   camera.Rotate(X_AXIS, 90.0f, LOCAL);
   camera.Translate(0.0f, 10.0f, 100.0f, LOCAL);

   // translate the light
   FnLight light;
   light.Object(lID);
   light.SetName("MainLight");
   light.Translate(-50.0f, -50.0f, 50.0f, GLOBAL);

   // set Hotkeys
   FyDefineHotKey(FY_ESCAPE, QuitGame, FALSE);

   // define some mouse functions
   FyBindMouseFunction(LEFT_MOUSE, InitPivot, PivotCam, NULL, NULL);
   FyBindMouseFunction(MIDDLE_MOUSE, InitZoom, ZoomCam, NULL, NULL);
   FyBindMouseFunction(RIGHT_MOUSE, InitMove, MoveCam, NULL, NULL);

   /* bind a timer, frame rate = 30 fps */
   FyBindTimer(0, 30.0f, GameAI, TRUE);

   // invoke the system
   FyInvokeTheFly(TRUE);
}


//------------------------------------------------------------------------------------
// timer callback function which will be invoked by TheFly3D System every 1/30 second
// C.Wang 0308, 2004
//------------------------------------------------------------------------------------
void GameAI(int skip)
{
   // render the scene
   FnViewport vp;
   vp.Object(vID);
   vp.Render(cID, TRUE, TRUE);

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
      oldX = x;
   }

   if (y != oldY) {
      model.Object(cID);
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