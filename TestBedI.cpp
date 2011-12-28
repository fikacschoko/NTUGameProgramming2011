/*==========================================================
  TestBed I Build 2

  (C)2009 All Rights Reserved, Chuan-Chang Wang
  Created : 0428, 2009

  Last Updated by Chuan-Chang Wang : 0717, 2010
 ===========================================================*/
#include "TheFlyWin32.h"
#include "FyUI.h"
#include "FyCommand.h"
#include "FyVFX.h"
#include "FyJob.h"

#include <commdlg.h>
#include <commctrl.h>

#define MAX_UI_SYSTEMS                64
#define MAX_COMMAND_SLOTS             256
#define MAX_STRING_CHARACTERS         512
#define MAX_GLOBALS                   2048
#define MAX_LOCALS                    512
#define MAX_SCRIPTS                   16
#define MAX_LOOP_STACKS               16
#define MAX_SCENE_BUTTONS             10


#define STRING_VARIABLE             0
#define FLOAT_VARIABLE              1
#define INTEGER_VARIABLE            2
#define BOOLEAN_VARIABLE            3


#define LOOP_TIMER                  0
#define RENDER_TIMER                1
#define BLINK_TIMER                 2

#define BUTTON_WIDTH                87
#define LONG_BUTTON_WIDTH           120
#define SHORT_BUTTON_WIDTH          30


#define TOOL_MODE                   1
#define PLAY_MODE                   2

// global environments
DWORD tbControlMode = TOOL_MODE;
WORLDid tbWorldID = FAILED_ID;
FnWorld tbWorld;

int tbWorldWidth = 1200;
int tbWorldHeight = 840;
int tbMinWorldWidth = 900;
int tbMinWorldHeight = 680;

int tbMinWorkSpaceWidth = 640;
int tbMinWorkSpaceHeight = 480;

int tbRenderGroup = 9;
BOOL tbBeUIPressed = FALSE;
BOOL tbShowWorkSpaceCaption = TRUE;
BOOL tbBePlay = FALSE;
DWORD tbPlayMode = LOOP;
BOOL tbBeShuFa = FALSE;
float tbPlaySpeed = 1.0f;
BOOL tbBeBoundingBoxOn = TRUE;
BOOL tbShowAux = TRUE;
int tbTestPoseStart = 0;
int tbTestPoseEnd = 0;
float tbTestPoseFrame = 0.0f;
BOOL tbBePlayPose = FALSE;
BOOL tbBeBase = TRUE;
BOOL tbBeRestoreCurrentDir = TRUE;
int tbRenderCamera = PERSPECTIVE;
float tbOrthScaleFactor = 1.0f;
float tbOrthScaleRate = 1.01f;

// TheFly
SCENEid tbMainSceneID = FAILED_ID;
SCENEid tbAuxSceneID = FAILED_ID;
SCENEid tbOrthoSceneID = FAILED_ID;
SCENEid tbDumperSceneID = FAILED_ID;
OBJECTid tbMainCamID = FAILED_ID;
OBJECTid tbMainCamRefID = FAILED_ID;
OBJECTid tbMainLightID = FAILED_ID;
OBJECTid tbAuxLightID = FAILED_ID;
OBJECTid tbAuxCamID = FAILED_ID;
OBJECTid tbOrthoCamID = FAILED_ID;
OBJECTid tbAxisID = FAILED_ID;
OBJECTid tbTestPlaneID = FAILED_ID;
OBJECTid tbOrthoPlaneID = FAILED_ID;

FnScene tbScene, tbAuxScene, tbDumperScene, tbOrthoScene;
FnCamera tbCamera, tbAuxCamera, tbOrthoCamera;
FnLight tbMainLight;
FnLight tbAuxLight;
FnObject tbCameraRef;

SHUFAid tbTinyEFontID = FAILED_ID;
FnShuFa tbTinyEFont;

int tbOldX, tbOldY;  // store mouse coordinates
float tbMoveSpeed = 0.2f, tbZoomSpeed = 1.0f, tbPivotSpeed = 1.0f;

float tbCameraPosition[3]    = { 0.0f,      -431.0f,     300.0f };
float tbCameraFaceDir[3]     = { 0.0f,       0.846513f, -0.532368f };
float tbCameraUpDir[3]       = { 0.0f,       0.532368f,  0.846513f };
float tbCameraRefPosition[3] = { 0.0f,  0.0f,  0.0f };
float tbCameraRefFaceDir[3]  = { 0.6205f,   -0.784216f,  0.0f };
float tbCameraRefUpDir[3]    = { 0.0f,       0.0f,       1.0f };
float tbTestPlaneSize = 100.0f;

char tbWorkDir[256];


extern int fyOXX;
extern int fyOXY;

struct TBMaterial
{
   float ambient[3];
   float diffuse[3];
   float specular[3];
   float emissive[3];
   float power;
   char shaderPath[256];
   char shaderFile[256];
   char effectName[256];

   char texturePath[256];
   char texture0[256];
   char texture1[256];
   char texture2[256];
   char texture3[256];
};


// object data structure used in testbed
class TBObject : public FnObject
{
private:
   ACTORid usedBy;
   int type;
   int boneID;

public:
   TBObject() { usedBy = FAILED_ID; boneID = NONE; type = NONE; };
  ~TBObject() {};

   void UsedBy(ACTORid aID) { usedBy = aID; };
   ACTORid UsedBy() { return usedBy; };
   void Data(int t, int b) { type = t; boneID = b; };
   int Type() { return type; }
   int BoneID() { return boneID; };
};


// actor data structure used in testbed
class TBActor : public FnActor
{
private:

public:
   TBActor() {};
  ~TBActor() {};
};


// scene data structure used in testbed
class TBScene : public FnScene
{
private:
   int id;
   OBJECTid camRefID;
   OBJECTid camID;
   OBJECTid mainLightID;
   OBJECTid auxLightID;

   UIid sceneButtonID;

   TBObject curObj;
   MATERIALid curMatID;

   int iCurObj;
   int numObj;
   int maxObj;
   TBObject **objList;

   TBActor curActor;
   int numActor;
   int maxActor;
   TBActor **actorList;
   
public:
   TBScene();
  ~TBScene();

   inline int ID() { return id; };
   inline void ID(int i) { id = i; };
   inline int GetActorNumber() { return numActor; };
   void AssignCamera(OBJECTid rID, OBJECTid cID);
   void GetCameras(OBJECTid *rID, OBJECTid *cID);
   void AssignLights(OBJECTid mainID, OBJECTid auxID);
   void GetLights(OBJECTid *mainID, OBJECTid *auxID);
   void InsertObject(OBJECTid oID, ACTORid aID, int type, int id);
   ACTORid RemoveObject(OBJECTid oID, BOOL beD);
   TBObject *GetTBObject(OBJECTid oID);
   void SetCurrentObject(OBJECTid oID);
   OBJECTid GetCurrentObject();
   OBJECTid FindObject(char *name);
   MATERIALid GetCurrentMaterial() { return curMatID; };
   void SetCurrentMaterial(MATERIALid mID) { curMatID = mID; };
   BOOL NextCurrentObject();
   BOOL PreviousCurrentObject();
   void InsertActor(ACTORid aID);
   void RemoveActor(ACTORid oID, BOOL beD);
   void SetCurrentActor(ACTORid aID);
   ACTORid GetCurrentActor();
   ACTORid GetActorByName(char *name);
   int GetEffectActorNumber();
   char *GetActorName(int id);
   void New();
   void ResetPlay(BOOL beBase);
   void Play(float skip, BOOL beBase);
};


// scene table
TBScene *tbCurScene;
int tbNumScene = 0;
int tbMaxScene = 0;
TBScene **tbSceneList = NULL;


// UIs
int tbNumUISys;
UISYSTEMid tbAllUISysID[MAX_UI_SYSTEMS];


// menu bar UIs
UISYSTEMid tbMenuBarID = FAILED_ID;
FnUISystem tbMenuBarUI;
int tbMenuBarOX = 0, tbMenuBarOY = 0, tbMenuBarWidth = 1200, tbMenuBarHeight = 30;

UIid tbMenuBarPlateID = FAILED_ID;
UIid tbSystemButtonID = FAILED_ID;
UIid tbScriptSystemButtonID = FAILED_ID;
UIid tbModelSystemButtonID = FAILED_ID;
UIid tbActorSystemButtonID = FAILED_ID;
UIid tbEnvironmentSystemButtonID = FAILED_ID;
UIid tbFaceSystemButtonID = FAILED_ID;
UIid tbAuxButtonID = FAILED_ID;
UIid tbOrthoButtonID = FAILED_ID;
UIid tbExitButtonID = FAILED_ID;

// main menu UIs
UISYSTEMid tbMenuID = FAILED_ID;
FnUISystem tbMenuUI;
int tbMenuOX = 0, tbMenuOY = 640, tbMenuWidth = 1200, tbMenuHeight = 160;

// side menu UIs
UISYSTEMid tbSideMenuID = FAILED_ID;
FnUISystem tbSideMenuUI;
int tbSideMenuOX = 0, tbSideMenuOY = 40, tbSideMenuWidth = 350, tbSideMenuHeight = 600;

UIid tbSideMenuPlateID = FAILED_ID;
UIid tbSideMenuSystemPlateID = FAILED_ID;
UIid tbSideMenuScriptPlateID = FAILED_ID;
UIid tbSideMenuModelPlateID = FAILED_ID;
UIid tbSideMenuActorPlateID = FAILED_ID;
UIid tbSideMenuEnvironmentPlateID = FAILED_ID;
UIid tbSideMenuFacePlateID = FAILED_ID;

UIid tbNewButtonID = FAILED_ID;
UIid tbScriptButtonID = FAILED_ID;
UIid tbModelButtonID = FAILED_ID;
UIid tbActorButtonID = FAILED_ID;
UIid tbEnvironmentButtonID = FAILED_ID;
UIid tbFaceButtonID = FAILED_ID;

// script loader UIs
UIid tbLoadScriptButtonID = FAILED_ID;

// model viewer UIs
UIid tbModelFileTabID = FAILED_ID;
UIid tbLoadModelButtonID = FAILED_ID;
UIid tbSaveModelButtonID = FAILED_ID;
UIid tbModelCurrentTabID = FAILED_ID;
UIid tbTexture0TabID = FAILED_ID;
UIid tbTexture1TabID = FAILED_ID;
UIid tbTexture2TabID = FAILED_ID;
UIid tbTexture3TabID = FAILED_ID;
UIid tbWireframeCurModelButtonID = FAILED_ID;
UIid tbPreviousModelButtonID = FAILED_ID;
UIid tbNextModelButtonID = FAILED_ID;
UIid tbSelectMaterialButtonID = FAILED_ID;

// actor viewer UIs
UIid tbActorFileTabID = FAILED_ID;
UIid tbActorCurrentTabID = FAILED_ID;
UIid tbPoseExploreTabID = FAILED_ID;
UIid tbLoadActorButtonID = FAILED_ID;
UIid tbSaveActorButtonID = FAILED_ID;
UIid tbDeleteActorButtonID = FAILED_ID;
UIid tbSelectActorComboID = FAILED_ID;
UIid tbSelectBodyComboID = FAILED_ID;
UIid tbSelectActionComboID = FAILED_ID;
UIid tbSelectPoseComboID = FAILED_ID;
UIid tbNewPoseButtonID = FAILED_ID;
UIid tbPoseStartID = FAILED_ID;
UIid tbPoseEndID = FAILED_ID;
UIid tbPlayPoseButtonID = FAILED_ID;
UIid tbMakeBaseMoveButtonID = FAILED_ID;
UIid tbEditActionButtonID = FAILED_ID;
UIid tbAddActionButtonID = FAILED_ID;
UIid tbDeleteActionButtonID = FAILED_ID;

// setup environment UIs
UIid tbWorkDirTypeinUI = FAILED_ID;
UIid tbShaderDirTypeinUI = FAILED_ID;
UIid tbModelDirTypeinUI = FAILED_ID;
UIid tbActorDirTypeinUI = FAILED_ID;
UIid tbTextureDirTypeinUI = FAILED_ID;

// play panel UIs
UIid tbControlPanelPlateUI = FAILED_ID;
UIid tbPlayID = FAILED_ID;
UIid tbStopID = FAILED_ID;
UIid tbPlayModeID = FAILED_ID;
UIid tbPlaySpeedID = FAILED_ID;
UIid tbPlayBarID = FAILED_ID;

int tbNumSceneButton = 0;
UIid tbSceneButton[MAX_SCENE_BUTTONS];

// object panel UIs
UIid tbEnvironmentPlanelPlateUI = FAILED_ID;

// warning dialogue UIs
UISYSTEMid tbWarnDialogueID = FAILED_ID;
FnUISystem tbWarnUI;
int tbWarnOX = 100, tbWarnOY = 100, tbWarnWidth = 300, tbWarnHeight = 300;
UIid tbWarnMessageID = FAILED_ID;
UIid tbWarnOKID = FAILED_ID;
UIid tbWarnCancelID = FAILED_ID;

// add action dialogue UIs
UISYSTEMid tbAddActionDialogueID = FAILED_ID;
FnUISystem tbAddActionUI;
int tbAddActionW = 250, tbAddActionH = 120, tbAddActionOX = 0, tbAddActionOY = 0;
UIid tbAddActionCancelID = FAILED_ID;
UIid tbAddActionOKID = FAILED_ID;
UIid tbAddActionBackgroundID = FAILED_ID;
UIid tbAddActionNameID = FAILED_ID;
UIid tbAddActionTypeID = FAILED_ID;
UIid tbAddActionFrontPoseID = FAILED_ID;
UIid tbAddActionEndPoseID = FAILED_ID;
UIid tbAddActionDurationID = FAILED_ID;
UIid tbAddActionWeight0ID = FAILED_ID;
UIid tbAddActionWeight1ID = FAILED_ID;

// current material editing UIs
UIid tbShaderFileID = FAILED_ID;
UIid tbEffectID = FAILED_ID;
UIid tbShaderLoaderID = FAILED_ID;
UIid tbShaderRemoveID = FAILED_ID;
UIid tbAmbientRID = FAILED_ID;
UIid tbAmbientGID = FAILED_ID;
UIid tbAmbientBID = FAILED_ID;
UIid tbDiffuseRID = FAILED_ID;
UIid tbDiffuseGID = FAILED_ID;
UIid tbDiffuseBID = FAILED_ID;
UIid tbSpecularRID = FAILED_ID;
UIid tbSpecularGID = FAILED_ID;
UIid tbSpecularBID = FAILED_ID;
UIid tbEmissiveRID = FAILED_ID;
UIid tbEmissiveGID = FAILED_ID;
UIid tbEmissiveBID = FAILED_ID;
UIid tbShinenessID = FAILED_ID;
UIid tbTexture0ID = FAILED_ID;
UIid tbTexture0AddID = FAILED_ID;
UIid tbTexture0RemoveID = FAILED_ID;
UIid tbTexture1ID = FAILED_ID;
UIid tbTexture1AddID = FAILED_ID;
UIid tbTexture1RemoveID = FAILED_ID;
UIid tbTexture2ID = FAILED_ID;
UIid tbTexture2AddID = FAILED_ID;
UIid tbTexture2RemoveID = FAILED_ID;
UIid tbTexture3ID = FAILED_ID;
UIid tbTexture3AddID = FAILED_ID;
UIid tbTexture3RemoveID = FAILED_ID;
UIid tbUpdateMaterialID = FAILED_ID;
UIid tbCancelMaterialID = FAILED_ID;
UIid tbDeleteModelButtonID = FAILED_ID;
int tbNumMatUI = 32;
UIid tbMatUI[32];
TBMaterial tbMatBuffer;

char tbTmpShaderPath[256];
char tbTmpTexturePath[256];

// workspace viewports
VIEWPORTid tbWorkSpaceBTID = FAILED_ID;
FnViewport tbWSBottomVP;
int tbBottomVPOX = 260, tbBottomVPOY = 40, tbBottomVPWidth = 955, tbBottomVPHeight = 600;

VIEWPORTid tbWorkSpaceID = FAILED_ID;
FnViewport tbWorkSpaceVP;
int tbWorkSpaceVPOX = 418, tbWorkSpaceVPOY = 100, tbWorkSpaceVPWidth = 800, tbWorkSpaceVPHeight = 600;


// etc
int tbResizeWidth = 1024, tbResizeHeight = 768;
BOOL tbBeResize = FALSE;
float tbCurResizeTime = 0.0f;
HCURSOR tbArrowCursor;
HCURSOR tbWaitCursor;
BOOL tbRunScript = FALSE;

void TbCancelMaterialEditing();
void TbPasteCurrentMaterialData(FnMaterial *mat);
void TbExtractMaterialUIData();
void TbUpdateCurrentMaterial(FnMaterial *mat);
void TbOpenQuit(UIid uiID, BOOL bePress);
void TbOpenNew(UIid uiID, BOOL bePress);
void TbOpenAddAction(UIid uiID, BOOL bePress);
void TbSwitchMenuBarRadios(UIid uiID, BOOL bePressed);
void TbUpdateCurrentBodyDataUI(FnActor *actor);
void TbDrawBlueBox(FnObject *object);


#ifdef MULTICORE
// criticision section for multithreading
struct TBCriticalSection
{
   CRITICAL_SECTION cs;        // critical section
   int id;
};

TBCriticalSection tbRenderCS;
TBCriticalSection tbScriptCS;
#endif


// all command functions
int tbNumCommandSlots = 200;
int tbNumCommandHistory = 200;
int tbCommandCursor = 0;
char *tbCurCommandPtr = NULL;
int tbCurCommand = 0;
char **tbCommandHistory = NULL;
BOOL tbShowCommand = TRUE;
BOOL tbBeBlink = FALSE;
BOOL tbBeDisplayCommandHistory = FALSE;
float tbLogOpacity = 512.0f;
char tbLogMessage[MAX_STRING_CHARACTERS];

BOOL TbCommandParser(char *command, char *argument, BOOL bePasteCommand);
void TbCommandInput(WORD a);
void TbCommandFunKey(WORLDid gID, BYTE code, BOOL value);
void TbSetLogMessage(char *log);
BOOL TbCheckCommandAttributeB(char *, char *, BOOL *, int);
BOOL TbCheckCommandAttributeI(char *, char *, int *, int);
BOOL TbCheckCommandAttributeF(char *, char *, float *, int);
BOOL TbCheckCommandAttributeS(char *, char *, char *, int);
BOOL TbSelectPoseCmd(char *arg);
BOOL TbPlayTestPoseCmd(char *arg);
BOOL TbSwitchToolModeCmd(char *arg);
BOOL TbSetPathCmd(char *arg);
void TbStopPlay(UIid uiID, BOOL bePress);


// script command
struct TBScript
{
   BOOL beRun;
   int length;
   int maxLength;
   char *systemString;
};


// script variables
struct TBVariable
{
   BOOL beUsed;
   DWORD type;
   char name[32];
   char raw[256];
   union
   {
      float f;
      int i;
      BOOL b;
   } value;
};


struct TBLoopStack
{
   char index[64];
   int startV;
   int endV;
   int incV;
   int curV;
   int loopEntry;
};


int tbCurScript = NONE;
TBScript tbScript[MAX_SCRIPTS];

int tbNumGlobal = 0;
int tbNumLocal = 0;
TBVariable tbGlobals[MAX_GLOBALS];
TBVariable tbLocals[MAX_LOCALS];

int tbCurLoopStackID = NONE;
TBLoopStack tbLoopStack[MAX_LOOP_STACKS];

BOOL tbBeUseGlobals = TRUE;

void TbCopySystemString(char *string, int maxLen);
int TbGetChar();
int TbGetNewLine(char *str);
void TbInitScriptSystem();
void TbDeinitScriptSystem();
void TbExecuteScript(int skip);
int TbRegisterVariable(char *name, char *raw, DWORD type, int arrayLen, BOOL beGlobal);
void TbUnregisterVariable(char *name, BOOL beGlobal);
char *TbExaminevariable(char *name, BOOL beGlobal);
char *TbSetVariableValue(char *cmd, char *argument);
BOOL TbLoadScript(char *arg);
BOOL TbSetIntegerCmd(char *arg, char *log, void *data);
BOOL TbSetBooleanCmd(char *arg, char *log, void *data);
BOOL TbSetFloatCmd(char *arg, char *log, void *data);


/*--------------------
  TbScene constructor
  C.Wang 0626, 2009
 ---------------------*/
TBScene::TBScene()
{
   id = NONE;
   iCurObj = NONE;
   numObj = 0;
   maxObj = 0;
   objList = NULL;
   numActor = 0;
   maxActor = 0;
   actorList = NULL;
   curMatID = FAILED_MATERIAL_ID;

   sceneButtonID = FAILED_ID;
   camRefID = camID = mainLightID = auxLightID = FAILED_ID;
}


/*-------------------
  TbScene destructor
  C.Wang 0626, 2009
 --------------------*/
TBScene::~TBScene()
{
   if (objList != NULL) {
      FyFree(objList);
      objList = NULL;
      numObj = 0;
      maxObj = 0;
   }

   if (actorList != NULL) {
      FyFree(actorList);
      actorList = NULL;
      numActor = 0;
      maxActor = 0;
   }

   tbWorld.DeleteScene(Object());
}


/*-------------------------------------
  clear the scene and reset the camera
  C.Wang 0630, 2010
 --------------------------------------*/
void TBScene::New()
{
   int i;
   if (objList != NULL) {
      for (i = 0; i < numObj; i++) {
         if (objList[i] != NULL) {
            if (objList[i]->UsedBy() == FAILED_ID) {
               this->DeleteObject(objList[i]->Object());
            }
            FyFree(objList[i]);
            objList[i] = NULL;
         }
      }
      FyFree(objList);
      objList = NULL;
      numObj = 0;
      maxObj = 0;
   }

   if (actorList != NULL) {
      for (i = 0; i < numActor; i++) {
         if (actorList[i] != NULL) {
            this->DeleteActor(actorList[i]->Object());
            FyFree(actorList[i]);
            actorList[i] = NULL;
         }
      }
      FyFree(actorList);
      actorList = NULL;
      numActor = 0;
      maxActor = 0;
   }

   iCurObj = NONE;
   curMatID = FAILED_MATERIAL_ID;
   curObj.Object(FAILED_ID);
   curActor.Object(FAILED_ID);

   // reset the cameras & lights
   FnObject ref;
   FnCamera cam;
   FnLight light;
   ref.Object(camRefID);
   cam.Object(camID);
   cam.Translate(0.0f, 0.0f, 0.0f, REPLACE);
   cam.Translate(0.0f, 0.0f, 200.0f, LOCAL);

   cam.SetDirection(tbCameraFaceDir, tbCameraUpDir);
   cam.SetPosition(tbCameraPosition);
   ref.SetDirection(tbCameraRefFaceDir, tbCameraRefUpDir);
   ref.SetPosition(tbCameraRefPosition);

   // create the main light
   light.Object(mainLightID);
   light.Translate(0.0f, 0.0f, 0.0f, REPLACE);
   light.Rotate(X_AXIS, -30.0f, LOCAL);
   light.Translate(60.0, -90.0f, 90.0f, LOCAL);

   // create the auxilary light & object
   light.Object(auxLightID);
   light.Rotate(X_AXIS, -30.0f, LOCAL);
}


/*------------------------------------------------------
  TbScene member function : reset the animation playing
  C.Wang 0629, 2010
 -------------------------------------------------------*/
void TBScene::ResetPlay(BOOL beBase)
{
   int i;
   for (i = 0; i < numActor; i++) {
      if (actorList[i] != NULL) {
         actorList[i]->Play(0, START, 0.0f, beBase, TRUE);

         // reset the position
         float pos[3];
         pos[0] = pos[1] = pos[2] = 0.0f;
         actorList[i]->SetPosition(pos);
      }
   }
}


/*---------------------------------------------
  TbScene member function : play all animation
  C.Wang 0629, 2010
 ----------------------------------------------*/
void TBScene::Play(float skipFrame, BOOL beBase)
{
   int i, f, r, u;
   for (i = 0; i < numActor; i++) {
      if (actorList[i] != NULL) {
		  if(FyCheckHotKeyStatus(FY_DOWN))
		  {
			  	if (!actorList[i]->Play(0, ONCE, 0, beBase, TRUE)) {
				actorList[i]->Play(0, START, 0.0f, beBase, TRUE);

				// reset the position
				float pos[3];
				pos[0] = pos[1] = pos[2] = 0.0f;
				actorList[i]->SetPosition(pos);
			 }
			 actorList[i]->QueryLastMovementResult(&f, &r, &u);
		  }
		  else
		  {
			 if (!actorList[i]->Play(0, ONCE, skipFrame, beBase, TRUE)) {
				actorList[i]->Play(0, START, 0.0f, beBase, TRUE);

				// reset the position
				float pos[3];
				pos[0] = pos[1] = pos[2] = 0.0f;
				actorList[i]->SetPosition(pos);
			 }
			 actorList[i]->QueryLastMovementResult(&f, &r, &u);
		  }
      }
   }
}


/*-----------------------------------------------------------
  TbScene member function : get current object for the scene
  C.Wang 0527, 2010
 ------------------------------------------------------------*/
OBJECTid TBScene::GetCurrentObject()
{
   return curObj.Object();
}


/*------------------------------------------------------------
  TbScene member function : find an object as the current one
  C.Wang 0707, 2010
 -------------------------------------------------------------*/
OBJECTid TBScene::FindObject(char *name)
{
   int i;

   for (i = 0; i < numObj; i++) {
      if (objList[i] != NULL) {
         if (FyCheckWord(objList[i]->GetName(), name)) {
            return objList[i]->Object();
         }
      }
   }
   return FAILED_ID;
}


/*----------------------------------------------------------
  TbScene member function : get current actor for the scene
  C.Wang 0624, 2010
 -----------------------------------------------------------*/
ACTORid TBScene::GetCurrentActor()
{
   return curActor.Object();
}


/*---------------------------------------------
  TbScene member function : assign the cameras 
  C.Wang 0626, 2009
 ----------------------------------------------*/
void TBScene::AssignCamera(OBJECTid rID, OBJECTid cID)
{
   camRefID = rID;
   camID = cID;
}


/*------------------------------------------
  TbScene member function : get the cameras 
  C.Wang 0626, 2009
 -------------------------------------------*/
void TBScene::GetCameras(OBJECTid *rID, OBJECTid *cID)
{
   if (rID != NULL) {
      *rID = camRefID;
   }
   if (cID != NULL) {
      *cID = camID;
   }
}


/*--------------------------------------------
  TbScene member function : assign the lights 
  C.Wang 0626, 2009
 ---------------------------------------------*/
void TBScene::AssignLights(OBJECTid mainID, OBJECTid auxID)
{
   mainLightID = mainID;
   auxLightID = auxID;
}


/*-----------------------------------------
  TbScene member function : get the lights 
  C.Wang 0626, 2009
 ------------------------------------------*/
void TBScene::GetLights(OBJECTid *mainID, OBJECTid *auxID)
{
   if (mainID != NULL) {
      *mainID = mainLightID;
   }
   if (auxID != NULL) {
      *auxID = auxLightID;
   }
}


/*-------------------------------------------
  TbScene member function : insert an object
  C.Wang 0630, 2010
 --------------------------------------------*/
void TBScene::InsertObject(OBJECTid oID, ACTORid aID, int type, int id)
{
   int i;
   if (objList == NULL) {
      maxObj = 1024;
      objList = (TBObject **)FyMalloc(sizeof(TBObject *)*maxObj);
      for (i = 0; i < maxObj; i++) {
         objList[i] = NULL;
      }
      numObj = 0;
   }
   else {
      if (numObj + 1 >= maxObj) {
         maxObj += 512;
         objList = (TBObject **)FyRealloc(objList, sizeof(TBObject *)*maxObj);
         for (i = numObj; i < maxObj; i++) {
            objList[i] = NULL;
         }
      }
   }

   // scan the object list
   for (i = 0; i < numObj; i++) {
      if (objList[i] == NULL) {
         objList[i] = (TBObject *)FyMalloc(sizeof(TBObject));
         objList[i]->Object(oID);
         objList[i]->UsedBy(aID);
         if (aID != FAILED_ID) {
            objList[i]->Data(type, id);
         }
         else {
            objList[i]->Data(NONE, NONE);
         }
         return;
      }
   }
   objList[numObj] = (TBObject *)FyMalloc(sizeof(TBObject));
   objList[numObj]->Object(oID);
   objList[numObj]->UsedBy(aID);
   if (aID != FAILED_ID) {
      objList[numObj]->Data(type, id);
   }
   else {
      objList[numObj]->Data(NONE, NONE);
   }
   numObj++;
}


/*-------------------------------------------
  TbScene member function : remove an object
  C.Wang 0622, 2010
 --------------------------------------------*/
ACTORid TBScene::RemoveObject(OBJECTid oID, BOOL beD)
{
   // scan the object list
   int i;
   ACTORid aID = FAILED_ID;
   for (i = 0; i < numObj; i++) {
      if (objList[i] != NULL) {
         if (objList[i]->Object() == oID) {
            aID = objList[i]->UsedBy();
            if (beD) {
               DeleteObject(oID);
            }
            FyFree(objList[i]);
            objList[i] = NULL;
            if (i == numObj - 1) numObj--;
            return aID;
         }
      }
   }
   return aID;
}


/*---------------------------------------------------------
  TbScene member function : get number of effective actors
  C.Wang 0626, 2010
 ----------------------------------------------------------*/
int TBScene::GetEffectActorNumber()
{
   int i, iCount;
   iCount = 0;
   for (i = 0; i < numActor; i++) {
      if (actorList[i] != NULL) {
         if (actorList[i]->Object() != FAILED_ID) {
            iCount++;
         }
      }
   }
   return iCount;
}


/*-------------------------------------------------------
  TbScene member function : get the actir ID by his name
  C.Wang 0626, 2010
 --------------------------------------------------------*/
ACTORid TBScene::GetActorByName(char *name)
{
   int i;
   for (i = 0; i < numActor; i++) {
      if (actorList[i] != NULL) {
         if (actorList[i]->Object() != FAILED_ID) {
            if (FyCheckWord(actorList[i]->GetName(), name)) {
               return actorList[i]->Object();
            }
         }
      }
   }

   return FAILED_ID;
}


/*-------------------------------------------
  TbScene member function : get actor's name
  C.Wang 0626, 2010
 --------------------------------------------*/
char *TBScene::GetActorName(int id)
{
   if (id < 0 || id >= numActor) return NULL;
   if (actorList[id] == NULL) return NULL;
   if (actorList[id]->Object() == FAILED_ID) return NULL;

   return actorList[id]->GetName();
}


/*------------------------------------------
  TbScene member function : insert an actor
  C.Wang 0626, 2009
 -------------------------------------------*/
void TBScene::InsertActor(ACTORid aID)
{
   int i;
   if (actorList == NULL) {
      maxActor = 256;
      actorList = (TBActor **)FyMalloc(sizeof(TBActor *)*maxActor);
      for (i = 0; i < maxActor; i++) {
         actorList[i] = NULL;
      }
      numActor = 0;
   }
   else {
      if (numActor + 1 >= maxActor) {
         maxActor += 128;
         actorList = (TBActor **)FyRealloc(actorList, sizeof(TBActor *)*maxActor);
         for (i = numActor; i < maxActor; i++) {
            actorList[i] = NULL;
         }
      }
   }

   // scan the actor list
   for (i = 0; i < numActor; i++) {
      if (actorList[i] == NULL) {
         actorList[i] = (TBActor *)FyMalloc(sizeof(TBActor));
         actorList[i]->Object(aID);
         return;
      }
   }
   actorList[numActor] = (TBActor *)FyMalloc(sizeof(TBActor));
   actorList[numActor]->Object(aID);
   numActor++;
}


/*------------------------------------------
  TbScene member function : remove an actor
  C.Wang 0626, 2010
 -------------------------------------------*/
void TBScene::RemoveActor(ACTORid aID, BOOL beD)
{
   // scan the actor list
   int i;
   for (i = 0; i < numActor; i++) {
      if (actorList[i] != NULL) {
         if (actorList[i]->Object() == aID) {
            if (beD) {
               DeleteActor(aID);
            }
            actorList[i] = NULL;
            if (i == numActor - 1) numActor--;
            return;
         }
      }
   }
}


/*-----------------------------------------
  TbScene member function : get the object
  C.Wang 0630, 2010
 ------------------------------------------*/
TBObject *TBScene::GetTBObject(OBJECTid oID)
{
   int i;
   for (i = 0; i < numObj; i++) {
      if (objList[i] != NULL) {
         if (objList[i]->Object() == oID) {
            return objList[i];
         }
      }
   }
   return NULL;
}


/*--------------------------------------------------
  TbScene member function : make the current object
  C.Wang 0624, 2010
 ---------------------------------------------------*/
void TBScene::SetCurrentObject(OBJECTid oID)
{
   // make no current object
   curObj.ShowBoundingBox(FALSE);

   // set new current object
   curObj.Object(oID);
   curObj.ShowBoundingBox(TRUE);

   // find the ID of current object
   int i;
   iCurObj = NONE;
   for (i = 0; i < numObj; i++) {
      if (objList[i] != NULL) {
         if (objList[i]->Object() == oID) {
            iCurObj = i;
         }
      }
   }

   // update UIs
   FnUiButton uiB;
   FnUiRadioButton uiRB;
   FnUiCombo uiC;
   FnUiTypein uiTy;
   if (curObj.Object() != FAILED_ID) {
      // enable the save button
      uiB.Object(tbSaveModelButtonID);
      uiB.Enable(TRUE);

      // enable the delete button
      uiB.Object(tbDeleteModelButtonID);
      uiB.Enable(TRUE);

      // enable the material combo
      uiC.Object(tbSelectMaterialButtonID);
      uiC.Enable(TRUE);

      // paste materials to combo
      int numMat = curObj.GetMaterials();
      MATERIALid mID[1024];
      curObj.GetMaterials(mID);

      FnMaterial mat;
      mat.Object(mID[0]);
      uiC.PasteComboValue(mat.GetName());
      curMatID = mID[0];

      if (curMatID == FAILED_MATERIAL_ID) {
         uiC.Enable(FALSE);
      }
      else {
         uiC.Enable(TRUE);
         uiC.ClearMessages();
         for (i = 0; i < numMat; i++) {
            mat.Object(mID[i]);
            uiC.InsertMessage(i, mat.GetName());
         }
      }

      uiRB.Object(tbWireframeCurModelButtonID);
      uiRB.Enable(TRUE);
      if (curObj.GetRenderMode() == WIREFRAME) {
         uiRB.ChangeState(TRUE);
      }
      else {
         uiRB.ChangeState(FALSE);
      }

      // paste all material data to current material UIs
      FnUiPlate ui;
      for (i = 0; i < tbNumMatUI; i++) {
         ui.Object(tbMatUI[i]);
         ui.Enable(TRUE);
      }
      ui.Object(tbUpdateMaterialID);
      ui.Enable(FALSE);

      mat.Object(mID[0]);
      TbPasteCurrentMaterialData(&mat);
   }
   else {
      // disable the save button
      uiB.Object(tbSaveModelButtonID);
      uiB.Enable(FALSE);

      // disable the delete button
      uiB.Object(tbDeleteModelButtonID);
      uiB.Enable(FALSE);

      // disable the material combo
      uiC.Object(tbSelectMaterialButtonID);
      uiC.Enable(FALSE);
      curMatID = FAILED_MATERIAL_ID;

      // clear the combo data
      uiC.PasteComboValue("");
      uiC.ClearMessages();

      // adjust UIs
      uiRB.Object(tbWireframeCurModelButtonID);
      uiRB.ChangeState(FALSE);
      uiRB.Enable(FALSE);

      FnUiPlate ui;
      for (i = 0; i < tbNumMatUI; i++) {
         ui.Object(tbMatUI[i]);
         ui.Enable(FALSE);

         if (ui.GetType() == TYPEIN_UI) {
            uiTy.Object(tbMatUI[i]);
            uiTy.PasteTypeinValue("");
         }
         else if (ui.GetType() == COMBO_UI) {
            uiC.Object(tbMatUI[i]);
            uiC.ClearMessages();
            uiC.PasteComboValue("");
         }
      }
   }
   strcpy(tbTmpShaderPath, tbWorld.GetShaderPath());
   strcpy(tbTmpTexturePath, tbWorld.GetTexturePath());

   // check how many objects
   int numModel = 0;
   for (i = 0; i < numObj; i++) {
      if (objList[i] != NULL) {
         numModel++;
      }
   }

   // update some UIs
   if (numModel > 0) {
      uiB.Object(tbPreviousModelButtonID);
      uiB.Enable(TRUE);
      uiB.Object(tbNextModelButtonID);
      uiB.Enable(TRUE);
   }
   else {
      uiB.Object(tbPreviousModelButtonID);
      uiB.Enable(FALSE);
      uiB.Object(tbNextModelButtonID);
      uiB.Enable(FALSE);
   }
}


/*-----------------------------------------------------------------
  TbScene member function : make next object as the current object
  C.Wang 0613, 2010
 ------------------------------------------------------------------*/
BOOL TBScene::NextCurrentObject()
{
   if (iCurObj < 0) iCurObj = 0;

   // find the ID of next current object
   int i, iOne;
   for (i = 1; i < numObj; i++) {
      iOne = i + iCurObj;
      if (iOne >= numObj) iOne -= numObj;
      if (iOne < 0) iOne += numObj;
      if (objList[iOne] != NULL) {
         iCurObj = iOne;
         break;
      }
   }
   if (iCurObj >= numObj) return FALSE;

   SetCurrentObject(objList[iCurObj]->Object());
   return TRUE;
}


/*---------------------------------------------------------------------
  TbScene member function : make previous object as the current object
  C.Wang 0613, 2010
 ----------------------------------------------------------------------*/
BOOL TBScene::PreviousCurrentObject()
{
   if (iCurObj < 0) iCurObj = 0;

   // find the ID of previous current object
   int i, iOne;
   for (i = 1; i < numObj; i++) {
      iOne = iCurObj - i;
      if (iOne >= numObj) iOne -= numObj;
      if (iOne < 0) iOne += numObj;
      if (objList[iOne] != NULL) {
         iCurObj = iOne;
         break;
      }
   }
   if (iCurObj >= numObj) return FALSE;

   SetCurrentObject(objList[iCurObj]->Object());
   return TRUE;
}


/*-------------------------------------------------
  TbScene member function : make the current actor
  C.Wang 0629, 2009
 --------------------------------------------------*/
void TBScene::SetCurrentActor(ACTORid aID)
{
   // backup current actor
   ACTORid oldA = curActor.Object();
   OBJECTid baseID = curActor.GetBaseObject();
   FnObject base;
   base.Object(baseID);
   base.Show(FALSE);

   // set current actor
   curActor.Object(aID);

   // update UIs
   FnUiButton uiB;
   FnUiCombo uiC;
   if (curActor.Object() != FAILED_ID) {
      // enable the save button
      uiB.Object(tbSaveActorButtonID);
      uiB.Enable(TRUE);

      // enable the delete button
      uiB.Object(tbDeleteActorButtonID);
      uiB.Enable(TRUE);

      // paste the actor to selection combo
      uiC.Object(tbSelectActorComboID);
      uiC.PasteComboValue(curActor.GetName());
   }
   else {
      // disable the save button
      uiB.Object(tbSaveActorButtonID);
      uiB.Enable(FALSE);

      // disable the delete button
      uiB.Object(tbDeleteActorButtonID);
      uiB.Enable(FALSE);

      // paste empty to selection combos
      uiC.Object(tbSelectActorComboID);
      uiC.PasteComboValue("");

      // check current model is part of former current actor
      if (curObj.UsedBy() == oldA) {
         SetCurrentObject(FAILED_ID);
      }
   }
   strcpy(tbTmpShaderPath, tbWorld.GetShaderPath());
   strcpy(tbTmpTexturePath, tbWorld.GetTexturePath());

   // show/hide the base node
   baseID = curActor.GetBaseObject();
   base.Object(baseID);
   base.Show(tbShowAux);
}


// quit command
class TbQuitCommand : public FcCommandBase
{
public:
   virtual void Init() { BeUndo(FALSE);
                         SetCommandName("Quit"); };                // initialize the command
   virtual void DeInit() {};                                       // deinitialize the command
   virtual BOOL Do(char *arg) { TbOpenQuit(tbExitButtonID, FALSE);
                                return TRUE; };                    // the "quit" command
   virtual void Undo() {};                                         // no undo
};


// new command
class TbNewCommand : public FcCommandBase
{
public:
   virtual void Init() { BeUndo(FALSE);
                         SetCommandName("New"); };                 // initialize the command
   virtual void DeInit() {};                                       // deinitialize the command
   virtual BOOL Do(char *arg) { TbOpenNew(tbNewButtonID, FALSE);
                                return TRUE; };                    // the "new" command
   virtual void Undo() {};                                         // no undo
};



// set wireframe command
class TbSetWireframeCommand : public FcCommandBase
{
protected:
   OBJECTid objID;
   int lastState;

public:
   virtual void Init() { lastState = TEXTURE; 
                         objID = FAILED_ID;
                         SetCommandName("WireFrame"); };    // initialize the command
   virtual void DeInit() { };                               // deinitialize the command
   virtual BOOL Do(char *arg);                              // the command
   virtual void Undo();                                     // undo
};


      /*---------------------------
        set wireframe command - Do
        C.Wang 0527, 2010
       ----------------------------*/
      BOOL TbSetWireframeCommand::Do(char *arg)
      {
         objID = tbCurScene->GetCurrentObject();

         FnObject model;
         model.Object(objID);
         lastState = model.GetRenderMode();

         if (arg != NULL) {
            char string[256];
            strcpy(string, arg);
            if (TbCheckCommandAttributeB(string, "-OFF", NULL, 0)) {
               model.SetRenderMode(TEXTURE);
            }
            else {
               model.SetRenderMode(WIREFRAME);
            }
         }
         else {
            model.SetRenderMode(WIREFRAME);
         }

         return TRUE;
      }


      /*-----------------------------
        set wireframe command - UnDo
        C.Wang 0527, 2010
       ------------------------------*/
      void TbSetWireframeCommand::Undo()
      {
         if (objID == FAILED_ID) return;

         FnObject model;
         model.Object(objID);
         model.SetRenderMode(lastState);
      }


// rename current material command
class TbRenameMaterialCommand : public FcCommandBase
{
protected:
   OBJECTid oID;
   MATERIALid matID;
   char name[256];

public:
   virtual void Init() { name[0] = '\0'; 
                         oID = FAILED_ID;
                         matID = FAILED_MATERIAL_ID;
                         SetCommandName("RenameMaterial"); };    // initialize the command
   virtual void DeInit() { };                                    // deinitialize the command
   virtual BOOL Do(char *arg);                                   // the command
   virtual void Undo();                                          // undo
};


      /*-------------------------------------
        rename current material command - Do
        C.Wang 0615, 2010
       --------------------------------------*/
      BOOL TbRenameMaterialCommand::Do(char *arg)
      {
         oID = tbCurScene->GetCurrentObject();
         matID = tbCurScene->GetCurrentMaterial();

         FnMaterial mat;
         mat.Object(matID);
         strcpy(name, mat.GetName());
         mat.SetName(arg);

         // update UIs
         int i, numMat;
         MATERIALid mID[1024];
         FnUiCombo uiC;
         FnObject model;
         model.Object(oID);

         uiC.Object(tbSelectMaterialButtonID);
         uiC.PasteComboValue(mat.GetName());
         uiC.ClearMessages();
         numMat = model.GetMaterials();
         model.GetMaterials(mID);
         for (i = 0; i < numMat; i++) {
            mat.Object(mID[i]);
            uiC.InsertMessage(i, mat.GetName());
         }

         return TRUE;
      }


      /*---------------------------------------
        rename current material command - UnDo
        C.Wang 0615, 2010
       ----------------------------------------*/
      void TbRenameMaterialCommand::Undo()
      {
         FnMaterial mat;
         mat.Object(matID);
         mat.SetName(name);

         // update UIs
         int i, numMat;
         MATERIALid mID[1024];
         FnUiCombo uiC;
         FnObject model;
         model.Object(oID);

         uiC.Object(tbSelectMaterialButtonID);
         uiC.PasteComboValue(mat.GetName());
         uiC.ClearMessages();
         numMat = model.GetMaterials();
         model.GetMaterials(mID);
         for (i = 0; i < numMat; i++) {
            mat.Object(mID[i]);
            uiC.InsertMessage(i, mat.GetName());
         }
      }


// set current material command
class TbSetMaterialCommand : public FcCommandBase
{
protected:
   MATERIALid matID;

public:
   virtual void Init() { matID = FAILED_MATERIAL_ID;
                         SetCommandName("SetMaterial"); };       // initialize the command
   virtual void DeInit() { };                                    // deinitialize the command
   virtual BOOL Do(char *arg);                                   // the command
   virtual void Undo();                                          // undo
};


      /*----------------------------------
        set current material command - Do
        C.Wang 0617, 2010
       -----------------------------------*/
      BOOL TbSetMaterialCommand::Do(char *arg)
      {
         // backup current material
         matID = tbCurScene->GetCurrentMaterial();

         // get new material
         OBJECTid oID = tbCurScene->GetCurrentObject();
         MATERIALid mID[1024];
         FnMaterial mat;
         FnUiCombo uiC;
         FnUiTypein uiTy;
         FnObject model;

         model.Object(oID);

         int i, numMat = model.GetMaterials();
         model.GetMaterials(mID);
         for (i = 0; i < numMat; i++) {
            mat.Object(mID[i]);
            if (FyCheckWord(mat.GetName(), arg)) {
               break;
            }
         }
         if (i == numMat) {
            return FALSE;
         }

         // set current material
         tbCurScene->SetCurrentMaterial(mID[i]);

         // paste the material name to typein
         uiC.Object(tbSelectMaterialButtonID);
         uiC.PasteComboValue(mat.GetName());

         // paste all material data to current material UIs
         FnUiPlate ui;
         for (i = 0; i < tbNumMatUI; i++) {
            ui.Object(tbMatUI[i]);
            ui.Enable(TRUE);
         }
         ui.Object(tbUpdateMaterialID);
         ui.Enable(FALSE);

         TbPasteCurrentMaterialData(&mat);
         return TRUE;
      }


      /*------------------------------------
        set current material command - UnDo
        C.Wang 0615, 2010
       -------------------------------------*/
      void TbSetMaterialCommand::Undo()
      {
         tbCurScene->SetCurrentMaterial(matID);

         // paste the material name to typein
         FnMaterial mat;
         FnUiCombo uiC;
         mat.Object(matID);
         uiC.Object(tbSelectMaterialButtonID);
         uiC.PasteComboValue(mat.GetName());

         // paste all material data to current material UIs
         TbPasteCurrentMaterialData(&mat);
      }


// update current material command
class TbUpdateMaterialCommand : public FcCommandBase
{
protected:
   TBMaterial matData;

public:
   virtual void Init() { memset(&matData, 0, sizeof(TBMaterial));
                         SetCommandName("UpdateMaterial"); };    // initialize the command
   virtual void DeInit() { };                                    // deinitialize the command
   virtual BOOL Do(char *arg);                                   // the command
   virtual void Undo();                                          // undo
};


      /*-------------------------------------
        update current material command - Do
        C.Wang 0617, 2010
       --------------------------------------*/
      BOOL TbUpdateMaterialCommand::Do(char *arg)
      {
         // backup current material data
         memcpy(&matData, &tbMatBuffer, sizeof(TBMaterial));

         // extract all data from UIs and update to tbMatBuffer
         TbExtractMaterialUIData();

         // update current material
         FnMaterial mat;
         mat.Object(tbCurScene->GetCurrentMaterial());
         TbUpdateCurrentMaterial(&mat);

         return TRUE;
      }


      /*---------------------------------------
        update current material command - UnDo
        C.Wang 0617, 2010
       ----------------------------------------*/
      void TbUpdateMaterialCommand::Undo()
      {
         // copy the data from backup
         memcpy(&tbMatBuffer, &matData, sizeof(TBMaterial));

         // update current material
         FnMaterial mat;
         mat.Object(tbCurScene->GetCurrentMaterial());
         TbUpdateCurrentMaterial(&mat);

         // recover the UIs
         TbCancelMaterialEditing();
      }


// load model command
class TbLoadModelCommand : public FcCommandBase
{
protected:
   int numObj;
   OBJECTid *backupObjID;
   OBJECTid lastObj;

public:
   virtual void Init() { numObj = 0; 
                         backupObjID = NULL;
                         lastObj = FAILED_ID;
                         SetCommandName("Model"); };    // initialize the command
   virtual void DeInit() { if (backupObjID != NULL) {
                              FyFree(backupObjID); backupObjID = NULL;
                           };
                         };                             // deinitialize the command
   virtual BOOL Do(char *arg);                          // the command
   virtual void Undo();                                 // undo
};


      /*-------------------------
        load models command - Do
        C.Wang 0525, 2010
       --------------------------*/
      BOOL TbLoadModelCommand::Do(char *arg)
      {
         // open the file browser for loading an object file
         int i;
         OPENFILENAME ofn;
         char openName[256], dir[256], log[256];
         BOOL beOK = FALSE;

         if (arg != NULL && strlen(arg) > 0) {
            strcpy(openName, arg);
            beOK = TRUE;
         }
         else {
            // get current directory
            GetCurrentDirectory(256, dir);

            // prepare to open the file browser
            openName[0] = '\0';
            memset(&ofn, 0, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = NULL;
            ofn.lpstrFilter = "TheFly Model CW3 {*.cw3}\0*.cw3\0"\
                              "TheFly Model in Binary CW4 {*.cw4}\0*.cw4\0"\
                              "TheFly Model in Maya OBJ {*.obj}\0*.obj\0"\
                              "TheFly Scene in ASCII CWN {*.cwn}\0*.cwn\0"\
                              "TheFly Scene in Binary CW4 {*.cw4}\0*.cw4\0"\
                              "All files {*.*}\0*.*\0\0";
            ofn.lpstrFile = openName;
            ofn.nMaxFile = 256;
            ofn.Flags = OFN_NOCHANGEDIR;
            ofn.lpstrDefExt = "cw3";
            ofn.lpstrInitialDir = dir;

            // start to load the model file
            if (GetOpenFileName(&ofn)) {
               // set the extra directory for data loading
               char name[256];
               GetCurrentDirectory(256, name);
               tbWorld.SetExtraDataPath(name);

               // success to get the file name
               beOK = TRUE;
            }

            // restore the current directory
            if (tbBeRestoreCurrentDir) {
               SetCurrentDirectory(dir);
            }
         }

         if (beOK) {
            // set waiting cursor
            SetCursor(tbWaitCursor);

            // check file format
            char path[256], name[256], ext[64];
            int format = FyCheckFileFormat(openName, path, name, ext);
            DWORD type;
            if (format == 0x0E) {
               type = 0;       // a CW3 model
            }
            else if (format == 0x10) {
               type = 1;       // a scene
            }
            else if (format == 0x17) {  // an OBJ model
               type = 4;
            }
            else {
               DWORD cw4Type = FyCheckCW4FileFormat(openName);
               if (cw4Type == CW4_MODEL) {
                  type = 0;
               }
               else if (cw4Type == CW4_SCENE) {
                  type = 2;
               }
               else {
                  type = 3;
               }
            }

            // create the object and load the file
            if (type == 0 || type == 4) {
               FnObject model;
               OBJECTid newObj = FAILED_ID;
               newObj = tbScene.CreateObject();
               model.Object(newObj);
               if (model.Load(openName)) {
                  model.SetRenderOption(SPECULAR, TRUE);

                  // insert the new object into the object list
                  tbCurScene->InsertObject(newObj, FAILED_ID, NONE, NONE);

                  // make the object as current one
                  lastObj = tbCurScene->GetCurrentObject();
                  tbCurScene->SetCurrentObject(newObj);

                  // restore the extra system directory
                  tbWorld.SetExtraDataPath(".");

                  // echo the log
                  sprintf(log, "Load model %s OK", openName);
                  TbSetLogMessage(log);

                  // backup the model
                  numObj = 1;
                  backupObjID = (OBJECTid *)FyMalloc(sizeof(OBJECTid)*numObj);
                  backupObjID[0] = newObj;
                  beOK = TRUE;
               }
               else {
                  TbSetLogMessage("No models loaded");
                  return FALSE;
               }
            }
            else if (type == 1 || type == 2) {
               // load a scene
               int numObjInFile = tbScene.QueryObjectNumberInFile(openName);
               if (numObjInFile <= 0) {
                  // load nothing
                  TbSetLogMessage("No models loaded");
                  return FALSE;
               }
               else {
                  numObj = numObjInFile;
                  backupObjID = (OBJECTid *)FyMalloc(sizeof(OBJECTid)*numObj);
                  if (type == 1) {
                     beOK = tbScene.Load(openName, backupObjID);
                  }
                  else {
                     beOK = tbScene.LoadCW4(openName, backupObjID);
                  }
                  if (!beOK) {
                     FyFree(backupObjID);
                     backupObjID = NULL;

                     TbSetLogMessage("No models loaded");
                     numObj = 0;
                     return FALSE;
                  }
                  
                  // insert the objects to current scene
                  FnObject model;
                  for (i = 0; i < numObj; i++) {
                     model.Object(backupObjID[i]);
                     model.SetRenderOption(SPECULAR, TRUE);

                     // insert the new object into the object list
                     tbCurScene->InsertObject(backupObjID[i], FAILED_ID, NONE, NONE);
                  }

                  // make the object as current one
                  lastObj = tbCurScene->GetCurrentObject();
                  tbCurScene->SetCurrentObject(backupObjID[0]);
               }
            }
            else {
               // load nothing
               TbSetLogMessage("No models loaded");
               return FALSE;
            }
            
            // check play control panel

            // check current model
            OBJECTid curObj = tbCurScene->GetCurrentObject();
            FnUiRadioButton uiRB;
            uiRB.Object(tbWireframeCurModelButtonID);
            if (curObj != FAILED_ID) {
               uiRB.Enable(TRUE);
            }
            else {
               uiRB.Enable(FALSE);
            }

            FnObject model;
            model.Object(curObj);
            if (model.GetRenderMode() == WIREFRAME) {
               uiRB.ChangeState(TRUE);
            }
            else {
               uiRB.ChangeState(FALSE);
            }

            // set standard cursor
            SetCursor(tbArrowCursor);
         }
         else {
            TbSetLogMessage("No models loaded");
            beOK = FALSE;
         }
         return beOK;
      }

      /*--------------------------
        load model command - UnDo
        C.Wang 0525, 2010
      ---------------------------*/
      void TbLoadModelCommand::Undo()
      {
         // delete the models
         if (backupObjID != NULL) {
            int i;
            for (i = 0; i < numObj; i++) {
               tbCurScene->RemoveObject(backupObjID[i], TRUE);
               backupObjID[i] = FAILED_ID;
            }
            FyFree(backupObjID);
            backupObjID = NULL;
            numObj = 0;
         }
         tbCurScene->SetCurrentObject(lastObj);
      }


// save model command
class TbSaveModelCommand : public FcCommandBase
{
public:
   virtual void Init() { SetCommandName("SaveModel"); };    // initialize the command
   virtual void DeInit() { };                               // deinitialize the command
   virtual BOOL Do(char *arg);                              // the command
   virtual void Undo() { };                                 // undo
};


      /*------------------------
        save model command - Do
        C.Wang 0613, 2010
       -------------------------*/
      BOOL TbSaveModelCommand::Do(char *arg)
      {
         // open the file browser for saving an object file
         OPENFILENAME ofn;
         char saveName[256], dir[256], log[256];
         BOOL beOK = FALSE;

         if (arg != NULL && strlen(arg) > 0) {
            strcpy(saveName, arg);
            beOK = TRUE;
         }
         else {
            // get current directory
            GetCurrentDirectory(256, dir);

            // prepare to open the file browser
            saveName[0] = '\0';
            memset(&ofn, 0, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = NULL;
            ofn.lpstrFilter = "TheFly Model CW3 {*.cw3}\0*.cw3\0"\
                              "TheFly Model in Binary CW4 {*.cw4}\0*.cw4\0"\
                              "All files {*.*}\0*.*\0\0";
            ofn.lpstrFile = saveName;
            ofn.nMaxFile = 256;
            ofn.Flags = OFN_FILEMUSTEXIST;
            ofn.lpstrDefExt = "cw3";
            ofn.lpstrInitialDir = dir;

            // get the model file name
            if (GetSaveFileName(&ofn)) {
               // set the extra directory for data loading
               char name[256];
               GetCurrentDirectory(256, name);
               tbWorld.SetExtraDataPath(name);

               // success to get the file name
               beOK = TRUE;
            }

            // restore the current directory
            if (tbBeRestoreCurrentDir) {
               SetCurrentDirectory(dir);
            }
         }

         if (beOK) {
            // set waiting cursor
            SetCursor(tbWaitCursor);

            OBJECTid oID = tbCurScene->GetCurrentObject();
            if (oID != FAILED_ID) {
               char path[256], name[256], ext[64];
               FnObject model;
               model.Object(oID);

               // check file format
               FyCheckFileFormat(saveName, path, name, ext);
               if (FyCheckWord(ext, "cw4", TRUE)) {
                  model.SaveCW4(saveName);
               }
               else {
                  model.Save(saveName);
               }
               sprintf(log, "The %s model is saved.", model.GetName());
               TbSetLogMessage(log);

               // set standard cursor
               SetCursor(tbArrowCursor);
            }
            else {
               strcpy(log, "Nothing to save");
               TbSetLogMessage(log);
            }
         }
         else {
            strcpy(log, "Nothing to save");
            TbSetLogMessage(log);
         }

         return beOK;
      }


// current model command
class TbCurrentModelCommand : public FcCommandBase
{
protected:
   OBJECTid backupObjID;

public:
   virtual void Init() { backupObjID = FAILED_ID;
                         SetCommandName("CurrentModel"); };    // initialize the command
   virtual void DeInit() { backupObjID = FAILED_ID; };         // deinitialize the command
   virtual BOOL Do(char *arg);                                 // the next model command
   virtual void Undo();                                        // undo
};


      /*-------------------------
        next models command - Do
        C.Wang 0707, 2010
       --------------------------*/
      BOOL TbCurrentModelCommand::Do(char *arg)
      {
         backupObjID = tbCurScene->GetCurrentObject();
         OBJECTid oID = tbCurScene->FindObject(arg);
         if (oID != FAILED_ID) {
            char log[256];
            FnObject model;
            tbCurScene->SetCurrentObject(oID);
            model.Object(tbCurScene->GetCurrentObject());
            sprintf(log, "New current model : %s", model.GetName());
            TbSetLogMessage(log);
         }
         else {
            TbSetLogMessage("Failed to get new current model");
         }
         return TRUE;
      }

      /*--------------------------
        next model command - UnDo
        C.Wang 0613, 2010
      ---------------------------*/
      void TbCurrentModelCommand::Undo()
      {
         tbCurScene->SetCurrentObject(backupObjID);
         backupObjID = FAILED_ID;
      }


// next model command
class TbNextModelCommand : public FcCommandBase
{
protected:
   OBJECTid backupObjID;

public:
   virtual void Init() { backupObjID = FAILED_ID;
                         SetCommandName("NextModel"); };    // initialize the command
   virtual void DeInit() { backupObjID = FAILED_ID; };      // deinitialize the command
   virtual BOOL Do(char *arg);                              // the next model command
   virtual void Undo();                                     // undo
};


      /*-------------------------
        next models command - Do
        C.Wang 0613, 2010
       --------------------------*/
      BOOL TbNextModelCommand::Do(char *arg)
      {
         backupObjID = tbCurScene->GetCurrentObject();
         BOOL beOK = tbCurScene->NextCurrentObject();
         if (beOK) {
            char log[256];
            FnObject model;
            model.Object(tbCurScene->GetCurrentObject());
            sprintf(log, "New current model : %s", model.GetName());
            TbSetLogMessage(log);
         }
         else {
            TbSetLogMessage("Failed to get next current model");
         }
         return beOK;
      }

      /*--------------------------
        next model command - UnDo
        C.Wang 0613, 2010
      ---------------------------*/
      void TbNextModelCommand::Undo()
      {
         tbCurScene->SetCurrentObject(backupObjID);
         backupObjID = FAILED_ID;
      }


// previous model command
class TbPreviousModelCommand : public FcCommandBase
{
protected:
   OBJECTid backupObjID;

public:
   virtual void Init() { backupObjID = FAILED_ID;
                         SetCommandName("PreviousModel"); };    // initialize the command
   virtual void DeInit() { backupObjID = FAILED_ID; };          // deinitialize the command
   virtual BOOL Do(char *arg);                                  // the next model command
   virtual void Undo();                                         // undo
};


      /*-----------------------------
        previous models command - Do
        C.Wang 0613, 2010
       ------------------------------*/
      BOOL TbPreviousModelCommand::Do(char *arg)
      {
         backupObjID = tbCurScene->GetCurrentObject();
         BOOL beOK = tbCurScene->PreviousCurrentObject();
         if (beOK) {
            char log[256];
            FnObject model;
            model.Object(tbCurScene->GetCurrentObject());
            sprintf(log, "New current model : %s", model.GetName());
            TbSetLogMessage(log);
         }
         else {
            TbSetLogMessage("Failed to get previous current model");
         }
         return beOK;
      }

      /*------------------------------
        previous model command - UnDo
        C.Wang 0613, 2010
      --------------------------------*/
      void TbPreviousModelCommand::Undo()
      {
         tbCurScene->SetCurrentObject(backupObjID);
         backupObjID = FAILED_ID;
      }


// delete current model command
class TbDeleteModelCommand : public FcCommandBase
{
protected:
   OBJECTid backupObjID;
   ACTORid actorID;
   int type;
   int boneID;

public:
   virtual void Init() { backupObjID = FAILED_ID;
                         actorID = FAILED_ID;
                         boneID = NONE;
                         type = NONE;
                         SetCommandName("DeleteModel"); };      // initialize the command
   virtual void DeInit() { backupObjID = FAILED_ID; };          // deinitialize the command
   virtual BOOL Do(char *arg);                                  // the next model command
   virtual void Undo();                                         // undo
};


      /*----------------------------------
        delete current model command - Do
        C.Wang 0629, 2010
       -----------------------------------*/
      BOOL TbDeleteModelCommand::Do(char *arg)
      {
         backupObjID = tbCurScene->GetCurrentObject();
         if (backupObjID == FAILED_ID) {
            type = NONE;
            boneID = NONE;
            return TRUE;
         }

         TBObject *tbObj = tbCurScene->GetTBObject(backupObjID);
         if (tbObj != NULL) {
            boneID = tbObj->BoneID();
            type = tbObj->Type();
         }
         else {
            type = NONE;
            boneID = NONE;
         }

         FnObject model;
         model.Object(backupObjID);
         model.ChangeScene(tbDumperSceneID);
         actorID = tbCurScene->RemoveObject(backupObjID, FALSE);
         tbCurScene->SetCurrentObject(FAILED_ID);

         if (actorID == FAILED_ID) {
            type = NONE;
            boneID = NONE;
         }

         char log[256];
         sprintf(log, "Delete current model : %s", model.GetName());
         TbSetLogMessage(log);
         return TRUE;
      }

      /*------------------------------------
        delete current model command - UnDo
        C.Wang 0630, 2010
       -------------------------------------*/
      void TbDeleteModelCommand::Undo()
      {
         if (backupObjID == FAILED_ID) return;

         FnObject model;
         model.Object(backupObjID);
         model.ChangeScene(tbMainSceneID);
         tbCurScene->InsertObject(backupObjID, actorID, type, boneID);
         tbCurScene->SetCurrentObject(backupObjID);

         if (actorID != FAILED_ID) {
            FnActor actor;
            actor.Object(actorID);
            if (type == 0) {
            }
            else if (type == 1) {
               actor.ApplySkin(backupObjID);
            }
            else if (type == 2) {
               actor.ApplyAttachment(backupObjID, boneID);
            }
         }

         backupObjID = FAILED_ID;
         actorID = FAILED_ID;
         boneID = NONE;
      }


// set paths command
class TbSetPathCommand : public FcCommandBase
{
protected:
   char work[256];
   char shader[256];
   char texture[256];
   char model[256];
   char actor[256];
   char audio[256];
   char extra[256];
   char scene[256];

public:
   virtual void Init() { work[0] = '\0'; shader[0] = '\0'; texture[0] = '\0'; model[0] = '\0'; actor[0] = '\0';
                         audio[256] = '\0'; extra[0] = '\0'; scene[0] = '\0';
                         SetCommandName("SetPath"); };    // initialize the command
   virtual void DeInit() {};                              // deinitialize the command
   virtual BOOL Do(char *arg);                            // the load model command
   virtual void Undo();                                   // undo
};

      /*-----------------------
        set paths command - Do
        C.Wang 0525, 2010
       ------------------------*/
      BOOL TbSetPathCommand::Do(char *arg)
      {
         // backup the paths
         strcpy(work, tbWorkDir);
         strcpy(shader, tbWorld.GetShaderPath());
         strcpy(model, tbWorld.GetObjectPath());
         strcpy(texture, tbWorld.GetTexturePath());
         strcpy(actor, tbWorld.GetCharacterPath());
         strcpy(scene, tbWorld.GetScenePath());
         strcpy(audio, tbWorld.GetAudioPath());
         strcpy(extra, tbWorld.GetExtraDataPath());

         // get new paths
         char path[256];
         BOOL beOKCmd = TbCheckCommandAttributeS(arg, "-Work", path, 256);
         if (beOKCmd) {
            SetCurrentDirectory(path);
            strcpy(tbWorkDir, path);
         }
         beOKCmd = TbCheckCommandAttributeS(arg, "-Texture", path, 256);
         if (beOKCmd) {
            tbWorld.SetTexturePath(path);
         }
         beOKCmd = TbCheckCommandAttributeS(arg, "-Scene", path, 256);
         if (beOKCmd) {
            tbWorld.SetScenePath(path);
         }
         beOKCmd = TbCheckCommandAttributeS(arg, "-Object", path, 256);
         if (beOKCmd) {
            tbWorld.SetObjectPath(path);
         }
         beOKCmd = TbCheckCommandAttributeS(arg, "-Shader", path, 256);
         if (beOKCmd) {
            tbWorld.SetShaderPath(path);
         }
         beOKCmd = TbCheckCommandAttributeS(arg, "-Actor", path, 256);
         if (beOKCmd) {
            tbWorld.SetCharacterPath(path);
         }
         beOKCmd = TbCheckCommandAttributeS(arg, "-Audio", path, 256);
         if (beOKCmd) {
            tbWorld.SetAudioPath(path);
         }
         beOKCmd = TbCheckCommandAttributeS(arg, "-Extra", path, 256);
         if (beOKCmd) {
            tbWorld.SetExtraDataPath(path);
         }

         // update UIs (currently we only UIs for shader, object, texture and actor)
         FnUiTypein uiTy;
         uiTy.Object(tbWorkDirTypeinUI);
         uiTy.PasteTypeinValue(tbWorkDir);
         uiTy.Object(tbShaderDirTypeinUI);
         uiTy.PasteTypeinValue(tbWorld.GetShaderPath());
         uiTy.Object(tbModelDirTypeinUI);
         uiTy.PasteTypeinValue(tbWorld.GetObjectPath());
         uiTy.Object(tbTextureDirTypeinUI);
         uiTy.PasteTypeinValue(tbWorld.GetTexturePath());
         uiTy.Object(tbActorDirTypeinUI);
         uiTy.PasteTypeinValue(tbWorld.GetCharacterPath());
         return TRUE;
      }


      /*-------------------------
        set paths command - UnDo
        C.Wang 0525, 2010
      --------------------------*/
      void TbSetPathCommand::Undo()
      {
         // recover the paths setting
         strcpy(tbWorkDir, work);
         tbWorld.SetShaderPath(shader);
         tbWorld.SetObjectPath(model);
         tbWorld.SetTexturePath(texture);
         tbWorld.SetCharacterPath(actor);
         tbWorld.SetAudioPath(audio);
         tbWorld.SetScenePath(scene);
         tbWorld.SetExtraDataPath(extra);

         // update UIs
         FnUiTypein uiTy;
         uiTy.Object(tbWorkDirTypeinUI);
         uiTy.PasteTypeinValue(tbWorkDir);
         uiTy.Object(tbShaderDirTypeinUI);
         uiTy.PasteTypeinValue(shader);
         uiTy.Object(tbModelDirTypeinUI);
         uiTy.PasteTypeinValue(model);
         uiTy.Object(tbTextureDirTypeinUI);
         uiTy.PasteTypeinValue(texture);
         uiTy.Object(tbActorDirTypeinUI);
         uiTy.PasteTypeinValue(actor);
      }


// load actor command
class TbLoadActorCommand : public FcCommandBase
{
protected:
   ACTORid backupActorID;
   ACTORid lastActor;

public:
   virtual void Init() { backupActorID = FAILED_ID;
                         lastActor = FAILED_ID;
                         SetCommandName("Actor"); };    // initialize the command
   virtual void DeInit() {};                            // deinitialize the command
   virtual BOOL Do(char *arg);                          // the load model command
   virtual void Undo();                                 // undo
};


      /*------------------------
        load actor command - Do
        C.Wang 0624, 2010
       -------------------------*/
      BOOL TbLoadActorCommand::Do(char *arg)
      {
         // open the file browser for loading an actor file
         OPENFILENAME ofn;
         char openName[256], dir[256], log[256];
         FnActor actor;
         BOOL beOK = FALSE;

         if (arg != NULL && strlen(arg) > 0) {
            strcpy(openName, arg);
            beOK = TRUE;
         }
         else {
            // get current directory
            GetCurrentDirectory(256, dir);

            // prepare to open the file browser
            openName[0] = '\0';
            memset(&ofn, 0, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = NULL;
            ofn.lpstrFilter = "TheFly Actor CWA {*.cwa}\0*.cwa\0"\
                              "TheFly Actor in CWC {*.cwc}\0*.cwc\0"\
                              "TheFly Actor in CW4 {*.cw4}\0*.cw4\0"\
                              "All files {*.*}\0*.*\0\0";
            ofn.lpstrFile = openName;
            ofn.nMaxFile = 256;
            ofn.Flags = OFN_FILEMUSTEXIST;
            ofn.lpstrDefExt = "cwa";
            ofn.lpstrInitialDir = dir;

            // start to load the actor file
            if (GetOpenFileName(&ofn)) {
               // set the extra directory for data loading
               char name[256];
               GetCurrentDirectory(256, name);
               tbWorld.SetExtraDataPath(name);

               // success to get the file name
               beOK = TRUE;
            }

            // restore the current directory
            if (tbBeRestoreCurrentDir) {
               SetCurrentDirectory(dir);
            }
         }

         if (beOK) {
            // set waiting cursor
            SetCursor(tbWaitCursor);

            ACTORid newActorID;
            OBJECTid objID;
            FnActor actor;

            // load the file
            newActorID = tbScene.LoadActor(openName);
            actor.Object(newActorID);

            // for loaded actor, we do 
            if (newActorID != FAILED_ID) {
               int i, numObj;

               // insert all actor's objects into the object list
               numObj = actor.GetBoneNumber();
               for (i = 0; i < numObj; i++) {
                  if (actor.IsGeometry(i)) {
                     objID = actor.GetBoneObject(actor.GetBoneName(i));
                     if (objID != FAILED_ID) {
                        tbCurScene->InsertObject(objID, newActorID, 0, i);
                     }
                  }
               }

               // insert all actor's skins into the object list
               numObj = actor.SkinNumber();
               for (i = 0; i < numObj; i++) {
                  objID = actor.GetSkin(i);
                  if (objID != FAILED_ID) {
                     tbCurScene->InsertObject(objID, newActorID, 1, i);
                  }
               }

               // insert all actor's attachments into the object list
               numObj = actor.AttachmentNumber();
               for (i = 0; i < numObj; i++) {
                  if (actor.IsActorAttachment(i)) {
                     // to-be-done
                  }
                  else {
                     objID = (OBJECTid) actor.GetAttachment(i);
                     if (objID != FAILED_ID) {
                        tbCurScene->InsertObject(objID, newActorID, 2, actor.GetAttachmentParentBoneID(i));
                     }
                  }
               }

               // make the 1st skin as current object
               objID = actor.GetSkin(0);
               if (objID != FAILED_ID) {
                  // check the current object
                  tbCurScene->SetCurrentObject(objID);
               }
               
               // check current model
               OBJECTid curObj = tbCurScene->GetCurrentObject();
               FnUiRadioButton uiRB;
               uiRB.Object(tbWireframeCurModelButtonID);
               if (curObj != FAILED_ID) {
                  uiRB.Enable(TRUE);
               }
               else {
                  uiRB.Enable(FALSE);
               }

               FnObject model;
               OBJECTid base = actor.GetBaseObject();
               model.Object(base);
               TbDrawBlueBox(&model);
               model.Show(TRUE);

               // insert the actor to database & make it as the current actor
               tbCurScene->InsertActor(newActorID);

               // restore the extra system directory
               tbWorld.SetExtraDataPath(".");

               // echo the log
               sprintf(log, "Load actor %s OK", openName);
               TbSetLogMessage(log);

               // set standard cursor
               SetCursor(tbArrowCursor);

               model.Object(curObj);
               if (model.GetRenderMode() == WIREFRAME) {
                  uiRB.ChangeState(TRUE);
               }
               else {
                  uiRB.ChangeState(FALSE);
               }

               // backup the actor
               lastActor = tbCurScene->GetCurrentActor();
               backupActorID = newActorID;
               tbCurScene->SetCurrentActor(newActorID);
               beOK = TRUE;

               // update UIs
               int numA = tbCurScene->GetEffectActorNumber();
               int iCount;
               FnUiCombo uiC;
               uiC.Object(tbSelectActorComboID);
               uiC.ClearMessages();
               if (numA > 0) {
                  char *name;
                  uiC.Enable(TRUE);
                  iCount = 0;
                  numA = tbCurScene->GetActorNumber();
                  for (i = 0; i < numA; i++) {
                     name = tbCurScene->GetActorName(i);
                     if (name != NULL) {
                        uiC.InsertMessage(iCount, name);
                        iCount++;
                     }
                  }

                  // paste current actor
                  FnActor actor;
                  actor.Object(newActorID);
                  uiC.PasteComboValue(actor.GetName());
               }
               else {
                  uiC.Enable(FALSE);
                  uiC.PasteComboValue("");
               }

               // find and paste the current body
               FnActor actor;
               actor.Object(tbCurScene->GetCurrentActor());
               TbUpdateCurrentBodyDataUI(&actor);

               // check if the actor has the current action to play

            }
            else {
               TbSetLogMessage("No actor loaded");
               beOK = FALSE;
            }
         }
         else {
            TbSetLogMessage("No actor loaded");
            beOK = FALSE;
         }
         return beOK;
      }

      /*--------------------------
        load actor command - UnDo
        C.Wang 0628, 2010
      ---------------------------*/
      void TbLoadActorCommand::Undo()
      {
         // delete the actor
         tbScene.DeleteActor(backupActorID);
         backupActorID = FAILED_ID;

         // recover current actor
         tbCurScene->SetCurrentActor(lastActor);

         int numA = tbCurScene->GetEffectActorNumber();
         int i, iCount;
         FnUiCombo uiC;
         uiC.Object(tbSelectActorComboID);
         uiC.ClearMessages();
         if (numA > 0) {
            char *name;
            uiC.Enable(TRUE);
            iCount = 0;
            numA = tbCurScene->GetActorNumber();
            for (i = 0; i < numA; i++) {
               name = tbCurScene->GetActorName(i);
               if (name != NULL) {
                  uiC.InsertMessage(iCount, name);
                  iCount++;
               }
            }
         }
         
         // find and paste the current body
         FnActor actor;
         actor.Object(tbCurScene->GetCurrentActor());
         TbUpdateCurrentBodyDataUI(&actor);
      }


// delete current actor command
class TbDeleteActorCommand : public FcCommandBase
{
protected:
   ACTORid backupActorID;

public:
   virtual void Init() { backupActorID = FAILED_ID;
                         SetCommandName("DeleteActor"); };      // initialize the command
   virtual void DeInit() { backupActorID = FAILED_ID; };        // deinitialize the command
   virtual BOOL Do(char *arg);                                  // the next model command
   virtual void Undo();                                         // undo
};


      /*----------------------------------
        delete current actor command - Do
        C.Wang 0628, 2010
       -----------------------------------*/
      BOOL TbDeleteActorCommand::Do(char *arg)
      {
         backupActorID = tbCurScene->GetCurrentActor();

         FnActor actor;
         actor.Object(backupActorID);
         actor.ChangeScene(tbDumperSceneID);
         tbCurScene->RemoveActor(backupActorID, FALSE);
         tbCurScene->SetCurrentActor(FAILED_ID);

         int numA = tbCurScene->GetEffectActorNumber();
         int i, iCount;
         FnUiCombo uiC;
         uiC.Object(tbSelectActorComboID);
         uiC.ClearMessages();
         if (numA > 0) {
            char *name;
            uiC.Enable(TRUE);
            iCount = 0;
            numA = tbCurScene->GetActorNumber();
            for (i = 0; i < numA; i++) {
               name = tbCurScene->GetActorName(i);
               if (name != NULL) {
                  uiC.InsertMessage(iCount, name);
                  iCount++;
               }
            }
         }
         else {
            uiC.Enable(FALSE);
         }

         actor.Object(FAILED_ID);
         TbUpdateCurrentBodyDataUI(&actor);

         char log[256];
         sprintf(log, "Delete current actor : %s", actor.GetName());
         TbSetLogMessage(log);
         return TRUE;
      }

      /*------------------------------------
        delete current actor command - UnDo
        C.Wang 0626, 2010
       -------------------------------------*/
      void TbDeleteActorCommand::Undo()
      {
         FnActor actor;
         actor.Object(backupActorID);
         actor.ChangeScene(tbMainSceneID);
         tbCurScene->InsertActor(backupActorID);
         tbCurScene->SetCurrentActor(backupActorID);
         backupActorID = FAILED_ID;

         int numA = tbCurScene->GetEffectActorNumber();
         int i, iCount;
         FnUiCombo uiC;
         uiC.Object(tbSelectActorComboID);
         uiC.ClearMessages();
         if (numA > 0) {
            char *name;
            uiC.Enable(TRUE);
            iCount = 0;
            numA = tbCurScene->GetActorNumber();
            for (i = 0; i < numA; i++) {
               name = tbCurScene->GetActorName(i);
               if (name != NULL) {
                  uiC.InsertMessage(iCount, name);
                  iCount++;
               }
            }
         }

         // find and paste the current body
         actor.Object(tbCurScene->GetCurrentActor());
         TbUpdateCurrentBodyDataUI(&actor);
      }


// delete current action of current actor command
class TbDeleteActionCommand : public FcCommandBase
{
protected:
   ACTORid backupActorID;
   char bodyName[256];
   int type;
   char actionName[256];
   char frontPoseName[256];
   char rearPoseName[256];
   int duration;
   float w0, w1;

public:
   virtual void Init() { backupActorID = FAILED_ID;
                         bodyName[0] = '\0';
                         type = SIMPLE_ACTION;
                         frontPoseName[0] = '\0';
                         rearPoseName[0] = '\0';
                         actionName[0] = '\0';
                         duration = 0;
                         w0 = 1.0f;
                         w1 = 0.0f;
                         SetCommandName("DeleteAction"); };     // initialize the command
   virtual void DeInit() { backupActorID = FAILED_ID; };        // deinitialize the command
   virtual BOOL Do(char *arg);                                  // the next model command
   virtual void Undo();                                         // undo
};


      /*-----------------------------------
        delete current action command - Do
        C.Wang 0701, 2010
       ------------------------------------*/
      BOOL TbDeleteActionCommand::Do(char *arg)
      {
         // backup data
         backupActorID = tbCurScene->GetCurrentActor();
         if (backupActorID == FAILED_ID) {
            TbSetLogMessage("No current actor");
            return TRUE;
         }

         FnActor actor;
         actor.Object(backupActorID);
         char *name = actor.GetCurrentBodyName();
         if (name == NULL) {
            backupActorID = FAILED_ID;
            TbSetLogMessage("No current body");
            return TRUE;
         }
         strcpy(bodyName, name);

         ACTIONid actionID = actor.GetCurrentAction(0, bodyName);
         if (actionID == FAILED_ID) {
            backupActorID = FAILED_ID;
            TbSetLogMessage("No current action");
            return TRUE;
         }

         FnAction action;
         action.Object(actionID);
         type = action.GetType();
         strcpy(frontPoseName, action.GetFrontPoseName());
         strcpy(rearPoseName, action.GetRearPoseName());
         duration = action.GetLength();
         strcpy(actionName, action.GetName());
         w0 = action.GetFrontWeight();
         w1 = action.GetRearWeight();

         // delete the action
         actor.DeleteAction(actionID);

         // make the actor no active action to play
         actor.MakeCurrentAction(0, bodyName, FAILED_ID);

         // update UIs
         int i, iCount, num;
         ACTIONid aTable[1024];
         FnUiCombo uiC;
         uiC.Object(tbSelectActionComboID);
         uiC.PasteComboValue("");
         uiC.ClearMessages();
         num = actor.GetBodyAllActions(bodyName, aTable, 1024);
         iCount = 0;
         for (i = 0;  i < num; i++) {
            if (aTable[i] != FAILED_ID) {
               action.Object(aTable[i]);
               uiC.InsertMessage(iCount, action.GetName());
               iCount++;
            }
         }
         FnUiButton uiB;
         uiB.Object(tbDeleteActionButtonID);
         uiB.Enable(FALSE);
         uiB.Object(tbEditActionButtonID);
         uiB.Enable(FALSE);
         uiB.Object(tbAddActionButtonID);
         uiB.Enable(TRUE);

         char log[256];
         sprintf(log, "Delete current action : %s", actor.GetName());
         TbSetLogMessage(log);
         return TRUE;
      }

      /*-------------------------------------
        delete current action command - UnDo
        C.Wang 0701, 2010
       --------------------------------------*/
      void TbDeleteActionCommand::Undo()
      {
         if (backupActorID == FAILED_ID) return;

         FnActor actor;
         actor.Object(backupActorID);
         ACTIONid actionID;
         if (type == CROSS_BLEND_ACTION) {
            actionID = actor.CreateCrossBlendAction(bodyName, actionName, frontPoseName, rearPoseName, duration);
         }
         else if (type == CONNECT_ACTION) {
            actionID = actor.CreateConnectAction(bodyName, actionName, frontPoseName, rearPoseName, duration);
         }
         else if (type == FULL_BLEND_ACTION) {
            actionID = actor.CreateFullBlendAction(bodyName, actionName, frontPoseName, rearPoseName, w0, w1);
         }
         else {
            actionID = actor.CreateAction(bodyName, actionName, frontPoseName);
         }

         // make the actor no active action to play
         actor.MakeCurrentAction(0, bodyName, actionID);
         actor.Play(0, START, 0.0f, TRUE, TRUE);

         // update UIs
         int i, iCount, num;
         ACTIONid aTable[1024];
         FnAction action;
         FnUiCombo uiC;
         uiC.Object(tbSelectActionComboID);
         uiC.PasteComboValue(actionName);
         uiC.ClearMessages();
         num = actor.GetBodyAllActions(bodyName, aTable, 1024);
         iCount = 0;
         for (i = 0;  i < num; i++) {
            if (aTable[i] != FAILED_ID) {
               action.Object(aTable[i]);
               uiC.InsertMessage(iCount, action.GetName());
               iCount++;
            }
         }
         FnUiButton uiB;
         uiB.Object(tbEditActionButtonID);
         uiB.Enable(TRUE);
         uiB.Object(tbDeleteActionButtonID);
         uiB.Enable(TRUE);
         uiB.Object(tbAddActionButtonID);
         uiB.Enable(TRUE);
      }


// edit current action of current actor command
class TbEditActionCommand : public FcCommandBase
{
protected:
   ACTORid backupActorID;
   char bodyName[256];
   int type;
   ACTIONid actionID;
   char frontPoseName[256];
   char rearPoseName[256];
   int duration;
   float w0, w1;

public:
   virtual void Init() { backupActorID = FAILED_ID;
                         bodyName[0] = '\0';
                         type = SIMPLE_ACTION;
                         frontPoseName[0] = '\0';
                         rearPoseName[0] = '\0';
                         duration = 0;
                         w0 = 1.0f;
                         w1 = 0.0f;
                         SetCommandName("EditAction"); };     // initialize the command
   virtual void DeInit() { backupActorID = FAILED_ID; };      // deinitialize the command
   virtual BOOL Do(char *arg);                                // the next model command
   virtual void Undo();                                       // undo
};


      /*---------------------------------
        edit current action command - Do
        C.Wang 0703, 2010
       ----------------------------------*/
      BOOL TbEditActionCommand::Do(char *arg)
      {
         // backup data
         backupActorID = tbCurScene->GetCurrentActor();
         if (backupActorID == FAILED_ID) {
            TbSetLogMessage("No current actor");
            return TRUE;
         }

         FnActor actor;
         actor.Object(backupActorID);
         char *name = actor.GetCurrentBodyName();
         if (name == NULL) {
            backupActorID = FAILED_ID;
            TbSetLogMessage("No current body");
            return TRUE;
         }
         strcpy(bodyName, name);

         actionID = actor.GetCurrentAction(0, bodyName);
         if (actionID == FAILED_ID) {
            backupActorID = FAILED_ID;
            TbSetLogMessage("No current action");
            return TRUE;
         }

         FnAction action;
         action.Object(actionID);
         type = action.GetType();
         strcpy(frontPoseName, action.GetFrontPoseName());
         strcpy(rearPoseName, action.GetRearPoseName());
         duration = action.GetLength();
         w0 = action.GetFrontWeight();
         w1 = action.GetRearWeight();

         // parse the data from argument
         char poseName[256];
         BOOL beOK = TbCheckCommandAttributeS(arg, "-front", poseName, 256);
         if (beOK) {
            action.SetFrontPose(poseName);
         }

         if (type != SIMPLE_ACTION) {
            beOK = TbCheckCommandAttributeS(arg, "-rear", poseName, 256);
            if (beOK) {
               action.SetRearPose(poseName);
            }

            beOK = TbCheckCommandAttributeI(arg, "-length", &duration, 1);
            if (beOK) {
               action.SetLength(duration);
            }
         }

         // make the actor play the new action data
         actor.Play(0, START, 0.0f, tbBeBase, TRUE);

         char log[256];
         sprintf(log, "Update current action : %s", action.GetName());
         TbSetLogMessage(log);
         return TRUE;
      }

      /*-----------------------------------
        edit current action command - UnDo
        C.Wang 0703, 2010
       ------------------------------------*/
      void TbEditActionCommand::Undo()
      {
         if (backupActorID == FAILED_ID) return;

         FnActor actor;
         actor.Object(backupActorID);

         FnAction action;
         action.Object(actionID);
         action.SetFrontPose(frontPoseName);
         if (type == CROSS_BLEND_ACTION) {
            action.SetRearPose(rearPoseName);
            action.SetLength(duration);
         }
         else if (type == CONNECT_ACTION) {
            action.SetRearPose(rearPoseName);
            action.SetLength(duration);
         }
         else if (type == FULL_BLEND_ACTION) {
            // to-be-done
            action.SetRearPose(rearPoseName);
         }

         // make the actor play the new action data
         actor.Play(0, START, 0.0f, tbBeBase, TRUE);
      }


// create a new action of current actor command
class TbNewActionCommand : public FcCommandBase
{
protected:
   ACTORid backupActorID;
   char bodyName[256];
   ACTIONid actionID;

public:
   virtual void Init() { backupActorID = FAILED_ID;
                         bodyName[0] = '\0';
                         actionID = FAILED_ID;
                         SetCommandName("NewAction"); };      // initialize the command
   virtual void DeInit() { backupActorID = FAILED_ID; };      // deinitialize the command
   virtual BOOL Do(char *arg);                                // the next model command
   virtual void Undo();                                       // undo
};


      /*---------------------------------
        create a new action command - Do
        C.Wang 0705, 2010
       ----------------------------------*/
      BOOL TbNewActionCommand::Do(char *arg)
      {
         // backup data
         backupActorID = tbCurScene->GetCurrentActor();
         if (backupActorID == FAILED_ID) {
            TbSetLogMessage("No current actor");
            return TRUE;
         }

         FnActor actor;
         actor.Object(backupActorID);
         char *name = actor.GetCurrentBodyName();
         if (name == NULL) {
            backupActorID = FAILED_ID;
            TbSetLogMessage("No current body");
            return TRUE;
         }
         strcpy(bodyName, name);

         actionID = actor.GetCurrentAction(0, bodyName);
         if (actionID == FAILED_ID) {
            backupActorID = FAILED_ID;
            TbSetLogMessage("No current action");
            return TRUE;
         }

         BOOL beOK;
         DWORD type = SIMPLE_ACTION;
         int duration = 0, len;
         char frontPoseName[256], rearPoseName[256], aName[256];
         char poseName[256], typeName[256];

         frontPoseName[0] = '\0';
         rearPoseName[0] = '\0';
         duration = 0;

         // parse the data from argument
         beOK = TbCheckCommandAttributeS(arg, "-name", poseName, 256);
         if (beOK) {
            strcpy(aName, poseName);
            if (FyCheckWord(aName, "-")) {
               backupActorID = FAILED_ID;
               TbSetLogMessage("Illegal Action Name");
               return TRUE;
            }
         }

         beOK = TbCheckCommandAttributeS(arg, "-front", poseName, 256);
         if (beOK) {
            strcpy(frontPoseName, poseName);
            if (FyCheckWord(frontPoseName, "-")) {
               backupActorID = FAILED_ID;
               TbSetLogMessage("Illegal Action");
               return TRUE;
            }
         }

         beOK = TbCheckCommandAttributeS(arg, "-rear", poseName, 256);
         if (beOK) {
            strcpy(rearPoseName, poseName);
         }

         beOK = TbCheckCommandAttributeI(arg, "-length", &len, 1);
         if (beOK) {
            duration = len;
         }

         beOK = TbCheckCommandAttributeS(arg, "-type", typeName, 256);
         if (beOK) {
            if (FyCheckWord(typeName, "Cross")) {
               type = CROSS_BLEND_ACTION;
            }
            else if (FyCheckWord(typeName, "Connection")) {
               type = CONNECT_ACTION;
            }
            else {
               type = SIMPLE_ACTION;
            }
         }

         // create a new action
         ACTIONid newActionID = FAILED_ID;
         if (type == CONNECT_ACTION) {
            newActionID = actor.CreateConnectAction(bodyName, aName, frontPoseName, rearPoseName, duration);
         }
         else if (type == CROSS_BLEND_ACTION) {
            newActionID = actor.CreateCrossBlendAction(bodyName, aName, frontPoseName, rearPoseName, duration);
         }
         else {
            newActionID = actor.CreateAction(bodyName, aName, frontPoseName);
         }

         // make the actor play the new action data
         actor.MakeCurrentAction(0, bodyName, newActionID);
         actor.Play(0, START, 0.0f, tbBeBase, TRUE);

         // update UIs
         int i, iCount, num;
         ACTIONid aTable[1024];
         FnAction action;
         FnUiCombo uiC;
         uiC.Object(tbSelectActionComboID);
         uiC.PasteComboValue(aName);
         uiC.ClearMessages();
         num = actor.GetBodyAllActions(bodyName, aTable, 1024);
         iCount = 0;
         for (i = 0;  i < num; i++) {
            if (aTable[i] != FAILED_ID) {
               action.Object(aTable[i]);
               uiC.InsertMessage(iCount, action.GetName());
               iCount++;
            }
         }
         FnUiButton uiB;
         uiB.Object(tbEditActionButtonID);
         uiB.Enable(TRUE);
         uiB.Object(tbDeleteActionButtonID);
         uiB.Enable(TRUE);
         uiB.Object(tbAddActionButtonID);
         uiB.Enable(TRUE);
         
         char log[256];
         sprintf(log, "Create a new action : %s", actor.GetName());
         TbSetLogMessage(log);
         return TRUE;
      }

      /*-----------------------------------
        create a new action command - UnDo
        C.Wang 0705, 2010
       ------------------------------------*/
      void TbNewActionCommand::Undo()
      {
         if (backupActorID == FAILED_ID) return;

         FnActor actor;
         actor.Object(backupActorID);

         // delete current action
         ACTIONid aID = actor.GetCurrentAction(0, bodyName);
         actor.DeleteAction(aID);

         // recover last current action
         actor.MakeCurrentAction(0, bodyName, actionID);

         // make the actor play the new action data
         actor.Play(0, START, 0.0f, tbBeBase, TRUE);

         FnAction action;
         action.Object(actionID);

         // update UIs
         int i, iCount, num;
         ACTIONid aTable[1024];
         FnUiCombo uiC;
         uiC.Object(tbSelectActionComboID);
         uiC.PasteComboValue(action.GetName());
         uiC.ClearMessages();
         num = actor.GetBodyAllActions(bodyName, aTable, 1024);
         iCount = 0;
         for (i = 0;  i < num; i++) {
            if (aTable[i] != FAILED_ID) {
               action.Object(aTable[i]);
               uiC.InsertMessage(iCount, action.GetName());
               iCount++;
            }
         }
         FnUiButton uiB;
         uiB.Object(tbEditActionButtonID);
         uiB.Enable(TRUE);
         uiB.Object(tbDeleteActionButtonID);
         uiB.Enable(TRUE);
         uiB.Object(tbAddActionButtonID);
         uiB.Enable(TRUE);
      }


// set current actor command
class TbSetActorCommand : public FcCommandBase
{
protected:
   ACTORid aID;

public:
   virtual void Init() { aID = FAILED_ID;
                         SetCommandName("SetActor"); };       // initialize the command
   virtual void DeInit() { };                                 // deinitialize the command
   virtual BOOL Do(char *arg);                                // the command
   virtual void Undo();                                       // undo
};


      /*-------------------------------
        set current actor command - Do
        C.Wang 0626, 2010
       --------------------------------*/
      BOOL TbSetActorCommand::Do(char *arg)
      {
         // backup current actor
         aID = tbCurScene->GetCurrentActor();

         // get new actor's name
         FnUiCombo uiC;
         uiC.Object(tbSelectActorComboID);
         uiC.PasteComboValue(arg);

         // find the actor
         ACTORid newID = tbCurScene->GetActorByName(arg);

         // set current actor
         tbCurScene->SetCurrentActor(newID);

         // find and paste the current body
         FnActor actor;
         actor.Object(tbCurScene->GetCurrentActor());
         TbUpdateCurrentBodyDataUI(&actor);
         return TRUE;
      }


      /*---------------------------------
        set current actor command - UnDo
        C.Wang 0626, 2010
       ----------------------------------*/
      void TbSetActorCommand::Undo()
      {
         tbCurScene->SetCurrentActor(aID);

         // paste the actor name to combo
         FnActor actor;
         actor.Object(tbCurScene->GetCurrentActor());
         TbUpdateCurrentBodyDataUI(&actor);
      }


// save actor command
class TbSaveActorCommand : public FcCommandBase
{
public:
   virtual void Init() { SetCommandName("SaveActor"); };    // initialize the command
   virtual void DeInit() { };                               // deinitialize the command
   virtual BOOL Do(char *arg);                              // the command
   virtual void Undo() { };                                 // undo
};


      /*------------------------
        save actor command - Do
        C.Wang 0624, 2010
       -------------------------*/
      BOOL TbSaveActorCommand::Do(char *arg)
      {
         // open the file browser for saving an actor file
         OPENFILENAME ofn;
         char saveName[256], dir[256], log[256];
         BOOL beOK = FALSE;

         if (arg != NULL && strlen(arg) > 0) {
            strcpy(saveName, arg);
            beOK = TRUE;
         }
         else {
            // get current directory
            GetCurrentDirectory(256, dir);

            // prepare to open the file browser
            saveName[0] = '\0';
            memset(&ofn, 0, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = NULL;
            ofn.lpstrFilter = "TheFly Actor CWA {*.cwa}\0*.cwa\0"\
                              "All files {*.*}\0*.*\0\0";
            ofn.lpstrFile = saveName;
            ofn.nMaxFile = 256;
            ofn.Flags = OFN_FILEMUSTEXIST;
            ofn.lpstrDefExt = "cwa";
            ofn.lpstrInitialDir = dir;

            // get the actor file name
            if (GetSaveFileName(&ofn)) {
               // set the extra directory for data loading
               char name[256];
               GetCurrentDirectory(256, name);
               tbWorld.SetExtraDataPath(name);

               // success to get the file name
               beOK = TRUE;
            }

            // restore the current directory
            if (tbBeRestoreCurrentDir) {
               SetCurrentDirectory(dir);
            }
         }

         if (beOK) {
            // set waiting cursor
            SetCursor(tbWaitCursor);

            ACTORid aID = tbCurScene->GetCurrentActor();
            if (aID != FAILED_ID) {
               FnActor actor;
               actor.Object(aID);

               // check file format
               actor.SaveActor(saveName);

               sprintf(log, "The %s actor is saved.", actor.GetName());
               TbSetLogMessage(log);

               // set standard cursor
               SetCursor(tbArrowCursor);
            }
            else {
               strcpy(log, "Nothing to save");
               TbSetLogMessage(log);
            }
         }
         else {
            strcpy(log, "Nothing to save");
            TbSetLogMessage(log);
         }

         return beOK;
      }


// set current body command
class TbSetBodyCommand : public FcCommandBase
{
protected:
   BOOL beCurrentActor;
   char bodyName[256];

public:
   virtual void Init() { bodyName[0] = '\0';
                         beCurrentActor = TRUE;
                         SetCommandName("SetBody"); };        // initialize the command
   virtual void DeInit() { };                                 // deinitialize the command
   virtual BOOL Do(char *arg);                                // the command
   virtual void Undo();                                       // undo
};


      /*------------------------------
        set current body command - Do
        C.Wang 0628, 2010
       -------------------------------*/
      BOOL TbSetBodyCommand::Do(char *arg)
      {
         // check if there's no current actor
         if (tbCurScene->GetCurrentActor() == FAILED_ID) {
            beCurrentActor = FALSE;
            return TRUE;
         }
         beCurrentActor = TRUE;

         // backup current body
         ACTORid aID = tbCurScene->GetCurrentActor();
         FnActor actor;
         actor.Object(aID);

         if (actor.GetCurrentBodyName() != NULL) {
            strcpy(bodyName, actor.GetCurrentBodyName());
         }
         else {
            beCurrentActor = FALSE;
            return TRUE;
         }

         actor.MakeCurrentBody(arg);

         actor.Object(aID);
         TbUpdateCurrentBodyDataUI(&actor);
         return TRUE;
      }


      /*--------------------------------
        set current body command - UnDo
        C.Wang 0628, 2010
       ---------------------------------*/
      void TbSetBodyCommand::Undo()
      {
         if (!beCurrentActor) return;

         FnActor actor;
         ACTORid aID = tbCurScene->GetCurrentActor();
         actor.Object(aID);
         TbUpdateCurrentBodyDataUI(&actor);
      }


// select current action for current body command
class TbSelectActionCommand : public FcCommandBase
{
protected:
   BOOL beCurrentActor;
   char bodyName[256];
   ACTIONid actionID;

public:
   virtual void Init() { actionID = FAILED_ID;
                         beCurrentActor = TRUE;
                         bodyName[0] = '\0';
                         SetCommandName("SelectAction"); };   // initialize the command
   virtual void DeInit() { };                                 // deinitialize the command
   virtual BOOL Do(char *arg);                                // the command
   virtual void Undo();                                       // undo
};


      /*--------------------------------------------
        select action for current body command - Do
        C.Wang 0629, 2010
       ---------------------------------------------*/
      BOOL TbSelectActionCommand::Do(char *arg)
      {
         // check if there's no current actor
         if (tbCurScene->GetCurrentActor() == FAILED_ID) {
            beCurrentActor = FALSE;
            return TRUE;
         }
         beCurrentActor = TRUE;

         // backup current action
         ACTORid aID = tbCurScene->GetCurrentActor();
         FnActor actor;
         actor.Object(aID);

         if (actor.GetCurrentBodyName() != NULL) {
            strcpy(bodyName, actor.GetCurrentBodyName());
         }
         else {
            beCurrentActor = FALSE;
            return TRUE;
         }
         actionID = actor.GetCurrentAction(0, bodyName);

         // make current action
         ACTIONid animID = actor.GetBodyAction(bodyName, arg);
         actor.MakeCurrentAction(0, bodyName, animID);
         actor.Play(0, START, 0.0f, tbBeBase, TRUE);

         FnUiCombo uiC;
         uiC.Object(tbSelectActionComboID);
         uiC.PasteComboValue(arg);

         FnUiButton uiB;
         uiB.Object(tbEditActionButtonID);
         uiB.Enable(TRUE);
         uiB.Object(tbDeleteActionButtonID);
         uiB.Enable(TRUE);
         uiB.Object(tbAddActionButtonID);
         uiB.Enable(TRUE);

         return TRUE;
      }


      /*----------------------------------------------
        select action for current body command - Undo
        C.Wang 0629, 2010
       -----------------------------------------------*/
      void TbSelectActionCommand::Undo()
      {
         if (!beCurrentActor) return;

         FnActor actor;
         ACTORid aID = tbCurScene->GetCurrentActor();
         actor.Object(aID);

         // recover current action
         actor.MakeCurrentAction(0, actor.GetCurrentBodyName(), actionID);
         actor.Play(0, START, 0.0f, tbBeBase, TRUE);
         FnAction action;
         action.Object(actionID);
         actionID = FAILED_ID;

         // update UI
         FnUiCombo uiC;
         uiC.Object(tbSelectActionComboID);
         uiC.PasteComboValue(action.GetName());

         FnUiButton uiB;
         uiB.Object(tbEditActionButtonID);
         uiB.Enable(TRUE);
         uiB.Object(tbDeleteActionButtonID);
         uiB.Enable(TRUE);
         uiB.Object(tbAddActionButtonID);
         uiB.Enable(TRUE);
      }


// rename current action command
class TbRenameActionCommand : public FcCommandBase
{
protected:
   ACTORid actorID;
   char bodyName[256];
   ACTIONid actionID;
   char name[256];

public:
   virtual void Init() { name[0] = '\0'; 
                         actorID = FAILED_ID;
                         actionID = FAILED_ID;
                         bodyName[0] = '\0';
                         SetCommandName("RenameAction"); };    // initialize the command
   virtual void DeInit() { };                                    // deinitialize the command
   virtual BOOL Do(char *arg);                                   // the command
   virtual void Undo();                                          // undo
};


      /*-----------------------------------
        rename current action command - Do
        C.Wang 0701, 2010
       ------------------------------------*/
      BOOL TbRenameActionCommand::Do(char *arg)
      {
         // backup the data
         FnActor actor;
         actorID = tbCurScene->GetCurrentActor();
         actor.Object(actorID);
         if (actorID == FAILED_ID) {
            return TRUE;
         }
         if (actor.GetCurrentBodyName() == NULL) {
            actorID = FAILED_ID;
            return TRUE;
         }
         strcpy(bodyName, actor.GetCurrentBodyName());
         actionID = actor.GetCurrentAction(0, bodyName);

         FnAction action;
         action.Object(actionID);
         strcpy(name, action.GetName());
         action.SetName(arg);

         // update the UI
         int i, iCount, num;
         ACTIONid aTable[1024];
         FnUiCombo uiC;
         uiC.Object(tbSelectActionComboID);
         uiC.PasteComboValue(arg);
         uiC.ClearMessages();
         num = actor.GetBodyAllActions(bodyName, aTable, 1024);
         iCount = 0;
         for (i = 0;  i < num; i++) {
            if (aTable[i] != FAILED_ID) {
               action.Object(aTable[i]);
               uiC.InsertMessage(iCount, action.GetName());
               iCount++;
            }
         }

         return TRUE;
      }


      /*-------------------------------------
        rename current action command - UnDo
        C.Wang 0701, 2010
       --------------------------------------*/
      void TbRenameActionCommand::Undo()
      {
         if (actorID == FAILED_ID || actionID == FAILED_ID) return;
         FnAction action;
         action.Object(actionID);
         action.SetName(name);

         // update the UI
         FnActor actor;
         int i, iCount, num;
         ACTIONid aTable[1024];
         FnUiCombo uiC;

         actor.Object(actorID);
         uiC.Object(tbSelectActionComboID);
         uiC.PasteComboValue(name);
         uiC.ClearMessages();
         num = actor.GetBodyAllActions(bodyName, aTable, 1024);
         iCount = 0;
         for (i = 0;  i < num; i++) {
            if (aTable[i] != FAILED_ID) {
               action.Object(aTable[i]);
               uiC.InsertMessage(iCount, action.GetName());
               iCount++;
            }
         }
      }


/*---------------------------------------
  paste the current material data to UIs
  C.Wang 0717, 2010
 ----------------------------------------*/
void TbPasteCurrentMaterialData(FnMaterial *mat)
{
   FnUiTypein uiTy;
   FnUiCombo uiC;
   FnUiButton uiB;
   char string[256];

   if (mat->Object() == FAILED_MATERIAL_ID) {
      uiTy.Object(tbAmbientRID);
      uiTy.Enable(FALSE);
      uiTy.Object(tbAmbientGID);
      uiTy.Enable(FALSE);
      uiTy.Object(tbAmbientBID);
      uiTy.Enable(FALSE);
      uiTy.Object(tbDiffuseRID);
      uiTy.Enable(FALSE);
      uiTy.Object(tbDiffuseGID);
      uiTy.Enable(FALSE);
      uiTy.Object(tbDiffuseBID);
      uiTy.Enable(FALSE);
      uiTy.Object(tbSpecularRID);
      uiTy.Enable(FALSE);
      uiTy.Object(tbSpecularGID);
      uiTy.Enable(FALSE);
      uiTy.Object(tbSpecularBID);
      uiTy.Enable(FALSE);
      uiTy.Object(tbEmissiveRID);
      uiTy.Enable(FALSE);
      uiTy.Object(tbEmissiveGID);
      uiTy.Enable(FALSE);
      uiTy.Object(tbEmissiveBID);
      uiTy.Enable(FALSE);
      uiTy.Object(tbShinenessID);
      uiTy.Enable(FALSE);
      uiTy.Object(tbShaderFileID);
      uiTy.Enable(FALSE);
      uiC.Object(tbEffectID);
      uiC.Enable(FALSE);
      uiTy.Object(tbTexture0ID);
      uiTy.Enable(FALSE);
      uiTy.Object(tbTexture1ID);
      uiTy.Enable(FALSE);
      uiTy.Object(tbTexture2ID);
      uiTy.Enable(FALSE);
      uiTy.Object(tbTexture3ID);
      uiTy.Enable(FALSE);
      uiB.Object(tbShaderLoaderID);
      uiB.Enable(FALSE);
      uiB.Object(tbShaderRemoveID);
      uiB.Enable(FALSE);
      uiB.Object(tbTexture0AddID);
      uiB.Enable(FALSE);
      uiB.Object(tbTexture0RemoveID);
      uiB.Enable(FALSE);
      uiB.Object(tbTexture1AddID);
      uiB.Enable(FALSE);
      uiB.Object(tbTexture1RemoveID);
      uiB.Enable(FALSE);
      uiB.Object(tbTexture2AddID);
      uiB.Enable(FALSE);
      uiB.Object(tbTexture2RemoveID);
      uiB.Enable(FALSE);
      uiB.Object(tbTexture3AddID);
      uiB.Enable(FALSE);
      uiB.Object(tbTexture3RemoveID);
      uiB.Enable(FALSE);
      return;
   }
   else {
      uiTy.Object(tbAmbientRID);
      uiTy.Enable(TRUE);
      uiTy.Object(tbAmbientGID);
      uiTy.Enable(TRUE);
      uiTy.Object(tbAmbientBID);
      uiTy.Enable(TRUE);
      uiTy.Object(tbDiffuseRID);
      uiTy.Enable(TRUE);
      uiTy.Object(tbDiffuseGID);
      uiTy.Enable(TRUE);
      uiTy.Object(tbDiffuseBID);
      uiTy.Enable(TRUE);
      uiTy.Object(tbSpecularRID);
      uiTy.Enable(TRUE);
      uiTy.Object(tbSpecularGID);
      uiTy.Enable(TRUE);
      uiTy.Object(tbSpecularBID);
      uiTy.Enable(TRUE);
      uiTy.Object(tbEmissiveRID);
      uiTy.Enable(TRUE);
      uiTy.Object(tbEmissiveGID);
      uiTy.Enable(TRUE);
      uiTy.Object(tbEmissiveBID);
      uiTy.Enable(TRUE);
      uiTy.Object(tbShinenessID);
      uiTy.Enable(TRUE);
      uiTy.Object(tbShaderFileID);
      uiTy.Enable(TRUE);
      uiC.Object(tbEffectID);
      uiC.Enable(TRUE);
      uiTy.Object(tbTexture0ID);
      uiTy.Enable(TRUE);
      uiTy.Object(tbTexture1ID);
      uiTy.Enable(TRUE);
      uiTy.Object(tbTexture2ID);
      uiTy.Enable(TRUE);
      uiTy.Object(tbTexture3ID);
      uiTy.Enable(TRUE);
      uiB.Object(tbShaderLoaderID);
      uiB.Enable(TRUE);
      uiB.Object(tbShaderRemoveID);
      uiB.Enable(TRUE);
      uiB.Object(tbTexture0AddID);
      uiB.Enable(TRUE);
      uiB.Object(tbTexture0RemoveID);
      uiB.Enable(TRUE);
      uiB.Object(tbTexture1AddID);
      uiB.Enable(TRUE);
      uiB.Object(tbTexture1RemoveID);
      uiB.Enable(TRUE);
      uiB.Object(tbTexture2AddID);
      uiB.Enable(TRUE);
      uiB.Object(tbTexture2RemoveID);
      uiB.Enable(TRUE);
      uiB.Object(tbTexture3AddID);
      uiB.Enable(TRUE);
      uiB.Object(tbTexture3RemoveID);
      uiB.Enable(TRUE);
   }

   float *data = mat->GetAmbient();
   if (data != NULL) {
      tbMatBuffer.ambient[0] = data[0];
      tbMatBuffer.ambient[1] = data[1];
      tbMatBuffer.ambient[2] = data[2];
   }
   else {
      tbMatBuffer.ambient[0] = tbMatBuffer.ambient[1] = tbMatBuffer.ambient[2] = 0.3f;
   }
   uiTy.Object(tbAmbientRID);
   sprintf(string, "%5.4f", tbMatBuffer.ambient[0]);
   uiTy.PasteTypeinValue(string);
   uiTy.Object(tbAmbientGID);
   sprintf(string, "%5.4f", tbMatBuffer.ambient[1]);
   uiTy.PasteTypeinValue(string);
   uiTy.Object(tbAmbientBID);
   sprintf(string, "%5.4f", tbMatBuffer.ambient[2]);
   uiTy.PasteTypeinValue(string);

   data = mat->GetDiffuse();
   if (data != NULL) {
      tbMatBuffer.diffuse[0] = data[0];
      tbMatBuffer.diffuse[1] = data[1];
      tbMatBuffer.diffuse[2] = data[2];
   }
   else {
      tbMatBuffer.diffuse[0] = tbMatBuffer.diffuse[1] = tbMatBuffer.diffuse[2] = 0.7f;
   }
   uiTy.Object(tbDiffuseRID);
   sprintf(string, "%5.4f", tbMatBuffer.diffuse[0]);
   uiTy.PasteTypeinValue(string);
   uiTy.Object(tbDiffuseGID);
   sprintf(string, "%5.4f", tbMatBuffer.diffuse[1]);
   uiTy.PasteTypeinValue(string);
   uiTy.Object(tbDiffuseBID);
   sprintf(string, "%5.4f", tbMatBuffer.diffuse[2]);
   uiTy.PasteTypeinValue(string);

   data = mat->GetSpecular();
   if (data != NULL) {
      tbMatBuffer.specular[0] = data[0];
      tbMatBuffer.specular[1] = data[1];
      tbMatBuffer.specular[2] = data[2];
   }
   else {
      tbMatBuffer.specular[0] = tbMatBuffer.specular[1] = tbMatBuffer.specular[2] = 0.2f;
   }
   uiTy.Object(tbSpecularRID);
   sprintf(string, "%5.4f", tbMatBuffer.specular[0]);
   uiTy.PasteTypeinValue(string);
   uiTy.Object(tbSpecularGID);
   sprintf(string, "%5.4f", tbMatBuffer.specular[1]);
   uiTy.PasteTypeinValue(string);
   uiTy.Object(tbSpecularBID);
   sprintf(string, "%5.4f", tbMatBuffer.specular[2]);
   uiTy.PasteTypeinValue(string);

   data = mat->GetEmissive();
   if (data != NULL) {
      tbMatBuffer.emissive[0] = data[0];
      tbMatBuffer.emissive[1] = data[1];
      tbMatBuffer.emissive[2] = data[2];
   }
   else {
      tbMatBuffer.emissive[0] = tbMatBuffer.emissive[1] = tbMatBuffer.emissive[2] = 0.0f;
   }
   uiTy.Object(tbEmissiveRID);
   sprintf(string, "%5.4f", tbMatBuffer.emissive[0]);
   uiTy.PasteTypeinValue(string);
   uiTy.Object(tbEmissiveGID);
   sprintf(string, "%5.4f", tbMatBuffer.emissive[1]);
   uiTy.PasteTypeinValue(string);
   uiTy.Object(tbEmissiveBID);
   sprintf(string, "%5.4f", tbMatBuffer.emissive[2]);
   uiTy.PasteTypeinValue(string);

   float d = mat->GetShineness();
   tbMatBuffer.power = d;
   uiTy.Object(tbShinenessID);
   sprintf(string, "%5.4f", d);
   uiTy.PasteTypeinValue(string);

   char name[256];
   char *sf = mat->GetShaderSourceName();
   char *ef = mat->GetShaderEffectName();
   uiTy.Object(tbShaderFileID);
   uiC.Object(tbEffectID);
   if (sf != NULL && strlen(sf) > 0) {
      uiTy.PasteTypeinValue(sf);
      uiC.PasteComboValue(ef);

      uiC.ClearMessages();
      int i, numShader = FyCheckShaderNumberInFile(tbWorldID, sf);
      if (numShader > 0) {
         for (i = 0; i < numShader; i++) {
            FyGetShaderName(tbWorldID, sf, i, name);
            uiC.InsertMessage(i, name);
         }
      }
   }
   else {
      uiTy.PasteTypeinValue("-");
      uiC.PasteComboValue("-");
   }

   if (sf != NULL) {
      strcpy(tbMatBuffer.shaderFile, sf);
   }
   else {
      strcpy(tbMatBuffer.shaderFile, "-");
   }

   if (ef != NULL) {
      strcpy(tbMatBuffer.effectName, ef);
   }
   else {
      strcpy(tbMatBuffer.effectName, "-");
   }

   char *value = mat->GetShaderPath();
   if (value != NULL) {
      strcpy(tbMatBuffer.shaderPath, value);
   }
   else {
      strcpy(tbMatBuffer.shaderPath, ".");
   }

   value = mat->GetTexturePath(0, 0);
   if (value != NULL) {
      strcpy(tbMatBuffer.texturePath, value);
   }
   else {
      strcpy(tbMatBuffer.texturePath, ".");
   }

   value = mat->GetTextureName(0, 0);
   if (value != NULL) {
      strcpy(tbMatBuffer.texture0, mat->GetTextureName(0, 0));
   }
   else {
      strcpy(tbMatBuffer.texture0, "-");
   }
   uiTy.Object(tbTexture0ID);
   uiTy.PasteTypeinValue(tbMatBuffer.texture0);

   value = mat->GetTextureName(0, 1);
   if (value != NULL) {
      strcpy(tbMatBuffer.texture1, mat->GetTextureName(0, 1));
   }
   else {
      strcpy(tbMatBuffer.texture1, "-");
   }
   uiTy.Object(tbTexture1ID);
   uiTy.PasteTypeinValue(tbMatBuffer.texture1);

   value = mat->GetTextureName(0, 2);
   if (value != NULL) {
      strcpy(tbMatBuffer.texture2, mat->GetTextureName(0, 2));
   }
   else {
      strcpy(tbMatBuffer.texture2, "-");
   }
   uiTy.Object(tbTexture2ID);
   uiTy.PasteTypeinValue(tbMatBuffer.texture2);

   value = mat->GetTextureName(0, 3);
   if (value != NULL) {
      strcpy(tbMatBuffer.texture3, mat->GetTextureName(0, 3));
   }
   else {
      strcpy(tbMatBuffer.texture3, "-");
   }
   uiTy.Object(tbTexture3ID);
   uiTy.PasteTypeinValue(tbMatBuffer.texture3);
}


/*-------------------------
  update current body data
  C.Wang 0629, 2010
 --------------------------*/
void TbUpdateCurrentBodyDataUI(FnActor *actor)
{
   FnUiCombo uiC, uiC2;
   FnUiTypein uiTy;
   FnUiRadioButton uiRB;
   FnUiButton uiB;
   int i, iCount;

   // find and paste the current body
   uiC.Object(tbSelectBodyComboID);
   uiC.ClearMessages();
   if (actor->Object() != FAILED_ID) {
      char *name = actor->GetCurrentBodyName();
      if (name == NULL) {
         name = actor->GetRootBody();
         actor->MakeCurrentBody(name);
      }
      uiC.PasteComboValue(name);
      uiC.Enable(TRUE);

      // update action data for current body
      uiC2.Object(tbSelectActionComboID);
      uiC2.ClearMessages();
      int numA = actor->GetBodyActionNumber(name);
      if (numA > 0) {
         int numAction;
         ACTIONid actionID[1024], curActionID;
         FnAction action;
         numAction = actor->GetBodyAllActions(name, actionID, 1024);
         iCount = 0;
         for (i = 0; i < numAction; i++) {
            action.Object(actionID[i]);
            if (action.GetName()) {
               uiC2.InsertMessage(iCount, action.GetName());
               iCount++;
            }
         }
         uiC2.Enable(TRUE);

         // check if there are current action ?
         curActionID = actor->GetCurrentAction(0, name);
         if (curActionID == FAILED_ID) {
            action.Object(actionID[0]);
            uiC2.PasteComboValue(action.GetName());
            actor->MakeCurrentAction(0, name, actionID[0]);
            actor->Play(0, START, 0.0f, tbBeBase, TRUE);
         }
         else {
            action.Object(curActionID);
            uiC2.PasteComboValue(action.GetName());
            actor->MakeCurrentAction(0, name, curActionID);
            actor->Play(0, START, 0.0f, tbBeBase, TRUE);
         }
         uiB.Object(tbEditActionButtonID);
         uiB.Enable(TRUE);
         uiB.Object(tbDeleteActionButtonID);
         uiB.Enable(TRUE);
         uiB.Object(tbAddActionButtonID);
         uiB.Enable(TRUE);
      }
      else {
         uiC2.Enable(FALSE);
         uiC2.PasteComboValue("");
         uiB.Object(tbEditActionButtonID);
         uiB.Enable(FALSE);
         uiB.Object(tbDeleteActionButtonID);
         uiB.Enable(FALSE);
         uiB.Object(tbAddActionButtonID);
         uiB.Enable(TRUE);
      }

      // loop to find all bodies
      BOOL beOK;
      int numB = actor->GetBodyNumber();
      int startFrame, endFrame;
      char poseName[256], string[256];
      iCount = 0;
      for (i = 0; i < numB; i++) {
         if (actor->GetBodyName(i) != NULL) {
            uiC.InsertMessage(iCount, actor->GetBodyName(i));
            iCount++;
         }
      }

      // Update pose UIs
      numB = actor->GetPoseNumber(name);
      uiC.Object(tbSelectPoseComboID);
      uiC.ClearMessages();
      if (numB > 0) {
         iCount = 0;
         for (i = 0; i < numB; i++) {
            beOK = actor->QueryPose(name, i, poseName, &startFrame, &endFrame);
            if (beOK) {
               uiC.InsertMessage(iCount, poseName);
               if (iCount == 0) {
                  uiC.PasteComboValue(poseName);
                  uiC.Enable(TRUE);

                  uiTy.Object(tbPoseStartID);
                  sprintf(string, "%d", startFrame);
                  uiTy.PasteTypeinValue(string);
                  //uiTy.Enable(TRUE);
                  tbTestPoseStart = startFrame;
                  tbTestPoseFrame = (float) startFrame;

                  uiTy.Object(tbPoseEndID);
                  sprintf(string, "%d", endFrame);
                  uiTy.PasteTypeinValue(string);
                  //uiTy.Enable(TRUE);
                  tbTestPoseEnd = endFrame;
               }
               iCount++;
            }
         }
         tbBePlayPose = FALSE;
         uiRB.Object(tbPlayPoseButtonID);
         uiRB.ChangeState(tbBePlayPose);
         uiRB.Enable(TRUE);
         uiB.Object(tbMakeBaseMoveButtonID);
         uiB.Enable(TRUE);
      }
      else {
         uiC.Object(tbSelectPoseComboID);
         uiC.ClearMessages();
         uiC.PasteComboValue("");
         uiC.Enable(FALSE);
         uiTy.Object(tbPoseStartID);
         uiTy.PasteTypeinValue("");
         uiTy.Enable(FALSE);
         uiTy.Object(tbPoseEndID);
         uiTy.PasteTypeinValue("");
         uiTy.Enable(FALSE);
         uiRB.Object(tbPlayPoseButtonID);
         tbBePlayPose = FALSE;
         uiRB.ChangeState(tbBePlayPose);
         uiRB.Enable(FALSE);
         uiB.Object(tbMakeBaseMoveButtonID);
         uiB.Enable(FALSE);
         tbTestPoseStart = tbTestPoseEnd = NONE;
         tbTestPoseFrame = (float) tbTestPoseStart;
      }
      //uiB.Object(tbNewPoseButtonID);
      //uiB.Enable(TRUE);
   }
   else {
      uiC.Enable(FALSE);
      uiC.PasteComboValue("");

      // update pose UIs
      uiC.Object(tbSelectPoseComboID);
      uiC.ClearMessages();
      uiC.PasteComboValue("");
      uiC.Enable(FALSE);
      uiTy.Object(tbPoseStartID);
      uiTy.PasteTypeinValue("");
      uiTy.Enable(FALSE);
      uiTy.Object(tbPoseEndID);
      uiTy.PasteTypeinValue("");
      uiTy.Enable(FALSE);
      uiRB.Object(tbPlayPoseButtonID);
      tbBePlayPose = FALSE;
      uiRB.ChangeState(tbBePlayPose);
      uiRB.Enable(FALSE);
      uiB.Object(tbNewPoseButtonID);
      uiB.Enable(FALSE);
      tbTestPoseStart = tbTestPoseEnd = NONE;
      tbTestPoseFrame = (float) tbTestPoseStart;
      uiC2.Object(tbSelectActionComboID);
      uiC2.ClearMessages();
      uiC2.PasteComboValue("");
      uiC2.Enable(FALSE);
      uiB.Object(tbDeleteActionButtonID);
      uiB.Enable(FALSE);
      uiB.Object(tbEditActionButtonID);
      uiB.Enable(FALSE);
      uiB.Object(tbAddActionButtonID);
      uiB.Enable(FALSE);
      uiB.Object(tbMakeBaseMoveButtonID);
      uiB.Enable(FALSE);
   }
}


/*-----------------------------------------
  extract the UIs' data to material buffer
  C.Wang 0618, 2010
 ------------------------------------------*/
void TbExtractMaterialUIData()
{
   FnUiTypein uiTy;
   FnUiCombo uiC;
   FnUiButton uiB;
   char value[256];

   uiTy.Object(tbAmbientRID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.ambient[0] = (float) atof(value);
   if (tbMatBuffer.ambient[0] < 0.0f) tbMatBuffer.ambient[0] = 0.0f;

   uiTy.Object(tbAmbientGID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.ambient[1] = (float) atof(value);
   if (tbMatBuffer.ambient[1] < 0.0f) tbMatBuffer.ambient[1] = 0.0f;

   uiTy.Object(tbAmbientBID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.ambient[2] = (float) atof(value);
   if (tbMatBuffer.ambient[2] < 0.0f) tbMatBuffer.ambient[2] = 0.0f;

   uiTy.Object(tbDiffuseRID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.diffuse[0] = (float) atof(value);
   if (tbMatBuffer.diffuse[0] < 0.0f) tbMatBuffer.diffuse[0] = 0.0f;

   uiTy.Object(tbDiffuseGID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.diffuse[1] = (float) atof(value);
   if (tbMatBuffer.diffuse[1] < 0.0f) tbMatBuffer.diffuse[1] = 0.0f;

   uiTy.Object(tbDiffuseBID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.diffuse[2] = (float) atof(value);
   if (tbMatBuffer.diffuse[2] < 0.0f) tbMatBuffer.diffuse[2] = 0.0f;

   uiTy.Object(tbSpecularRID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.specular[0] = (float) atof(value);
   if (tbMatBuffer.specular[0] < 0.0f) tbMatBuffer.specular[0] = 0.0f;

   uiTy.Object(tbSpecularGID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.specular[1] = (float) atof(value);
   if (tbMatBuffer.specular[1] < 0.0f) tbMatBuffer.specular[1] = 0.0f;

   uiTy.Object(tbSpecularBID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.specular[2] = (float) atof(value);
   if (tbMatBuffer.specular[2] < 0.0f) tbMatBuffer.specular[2] = 0.0f;

   uiTy.Object(tbEmissiveRID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.emissive[0] = (float) atof(value);
   if (tbMatBuffer.emissive[0] < 0.0f) tbMatBuffer.emissive[0] = 0.0f;

   uiTy.Object(tbEmissiveGID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.emissive[1] = (float) atof(value);
   if (tbMatBuffer.emissive[1] < 0.0f) tbMatBuffer.emissive[1] = 0.0f;

   uiTy.Object(tbEmissiveBID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.emissive[2] = (float) atof(value);
   if (tbMatBuffer.emissive[2] < 0.0f) tbMatBuffer.emissive[2] = 0.0f;

   uiTy.Object(tbShinenessID);
   uiTy.GetTypeinValue(value);
   tbMatBuffer.power = (float) atof(value);
   if (tbMatBuffer.power < 0.0f) tbMatBuffer.power = 0.0f;

   uiTy.Object(tbShaderFileID);
   uiTy.GetTypeinValue(value);
   strcpy(tbMatBuffer.shaderFile, value);

   uiC.Object(tbEffectID);
   uiC.GetComboValue(value);
   strcpy(tbMatBuffer.effectName, value);

   strcpy(tbMatBuffer.shaderPath, tbTmpShaderPath);
   strcpy(tbMatBuffer.texturePath, tbTmpTexturePath);

   uiTy.Object(tbTexture0ID);
   uiTy.GetTypeinValue(value);
   strcpy(tbMatBuffer.texture0, value);

   uiTy.Object(tbTexture1ID);
   uiTy.GetTypeinValue(value);
   strcpy(tbMatBuffer.texture1, value);

   uiTy.Object(tbTexture2ID);
   uiTy.GetTypeinValue(value);
   strcpy(tbMatBuffer.texture2, value);

   uiTy.Object(tbTexture3ID);
   uiTy.GetTypeinValue(value);
   strcpy(tbMatBuffer.texture3, value);

   uiB.Object(tbUpdateMaterialID);
   uiB.Enable(FALSE);
}


/*---------------------------------------------
  update current material with material buffer
  C.Wang 0618, 2010
 ----------------------------------------------*/
void TbUpdateCurrentMaterial(FnMaterial *mat)
{
   // update ambient
   mat->SetAmbient(tbMatBuffer.ambient);

   // update diffuse
   mat->SetDiffuse(tbMatBuffer.diffuse);

   // update specular
   mat->SetSpecular(tbMatBuffer.specular);

   // update emissive
   mat->SetEmissive(tbMatBuffer.emissive);

   // update shineness
   mat->SetShineness(tbMatBuffer.power);

   // update shader
   if (FyCheckWord(tbMatBuffer.shaderFile, "-")) {
      mat->ReplaceShader(FAILED_ID);
   }
   else {
      // add the new shader
      char fileName[256];
      sprintf(fileName, "%s\\%s", tbMatBuffer.shaderPath, tbMatBuffer.shaderFile);

      mat->AddShaderEffect(fileName, tbMatBuffer.effectName);
   }

   // update textures
   if (FyCheckWord(tbMatBuffer.texture0, "-")) {
      mat->RemoveTexture(0, 0);
   }
   else {
      char fileName[256];
      sprintf(fileName, "%s\\%s", tbMatBuffer.texturePath, tbMatBuffer.texture0);
      mat->AddTexture(0, 0, fileName);
   }

   if (FyCheckWord(tbMatBuffer.texture1, "-")) {
      mat->RemoveTexture(0, 1);
   }
   else {
      char fileName[256];
      sprintf(fileName, "%s\\%s", tbMatBuffer.texturePath, tbMatBuffer.texture1);
      mat->AddTexture(0, 1, fileName);
   }

   if (FyCheckWord(tbMatBuffer.texture2, "-")) {
      mat->RemoveTexture(0, 2);
   }
   else {
      char fileName[256];
      sprintf(fileName, "%s\\%s", tbMatBuffer.texturePath, tbMatBuffer.texture2);
      mat->AddTexture(0, 2, fileName);
   }

   if (FyCheckWord(tbMatBuffer.texture3, "-")) {
      mat->RemoveTexture(0, 3);
   }
   else {
      char fileName[256];
      sprintf(fileName, "%s\\%s", tbMatBuffer.texturePath, tbMatBuffer.texture3);
      mat->AddTexture(0, 3, fileName);
   }
}


/*-----------------------------------------
  cancel the current material data editing
  C.Wang 0618, 2010
 ------------------------------------------*/
void TbCancelMaterialEditing()
{
   FnUiTypein uiTy;
   FnUiCombo uiC;
   FnUiButton uiB;
   char string[256];

   float *data = tbMatBuffer.ambient;
   uiTy.Object(tbAmbientRID);
   sprintf(string, "%5.4f", data[0]);
   uiTy.PasteTypeinValue(string);
   uiTy.Object(tbAmbientGID);
   sprintf(string, "%5.4f", data[1]);
   uiTy.PasteTypeinValue(string);
   uiTy.Object(tbAmbientBID);
   sprintf(string, "%5.4f", data[2]);
   uiTy.PasteTypeinValue(string);

   data = tbMatBuffer.diffuse;
   uiTy.Object(tbDiffuseRID);
   sprintf(string, "%5.4f", data[0]);
   uiTy.PasteTypeinValue(string);
   uiTy.Object(tbDiffuseGID);
   sprintf(string, "%5.4f", data[1]);
   uiTy.PasteTypeinValue(string);
   uiTy.Object(tbDiffuseBID);
   sprintf(string, "%5.4f", data[2]);
   uiTy.PasteTypeinValue(string);

   data = tbMatBuffer.specular;
   uiTy.Object(tbSpecularRID);
   sprintf(string, "%5.4f", data[0]);
   uiTy.PasteTypeinValue(string);
   uiTy.Object(tbSpecularGID);
   sprintf(string, "%5.4f", data[1]);
   uiTy.PasteTypeinValue(string);
   uiTy.Object(tbSpecularBID);
   sprintf(string, "%5.4f", data[2]);
   uiTy.PasteTypeinValue(string);

   data = tbMatBuffer.emissive;
   uiTy.Object(tbEmissiveRID);
   sprintf(string, "%5.4f", data[0]);
   uiTy.PasteTypeinValue(string);
   uiTy.Object(tbEmissiveGID);
   sprintf(string, "%5.4f", data[1]);
   uiTy.PasteTypeinValue(string);
   uiTy.Object(tbEmissiveBID);
   sprintf(string, "%5.4f", data[2]);
   uiTy.PasteTypeinValue(string);

   uiTy.Object(tbShinenessID);
   sprintf(string, "%5.4f", tbMatBuffer.power);
   uiTy.PasteTypeinValue(string);

   uiTy.Object(tbShaderFileID);
   uiC.Object(tbEffectID);
   uiTy.PasteTypeinValue(tbMatBuffer.shaderFile);
   uiC.PasteComboValue(tbMatBuffer.effectName);

   uiTy.Object(tbTexture0ID);
   uiTy.PasteTypeinValue(tbMatBuffer.texture0);
   uiTy.Object(tbTexture1ID);
   uiTy.PasteTypeinValue(tbMatBuffer.texture1);
   uiTy.Object(tbTexture2ID);
   uiTy.PasteTypeinValue(tbMatBuffer.texture2);
   uiTy.Object(tbTexture3ID);
   uiTy.PasteTypeinValue(tbMatBuffer.texture3);
   
   uiB.Object(tbUpdateMaterialID);
   uiB.Enable(FALSE);
}


/*------------------------------------------
  clear shader data at material editing UIs
  C.Wang 0617, 2010
 -------------------------------------------*/
void TbClearShaderData(UIid uiID, BOOL bePress)
{
   if (!bePress) {
      FnUiTypein uiTy;
      FnUiCombo uiC;
      FnUiButton uiB;

      uiTy.Object(tbShaderFileID);
      uiC.Object(tbEffectID);

      uiTy.PasteTypeinValue("-");
      uiC.PasteComboValue("-");
      uiC.ClearMessages();

      uiB.Object(tbUpdateMaterialID);
      uiB.Enable(TRUE);
   }
}


/*-------------------------------------------
  clear texture data at material editing UIs
  C.Wang 0618, 2010
 --------------------------------------------*/
void TbClearTextureData(UIid uiID, BOOL bePress)
{
   if (!bePress) {
      FnUiTypein uiTy;
      FnUiButton uiB;

      if (uiID == tbTexture0RemoveID) {
         uiTy.Object(tbTexture0ID);
      }
      else if (uiID == tbTexture1RemoveID) {
         uiTy.Object(tbTexture1ID);
      }
      else if (uiID == tbTexture2RemoveID) {
         uiTy.Object(tbTexture2ID);
      }
      else if (uiID == tbTexture3RemoveID) {
         uiTy.Object(tbTexture3ID);
      }
      uiTy.PasteTypeinValue("-");

      uiB.Object(tbUpdateMaterialID);
      uiB.Enable(TRUE);
   }
}


/*------------------------------------------
  add shader data at material editing UIs
  C.Wang 0621, 2010
 -------------------------------------------*/
void TbAddShaderData(UIid uiID, BOOL bePress)
{
   if (!bePress) {
      // open the file browser to load the shader
      char dir[128], fileName[256];
      OPENFILENAME ofn;
      GetCurrentDirectory(128, dir);
      BOOL beOK = FALSE;

      // prepare to open the file browser
      fileName[0] = '\0';
      memset(&ofn, 0, sizeof(OPENFILENAME));
      ofn.lStructSize = sizeof(OPENFILENAME);
      ofn.hwndOwner = NULL;
      ofn.lpstrFilter = "TheFly CWS Shader Description File {*.cws}\0*.cws\0"\
                        "All files {*.*}\0*.*\0\0";
      ofn.lpstrFile = fileName;
      ofn.nMaxFile = 256;
      ofn.Flags = OFN_FILEMUSTEXIST;
      ofn.lpstrDefExt = "cws";
      ofn.lpstrInitialDir = dir;

      // start to load the model file
      if (GetOpenFileName(&ofn)) {
         // set the extra directory for data loading
         char name[128];
         GetCurrentDirectory(128, name);
         tbWorld.SetExtraDataPath(name);

         // success to get the file name
         beOK = TRUE;
      }

      if (beOK) {
         char path[256], file[256], ext[256], name[256];
         FyCheckFileFormat(fileName, path, file, ext);

         // paste the name to UIs
         FnUiTypein uiTy;
         FnUiCombo uiC;
         FnUiButton uiB;
         uiTy.Object(tbShaderFileID);
         uiTy.PasteTypeinValue(file);

         strcpy(tbTmpShaderPath, path);
         uiC.Object(tbEffectID);
         int i, numShader = FyCheckShaderNumberInFile(tbWorldID, fileName);
         uiC.ClearMessages();
         if (numShader > 0) {
            for (i = 0; i < numShader; i++) {
               FyGetShaderName(tbWorldID, fileName, i, name);
               if (i == 0) {
                  uiC.PasteComboValue(name);
               }
               uiC.InsertMessage(i, name);
            }
         }
         else {
            uiC.PasteComboValue("-");
         }

         uiB.Object(tbUpdateMaterialID);
         uiB.Enable(TRUE);
      }

      // restore the current directory
      if (tbBeRestoreCurrentDir) {
         SetCurrentDirectory(dir);
      }
   }
}


/*-----------------------------------------
  add texture data at material editing UIs
  C.Wang 0618, 2010
 ------------------------------------------*/
void TbAddTextureData(UIid uiID, BOOL bePress)
{
   if (!bePress) {
      // open the file browser to load the shader
      char dir[128], fileName[256];
      OPENFILENAME ofn;
      GetCurrentDirectory(128, dir);
      BOOL beOK = FALSE;

      // prepare to open the file browser
      fileName[0] = '\0';
      memset(&ofn, 0, sizeof(OPENFILENAME));
      ofn.lStructSize = sizeof(OPENFILENAME);
      ofn.hwndOwner = NULL;
      ofn.lpstrFilter = "TheFly DDS texture File {*.dds}\0*.dds\0"\
                        "TheFly JPEG texture File {*.jpg}\0*.jpg\0"\
                        "TheFly Targa texture File {*.tga}\0*.tga\0"\
                        "TheFly bitmap texture File {*.bmp}\0*.bmp\0"\
                        "All files {*.*}\0*.*\0\0";
      ofn.lpstrFile = fileName;
      ofn.nMaxFile = 256;
      ofn.Flags = OFN_FILEMUSTEXIST;
      ofn.lpstrDefExt = "dds";
      ofn.lpstrInitialDir = dir;

      // start to load the model file
      if (GetOpenFileName(&ofn)) {
         // set the extra directory for data loading
         char name[128];
         GetCurrentDirectory(128, name);
         tbWorld.SetExtraDataPath(name);

         // success to get the file name
         beOK = TRUE;
      }

      if (beOK) {
         char path[256], file[256], ext[256];
         FyCheckFileFormat(fileName, path, file, ext);

         // paste the name to UIs
         FnUiTypein uiTy;
         FnUiButton uiB;

         if (uiID == tbTexture0AddID) {
            uiTy.Object(tbTexture0ID);
         }
         else if (uiID == tbTexture1AddID) {
            uiTy.Object(tbTexture1ID);
         }
         else if (uiID == tbTexture2AddID) {
            uiTy.Object(tbTexture2ID);
         }
         else if (uiID == tbTexture3AddID) {
            uiTy.Object(tbTexture3ID);
         }
         uiTy.PasteTypeinValue(file);

         strcpy(tbTmpTexturePath, path);

         uiB.Object(tbUpdateMaterialID);
         uiB.Enable(TRUE);
      }

      // restore the current directory
      if (tbBeRestoreCurrentDir) {
         SetCurrentDirectory(dir);
      }
   }
}


/*------------------
  quit the program
  C.Wang 0513, 2009
 -------------------*/
void TbQuitTool(UIid uiID, BOOL bePress)
{
   if (!bePress) {
      // de-initialize the job system
      FjUseJobSystem(0);

      // empty the command history
      int i;
      for (i = 0; i < tbNumCommandHistory; i++) {
         FyFree(tbCommandHistory[i]);
         tbCommandHistory[i] = NULL;
      }
      FyFree(tbCommandHistory);
      tbCommandHistory = NULL;

      // empty the command queue
      FcSetupCommandQueue(0);

      // delete all UIs
      FyEndUISystem(tbMenuBarID);
      FyEndUISystem(tbMenuID);
      FyEndUISystem(tbSideMenuID);
      FyEndUISystem(tbWarnDialogueID);
      FyEndUISystem(tbAddActionDialogueID);

      // end the game world
      FyWin32EndWorld(tbWorldID);

      // de-initialize the script system
      TbDeinitScriptSystem();
   }
}


/*------------------
  new the system
  C.Wang 0701, 2010
 -------------------*/
void TbNewTool(UIid uiID, BOOL bePress)
{
   if (!bePress) {
      tbWarnUI.PopUp(FALSE);

      // clear command list
      FcClearCommand();

      // clear/new current scene
      tbCurScene->New();

      // remove the dumper scene
      tbWorld.DeleteScene(tbDumperSceneID);
      tbDumperSceneID = tbWorld.CreateScene(1);
      tbDumperScene.Object(tbDumperSceneID);

      // reset the UIs
      tbCurScene->SetCurrentObject(FAILED_ID);
      tbCurScene->SetCurrentActor(FAILED_ID);

      FnUiCombo uiC;
      FnUiTypein uiTy;
      FnUiRadioButton uiRB;
      FnUiButton uiB;
      uiC.Object(tbSelectActorComboID);
      uiC.ClearMessages();
      uiC.PasteComboValue("");
      uiC.Enable(FALSE);

      // clear current body
      uiC.Object(tbSelectBodyComboID);
      uiC.ClearMessages();
      uiC.Enable(FALSE);
      uiC.PasteComboValue("");

      // clear current action
      uiC.Object(tbSelectActionComboID);
      uiC.ClearMessages();
      uiC.Enable(FALSE);
      uiC.PasteComboValue("");
      uiB.Object(tbEditActionButtonID);
      uiB.Enable(FALSE);
      uiB.Object(tbDeleteActionButtonID);
      uiB.Enable(FALSE);
      uiB.Object(tbAddActionButtonID);
      uiB.Enable(FALSE);

      // update pose UIs
      uiC.Object(tbSelectBodyComboID);
      uiC.ClearMessages();
      uiC.PasteComboValue("");
      uiC.Enable(FALSE);
      uiTy.Object(tbPoseStartID);
      uiTy.PasteTypeinValue("");
      uiTy.Enable(FALSE);
      uiTy.Object(tbPoseEndID);
      uiTy.PasteTypeinValue("");
      uiTy.Enable(FALSE);
      uiRB.Object(tbPlayPoseButtonID);
      uiRB.ChangeState(FALSE);
      uiRB.Enable(FALSE);
      uiB.Object(tbMakeBaseMoveButtonID);
      uiB.Enable(FALSE);
      uiB.Object(tbNewPoseButtonID);
      uiB.Enable(FALSE);
      tbBePlayPose = FALSE;

      TbStopPlay(tbStopID, TRUE);

      tbPlaySpeed = 1.0f;
      uiTy.Object(tbPlaySpeedID);
      uiTy.PasteTypeinValue("1.000");
   }
}


/*--------------------------------
  make the system to model viewer
  C.Wang 0524, 2010
 ---------------------------------*/
void TbModelUICommand(UIid uiID, BOOL bePress)
{
   if (!bePress) {
      FnUiRadioButton button;
      button.Object(tbModelSystemButtonID);
      button.Show(TRUE);
      button.ChangeState(TRUE);

      // hide the others
      button.Object(tbActorSystemButtonID);
      button.Show(FALSE);
      button.Object(tbEnvironmentSystemButtonID);
      button.Show(FALSE);
      button.Object(tbScriptSystemButtonID);
      button.Show(FALSE);
      button.Object(tbFaceSystemButtonID);
      button.Show(FALSE);

      TbSwitchMenuBarRadios(tbModelSystemButtonID, TRUE);
   }
}


/*---------------------------------
  make the system to script loader
  C.Wang 0525, 2010
 ----------------------------------*/
void TbScriptUICommand(UIid uiID, BOOL bePress)
{
   if (!bePress) {
      FnUiRadioButton button;
      button.Object(tbScriptSystemButtonID);
      button.Show(TRUE);
      button.ChangeState(TRUE);

      // hide the others
      button.Object(tbActorSystemButtonID);
      button.Show(FALSE);
      button.Object(tbModelSystemButtonID);
      button.Show(FALSE);
      button.Object(tbEnvironmentSystemButtonID);
      button.Show(FALSE);
      button.Object(tbFaceSystemButtonID);
      button.Show(FALSE);

      TbSwitchMenuBarRadios(tbScriptSystemButtonID, TRUE);
   }
}


/*----------------------------------
  open the load script file browser
  C.Wang 0525, 2010
 -----------------------------------*/
void TbLoadScriptUICommand(UIid uiID, BOOL bePress)
{
   if (!bePress) {   
      TbCommandParser("Load", NULL, FALSE);
   }
}


/*---------------------------------
  open the load model file browser
  C.Wang 0513, 2009
 ----------------------------------*/
void TbLoadModelUICommand(UIid uiID, BOOL bePress)
{
   if (!bePress) {   
      TbCommandParser("Model", NULL, FALSE);
   }
}


/*---------------------------------
  open the save model file browser
  C.Wang 0613, 2010
 ----------------------------------*/
void TbSaveModelUICommand(UIid uiID, BOOL bePress)
{
   if (!bePress) {   
      TbCommandParser("SaveModel", NULL, FALSE);
   }
}


/*-------------------------------------
  edit current action for current body
  C.Wang 0701, 2010
 --------------------------------------*/
void TbEditActionUICommand(UIid uiID, BOOL bePress)
{
   if (!bePress) {
      tbAddActionUI.PopUp(FALSE);

      // extract the data from UIs
      FnUiCombo uiC;
      FnUiTypein uiTy;
      char argument[256], value[256];
      argument[0] = '\0';

      strcat(argument, "-front ");
      uiC.Object(tbAddActionFrontPoseID);
      uiC.GetComboValue(value);
      strcat(argument, value);

      strcat(argument, " -rear ");
      uiC.Object(tbAddActionEndPoseID);
      uiC.GetComboValue(value);
      strcat(argument, value);

      strcat(argument, " -length ");
      uiTy.Object(tbAddActionDurationID);
      uiTy.GetTypeinValue(value);
      strcat(argument, value);

      // execute command to modify current action
      TbCommandParser("EditAction", argument, FALSE);
   }
}


/*-------------------------------------
  create a new action for current body
  C.Wang 0701, 2010
 --------------------------------------*/
void TbNewActionUICommand(UIid uiID, BOOL bePress)
{
   if (!bePress) {   
      tbAddActionUI.PopUp(FALSE);

      // extract the data from UIs
      FnUiCombo uiC;
      FnUiTypein uiTy;
      char argument[256], value[256];
      argument[0] = '\0';

      strcat(argument, "-name ");
      uiTy.Object(tbAddActionNameID);
      uiTy.GetTypeinValue(value);
      strcat(argument, value);

      strcat(argument, " -type ");
      uiC.Object(tbAddActionTypeID);
      uiC.GetComboValue(value);
      strcat(argument, value);

      strcat(argument, " -front ");
      uiC.Object(tbAddActionFrontPoseID);
      uiC.GetComboValue(value);
      strcat(argument, value);

      strcat(argument, " -rear ");
      uiC.Object(tbAddActionEndPoseID);
      uiC.GetComboValue(value);
      strcat(argument, value);

      strcat(argument, " -length ");
      uiTy.Object(tbAddActionDurationID);
      uiTy.GetTypeinValue(value);
      strcat(argument, value);

      // execute command to create a new action
      TbCommandParser("NewAction", argument, FALSE);
   }
}


/*---------------------
  delete current model
  C.Wang 0622, 2010
 ----------------------*/
void TbDeleteModelUICommand(UIid uiID, BOOL bePress)
{
   if (!bePress) {   
      TbCommandParser("DeleteModel", NULL, FALSE);
   }
}


/*------------------------------------
  make previous object as current one
  C.Wang 0613, 2010
 -------------------------------------*/
void TbPreviousModelUICommand(UIid uiID, BOOL bePress)
{
   if (!bePress) {   
      TbCommandParser("PreviousModel", NULL, FALSE);
   }
}


/*--------------------------------
  make next object as current one
  C.Wang 0613, 2010
 ---------------------------------*/
void TbNextModelUICommand(UIid uiID, BOOL bePress)
{
   if (!bePress) {   
      TbCommandParser("NextModel", NULL, FALSE);
   }
}


/*------------------------
  rename current material
  C.Wang 0614, 2010
 -------------------------*/
void TbRenameMaterialUICommand(UIid uiID, char *value)
{
   // get string on typein
   char arg[256];
   sprintf(arg, "-Material %s", value);
   TbCommandParser("Rename", arg, FALSE);
}


/*---------------------
  set current material
  C.Wang 0617, 2010
 ----------------------*/
void TbMakeCurrentMaterialUICommand(UIid uiID, char *value)
{
   // get string on typein
   TbCommandParser("SetMaterial", value, FALSE);
}


/*------------------------
  update current material
  C.Wang 0617, 2010
 -------------------------*/
void TbUpdateMaterialUICommand(UIid uiID, BOOL bePress)
{
   if (!bePress) {
      TbCommandParser("UpdateMaterial", NULL, FALSE);
   }
}



/*-----------------------------------------
  set the rendering mode for current model
  C.Wang 0527, 2010
 ------------------------------------------*/
void TbWireframeModelUICommand(UIid uiID, BOOL bePress)
{
   FnUiRadioButton uiRB;
   uiRB.Object(uiID);
   if (bePress) {
      TbCommandParser("Wireframe", NULL, FALSE);
   }
   else {
      char arg[256];
      sprintf(arg, "-OFF");
      TbCommandParser("Wireframe", arg, FALSE);
   }
}


/*--------------------------------
  make the system to actor viewer
  C.Wang 0524, 2010
 ---------------------------------*/
void TbActorUICommand(UIid uiID, BOOL bePress)
{
   if (!bePress) {
      FnUiRadioButton button;
      button.Object(tbActorSystemButtonID);
      button.Show(TRUE);
      button.ChangeState(TRUE);

      // hide the others
      button.Object(tbModelSystemButtonID);
      button.Show(FALSE);
      button.Object(tbEnvironmentSystemButtonID);
      button.Show(FALSE);
      button.Object(tbScriptSystemButtonID);
      button.Show(FALSE);
      button.Object(tbFaceSystemButtonID);
      button.Show(FALSE);

      TbSwitchMenuBarRadios(tbActorSystemButtonID, TRUE);
   }
}


/*-----------------------
  select the action type
  C.Wang 0701, 2010
 ------------------------*/
void TbSelectActionType(UIid uiID, char *value)
{
   FnUiCombo uiC;
   FnUiTypein uiTy;

   uiC.Object(uiID);
   uiC.PasteComboValue(value);

   // according to the selected action type, update the UIs
   if (FyCheckWord(value, "Cross Fade Blending")) {
      uiC.Object(tbAddActionFrontPoseID);
      uiC.Enable(TRUE);
      uiC.Object(tbAddActionEndPoseID);
      uiC.Enable(TRUE);
      uiTy.Object(tbAddActionDurationID);
      uiTy.Enable(TRUE);
   }
   else if (FyCheckWord(value, "Connection")) {
      uiC.Object(tbAddActionFrontPoseID);
      uiC.Enable(TRUE);
      uiC.Object(tbAddActionEndPoseID);
      uiC.Enable(TRUE);
      uiTy.Object(tbAddActionDurationID);
      uiTy.Enable(TRUE);
   }
   else {
      uiC.Object(tbAddActionFrontPoseID);
      uiC.Enable(TRUE);
      uiC.Object(tbAddActionEndPoseID);
      uiC.Enable(FALSE);
      uiTy.Object(tbAddActionDurationID);
      uiTy.Enable(FALSE);
   }
}


/*-----------------------
  select the action pose
  C.Wang 0701, 2010
 ------------------------*/
void TbSelectActionPose(UIid uiID, char *value)
{
   FnUiCombo uiC;
   FnUiTypein uiTy;

   uiC.Object(uiID);
   uiC.PasteComboValue(value);
}


/*------------------
  set current actor
  C.Wang 0626, 2010
 -------------------*/
void TbMakeCurrentActorUICommand(UIid uiID, char *value)
{
   // send command
   TbCommandParser("SetActor", value, FALSE);
}


/*------------------
  set current body
  C.Wang 0626, 2010
 -------------------*/
void TbMakeCurrentBodyUICommand(UIid uiID, char *value)
{
   // send command
   TbCommandParser("SetBody", value, FALSE);
}


/*---------------------------------------
  select current action for current body
  C.Wang 0629, 2010
 ----------------------------------------*/
void TbSelectCurrentBodyActionUICommand(UIid uiID, char *value)
{
   // send command
   TbCommandParser("SelectAction", value, FALSE);
}


/*----------------------
  rename current action
  C.Wang 0629, 2010
 -----------------------*/
void TbRenameActionUICommand(UIid uiID, char *value)
{
   // send command
   char arg[256];
   sprintf(arg, "-ActorAction %s", value);
   TbCommandParser("Rename", arg, FALSE);
}


/*----------------------
  delete current action
  C.Wang 0701, 2010
 -----------------------*/
void TbDeleteActionUICommand(UIid uiID, BOOL beP)
{
   if (!beP) {
      // send command
      TbCommandParser("DeleteAction", NULL, FALSE);
   }
}


/*---------------------------------
  add a new action to current body
  C.Wang 0701, 2010
 ----------------------------------*/
void TbAddActionUICommand(UIid uiID, BOOL beP)
{
   if (!beP) {
      // open the add action dialogue      
   }
}


/*---------------------------
  select pose for previewing
  C.Wang 0629, 2010
 ----------------------------*/
void TbSelectPoseUICommand(UIid uiID, char *value)
{
   // send command
   TbCommandParser("SelectPose", value, FALSE);
}


/*------------------
  play test pose
  C.Wang 0629, 2010
 -------------------*/
void TbPlayTestPoseUICommand(UIid uiID, BOOL beP)
{
   // send command
   if (beP) {
      TbCommandParser("PlayPose", NULL, FALSE);
   }
   else {
      TbCommandParser("PlayPose", "-off", FALSE);
   }
}


/*-----------------------
  make base move command
  C.Wang 0717, 2010
 ------------------------*/
void TbMakeBaseMoveUICommand(UIid uiID, BOOL beP)
{
   // extract pose data
   char string[256], poseName[256];
   FnActor actor;
   FnUiCombo uiC;
   int iStart, iEnd;

   uiC.Object(tbSelectPoseComboID);
   uiC.GetComboValue(poseName);

   actor.Object(tbCurScene->GetCurrentActor());
   actor.QueryPose(actor.GetCurrentBodyName(), poseName, &iStart, &iEnd);


   sprintf(string, "-Frame %d %d", iStart, iEnd);
   TbCommandParser("MakeBaseMove", string, FALSE);
}


/*---------------------------------------------
  make the system to working environment setup
  C.Wang 0524, 2010
 ----------------------------------------------*/
void TbEnvironmentUICommand(UIid uiID, BOOL bePress)
{
   if (!bePress) {
      FnUiRadioButton button;
      button.Object(tbEnvironmentSystemButtonID);
      button.Show(TRUE);
      button.ChangeState(TRUE);

      // hide the others
      button.Object(tbModelSystemButtonID);
      button.Show(FALSE);
      button.Object(tbActorSystemButtonID);
      button.Show(FALSE);
      button.Object(tbScriptSystemButtonID);
      button.Show(FALSE);
      button.Object(tbFaceSystemButtonID);
      button.Show(FALSE);

      TbSwitchMenuBarRadios(tbEnvironmentSystemButtonID, TRUE);
   }
}


/*----------------------------------
  make the system to face functions
  C.Wang 0524, 2010
 -----------------------------------*/
void TbFaceUICommand(UIid uiID, BOOL bePress)
{
   if (!bePress) {
      FnUiRadioButton button;
      button.Object(tbFaceSystemButtonID);
      button.Show(TRUE);
      button.ChangeState(TRUE);

      // hide the others
      button.Object(tbModelSystemButtonID);
      button.Show(FALSE);
      button.Object(tbActorSystemButtonID);
      button.Show(FALSE);
      button.Object(tbScriptSystemButtonID);
      button.Show(FALSE);
      button.Object(tbEnvironmentSystemButtonID);
      button.Show(FALSE);

      TbSwitchMenuBarRadios(tbFaceSystemButtonID, TRUE);
   }
}


/*---------------------------------
  open the load actor file browser
  C.Wang 0604, 2009
 ----------------------------------*/
void TbLoadActorUICommand(UIid uiID, BOOL bePress)
{
   if (!bePress) {   
      TbCommandParser("Actor", NULL, FALSE);
   }
}


/*---------------------
  delete current actor
  C.Wang 0626, 2010
 ----------------------*/
void TbDeleteActorUICommand(UIid uiID, BOOL bePress)
{
   if (!bePress) {   
      TbCommandParser("DeleteActor", NULL, FALSE);
   }
}


/*---------------------------------
  open the save actor file browser
  C.Wang 0624, 2010
 ----------------------------------*/
void TbSaveActorUICommand(UIid uiID, BOOL bePress)
{
   if (!bePress) {   
      TbCommandParser("SaveActor", NULL, FALSE);
   }
}


/*---------------------------------
  open the load actor file browser
  C.Wang 0604, 2009
 ----------------------------------*/
void TbCancelMaterialEditUICommand(UIid uiID, BOOL bePress)
{
   if (!bePress) {   
      TbCancelMaterialEditing();
   }
}


#ifdef MULTICORE
/*------------------------------
  initialize a critical section
  C.Wang 0611, 2007
 -------------------------------*/
void TbInitializeCriticalSection(TBCriticalSection *cs)
{
   InitializeCriticalSection(&cs->cs);
   cs->id = NONE;
}


/*-------------------------
  enter a critical section
  C.Wang 0626, 2007
 --------------------------*/
BOOL TbEnterCriticalSection(TBCriticalSection *cs, int owner)
{
   if (cs == NULL) return FALSE;

   // these are the magic setting for tbRenderCS
   if (cs->id == 0 && owner == 0) return FALSE;
   if (cs->id == 1 && owner == 1) return FALSE;

   // make the critical section locked
   EnterCriticalSection(&cs->cs);
   cs->id = owner;
   return TRUE;
}


/*-------------------------
  leave a critical section
  C.Wang 0622, 2007
 --------------------------*/
void TbLeaveCriticalSection(TBCriticalSection *cs)
{
   LeaveCriticalSection(&cs->cs);
   cs->id = NONE;
}


/*--------------------------
  delete a critical section
  C.Wang 0626, 2007
 ---------------------------*/
void TbDeleteCriticalSection(TBCriticalSection *cs)
{
   DeleteCriticalSection(&cs->cs);
}

#endif


/*----------------------------------
  twiddle on/off the auxiliary view
  C.Wang 0624, 2010
 -----------------------------------*/
void TbShowAuxView(UIid uiID, BOOL bePress)
{
   FnUiRadioButton radio;
   radio.Object(tbAuxButtonID);
   tbShowAux = radio.GetState();
   FyBeRenderBoundingBox(tbShowAux);

   // show/hide current actor's base
   ACTORid aID = tbCurScene->GetCurrentActor();
   if (aID != FAILED_ID) {
      FnActor actor;
      actor.Object(aID);

      FnObject base;
      base.Object(actor.GetBaseObject());
      base.Show(tbShowAux);
   }
}


/*-----------------------------------
  twiddle on/off the orthogonal view
  C.Wang 0624, 2010
 ------------------------------------*/
void TbShowOrthoView(UIid uiID, BOOL bePress)
{
   FnUiRadioButton radio;
   FnUiRadioButton radioAux;

   radio.Object(tbOrthoButtonID);
   radioAux.Object(tbAuxButtonID);
   if (radio.GetState()) {
      tbRenderCamera = ORTHOGONAL;
      radioAux.Enable(FALSE);
   }
   else {
      tbRenderCamera = PERSPECTIVE;
      radioAux.Enable(TRUE);
   }
}


/*------------------------------------------------------
  command to setup the working and data searching paths
  C.Wang 0710, 2009
 -------------------------------------------------------*/
BOOL TbSetPathCmd(char *argument)
{
   BOOL beOK = FALSE;
   char path[256];

   // check argument
   BOOL beOKCmd = TbCheckCommandAttributeS(argument, "-Work", path, 256);
   if (beOKCmd) {
      SetCurrentDirectory(path);
      beOK = TRUE;
   }

   beOKCmd = TbCheckCommandAttributeS(argument, "-Texture", path, 256);
   if (beOKCmd) {
      tbWorld.SetTexturePath(path);
      beOK = TRUE;
   }

   beOKCmd = TbCheckCommandAttributeS(argument, "-Scene", path, 256);
   if (beOKCmd) {
      tbWorld.SetScenePath(path);
      beOK = TRUE;
   }

   beOKCmd = TbCheckCommandAttributeS(argument, "-Object", path, 256);
   if (beOKCmd) {
      tbWorld.SetObjectPath(path);
      beOK = TRUE;
   }

   beOKCmd = TbCheckCommandAttributeS(argument, "-Shader", path, 256);
   if (beOKCmd) {
      tbWorld.SetShaderPath(path);
      beOK = TRUE;
   }

   beOKCmd = TbCheckCommandAttributeS(argument, "-Actor", path, 256);
   if (beOKCmd) {
      tbWorld.SetCharacterPath(path);
      beOK = TRUE;
   }

   beOKCmd = TbCheckCommandAttributeS(argument, "-Audio", path, 256);
   if (beOKCmd) {
      tbWorld.SetAudioPath(path);
      beOK = TRUE;
   }

   beOKCmd = TbCheckCommandAttributeS(argument, "-Extra", path, 256);
   if (beOKCmd) {
      tbWorld.SetExtraDataPath(path);
      beOK = TRUE;
   }

   if (beOK) {
      TbSetLogMessage("Set paths OK");
   }
   else {
      TbSetLogMessage("Nothing set!");
   }
   return beOK;
}


/*-------------------------------------------
  command to change the tool mode to
  Model Viewer, Actor Viwer, ...
  C.Wang 0525, 2010
 --------------------------------------------*/
BOOL TbSwitchToolModeCmd(char *argument)
{
   // check argument
   BOOL beOK = FALSE;
   BOOL beOKCmd = TbCheckCommandAttributeS(argument, "-Model", NULL, 256);
   if (beOKCmd) {
      TbModelUICommand(tbModelButtonID, FALSE);
      TbSetLogMessage("Switch to Model Viewer");
      beOK = TRUE;
   }

   beOKCmd = TbCheckCommandAttributeS(argument, "-Script", NULL, 256);
   if (beOKCmd) {
      TbScriptUICommand(tbScriptButtonID, FALSE);
      TbSetLogMessage("Switch to Script Loader");
      beOK = TRUE;
   }

   beOKCmd = TbCheckCommandAttributeS(argument, "-Actor", NULL, 256);
   if (beOKCmd) {
      TbActorUICommand(tbActorButtonID, FALSE);
      TbSetLogMessage("Switch to Actor Viewer");
      beOK = TRUE;
   }

   beOKCmd = TbCheckCommandAttributeS(argument, "-Environment", NULL, 256);
   if (beOKCmd) {
      TbEnvironmentUICommand(tbEnvironmentButtonID, FALSE);
      TbSetLogMessage("Switch to Setup Working Environment");
      beOK = TRUE;
   }

   return beOK;
}


/*---------------------------------------
  play speed command - set playing speed
  C.Wang 0522, 2009
 ----------------------------------------*/
BOOL TbSetPlaySpeedCmd(char *arg)
{
   // find the speed value
   char string[256], log[256], *key;
   float speed = 1.0f;

   strcpy(string, arg);
   key = strtok(string, " ");
   if (key != NULL) {
      speed = (float) atof(key);
   }
   tbPlaySpeed = FYABS(speed);

   // paste the play seed data to UI
   FnUiTypein uiTy;
   char value[256];
   uiTy.Object(tbPlaySpeedID);
   sprintf(value, "%5.3f", tbPlaySpeed);
   uiTy.PasteTypeinValue(value);

   sprintf(log, "Set playing speed = %6.3f", tbPlaySpeed);
   TbSetLogMessage(log);
   return TRUE;
}


/*--------------------------------------
  command to select pose for previewing
  C.Wang 0629, 2010
 ---------------------------------------*/
BOOL TbSelectPoseCmd(char *argument)
{
   // extract pose data
   FnActor actor;
   actor.Object(tbCurScene->GetCurrentActor());
   actor.QueryPose(actor.GetCurrentBodyName(), argument, &tbTestPoseStart, &tbTestPoseEnd);
   tbTestPoseFrame = (float) tbTestPoseStart;

   char string[256];
   FnUiTypein uiTy;
   uiTy.Object(tbPoseStartID);
   sprintf(string, "%d", tbTestPoseStart);
   uiTy.PasteTypeinValue(string);
   //uiTy.Enable(TRUE);
   
   uiTy.Object(tbPoseEndID);
   sprintf(string, "%d", tbTestPoseEnd);
   uiTy.PasteTypeinValue(string);
   //uiTy.Enable(TRUE);

   return TRUE;
}


/*--------------------------------------
  command to select pose for previewing
  C.Wang 0629, 2010
 ---------------------------------------*/
BOOL TbPlayTestPoseCmd(char *argument)
{
   // check argument
   if (argument == NULL) {
      tbBePlayPose = TRUE;
   }
   else {
      if (FyCheckWord(argument, "-off")) {
         tbBePlayPose = FALSE;
      }
      else {
         tbBePlayPose = TRUE;
      }
   }

   return TRUE;
}


/*---------------------------------------------
  command to twiddle on/off the auxiliary view
  C.Wang 0624, 2010
 ----------------------------------------------*/
BOOL TbShowAuxViewCmd(char *argument)
{
   // check argument
   BOOL beOK = TbCheckCommandAttributeS(argument, "-OFF", NULL, 256);
   FnUiRadioButton radio;
   radio.Object(tbAuxButtonID);
   if (beOK) {
      radio.ChangeState(FALSE);
      TbShowAuxView(tbAuxButtonID, FALSE);
   }
   else {
      radio.ChangeState(TRUE);
      TbShowAuxView(tbAuxButtonID, TRUE);
   }

   return TRUE;
}


/*----------------------------------------------
  command to twiddle on/off the orthogonal view
  C.Wang 0715, 2010
 -----------------------------------------------*/
BOOL TbShowOrthoViewCmd(char *argument)
{
   // check argument
   BOOL beOK = TbCheckCommandAttributeS(argument, "-OFF", NULL, 256);
   FnUiRadioButton radio;
   radio.Object(tbOrthoButtonID);
   if (beOK) {
      radio.ChangeState(FALSE);
      TbShowOrthoView(tbOrthoButtonID, FALSE);
   }
   else {
      radio.ChangeState(TRUE);
      TbShowOrthoView(tbOrthoButtonID, TRUE);
   }

   return TRUE;
}


/*------------------
  make base to move
  C.Wang 0717, 2010
 -------------------*/
BOOL TbMakeBaseMoveCmd(char *argument)
{
   FnActor actor;
   actor.Object(tbCurScene->GetCurrentActor());
   if (actor.Object() == FAILED_ID) {
      return FALSE;
   }

   int iStart, iEnd, frame[2];
   iStart = 0;
   iEnd = 30;
   BOOL beOK = TbCheckCommandAttributeI(argument, "-Frame", frame, 2);
   if (beOK) {
      iStart = frame[0];
      iEnd = frame[1];
   }

   // check if the part's parent is the base
   char *rootName = actor.GetRootName(0);
   actor.MakeBaseMove(rootName, iStart, iEnd, FALSE, NULL);
   actor.PlayFrame(tbTestPoseFrame, tbBeBase, tbPlaySpeed);

   char log[256];
   sprintf(log, "Make the base of %s to move from %d to %d", actor.GetName(), iStart, iEnd);
   TbSetLogMessage(log);
   return TRUE;
}


/*---------------------------------------
  make the material update flag to dirty
  C.Wang 0621, 2010
 ----------------------------------------*/
void TbMakeUpdateDirtyTy(UIid uiID, char *data)
{
   FnUiButton uiB;
   uiB.Object(tbUpdateMaterialID);
   uiB.Enable(TRUE);
}


/*---------------------------------------
  make the material update flag to dirty
  C.Wang 0621, 2010
 ----------------------------------------*/
void TbMakeUpdateDirtyC(UIid uiID, char *data)
{
   FnUiButton uiB;
   uiB.Object(tbUpdateMaterialID);
   uiB.Enable(TRUE);
}


/*-----------------------
  open the quit dialogue
  C.Wang 0508, 2009
 ------------------------*/
void TbOpenQuit(UIid uiID, BOOL bePress)
{
   if (!bePress) {
      // set warning message
      FnUiPlate msg;
      msg.Object(tbWarnMessageID);
      msg.SetUIMessage(0, LEFT_TOP, 18, 40, "Are You Really Want to Quit ?", 0.5f, 0.0f, 0.0f);

      // bind the callback to the OK button
      FnUiButton okButton;
      okButton.Object(tbWarnOKID);
      okButton.BindCallback(TbQuitTool);

      // popup the dialogue
      tbWarnUI.PopUp(TRUE);
   }
}


/*------------------------
  open the "new" dialogue
  C.Wang 0624, 2010
 -------------------------*/
void TbOpenNew(UIid uiID, BOOL bePress)
{
   if (!bePress) {
      // set warning message
      FnUiPlate msg;
      msg.Object(tbWarnMessageID);
      msg.SetUIMessage(0, LEFT_TOP, 18, 40, "Are You Want to Reset the System ?", 0.5f, 0.0f, 0.0f);

      // bind the callback to the OK button
      FnUiButton okButton;
      okButton.Object(tbWarnOKID);
      okButton.BindCallback(TbNewTool);

      // popup the dialogue
      tbWarnUI.PopUp(TRUE);
   }
}


/*-------------------------------
  open the "add action" dialogue
  C.Wang 0701, 2010
 --------------------------------*/
void TbOpenAddAction(UIid uiID, BOOL bePress)
{
   // get current action data
   ACTORid actorID = tbCurScene->GetCurrentActor();
   if (actorID == FAILED_ID) return;

   FnActor actor;
   actor.Object(actorID);

   char *bodyName = actor.GetCurrentBodyName();
   ACTIONid actionID = actor.GetCurrentAction(0, bodyName);
   if (actionID == FAILED_ID) return;

   FnAction action;
   action.Object(actionID);

   FnUiPlate uiP;
   FnUiCombo uiCType;
   FnUiCombo uiCFront, uiCEnd;
   FnUiTypein uiTyName, uiTyLen;
   uiP.Object(tbAddActionBackgroundID);
   uiCType.Object(tbAddActionTypeID);
   uiTyName.Object(tbAddActionNameID);
   uiCFront.Object(tbAddActionFrontPoseID);
   uiCEnd.Object(tbAddActionEndPoseID);
   uiTyLen.Object(tbAddActionDurationID);

   BOOL beOK;
   char poseName[256], string[256];
   int i, iCount, numB = actor.GetPoseNumber(bodyName);
   uiCFront.ClearMessages();
   uiCEnd.ClearMessages();
   if (numB > 0) {
      iCount = 0;
      for (i = 0; i < numB; i++) {
         beOK = actor.QueryPose(bodyName, i, poseName, NULL, NULL);
         if (beOK) {
            uiCFront.InsertMessage(iCount, poseName);
            uiCEnd.InsertMessage(iCount, poseName);
            iCount++;
         }
      }
   }

   if (!bePress) {
      if (uiID == tbAddActionButtonID) {
         uiP.SetCaption(LEFT_TOP, 6, 7, "Add New Action", 0.0f, 0.0f, 0.0f);
         uiCType.PasteComboValue("Simple Action");
         uiTyName.PasteTypeinValue("-");
         uiCFront.PasteComboValue("-");
         uiCEnd.PasteComboValue("-");
         uiTyLen.PasteTypeinValue("-");

         uiCType.Enable(TRUE);
         uiTyName.Enable(TRUE);
         uiCEnd.Enable(FALSE);
         uiTyLen.Enable(FALSE);
      }
      else {
         uiP.SetCaption(LEFT_TOP, 6, 7, "Edit Action", 0.0f, 0.0f, 0.0f);
         uiTyName.PasteTypeinValue(action.GetName());
         uiTyName.Enable(FALSE);
         
         DWORD type = action.GetType();
         if (type == CROSS_BLEND_ACTION) {
            uiCType.PasteComboValue("Cross Fade Blending");
            uiCFront.PasteComboValue(action.GetFrontPoseName());
            uiCEnd.PasteComboValue(action.GetRearPoseName());
            sprintf(string, "%d", action.GetLength());
            uiTyLen.PasteTypeinValue(string);

            uiCEnd.Enable(TRUE);
            uiTyLen.Enable(TRUE);
         }
         else if (type == CONNECT_ACTION) {
            uiCType.PasteComboValue("Connection");
            uiCFront.PasteComboValue(action.GetFrontPoseName());
            uiCEnd.PasteComboValue(action.GetRearPoseName());
            sprintf(string, "%d", action.GetLength());
            uiTyLen.PasteTypeinValue(string);

            uiCEnd.Enable(TRUE);
            uiTyLen.Enable(TRUE);
         }
         else {
            uiCType.PasteComboValue("Simple Action");
            uiCFront.PasteComboValue(action.GetFrontPoseName());
            uiCEnd.PasteComboValue("-");
            uiTyLen.PasteTypeinValue("-");

            uiCEnd.Enable(FALSE);
            uiTyLen.Enable(FALSE);
         }
         uiCType.Enable(FALSE);
      }

      // bind the callback to the OK button
      FnUiButton okButton;
      okButton.Object(tbAddActionOKID);
      if (uiID == tbAddActionButtonID) {
         okButton.BindCallback(TbNewActionUICommand);
      }
      else {
         okButton.BindCallback(TbEditActionUICommand);
      }

      // popup the dialogue
      tbAddActionUI.PopUp(TRUE);
   }
}


/*----------------------------------
  switch between play or pause mode
  C.Wang 0629, 2010
 -----------------------------------*/
void TbMakePlay(UIid uiID, BOOL bePress)
{
   tbBePlay = bePress;
   if (tbBePlay) {
      FnUiRadioButton uiRB;
      uiRB.Object(tbPlayPoseButtonID);
      uiRB.ChangeState(FALSE);
      tbBePlayPose = FALSE;
      uiRB.Enable(FALSE);
   }
   else {
      FnUiRadioButton uiRB;
      uiRB.Object(tbPlayPoseButtonID);
      uiRB.Enable(TRUE);
   }
}


/*------------------
  stop the playing
  C.Wang 0629, 2010
 -------------------*/
void TbStopPlay(UIid uiID, BOOL bePress)
{
   // force to stop the playing
   tbBePlay = FALSE;

   // update the associated UIs
   FnUiRadioButton uiRB;
   uiRB.Object(tbPlayID);
   uiRB.ChangeState(FALSE);

   // reset all actors' pose
   tbCurScene->ResetPlay(tbBeBase);

   uiRB.Object(tbPlayPoseButtonID);
   uiRB.Enable(TRUE);
}


/*------------------------
  enter working directory
  C.Wang 0525, 2010
 -------------------------*/
void TbSetDir(UIid uiID, char *value)
{
   // prepare the command
   char argument[256];

   if (uiID == tbWorkDirTypeinUI) {
      sprintf(argument, "-work %s", value);
   }
   else if (uiID == tbShaderDirTypeinUI) {
      sprintf(argument, "-shader %s", value);
   }
   else if (uiID == tbModelDirTypeinUI) {
      sprintf(argument, "-object %s", value);
   }
   else if (uiID == tbActorDirTypeinUI) {
      sprintf(argument, "-actor %s", value);
   }
   else if (uiID == tbTextureDirTypeinUI) {
      sprintf(argument, "-texture %s", value);
   }

   // execute the command
   TbCommandParser("SetPath", argument, FALSE);
}


/*------------------
  set play speed
  C.Wang 0717, 2010
 -------------------*/
void TbPlaySpeedUICommand(UIid uiID, char *value)
{
   // execute the command
   TbCommandParser("Speed", value, FALSE);
}


/*-------------------------------------
  recalculate the UI position and size
  C.Wang 0429, 2009
 --------------------------------------*/
void TbViewportLayout(int width, int height)
{
   int w, h;
   FyQueryWindowFrameSize(&w, &h);

   // check extreme
   if (width < tbMinWorldWidth + w || height < tbMinWorldHeight + h) return;

   // top menu bar
   tbMenuBarOX = 0;
   tbMenuBarOY = 0;
   tbMenuBarWidth = width;

   // main menu
   tbMenuOX = tbSideMenuWidth;
   tbMenuOY = height - tbMenuHeight;
   tbMenuWidth = width - tbSideMenuWidth;

   // work space template
   tbBottomVPOX = tbSideMenuWidth;
   tbBottomVPOY = tbMenuBarHeight;
   tbBottomVPWidth = width - tbSideMenuWidth;
   tbBottomVPHeight = height - tbMenuBarHeight - tbMenuHeight;

   // work space
   //if (tbBottomVPWidth - tbWorkSpaceVPWidth > 30) {
      tbWorkSpaceVPWidth = tbBottomVPWidth - 30;
   //}
   //if (tbBottomVPHeight - tbWorkSpaceVPHeight > 20) {
      tbWorkSpaceVPHeight = tbBottomVPHeight - 20;
   //}
   tbWorkSpaceVPOX = tbSideMenuWidth + (tbBottomVPWidth - tbWorkSpaceVPWidth)/2;
   tbWorkSpaceVPOY = tbMenuBarHeight + (tbBottomVPHeight - tbWorkSpaceVPHeight)/2;

   // side menu
   tbSideMenuOX = 0;
   tbSideMenuOY = tbMenuBarHeight;
   tbSideMenuHeight = height - tbMenuBarHeight;
}


/*----------------------------------
  manage the menu bar radio buttons
  C.Wang 0525, 2010
 -----------------------------------*/
void TbSwitchMenuBarRadios(UIid uiID, BOOL bePressed)
{
   FnUiPlate plate;

   if (bePressed) {
      if (uiID == tbSystemButtonID) {
         // hide all not used UIs
         plate.Object(tbSideMenuModelPlateID);
         plate.Show(FALSE);
         plate.Object(tbSideMenuActorPlateID);
         plate.Show(FALSE);
         plate.Object(tbSideMenuEnvironmentPlateID);
         plate.Show(FALSE);
         plate.Object(tbEnvironmentPlanelPlateUI);
         plate.Show(FALSE);
         plate.Object(tbSideMenuScriptPlateID);
         plate.Show(FALSE);
         plate.Object(tbSideMenuFacePlateID);
         plate.Show(FALSE);

         // show the associated UIs
         plate.Object(tbSideMenuSystemPlateID);
         plate.Show(TRUE);
      }
	   else if (uiID == tbScriptSystemButtonID) {
         // hide all not used UIs
         plate.Object(tbSideMenuSystemPlateID);
         plate.Show(FALSE);
         plate.Object(tbSideMenuActorPlateID);
         plate.Show(FALSE);
         plate.Object(tbSideMenuEnvironmentPlateID);
         plate.Show(FALSE);
         plate.Object(tbEnvironmentPlanelPlateUI);
         plate.Show(FALSE);
         plate.Object(tbSideMenuModelPlateID);
         plate.Show(FALSE);
         plate.Object(tbSideMenuFacePlateID);
         plate.Show(FALSE);

         // show the associated UIs
         plate.Object(tbSideMenuScriptPlateID);
         plate.Show(TRUE);
	   }
	   else if (uiID == tbModelSystemButtonID) {
         // hide all not used UIs
         plate.Object(tbSideMenuSystemPlateID);
         plate.Show(FALSE);
         plate.Object(tbSideMenuActorPlateID);
         plate.Show(FALSE);
         plate.Object(tbSideMenuEnvironmentPlateID);
         plate.Show(FALSE);
         plate.Object(tbEnvironmentPlanelPlateUI);
         plate.Show(FALSE);
         plate.Object(tbSideMenuScriptPlateID);
         plate.Show(FALSE);
         plate.Object(tbSideMenuFacePlateID);
         plate.Show(FALSE);

         // show the associated UIs
         plate.Object(tbSideMenuModelPlateID);
         plate.Show(TRUE);
	   }
      else if (uiID == tbActorSystemButtonID) {
         // hide all not used UIs
         plate.Object(tbSideMenuSystemPlateID);
         plate.Show(FALSE);
         plate.Object(tbSideMenuModelPlateID);
         plate.Show(FALSE);
         plate.Object(tbSideMenuEnvironmentPlateID);
         plate.Show(FALSE);
         plate.Object(tbEnvironmentPlanelPlateUI);
         plate.Show(FALSE);
         plate.Object(tbSideMenuScriptPlateID);
         plate.Show(FALSE);
         plate.Object(tbSideMenuFacePlateID);
         plate.Show(FALSE);

         // show the associated UIs
         plate.Object(tbSideMenuActorPlateID);
         plate.Show(TRUE);
      }
      else if (uiID == tbEnvironmentSystemButtonID) {
         // hide all not used UIs
         plate.Object(tbSideMenuSystemPlateID);
         plate.Show(FALSE);
         plate.Object(tbSideMenuModelPlateID);
         plate.Show(FALSE);
         plate.Object(tbSideMenuActorPlateID);
         plate.Show(FALSE);
         plate.Object(tbSideMenuScriptPlateID);
         plate.Show(FALSE);
         plate.Object(tbSideMenuFacePlateID);
         plate.Show(FALSE);

         // show the associated UIs
         plate.Object(tbSideMenuEnvironmentPlateID);
         plate.Show(TRUE);
         plate.Object(tbEnvironmentPlanelPlateUI);
         plate.Show(TRUE);
      }
	   else if (uiID == tbFaceSystemButtonID) {
         // hide all not used UIs
         plate.Object(tbSideMenuSystemPlateID);
         plate.Show(FALSE);
         plate.Object(tbSideMenuModelPlateID);
         plate.Show(FALSE);
         plate.Object(tbSideMenuActorPlateID);
         plate.Show(FALSE);
         plate.Object(tbSideMenuScriptPlateID);
         plate.Show(FALSE);
         plate.Object(tbSideMenuEnvironmentPlateID);
         plate.Show(FALSE);

         // show the associated UIs
         plate.Object(tbSideMenuFacePlateID);
         plate.Show(TRUE);
	   }

      plate.Object(tbSideMenuPlateID);
      plate.Show(FALSE);
   }
   else {
      // hide all associated UIs
      if (uiID == tbSystemButtonID) {
         plate.Object(tbSideMenuSystemPlateID);
         plate.Show(FALSE);
      }
      else if (uiID == tbScriptSystemButtonID) {
         plate.Object(tbSideMenuScriptPlateID);
         plate.Show(FALSE);
      }
      else if (uiID == tbModelSystemButtonID) {
         plate.Object(tbSideMenuModelPlateID);
         plate.Show(FALSE);
      }
      else if (uiID == tbActorSystemButtonID) {
         plate.Object(tbSideMenuActorPlateID);
         plate.Show(FALSE);
      }
      else if (uiID == tbFaceSystemButtonID) {
         plate.Object(tbSideMenuFacePlateID);
         plate.Show(FALSE);
      }
      else if (uiID == tbEnvironmentSystemButtonID) {
         plate.Object(tbSideMenuEnvironmentPlateID);
         plate.Show(FALSE);
         plate.Object(tbEnvironmentPlanelPlateUI);
         plate.Show(FALSE);
     }

      // show empty plate
      plate.Object(tbSideMenuPlateID);
      plate.Show(TRUE);
   }
}


/*--------------------------------------------------------
  callback function for double-clicking left mouse button
  C.Wang 0521, 2009
 ---------------------------------------------------------*/
void TbLeftMouseDoubleClick(WORLDid g, int x, int y)
{
   // check if the mouse pressing a UI
   if (FyCheckAllLeftMouseDoubleClick(tbAllUISysID, tbNumUISys, x, y)) return;
}


/*-------------------------------------------------
  callback function for pressing left mouse button
  C.Wang 0505, 2009
 --------------------------------------------------*/
void TbLeftMousePress(WORLDid g, int x, int y)
{
   // check if the mouse pressing a UI
   if (FyCheckAllLeftMousePress(tbAllUISysID, tbNumUISys, x, y)) {
      tbBeUIPressed = TRUE;
      return;
   }

   // check if the mouse inside the workspace
   int xx, yy;
   xx = x - tbBottomVPOX;
   yy = y - tbBottomVPOY;
   if (xx > 0 && xx < tbBottomVPWidth && yy > 0 && yy < tbBottomVPHeight) {
      FyBindInputFunction(TbCommandInput);
      FyDefineHotKey(FY_LEFT, TbCommandFunKey, TRUE);
      FyDefineHotKey(FY_UP, TbCommandFunKey);
      FyDefineHotKey(FY_RIGHT, TbCommandFunKey, TRUE);
      FyDefineHotKey(FY_DOWN, TbCommandFunKey);
   }
   tbBeUIPressed = FALSE;

   // check for camera
   tbOldX = x;
   tbOldY = y;
}


/*-------------------------------------------------
  callback function for dragging left mouse button
  C.Wang 0601, 2009
 --------------------------------------------------*/
void TbLeftMouseDrag(WORLDid g, int x, int y)
{
   // check for camera
   if (!tbBeUIPressed) {
      if (tbRenderCamera == PERSPECTIVE) {
         if (x != tbOldX) {
            tbCameraRef.Rotate(Z_AXIS, (float) (tbOldX - x)*tbPivotSpeed, LOCAL);
         }
    
         if (y != tbOldY) {
            tbCamera.Rotate(X_AXIS, (float) (tbOldY - y)*tbPivotSpeed, GLOBAL);    
         }
      }

      // update x and y position
      tbOldX = x; tbOldY = y;
   }
   else {
      FyCheckAllLeftMouseDrag(tbAllUISysID, tbNumUISys, x, y);
   }
}


/*--------------------------------------------------
  callback function for releasing left mouse button
  C.Wang 0601, 2009
 ---------------------------------------------------*/
void TbLeftMouseRelease(WORLDid g, int x, int y)
{
   // check if the mouse releasing a UI
   if (tbBeUIPressed) {
      FyCheckAllLeftMouseRelease(tbAllUISysID, tbNumUISys, x, y);
      tbBeUIPressed = FALSE;
   }
}


/*--------------------------------------------------
  callback function for pressing right mouse button
  C.Wang 0601, 2009
 ---------------------------------------------------*/
void TbRightMousePress(WORLDid g, int x, int y)
{
   // check for camera
   tbOldX = x;
   tbOldY = y;
}


/*--------------------------------------------------
  callback function for dragging right mouse button
  C.Wang 0601, 2009
 ---------------------------------------------------*/
void TbRightMouseDrag(WORLDid g, int x, int y)
{
   // check for camera
   // if FY_CTRL key is down, pan->pivot
   if (FyCheckHotKeyStatus(FY_ALT)) {
      if (tbRenderCamera == PERSPECTIVE) {
         if (x != tbOldX) {
            tbCameraRef.Translate((float) (tbOldX - x)*tbMoveSpeed, 0.0f, 0.0f, LOCAL);
         }
        
         if (y != tbOldY) {
            tbCamera.Translate(0.0f, (float) (y - tbOldY)*tbMoveSpeed, 0.0f, LOCAL);
         }
      }
      else {
         if (x != tbOldX) {
            tbOrthoCamera.Translate((float) (tbOldX - x)*tbMoveSpeed*0.01f, 0.0f, 0.0f, LOCAL);
         }
        
         if (y != tbOldY) {
            tbOrthoCamera.Translate(0.0f, (float) (y - tbOldY)*tbMoveSpeed*0.01f, 0.0f, LOCAL);
         }
      }
   }

   // update x and y position
   tbOldX = x;
   tbOldY = y;
}


/*---------------------------------------------------
  callback function for releasing right mouse button
  C.Wang 0601, 2009
 ----------------------------------------------------*/
void TbRightMouseRelease(WORLDid g, int x, int y)
{
}


/*---------------------------------------------------
  callback function for pressing middle mouse button
  C.Wang 0601, 2009
 ----------------------------------------------------*/
void TbMiddleMousePress(WORLDid g, int x, int y)
{
   // check for camera
   tbOldX = x;
   tbOldY = y;
}


/*---------------------------------------------------
  callback function for dragging middle mouse button
  C.Wang 0601, 2009
 ----------------------------------------------------*/
void TbMiddleMouseDrag(WORLDid g, int x, int y)
{
   // check for camera
   if (FyCheckHotKeyStatus(FY_ALT)) {
      if (tbRenderCamera == PERSPECTIVE) {
         if (x != tbOldX) {
            tbCamera.Translate(0.0f, 0.0f, (float)(tbOldX - x)*tbZoomSpeed, LOCAL);
         }

         if (y != tbOldY) {
            tbCamera.Translate(0.0f, 0.0f, (float)(tbOldY - y)*tbZoomSpeed, LOCAL);
         }
      }
      else {
         if (x != tbOldX) {
            if (tbOldX - x > 0) {
               tbOrthScaleFactor *= tbOrthScaleRate;
            }
            else {
               tbOrthScaleFactor /= tbOrthScaleRate;
            }
            tbOrthoCamera.SetScaleFactor(tbOrthScaleFactor);
         }
      }
   }

   tbOldX = x;
   tbOldY = y;
}


/*----------------------------------------------------
  callback function for releasing middle mouse button
  C.Wang 0601, 2009
 -----------------------------------------------------*/
void TbMiddleMouseRelease(WORLDid g, int x, int y)
{
}


/*-------------------
  create menu bar UI
  C.Wang 0430, 2009
 --------------------*/
void TbCreateMenuBarUI()
{
   FnUiPlate uiP;
   FnUiButton uiB;
   FnUiRadioButton uiRB;

   // create the menu bar Ui system
   tbMenuBarID = FyBeginUISystem(tbWorldID, tbMenuBarOX, tbMenuBarOY, tbMenuBarWidth, tbMenuBarHeight);
   tbMenuBarUI.Object(tbMenuBarID);
   tbMenuBarUI.SetBackgroundColor(0.45f, 0.45f, 0.45f);
   tbAllUISysID[tbNumUISys++] = tbMenuBarID;

   // create the plate
   tbMenuBarPlateID = tbMenuBarUI.CreatePlate("BarPlate", 0, 0, tbMenuBarWidth - 1, tbMenuBarHeight - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.Object(tbMenuBarPlateID);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);

   // create the "System" function
   tbSystemButtonID = tbMenuBarUI.CreateRadioButton("SystemRadioButton", 5, 5, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiRB.Object(tbSystemButtonID);
   uiRB.SetParent(tbMenuBarPlateID);
   uiRB.SetCaption(CENTERED, 0, 0, "System", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetGroup(1);
   uiRB.BindCallback(TbSwitchMenuBarRadios);

   // create the "script loader" function
   tbScriptSystemButtonID = tbMenuBarUI.CreateRadioButton("ScriptRadioButton", 5 + BUTTON_WIDTH + 3, 5,
                                                           LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiRB.Object(tbScriptSystemButtonID);
   uiRB.SetParent(tbMenuBarPlateID);
   uiRB.SetCaption(CENTERED, 0, 0, "Script", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetGroup(1);
   uiRB.BindCallback(TbSwitchMenuBarRadios);
   uiRB.Show(FALSE);

   // create the "Model Viewer" function
   tbModelSystemButtonID = tbMenuBarUI.CreateRadioButton("ModelRadioButton", 5 + BUTTON_WIDTH + 3, 5,
                                                          LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiRB.Object(tbModelSystemButtonID);
   uiRB.SetParent(tbMenuBarPlateID);
   uiRB.SetCaption(CENTERED, 0, 0, "Models", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetGroup(1);
   uiRB.BindCallback(TbSwitchMenuBarRadios);
   uiRB.Show(FALSE);

   // create the "Actor Viewer" function
   tbActorSystemButtonID = tbMenuBarUI.CreateRadioButton("ActorRadioButton", 5 + BUTTON_WIDTH + 3, 5,
                                                          LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiRB.Object(tbActorSystemButtonID);
   uiRB.SetParent(tbMenuBarPlateID);
   uiRB.SetCaption(CENTERED, 0, 0, "Actors", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetGroup(1);
   uiRB.BindCallback(TbSwitchMenuBarRadios);
   uiRB.Show(FALSE);

   // create the "Working Environment" function
   tbEnvironmentSystemButtonID = tbMenuBarUI.CreateRadioButton("EnvironmentRadioButton", 5 + BUTTON_WIDTH + 3, 5,
                                                          LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiRB.Object(tbEnvironmentSystemButtonID);
   uiRB.SetParent(tbMenuBarPlateID);
   uiRB.SetCaption(CENTERED, 0, 0, "Environment", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetGroup(1);
   uiRB.BindCallback(TbSwitchMenuBarRadios);
   uiRB.Show(FALSE);

   // create the "Face Tools" function
   tbFaceSystemButtonID = tbMenuBarUI.CreateRadioButton("FaceRadioButton", 5 + BUTTON_WIDTH + 3, 5,
                                                        LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiRB.Object(tbFaceSystemButtonID);
   uiRB.SetParent(tbMenuBarPlateID);
   uiRB.SetCaption(CENTERED, 0, 0, "Faces", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetGroup(1);
   uiRB.BindCallback(TbSwitchMenuBarRadios);
   uiRB.Show(FALSE);

   // create the "Aux View" function
   tbOrthoButtonID = tbMenuBarUI.CreateRadioButton("OrthoButton", tbMenuBarWidth - BUTTON_WIDTH*3 - 5 - 6, 5, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiRB.Object(tbOrthoButtonID);
   uiRB.SetParent(tbMenuBarPlateID);
   uiRB.SetCaption(CENTERED, 0, 0, "Ortho View", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   if (tbRenderCamera == PERSPECTIVE) {
      uiRB.ChangeState(FALSE);
   }
   else {
      uiRB.ChangeState(TRUE);
   }
   uiRB.BindCallback(TbShowOrthoView);

   tbAuxButtonID = tbMenuBarUI.CreateRadioButton("AuxButton", tbMenuBarWidth - BUTTON_WIDTH*2 - 5 - 3, 5, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiRB.Object(tbAuxButtonID);
   uiRB.SetParent(tbMenuBarPlateID);
   uiRB.SetCaption(CENTERED, 0, 0, "Aux View", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.ChangeState(tbShowAux);
   uiRB.BindCallback(TbShowAuxView);

   // create the "Exit" function
   tbExitButtonID = tbMenuBarUI.CreateButton("ExitButton", tbMenuBarWidth - BUTTON_WIDTH - 5, 5, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbExitButtonID);
   uiB.SetParent(tbMenuBarPlateID);
   uiB.SetCaption(CENTERED, 0, 0, "Exit", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.BindCallback(TbOpenQuit);
}


/*--------------------
  create main menu UI
  C.Wang 0430, 2009
 ---------------------*/
void TbCreateMenuUI()
{
   FnUiPlate uiP;
   FnUiButton uiB;
   FnUiRadioButton uiRB;
   FnUiTypein uiTy;
   int ox, oy;
   char value[256];

   // create the menu Ui system
   tbMenuID = FyBeginUISystem(tbWorldID, tbMenuOX, tbMenuOY, tbMenuWidth, tbMenuHeight);
   tbMenuUI.Object(tbMenuID);
   tbMenuUI.SetBackgroundColor(0.45f, 0.45f, 0.45f);
   tbAllUISysID[tbNumUISys++] = tbMenuID;

   // create the plates
   tbControlPanelPlateUI = tbMenuUI.CreatePlate("Control Panel", 0, 0, tbMenuWidth - 1, tbMenuHeight - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.Object(tbControlPanelPlateUI);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);

   // create the play/stop buttons
   ox = tbMenuWidth - tbSideMenuWidth;
   oy = 10;
   tbStopID = tbMenuUI.CreateButton("StopButton", ox, oy, SHORT_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbStopID);
   uiB.SetCaption(CENTERED, 0, 0, "¡½", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(1);
   uiB.SetParent(tbControlPanelPlateUI);
   uiB.UseChineseFontSet(TRUE);
   uiB.BindCallback(TbStopPlay);

   ox += SHORT_BUTTON_WIDTH + 3;
   tbPlayID = tbMenuUI.CreateRadioButton("PlayButton", ox, oy, SHORT_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiRB.Object(tbPlayID);
   uiRB.SetCaption(CENTERED, 0, 0, ">", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetLayer(1);
   uiRB.SetParent(tbControlPanelPlateUI);
   uiRB.BindCallback(TbMakePlay);

   oy -= 5;
   ox += SHORT_BUTTON_WIDTH + 3;
   tbPlaySpeedID = tbMenuUI.CreateTypein("PlaySpeedTypein", ox, oy, 120, 30, 256, 0.45f, 0.45f, 0.45f);
   uiTy.Object(tbPlaySpeedID);
   uiTy.SetParent(tbControlPanelPlateUI);
   uiTy.SetTypein(LEFT_CENTERED, 50, 2, 50);
   uiTy.SetCaption(LEFT_CENTERED, 12, 2, "Speed", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   sprintf(value, "%5.3f", tbPlaySpeed);
   uiTy.PasteTypeinValue(value);
   uiTy.BindCallbacks(TbPlaySpeedUICommand);

   // create the UIs for seting up the directories
   int objectPanelWidth = 450;
   ox = 5;
   oy = 2;
   tbEnvironmentPlanelPlateUI = tbMenuUI.CreatePlate("Directory Panel", ox, oy, objectPanelWidth, tbMenuHeight - 5, 0, 0.45f, 0.45f, 0.45f);
   uiP.Object(tbEnvironmentPlanelPlateUI);
   uiP.SetParent(tbControlPanelPlateUI);
   uiP.SetLayer(1);
   uiP.Show(FALSE);

   tbWorkDirTypeinUI = tbMenuUI.CreateTypein("WorkEnvTypein", ox, oy, objectPanelWidth - 20, 30, 256, 0.45f, 0.45f, 0.45f);
   uiTy.Object(tbWorkDirTypeinUI);
   uiTy.SetParent(tbEnvironmentPlanelPlateUI);
   uiTy.SetTypein(LEFT_CENTERED, 70, 5, objectPanelWidth - 100);
   uiTy.SetCaption(LEFT_CENTERED, 5, 5, "Working >", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.BindCallbacks(TbSetDir);

   // get working directory and paste it on UI
   char dir[256];
   GetCurrentDirectory(256, dir);
   uiTy.PasteTypeinValue(dir);
   strcpy(tbWorkDir, dir);

   oy += 25;
   char *dirS = tbWorld.GetObjectPath();
   tbModelDirTypeinUI = tbMenuUI.CreateTypein("WorkModelEnvTypein", ox, oy, objectPanelWidth - 20, 30, 256, 0.45f, 0.45f, 0.45f);
   uiTy.Object(tbModelDirTypeinUI);
   uiTy.SetParent(tbEnvironmentPlanelPlateUI);
   uiTy.SetTypein(LEFT_CENTERED, 70, 5, objectPanelWidth - 100);
   uiTy.SetCaption(LEFT_CENTERED, 5, 5, "Model >", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue(dirS);
   uiTy.BindCallbacks(TbSetDir);

   oy += 25;
   dirS = tbWorld.GetTexturePath();
   tbTextureDirTypeinUI = tbMenuUI.CreateTypein("WorkTextureEnvTypein", ox, oy, objectPanelWidth - 20, 30, 256, 0.45f, 0.45f, 0.45f);
   uiTy.Object(tbTextureDirTypeinUI);
   uiTy.SetParent(tbEnvironmentPlanelPlateUI);
   uiTy.SetTypein(LEFT_CENTERED, 70, 5, objectPanelWidth - 100);
   uiTy.SetCaption(LEFT_CENTERED, 5, 5, "Texture >", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue(dirS);
   uiTy.BindCallbacks(TbSetDir);

   oy += 25;
   dirS = tbWorld.GetShaderPath();
   tbShaderDirTypeinUI = tbMenuUI.CreateTypein("WorkShaderEnvTypein", ox, oy, objectPanelWidth - 20, 30, 256, 0.45f, 0.45f, 0.45f);
   uiTy.Object(tbShaderDirTypeinUI);
   uiTy.SetParent(tbEnvironmentPlanelPlateUI);
   uiTy.SetTypein(LEFT_CENTERED, 70, 5, objectPanelWidth - 100);
   uiTy.SetCaption(LEFT_CENTERED, 5, 5, "Shader >", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue(dirS);
   uiTy.BindCallbacks(TbSetDir);

   oy += 25;
   dirS = tbWorld.GetCharacterPath();
   tbActorDirTypeinUI = tbMenuUI.CreateTypein("WorkActorEnvTypein", ox, oy, objectPanelWidth - 20, 30, 256, 0.45f, 0.45f, 0.45f);
   uiTy.Object(tbActorDirTypeinUI);
   uiTy.SetParent(tbEnvironmentPlanelPlateUI);
   uiTy.SetTypein(LEFT_CENTERED, 70, 5, objectPanelWidth - 100);
   uiTy.SetCaption(LEFT_CENTERED, 5, 5, "Actor >", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue(dirS);
   uiTy.BindCallbacks(TbSetDir);
}


/*--------------------
  create side menu UI
  C.Wang 0614, 2010
 ---------------------*/
void TbCreateSideMenuUI()
{
   int ox, oy;
   FnUiPlate uiP;
   FnUiButton uiB;
   FnUiRadioButton uiRB;
   FnUiTab uiT;
   FnUiCombo uiC;
   FnUiTypein uiTy;

   // create the menu Ui system
   tbSideMenuID = FyBeginUISystem(tbWorldID, tbSideMenuOX, tbSideMenuOY, tbSideMenuWidth, tbSideMenuHeight);
   tbSideMenuUI.Object(tbSideMenuID);
   tbSideMenuUI.SetBackgroundColor(0.45f, 0.45f, 0.45f);
   tbAllUISysID[tbNumUISys++] = tbSideMenuID;

   // create the UIs
   tbSideMenuPlateID = tbSideMenuUI.CreatePlate("SideMenuPlate", 0, 0, tbSideMenuWidth - 1, tbSideMenuHeight - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.Object(tbSideMenuPlateID);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);
   uiP.Show(TRUE);

   // for system menu
   tbSideMenuSystemPlateID = tbSideMenuUI.CreatePlate("SystemSideMenuPlate", 0, 0, tbSideMenuWidth - 1, tbSideMenuHeight - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.Object(tbSideMenuSystemPlateID);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);
   uiP.Show(FALSE);

   ox = 5;
   oy = 5;
   tbNewButtonID = tbSideMenuUI.CreateButton("NewButton", ox, oy, LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbNewButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "New", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(1);
   uiB.SetParent(tbSideMenuSystemPlateID);
   uiB.BindCallback(TbOpenNew);

   oy += 23;
   tbScriptButtonID = tbSideMenuUI.CreateButton("ScriptButton", ox, oy, LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbScriptButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Load Script", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(1);
   uiB.SetParent(tbSideMenuSystemPlateID);
   uiB.BindCallback(TbScriptUICommand);

   oy += 23;
   tbModelButtonID = tbSideMenuUI.CreateButton("ModelButton", ox, oy, LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbModelButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Model Viewer", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(1);
   uiB.SetParent(tbSideMenuSystemPlateID);
   uiB.BindCallback(TbModelUICommand);

   oy += 23;
   tbActorButtonID = tbSideMenuUI.CreateButton("ActorButton", ox, oy, LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbActorButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Actor Viewer", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(1);
   uiB.SetParent(tbSideMenuSystemPlateID);
   uiB.BindCallback(TbActorUICommand);

   oy += 23;
   tbEnvironmentButtonID = tbSideMenuUI.CreateButton("EnvironmentButton", ox, oy, LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbEnvironmentButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Environment", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(1);
   uiB.SetParent(tbSideMenuSystemPlateID);
   uiB.BindCallback(TbEnvironmentUICommand);

   oy += 23;
   tbFaceButtonID = tbSideMenuUI.CreateButton("FaceButton", ox, oy, LONG_BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbFaceButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Face Tool", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(1);
   uiB.SetParent(tbSideMenuSystemPlateID);
   uiB.BindCallback(TbFaceUICommand);

   // for script menu
   tbSideMenuScriptPlateID = tbSideMenuUI.CreatePlate("ScriptSideMenuPlate", 0, 0, tbSideMenuWidth - 1, tbSideMenuHeight - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.Object(tbSideMenuScriptPlateID);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);
   uiP.Show(FALSE);

   ox = 5;
   oy = 5;
   tbLoadScriptButtonID = tbSideMenuUI.CreateButton("LoadScriptButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbLoadScriptButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Load", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(1);
   uiB.SetParent(tbSideMenuScriptPlateID);
   uiB.BindCallback(TbLoadScriptUICommand);

   // for model menu
   tbSideMenuModelPlateID = tbSideMenuUI.CreatePlate("ModelSideMenuPlate", 0, 0, tbSideMenuWidth - 1, tbSideMenuHeight - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.Object(tbSideMenuModelPlateID);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);
   uiP.Show(FALSE);

   ox = 2;
   oy = 2;
   tbModelFileTabID = tbSideMenuUI.CreateTab("ModelFileTab", ox, oy, tbSideMenuWidth - 5, 75, 1, 0.45f, 0.45f, 0.45f, 1.0f, FALSE, TRUE);
   uiT.Object(tbModelFileTabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuModelPlateID);
   uiT.SetCaption("Models", 0.0f, 0.0f, 0.0f);
   uiT.Open(TRUE);

   ox = 15;
   oy = 25;
   tbLoadModelButtonID = tbSideMenuUI.CreateButton("LoadModelButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbLoadModelButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Load", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbModelFileTabID);
   uiB.BindCallback(TbLoadModelUICommand);

   ox += BUTTON_WIDTH + 5;
   tbPreviousModelButtonID = tbSideMenuUI.CreateButton("PreviousModelButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbPreviousModelButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Previous", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbModelFileTabID);
   uiB.BindCallback(TbPreviousModelUICommand);
   uiB.Enable(FALSE);

   ox += BUTTON_WIDTH + 5;
   tbNextModelButtonID = tbSideMenuUI.CreateButton("NextModelButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbNextModelButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Next", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbModelFileTabID);
   uiB.BindCallback(TbNextModelUICommand);
   uiB.Enable(FALSE);

   ox = 15;
   oy += 23;
   tbSaveModelButtonID = tbSideMenuUI.CreateButton("SaveModelButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbSaveModelButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Save", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbModelFileTabID);
   uiB.BindCallback(TbSaveModelUICommand);
   uiB.Enable(FALSE);

   ox += (BUTTON_WIDTH + 5)*2;
   tbDeleteModelButtonID = tbSideMenuUI.CreateButton("DeleteModelButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbDeleteModelButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Delete", 1.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbModelFileTabID);
   uiB.BindCallback(TbDeleteModelUICommand);
   uiB.Enable(FALSE);

   ox = 2;
   oy = 2 + 76;
   tbModelCurrentTabID = tbSideMenuUI.CreateTab("CurrentModelTab", ox, oy, tbSideMenuWidth - 5, 250, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.Object(tbModelCurrentTabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuModelPlateID);
   uiT.SetCaption("Current Model Material", 0.0f, 0.0f, 0.0f);
   uiT.Open(TRUE);

   ox = 2;
   oy = 2 + 76 + 251;
   tbTexture0TabID = tbSideMenuUI.CreateTab("Texture0Tab", ox, oy, tbSideMenuWidth - 5, 54, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.Object(tbTexture0TabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuModelPlateID);
   uiT.SetCaption("Texture Layer 0", 0.0f, 0.0f, 0.0f);
   uiT.Open(FALSE);

   ox = 2;
   oy = 2 + 76 + 251 + 55;
   tbTexture1TabID = tbSideMenuUI.CreateTab("Texture1Tab", ox, oy, tbSideMenuWidth - 5, 54, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.Object(tbTexture1TabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuModelPlateID);
   uiT.SetCaption("Texture Layer 1", 0.0f, 0.0f, 0.0f);
   uiT.Open(FALSE);

   ox = 2;
   oy = 2 + 76 + 251 + 55 + 55;
   tbTexture2TabID = tbSideMenuUI.CreateTab("Texture2Tab", ox, oy, tbSideMenuWidth - 5, 54, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.Object(tbTexture2TabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuModelPlateID);
   uiT.SetCaption("Texture Layer 2", 0.0f, 0.0f, 0.0f);
   uiT.Open(FALSE);

   ox = 2;
   oy = 2 + 76 + 251 + 55 + 55 + 55;
   tbTexture3TabID = tbSideMenuUI.CreateTab("Texture3Tab", ox, oy, tbSideMenuWidth - 5, 54, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.Object(tbTexture3TabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuModelPlateID);
   uiT.SetCaption("Texture Layer 3", 0.0f, 0.0f, 0.0f);
   uiT.Open(FALSE);

   ox = 15;
   oy = 25;
   tbWireframeCurModelButtonID = tbSideMenuUI.CreateRadioButton("WireframeModelButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f, NONE, CHECK_BOX);
   uiRB.Object(tbWireframeCurModelButtonID);
   uiRB.SetCaption(LEFT_CENTERED, 25, 0, "Wireframe", 0.0f, 0.0f, 0.0f);
   uiRB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiRB.SetLayer(2);
   uiRB.SetParent(tbModelCurrentTabID);
   uiRB.BindCallback(TbWireframeModelUICommand);
   uiRB.Enable(FALSE);

   ox += BUTTON_WIDTH + 5;
   tbUpdateMaterialID = tbSideMenuUI.CreateButton("UpdateMaterialButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[29] = tbUpdateMaterialID;
   uiB.Object(tbUpdateMaterialID);
   uiB.SetCaption(CENTERED, 0, 0, "Update", 1.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbModelCurrentTabID);
   uiB.BindCallback(TbUpdateMaterialUICommand);
   uiB.Enable(FALSE);

   ox += BUTTON_WIDTH + 5;
   tbCancelMaterialID = tbSideMenuUI.CreateButton("CancelMaterialButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[30] = tbCancelMaterialID;
   uiB.Object(tbCancelMaterialID);
   uiB.SetCaption(CENTERED, 0, 0, "Reset", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbModelCurrentTabID);
   uiB.BindCallback(TbCancelMaterialEditUICommand);
   uiB.Enable(FALSE);

   ox = 15;
   oy += 30;
   tbSelectMaterialButtonID = tbSideMenuUI.CreateCombo("SelectMaterialCombo", ox, oy, tbSideMenuWidth - ox - ox/3, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                                       tbSideMenuWidth - ox - ox/3 - 60);
   uiC.Object(tbSelectMaterialButtonID);
   uiC.SetLayer(2);
   uiC.SetParent(tbModelCurrentTabID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "Material", 0.0f, 0.0f, 0.0f);
   uiC.BindCallbacks(TbRenameMaterialUICommand, TbMakeCurrentMaterialUICommand);
   uiC.Enable(FALSE);

   oy += 22;
   tbShaderFileID = tbSideMenuUI.CreateTypein("ShaderFileTypein", ox, oy, tbSideMenuWidth - ox - ox/3 - 45, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[0] = tbShaderFileID;
   uiTy.Object(tbShaderFileID);
   uiTy.SetParent(tbModelCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, tbSideMenuWidth - ox - ox/3 - 65 - 40);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Shader", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += tbSideMenuWidth - ox - ox/3 - 45;
   oy += 4;
   tbShaderLoaderID = tbSideMenuUI.CreateButton("AddShaderButton", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[1] = tbShaderLoaderID;
   uiB.Object(tbShaderLoaderID);
   uiB.SetCaption(CENTERED, 0, 0, "+", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbModelCurrentTabID);
   uiB.BindCallback(TbAddShaderData);
   uiB.Enable(FALSE);

   ox += 20;
   tbShaderRemoveID = tbSideMenuUI.CreateButton("RemoveShaderButton", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[2] = tbShaderRemoveID;
   uiB.Object(tbShaderRemoveID);
   uiB.SetCaption(CENTERED, 0, 0, "-", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbModelCurrentTabID);
   uiB.BindCallback(TbClearShaderData);
   uiB.Enable(FALSE);

   ox = 15;
   oy += 22;
   tbEffectID = tbSideMenuUI.CreateCombo("SelectEffectCombo", ox, oy, tbSideMenuWidth - ox - ox/3, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                          tbSideMenuWidth - ox - ox/3 - 60);
   tbMatUI[3] = tbEffectID;
   uiC.Object(tbEffectID);
   uiC.SetLayer(2);
   uiC.SetParent(tbModelCurrentTabID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "Effect", 0.0f, 0.0f, 0.0f);
   uiC.BindCallbacks(TbMakeUpdateDirtyC, TbMakeUpdateDirtyC);
   uiC.Enable(FALSE);

   ox = 15;
   oy += 22;
   tbAmbientRID = tbSideMenuUI.CreateTypein("RedAmbientTypein", ox, oy, 55 + 40 + 14, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[4] = tbAmbientRID;
   uiTy.Object(tbAmbientRID);
   uiTy.SetParent(tbModelCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, 50);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Ambient", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += 55 + 40 + 14;
   tbAmbientGID = tbSideMenuUI.CreateTypein("GreenAmbientTypein", ox, oy, 50, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[5] = tbAmbientGID;
   uiTy.Object(tbAmbientGID);
   uiTy.SetParent(tbModelCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 50);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += 55;
   tbAmbientBID = tbSideMenuUI.CreateTypein("BlueAmbientTypein", ox, oy, 50, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[6] = tbAmbientBID;
   uiTy.Object(tbAmbientBID);
   uiTy.SetParent(tbModelCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 50);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox = 15;
   oy += 23;
   tbDiffuseRID = tbSideMenuUI.CreateTypein("RedDiffuseTypein", ox, oy, 55 + 40 + 14, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[7] = tbDiffuseRID;
   uiTy.Object(tbDiffuseRID);
   uiTy.SetParent(tbModelCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, 50);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Diffuse", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += 55 + 40 + 14;
   tbDiffuseGID = tbSideMenuUI.CreateTypein("GreenDiffuseTypein", ox, oy, 50, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[8] = tbDiffuseGID;
   uiTy.Object(tbDiffuseGID);
   uiTy.SetParent(tbModelCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 50);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += 55;
   tbDiffuseBID = tbSideMenuUI.CreateTypein("BlueDiffuseTypein", ox, oy, 50, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[9] = tbDiffuseBID;
   uiTy.Object(tbDiffuseBID);
   uiTy.SetParent(tbModelCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 50);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox = 15;
   oy += 23;
   tbSpecularRID = tbSideMenuUI.CreateTypein("RedSpecularTypein", ox, oy, 55 + 40 + 14, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[10] = tbSpecularRID;
   uiTy.Object(tbSpecularRID);
   uiTy.SetParent(tbModelCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, 50);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Specular", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += 55 + 40 + 14;
   tbSpecularGID = tbSideMenuUI.CreateTypein("GreenSpecularTypein", ox, oy, 50, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[11] = tbSpecularGID;
   uiTy.Object(tbSpecularGID);
   uiTy.SetParent(tbModelCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 50);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += 55;
   tbSpecularBID = tbSideMenuUI.CreateTypein("BlueSpecularTypein", ox, oy, 50, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[12] = tbSpecularBID;
   uiTy.Object(tbSpecularBID);
   uiTy.SetParent(tbModelCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 50);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox = 15;
   oy += 23;
   tbShinenessID = tbSideMenuUI.CreateTypein("ShinenessTypein", ox, oy, 55 + 90 + 14, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[13] = tbShinenessID;
   uiTy.Object(tbShinenessID);
   uiTy.SetParent(tbModelCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, 100);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Power", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox = 15;
   oy += 23;
   tbEmissiveRID = tbSideMenuUI.CreateTypein("RedEmissiveTypein", ox, oy, 55 + 40 + 14, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[14] = tbEmissiveRID;
   uiTy.Object(tbEmissiveRID);
   uiTy.SetParent(tbModelCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, 50);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Emissive", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += 55 + 40 + 14;
   tbEmissiveGID = tbSideMenuUI.CreateTypein("GreenEmissiveTypein", ox, oy, 50, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[15] = tbEmissiveGID;
   uiTy.Object(tbEmissiveGID);
   uiTy.SetParent(tbModelCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 50);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += 55;
   tbEmissiveBID = tbSideMenuUI.CreateTypein("BlueEmissiveTypein", ox, oy, 50, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[16] = tbEmissiveBID;
   uiTy.Object(tbEmissiveBID);
   uiTy.SetParent(tbModelCurrentTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 1, 5, 50);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox = 15;
   oy = 25;
   tbTexture0ID = tbSideMenuUI.CreateTypein("Texture0Typein", ox, oy, tbSideMenuWidth - ox - ox/3 - 45, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[17] = tbTexture0ID;
   uiTy.Object(tbTexture0ID);
   uiTy.SetParent(tbTexture0TabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, tbSideMenuWidth - ox - ox/3 - 65 - 40);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Texture", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += tbSideMenuWidth - ox - ox/3 - 45;
   oy += 4;
   tbTexture0AddID = tbSideMenuUI.CreateButton("AddTexture0Button", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[18] = tbTexture0AddID;
   uiB.Object(tbTexture0AddID);
   uiB.SetCaption(CENTERED, 0, 0, "+", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbTexture0TabID);
   uiB.BindCallback(TbAddTextureData);
   uiB.Enable(FALSE);

   ox += 20;
   tbTexture0RemoveID = tbSideMenuUI.CreateButton("RemoveTexture0Button", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[19] = tbTexture0RemoveID;
   uiB.Object(tbTexture0RemoveID);
   uiB.SetCaption(CENTERED, 0, 0, "-", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbTexture0TabID);
   uiB.BindCallback(TbClearTextureData);
   uiB.Enable(FALSE);

   ox = 15;
   oy = 25;
   tbTexture1ID = tbSideMenuUI.CreateTypein("Texture1Typein", ox, oy, tbSideMenuWidth - ox - ox/3 - 45, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[20] = tbTexture1ID;
   uiTy.Object(tbTexture1ID);
   uiTy.SetParent(tbTexture1TabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, tbSideMenuWidth - ox - ox/3 - 65 - 40);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Texture", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += tbSideMenuWidth - ox - ox/3 - 45;
   oy += 4;
   tbTexture1AddID = tbSideMenuUI.CreateButton("AddTexture1Button", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[21] = tbTexture1AddID;
   uiB.Object(tbTexture1AddID);
   uiB.SetCaption(CENTERED, 0, 0, "+", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbTexture1TabID);
   uiB.BindCallback(TbAddTextureData);
   uiB.Enable(FALSE);

   ox += 20;
   tbTexture1RemoveID = tbSideMenuUI.CreateButton("RemoveTexture1Button", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[22] = tbTexture1RemoveID;
   uiB.Object(tbTexture1RemoveID);
   uiB.SetCaption(CENTERED, 0, 0, "-", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbTexture1TabID);
   uiB.BindCallback(TbClearTextureData);
   uiB.Enable(FALSE);

   ox = 15;
   oy = 25;
   tbTexture2ID = tbSideMenuUI.CreateTypein("Texture2Typein", ox, oy, tbSideMenuWidth - ox - ox/3 - 45, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[23] = tbTexture2ID;
   uiTy.Object(tbTexture2ID);
   uiTy.SetParent(tbTexture2TabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, tbSideMenuWidth - ox - ox/3 - 65 - 40);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Texture", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += tbSideMenuWidth - ox - ox/3 - 45;
   oy += 4;
   tbTexture2AddID = tbSideMenuUI.CreateButton("AddTexture2Button", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[24] = tbTexture2AddID;
   uiB.Object(tbTexture2AddID);
   uiB.SetCaption(CENTERED, 0, 0, "+", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbTexture2TabID);
   uiB.BindCallback(TbAddTextureData);
   uiB.Enable(FALSE);

   ox += 20;
   tbTexture2RemoveID = tbSideMenuUI.CreateButton("RemoveTexture2Button", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[25] = tbTexture2RemoveID;
   uiB.Object(tbTexture2RemoveID);
   uiB.SetCaption(CENTERED, 0, 0, "-", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbTexture2TabID);
   uiB.BindCallback(TbClearTextureData);
   uiB.Enable(FALSE);

   ox = 15;
   oy = 25;
   tbTexture3ID = tbSideMenuUI.CreateTypein("Texture3Typein", ox, oy, tbSideMenuWidth - ox - ox/3 - 45, 25, 256, 0.45f, 0.45f, 0.45f);
   tbMatUI[26] = tbTexture3ID;
   uiTy.Object(tbTexture3ID);
   uiTy.SetParent(tbTexture3TabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, tbSideMenuWidth - ox - ox/3 - 65 - 40);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Texture", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += tbSideMenuWidth - ox - ox/3 - 45;
   oy += 4;
   tbTexture3AddID = tbSideMenuUI.CreateButton("AddTexture3Button", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[27] = tbTexture3AddID;
   uiB.Object(tbTexture3AddID);
   uiB.SetCaption(CENTERED, 0, 0, "+", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbTexture3TabID);
   uiB.BindCallback(TbAddTextureData);
   uiB.Enable(FALSE);

   ox += 20;
   tbTexture3RemoveID = tbSideMenuUI.CreateButton("RemoveTexture3Button", ox, oy, 16, 16, 1, 0.45f, 0.45f, 0.45f);
   tbMatUI[28] = tbTexture3RemoveID;
   uiB.Object(tbTexture3RemoveID);
   uiB.SetCaption(CENTERED, 0, 0, "-", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbTexture3TabID);
   uiB.BindCallback(TbClearTextureData);
   uiB.Enable(FALSE);

   // setup the connection between tab UIs
   uiT.Object(tbModelFileTabID);
   uiT.NextTab(tbModelCurrentTabID);
   uiT.Object(tbModelCurrentTabID);
   uiT.NextTab(tbTexture0TabID);
   uiT.Object(tbTexture0TabID);
   uiT.NextTab(tbTexture1TabID);
   uiT.Object(tbTexture1TabID);
   uiT.NextTab(tbTexture2TabID);
   uiT.Object(tbTexture2TabID);
   uiT.NextTab(tbTexture3TabID);

   // for actor menu
   tbSideMenuActorPlateID = tbSideMenuUI.CreatePlate("ActorSideMenuPlate", 0, 0, tbSideMenuWidth - 1, tbSideMenuHeight - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.Object(tbSideMenuActorPlateID);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);
   uiP.Show(FALSE);

   ox = 2;
   oy = 2;
   tbActorFileTabID = tbSideMenuUI.CreateTab("ActorFileTab", ox, oy, tbSideMenuWidth - 5, 75, 1, 0.45f, 0.45f, 0.45f, 1.0f, FALSE, TRUE);
   uiT.Object(tbActorFileTabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuActorPlateID);
   uiT.SetCaption("Actors", 0.0f, 0.0f, 0.0f);
   uiT.Open(TRUE);

   ox = 15;
   oy = 25;
   tbLoadActorButtonID = tbSideMenuUI.CreateButton("LoadActorButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbLoadActorButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Load", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(1);
   uiB.SetParent(tbActorFileTabID);
   uiB.BindCallback(TbLoadActorUICommand);

   ox = 15;
   oy += 23;
   tbSaveActorButtonID = tbSideMenuUI.CreateButton("SaveActorButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbSaveActorButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Save", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbActorFileTabID);
   uiB.BindCallback(TbSaveActorUICommand);
   uiB.Enable(FALSE);

   ox += (BUTTON_WIDTH + 5)*2;
   tbDeleteActorButtonID = tbSideMenuUI.CreateButton("DeleteActorButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbDeleteActorButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Delete", 1.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbActorFileTabID);
   uiB.BindCallback(TbDeleteActorUICommand);
   uiB.Enable(FALSE);

   ox = 2;
   oy = 2 + 76;
   tbActorCurrentTabID = tbSideMenuUI.CreateTab("CurrentActorTab", ox, oy, tbSideMenuWidth - 5, 125, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.Object(tbActorCurrentTabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuActorPlateID);
   uiT.SetCaption("Current Actor", 0.0f, 0.0f, 0.0f);
   uiT.Open(TRUE);

   ox = 15;
   oy = 25;
   tbSelectActorComboID = tbSideMenuUI.CreateCombo("SelectActorCombo", ox, oy, tbSideMenuWidth - ox - ox/3, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                                    tbSideMenuWidth - ox - ox/3 - 60);
   uiC.Object(tbSelectActorComboID);
   uiC.SetLayer(2);
   uiC.SetParent(tbActorCurrentTabID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "Actor >", 0.0f, 0.0f, 0.0f);
   uiC.BindCallbacks(NULL, TbMakeCurrentActorUICommand);
   uiC.Enable(FALSE);

   ox = 15;
   oy += 23;
   tbSelectBodyComboID = tbSideMenuUI.CreateCombo("SelectBodyCombo", ox, oy, tbSideMenuWidth - ox - ox/3, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                                   tbSideMenuWidth - ox - ox/3 - 60);
   uiC.Object(tbSelectBodyComboID);
   uiC.SetLayer(2);
   uiC.SetParent(tbActorCurrentTabID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "   Body", 0.0f, 0.0f, 0.0f);
   uiC.BindCallbacks(NULL, TbMakeCurrentBodyUICommand);
   uiC.Enable(FALSE);

   ox = 15;
   oy += 23;
   tbSelectActionComboID = tbSideMenuUI.CreateCombo("SelectActionCombo", ox, oy, tbSideMenuWidth - ox - ox/3, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                                   tbSideMenuWidth - ox - ox/3 - 60);
   uiC.Object(tbSelectActionComboID);
   uiC.SetLayer(2);
   uiC.SetParent(tbActorCurrentTabID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "   Action", 0.0f, 0.0f, 0.0f);
   uiC.BindCallbacks(TbRenameActionUICommand, TbSelectCurrentBodyActionUICommand);
   uiC.Enable(FALSE);

   ox = 15;
   oy += 28;
   tbEditActionButtonID = tbSideMenuUI.CreateButton("EditButtonButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbEditActionButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Edit Action", 0.0f, 1.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbActorCurrentTabID);
   uiB.BindCallback(TbOpenAddAction);
   uiB.Enable(FALSE);

   ox += 5 + BUTTON_WIDTH;
   tbAddActionButtonID = tbSideMenuUI.CreateButton("AddButtonButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbAddActionButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Add Action", 0.0f, 0.0f, 1.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbActorCurrentTabID);
   uiB.BindCallback(TbOpenAddAction);
   uiB.Enable(FALSE);

   ox += 5 + BUTTON_WIDTH;
   tbDeleteActionButtonID = tbSideMenuUI.CreateButton("AddButtonButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbDeleteActionButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Delete Action", 1.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbActorCurrentTabID);
   uiB.BindCallback(TbDeleteActionUICommand);
   uiB.Enable(FALSE);

   ox = 2;
   oy = 2 + 76 + 76;
   tbPoseExploreTabID = tbSideMenuUI.CreateTab("PoseViewerTab", ox, oy, tbSideMenuWidth - 5, 103, 1, 0.45f, 0.45f, 0.45f, 1.0f, TRUE, TRUE);
   uiT.Object(tbPoseExploreTabID);
   uiT.SetLayer(1);
   uiT.SetParent(tbSideMenuActorPlateID);
   uiT.SetCaption("Pose Viewer", 0.0f, 0.0f, 0.0f);
   uiT.Open(FALSE);

   ox = 15;
   oy = 25;
   tbSelectPoseComboID = tbSideMenuUI.CreateCombo("SelectPoseCombo", ox, oy, tbSideMenuWidth - 2*ox - BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f, 1.0f,
                                                   tbSideMenuWidth - ox - ox/3 - 60, 20);
   uiC.Object(tbSelectPoseComboID);
   uiC.SetLayer(2);
   uiC.SetParent(tbPoseExploreTabID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "   Poses", 0.0f, 0.0f, 0.0f);
   uiC.BindCallbacks(NULL, TbSelectPoseUICommand);
   uiC.Enable(FALSE);

   ox = tbSideMenuWidth - BUTTON_WIDTH - ox;
   tbNewPoseButtonID = tbSideMenuUI.CreateButton("NewPoseButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbNewPoseButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "New Pose", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbPoseExploreTabID);
   uiB.Enable(FALSE);

   ox = 15;
   oy += 23;
   tbPoseStartID = tbSideMenuUI.CreateTypein("StartPoseTypein", ox, oy, 80 + 40 + 14, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.Object(tbPoseStartID);
   uiTy.SetParent(tbPoseExploreTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, 75);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "   Frame", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.PasteTypeinValue("");
   //uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox += 80 + 40 + 14;
   tbPoseEndID = tbSideMenuUI.CreateTypein("EndPoseTypein", ox, oy, 80 + 10, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.Object(tbPoseEndID);
   uiTy.SetParent(tbPoseExploreTabID);
   uiTy.SetLayer(2);
   uiTy.SetTypein(LEFT_CENTERED, 10, 5, 75);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, ":", 0.0f, 0.0f, 0.0f);
   uiTy.PasteTypeinValue("");
   //uiTy.BindCallbacks(NULL, TbMakeUpdateDirtyTy);
   uiTy.Enable(FALSE);

   ox = tbSideMenuWidth - BUTTON_WIDTH - 15;
   oy += 2;
   tbPlayPoseButtonID = tbSideMenuUI.CreateRadioButton("PlayPoseButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbPlayPoseButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Play Pose", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbPoseExploreTabID);
   uiB.BindCallback(TbPlayTestPoseUICommand);
   uiB.Enable(FALSE);

   ox = tbSideMenuWidth - BUTTON_WIDTH - 15;
   oy += 25;
   tbMakeBaseMoveButtonID = tbSideMenuUI.CreateButton("MakeBaseMoveButton", ox, oy, BUTTON_WIDTH, 20, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbMakeBaseMoveButtonID);
   uiB.SetCaption(CENTERED, 0, 0, "Root -> Base", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.SetLayer(2);
   uiB.SetParent(tbPoseExploreTabID);
   uiB.BindCallback(TbMakeBaseMoveUICommand);
   uiB.Enable(FALSE);

   uiT.Object(tbActorFileTabID);
   uiT.NextTab(tbActorCurrentTabID);
   uiT.Object(tbActorCurrentTabID);
   uiT.NextTab(tbPoseExploreTabID);

   // for working environment
   tbSideMenuEnvironmentPlateID = tbSideMenuUI.CreatePlate("EnvironmentSideMenuPlate", 0, 0, tbSideMenuWidth - 1, tbSideMenuHeight - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.Object(tbSideMenuEnvironmentPlateID);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);
   uiP.Show(FALSE);

   // for face tool
   tbSideMenuFacePlateID = tbSideMenuUI.CreatePlate("FaceSideMenuPlate", 0, 0, tbSideMenuWidth - 1, tbSideMenuHeight - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.Object(tbSideMenuFacePlateID);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);
   uiP.Show(FALSE);
}


/*--------------------
  create dialogue UIs
  C.Wang 0514, 2009
 ---------------------*/
void TbCreateDialogues()
{
   FnUiPlate uiP;
   FnUiButton uiB;
   FnUiCombo uiC;
   FnUiTypein uiTy;

   // position the dialogue
   tbWarnWidth = 250;
   tbWarnHeight = 120;
   tbWarnOX = tbBottomVPOX + (tbBottomVPWidth - tbWarnWidth)/2;
   tbWarnOY = tbBottomVPOY + (tbBottomVPHeight - tbWarnHeight)/2;

   // create the menu Ui system
   tbWarnDialogueID = FyBeginUISystem(tbWorldID, tbWarnOX, tbWarnOY, tbWarnWidth, tbWarnHeight);
   tbWarnUI.Object(tbWarnDialogueID);
   tbWarnUI.SetBackgroundColor(0.45f, 0.45f, 0.45f);
   tbAllUISysID[tbNumUISys++] = tbWarnDialogueID;

   // create the plates
   tbWarnMessageID = tbWarnUI.CreatePlate("Warning", 0, 0, tbWarnWidth - 1, tbWarnHeight - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.Object(tbWarnMessageID);
   uiP.SetCaption(LEFT_TOP, 6, 7, "Warning ....", 0.0f, 0.0f, 0.0f);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);
   int vi[10];
   vi[0] = 8; vi[1] = 30;
   vi[2] = tbWarnWidth - 13; vi[3] = 30;
   vi[4] = tbWarnWidth - 13; vi[5] = tbWarnHeight - 40;
   vi[6] = 8; vi[7] = tbWarnHeight - 40;
   vi[8] = 8; vi[9] = 30;
   uiP.Lines(vi, 5, 0.0f, 0.0f, 0.0f);
   uiP.CreateUIMessageSlots(3);

   // create buttons
   tbWarnOKID = tbWarnUI.CreateButton("OK", tbWarnWidth - BUTTON_WIDTH*2 - 20, tbWarnHeight - 30, BUTTON_WIDTH, 22, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbWarnOKID);
   uiB.SetParent(tbWarnMessageID);
   uiB.SetLayer(1);
   uiB.SetCaption(CENTERED, 0, 0, "OK", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);

   tbWarnCancelID = tbWarnUI.CreateButton("Cancel", tbWarnWidth - BUTTON_WIDTH - 10, tbWarnHeight - 30, BUTTON_WIDTH, 22, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbWarnCancelID);
   uiB.SetParent(tbWarnMessageID);
   uiB.SetLayer(1);
   uiB.SetCaption(CENTERED, 0, 0, "Cancel", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.BindCallback(FyStandardPopupQuit);

   // make it a popup UI
   tbWarnUI.BePopUp(TRUE);

   // create the add-action dialogue
   tbAddActionW = 400;
   tbAddActionH = 250;
   tbAddActionOX = tbBottomVPOX + (tbBottomVPWidth - tbAddActionW)/2;
   tbAddActionOY = tbBottomVPOY + (tbBottomVPHeight - tbAddActionH)/2;
   tbAddActionDialogueID = FyBeginUISystem(tbWorldID, tbAddActionOX, tbAddActionOY, tbAddActionW, tbAddActionH);
   tbAddActionUI.Object(tbAddActionDialogueID);
   tbAddActionUI.SetBackgroundColor(0.45f, 0.45f, 0.45f);
   tbAllUISysID[tbNumUISys++] = tbAddActionDialogueID;

   // create the plates
   tbAddActionBackgroundID = tbAddActionUI.CreatePlate("AddNewAction", 0, 0, tbAddActionW - 1, tbAddActionH - 1, 0, 0.45f, 0.45f, 0.45f);
   uiP.Object(tbAddActionBackgroundID);
   uiP.SetCaption(LEFT_TOP, 6, 7, "Add New Action", 0.0f, 0.0f, 0.0f);
   uiP.SetBorder(SHADOW_BORDER, 0.8f, 0.8f, 0.8f);
   vi[0] = 8; vi[1] = 30;
   vi[2] = tbAddActionW - 13; vi[3] = 30;
   vi[4] = tbAddActionW - 13; vi[5] = tbAddActionH - 40;
   vi[6] = 8; vi[7] = tbAddActionH - 40;
   vi[8] = 8; vi[9] = 30;
   uiP.Lines(vi, 5, 0.0f, 0.0f, 0.0f);

   tbAddActionOKID = tbAddActionUI.CreateButton("AActionOK", tbAddActionW - BUTTON_WIDTH*2 - 20, tbAddActionH - 30, BUTTON_WIDTH, 22, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbAddActionOKID);
   uiB.SetParent(tbAddActionBackgroundID);
   uiB.SetLayer(2);
   uiB.SetCaption(CENTERED, 0, 0, "OK", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);

   tbAddActionCancelID = tbAddActionUI.CreateButton("AActionCancel", tbAddActionW - BUTTON_WIDTH - 10, tbAddActionH - 30, BUTTON_WIDTH, 22, 1, 0.45f, 0.45f, 0.45f);
   uiB.Object(tbAddActionCancelID);
   uiB.SetParent(tbAddActionBackgroundID);
   uiB.SetLayer(2);
   uiB.SetCaption(CENTERED, 0, 0, "Cancel", 0.0f, 0.0f, 0.0f);
   uiB.SetBorder(SINGLE_BORDER, 0.0f, 0.0f, 0.0f);
   uiB.BindCallback(FyStandardPopupQuit);

   int ox, oy;

   ox = 15;
   oy = 40;
   tbAddActionNameID = tbAddActionUI.CreateTypein("AActionName", ox, oy, tbAddActionW - 30, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.Object(tbAddActionNameID);
   uiTy.SetParent(tbAddActionBackgroundID);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, tbAddActionW - 100);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "  Name", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);

   ox = 15;
   oy += 26;
   tbAddActionTypeID = tbAddActionUI.CreateCombo("AActionType", ox, oy, tbAddActionW - 35, 20, 2, 0.45f, 0.45f, 0.45f, 1.0f,
                                                 tbAddActionW - ox - ox/3 - 90, 3, FALSE);
   uiC.Object(tbAddActionTypeID);
   uiC.SetLayer(3);
   uiC.SetParent(tbAddActionBackgroundID);
   uiC.SetCaption(LEFT_CENTERED, 2, 5, "  Type", 0.0f, 0.0f, 0.0f);
   uiC.InsertMessage(0, "Simple Action");
   uiC.InsertMessage(1, "Cross Fade Blending");
   uiC.InsertMessage(2, "Connection");
   uiC.PasteComboValue("Simple Action");
   uiC.BindCallbacks(NULL, TbSelectActionType);

   ox = 15;
   oy += 24;
   tbAddActionFrontPoseID = tbAddActionUI.CreateCombo("AActionFrontPose", ox, oy, (tbAddActionW - 40)/2, 20, 2, 0.45f, 0.45f, 0.45f, 1.0f,
                                                      (tbAddActionW - 80)/2, 6);
   uiC.Object(tbAddActionFrontPoseID);
   uiC.SetLayer(3);
   uiC.SetParent(tbAddActionBackgroundID);
   uiC.SetCaption(LEFT_CENTERED, 0, 5, "Front P", 0.0f, 0.0f, 0.0f);
   uiC.BindCallbacks(NULL, TbSelectActionPose);

   ox += (tbAddActionW - 30)/2;
   tbAddActionEndPoseID = tbAddActionUI.CreateCombo("AActionRearPose", ox, oy, (tbAddActionW - 40)/2, 20, 2, 0.45f, 0.45f, 0.45f, 1.0f,
                                                     (tbAddActionW - 80)/2, 6);
   uiC.Object(tbAddActionEndPoseID);
   uiC.SetLayer(3);
   uiC.SetParent(tbAddActionBackgroundID);
   uiC.SetCaption(LEFT_CENTERED, 5, 5, "Rear P", 0.0f, 0.0f, 0.0f);
   uiC.BindCallbacks(NULL, TbSelectActionPose);

   ox = 15;
   oy += 23;
   tbAddActionDurationID = tbAddActionUI.CreateTypein("AActionDuration", ox, oy, tbAddActionW/2, 25, 256, 0.45f, 0.45f, 0.45f);
   uiTy.Object(tbAddActionDurationID);
   uiTy.SetParent(tbAddActionBackgroundID);
   uiTy.SetTypein(LEFT_CENTERED, 55, 5, tbAddActionW/2 - 55);
   uiTy.SetCaption(LEFT_CENTERED, 2, 5, "Length", 0.0f, 0.0f, 0.0f);
   uiTy.SetTypeinColor(0.5f, 0.4f, 0.3f);

   // make it a popup UI
   tbAddActionUI.BePopUp(TRUE);
}


/*------------------------
  quit function for win32
  C.Wang 0511, 2009
 -------------------------*/
void TbQuitFun()
{
   TbOpenQuit(tbExitButtonID, FALSE);
}


/*----------------------------------------------
  the timer callback for testbed used as a tool
  C.Wang 0710, 2010
 -----------------------------------------------*/
void TbToolMode(int skip)
{
   // check current actor
   FnActor actor;
   actor.Object(tbCurScene->GetCurrentActor());

   // play the animation
   if (tbBePlay) {
      // play all actors' action
      tbCurScene->Play(skip*tbPlaySpeed, tbBeBase);
   }

   // play test pose
   if (tbBePlayPose) {
      if (tbTestPoseStart >= 0 && tbTestPoseEnd >= 0) {
         actor.PlayFrame(tbTestPoseFrame, tbBeBase, skip*tbPlaySpeed);
         tbTestPoseFrame += skip*tbPlaySpeed;
         if (tbTestPoseFrame > (float) tbTestPoseEnd) {
            tbTestPoseFrame = (float) tbTestPoseStart;

            // reset the character's position
            float pos[3];
            pos[0] = pos[1] = pos[2] = 0.0f;
            actor.SetPosition(pos);
         }
      }
   }

   // control the log message's opacity
   if (tbLogOpacity >= 0) {
      tbLogOpacity -= 1;
   }
}


/*------------------------------------------------------
  the timer callback for testbed used as a game control
  C.Wang 0710, 2010
 -------------------------------------------------------*/
void TbPlayMode(int skip)
{
   // play the main actor's motion
}


/*------------------------------------------------
  this is a timer callback for main loop in 30fps
  C.Wang 0710, 2010
 -------------------------------------------------*/
void TbMainLoop(int skip)
{
   // do the batch jobs
   FjDoJobs(skip);

   // execute the script
   TbExecuteScript(skip);

   // check the control mode
   if (tbControlMode == TOOL_MODE) {
      TbToolMode(skip);
   }
   else if (tbControlMode == PLAY_MODE) {
      TbPlayMode(skip);
   }
}


/*--------------------------------------
  display the command & command history
  C.Wang 0514, 2009
 ---------------------------------------*/
void TbDisplayCommands()
{
   int i, ox, oy, oxCmd;
   char msg[MAX_STRING_CHARACTERS];

   // begin to display console messages
   if (!tbBeShuFa) {
      tbTinyEFont.Begin(tbWorkSpaceBTID);
   }

   // display console prompt & messages
   ox = tbBottomVPOX + 2;
   oy = tbBottomVPOY + tbBottomVPHeight - 20;

   // the prompt
   ox = tbTinyEFont.Write(" >", ox, oy, 0, 255, 0);
   oxCmd = ox;

   // the entering command
   strcpy(msg, tbCurCommandPtr);

   if (tbBeBlink) {
      // cursor
      msg[tbCommandCursor] = '_';
      msg[tbCommandCursor+1] = '\0';
   } 
   ox = tbTinyEFont.Write(msg, ox, oy, 0, 255, 0);

   // show log
   oy -= 15;
   if (tbLogOpacity >= 0.0f) {
      ox = tbSideMenuWidth + 5;
      int o = (int) tbLogOpacity;
      if (o > 255) o = 255;
      ox = tbTinyEFont.Write(tbLogMessage, ox, oy, 0, 255, 200, o);
      tbLogOpacity -= 0.5f;
   }

   // the command history
   if (tbBeDisplayCommandHistory) {
      oy -= 5;
      for (i = tbCurCommand - 1; i >= 0; i--) {
         ox = oxCmd;
         oy -= 15;
         ox = tbTinyEFont.Write(tbCommandHistory[i], ox, oy, 255, 255, 0, 200);
      }
   }

   // end to display
   if (!tbBeShuFa) {
      tbTinyEFont.End();
   }
}


/*---------------------------------------
  this is a timer callback for rendering
  C.Wang 0519, 2009
 ----------------------------------------*/
void TbRenderIt(int skip)
{
   char string[256];

   // render the viewports
   tbWSBottomVP.Render(0, TRUE, TRUE);

   if (tbRenderCamera == ORTHOGONAL) {
      // render the work space
      tbWorkSpaceVP.Render(tbOrthoCamID, TRUE, TRUE);
   }
   else {
      // render the work space
      tbWorkSpaceVP.Render(tbMainCamID, TRUE, TRUE);

      if (tbShowAux) {
         // synchronize the aux camera with the main one
         float *M16, M12[12];
         M16 = tbCamera.GetMatrix(TRUE);
         M12[0] = M16[0];  M12[1] = M16[1];   M12[2] = M16[2];
         M12[3] = M16[4];  M12[4] = M16[5];   M12[5] = M16[6];
         M12[6] = M16[8];  M12[7] = M16[9];   M12[8] = M16[10];
         M12[9] = M16[12]; M12[10] = M16[13]; M12[11] = M16[14];
         tbAuxCamera.SetMatrix(M12, REPLACE);

         // render the auxiliary scene
         tbWorkSpaceVP.Render(tbAuxCamID, FALSE, FALSE);
      }
   }

   // show workspace caption
   if (tbShowWorkSpaceCaption) {
      if (!tbBeShuFa) {
         tbTinyEFont.Begin(tbWorkSpaceBTID);
         tbBeShuFa = TRUE;
      }

      FnActor actor;
      ACTORid curA = tbCurScene->GetCurrentActor();
      int iOne = 0;
      int ox = tbBottomVPOX;
      int oy = tbBottomVPOY;
      strcpy(string, "3D");
      if (tbBePlay) {
         strcat(string, " - Playing");
         ox = tbTinyEFont.Write(string, ox, oy, 0, 255, 0);

         if (curA != FAILED_ID) {
            actor.Object(curA);
            ACTIONid actionID = actor.GetCurrentAction(0, NULL);
            FnAction action;
            if (actionID != FAILED_ID) {
               action.Object(actionID);
               sprintf(string, " %s", action.GetName());
               ox = tbTinyEFont.Write(string, ox, oy, 128, 255, 0);
            }


         }
      }
      else {
         ox = tbTinyEFont.Write(string, ox, oy, 0, 255, 0);
      }
      iOne++;

      // show current Actor's name
      if (curA != FAILED_ID) {
         char string[256];

         actor.Object(curA);
         sprintf(string, "Actor : %s", actor.GetName());
         ox = tbBottomVPOX;
         oy = tbBottomVPOY + 15*iOne;
         ox = tbTinyEFont.Write(string, ox, oy, 128, 255, 0);
         iOne++;
      }

      // show current object's name
      OBJECTid curObj = tbCurScene->GetCurrentObject();
      if (curObj != FAILED_ID) {
         FnObject model;
         char string[256];

         model.Object(curObj);
         sprintf(string, "Model : %s", model.GetName());
         ox = tbBottomVPOX;
         oy += 15;
         ox = tbTinyEFont.Write(string, ox, oy, 0, 255, 0);
         iOne++;
      }

		//current frame
		{
			actor.Object(curA);
            ACTIONid actionID = actor.GetCurrentAction(0, NULL);
            FnAction action;
            if (actionID != FAILED_ID) {
				action.Object(actionID);
				char string[256];
				sprintf(string, "CurrentFrame : %f", action.GetCurrentFrame() );
				ox = tbBottomVPOX;
				oy = tbBottomVPOY + 15*iOne;
				ox = tbTinyEFont.Write(string, ox, oy, 128, 255, 0);
				iOne++;
			}
		}
      if (tbBeShuFa) {
         tbTinyEFont.End();
         tbBeShuFa = FALSE;
      }
   }

   // render the UI
   FyRenderAllUISystem(tbAllUISysID, tbNumUISys);

   // show console
   if (FyGetInputFunction() == TbCommandInput) {
      TbDisplayCommands();
   }

   if (tbBeShuFa) {
      tbTinyEFont.End();
      tbBeShuFa = FALSE;
   }

   // swap buffers
   tbWorld.SwapBuffers();
}


/*--------------------------------
  blinking control timer callback
  fps = 1
  C.Wang 0513, 2009
 ---------------------------------*/
void TbBlinkControl(int skip)
{
   tbBeBlink = !tbBeBlink;
}


/*---------------------------
  create the axis/test plane
  C.Wang 0610, 2009
 ----------------------------*/
void TbCreateAxis()
{
   FnObject object;
   MATERIALid mID;
   float v[6];
   float color[3];

   // create the axis object
   tbAxisID = tbAuxScene.CreateObject();
   object.Object(tbAxisID);

   v[0] = 0.0f; v[1] = 0.0f; v[2] = 0.0f;
   v[3] = 10.0f; v[4] = 0.0f; v[5] = 0.0f;
   color[0] = 1.0f; color[1] = 0.0f; color[2] = 0.0f;
   mID = tbWorld.CreateMaterial(color, color, color, 20.0f, color);
   object.Lines(LINE_SEGMENTS, mID, v, 2, TRUE);

   v[3] = 0.0f; v[4] = 10.0f; v[5] = 0.0f;
   color[0] = 0.0f; color[1] = 1.0f; color[2] = 0.0f;
   mID = tbWorld.CreateMaterial(color, color, color, 20.0f, color);
   object.Lines(LINE_SEGMENTS, mID, v, 2, TRUE);

   v[3] = 0.0f; v[4] = 0.0f; v[5] = 10.0f;
   color[0] = 0.0f; color[1] = 0.0f; color[2] = 1.0f;
   mID = tbWorld.CreateMaterial(color, color, color, 20.0f, color);
   object.Lines(LINE_SEGMENTS, mID, v, 2, TRUE);

   // create the test plane
   tbTestPlaneID = tbAuxScene.CreateObject();
   object.Object(tbTestPlaneID);
   object.SetOpacity(0.2f);

   color[0] = 0.2f; color[1] = 0.2f; color[2] = 0.2f;
   mID = tbWorld.CreateMaterial(color, color, color, 20.0f, color);

   int i, div = 10;
   float x, dx;
   dx = tbTestPlaneSize/div;
   x = -tbTestPlaneSize;
   for (i = -div; i <= div; i++) {
      v[0] = -tbTestPlaneSize; v[1] = x; v[2] = 0.0f;
      v[3] = tbTestPlaneSize; v[4] = x; v[5] = 0.0f;
      object.Lines(LINE_SEGMENTS, mID, v, 2, TRUE);
      x += dx;
   }

   x = -tbTestPlaneSize;
   for (i = -div; i <= div; i++) {
      v[1] = -tbTestPlaneSize; v[0] = x; v[2] = 0.0f;
      v[4] = tbTestPlaneSize; v[3] = x; v[5] = 0.0f;
      object.Lines(LINE_SEGMENTS, mID, v, 2, TRUE);
      x += dx;
   }

   // create the grid plane for texture space
   tbOrthoPlaneID = tbOrthoScene.CreateObject();
   object.Object(tbOrthoPlaneID);

   div = 2;
   dx = 2.0f/div;
   x = -2.0f;
   for (i = -div; i <= div; i++) {
      v[0] = -2.0f; v[1] = x; v[2] = 0.0f;
      v[3] = 2.0f; v[4] = x; v[5] = 0.0f;
      object.Lines(LINE_SEGMENTS, mID, v, 2, TRUE);
      x += dx;
   }

   x = -2.0f;
   for (i = -div; i <= div; i++) {
      v[1] = -2.0f; v[0] = x; v[2] = 0.0f;
      v[4] = 2.0f; v[3] = x; v[5] = 0.0f;
      object.Lines(LINE_SEGMENTS, mID, v, 2, TRUE);
      x += dx;
   }
}


/*-------------------------
  draw a blue box in lines
  C.Wang 1004, 2006
 --------------------------*/
void TbDrawBlueBox(FnObject *object)
{
   // draw a box
   float color[3];
   color[0] = 0.0f; color[1] = color[2] = 1.0f;

   MATERIALid mID = tbWorld.CreateMaterial(color, color, color, 1.0f, color);
   float v[24];
   v[0] = -5.0f; v[1] = -5.0f; v[2] = 0.0f;
   v[3] =  5.0f; v[4] = -5.0f; v[5] = 0.0f;
   v[6] =  5.0f; v[7] =  5.0f; v[8] = 0.0f;
   v[9] = -5.0f; v[10] = 5.0f; v[11] = 0.0f;
   object->Lines(CLOSE_POLYLINE, mID, v, 4, TRUE);
   v[0] = -5.0f; v[1] = -5.0f; v[2] = 5.0f;
   v[3] =  5.0f; v[4] = -5.0f; v[5] = 5.0f;
   v[6] =  5.0f; v[7] =  5.0f; v[8] = 5.0f;
   v[9] = -5.0f; v[10] = 5.0f; v[11] = 5.0f;
   object->Lines(CLOSE_POLYLINE, mID, v, 4, TRUE);
   v[0] = -5.0f; v[1] = -5.0f; v[2] = 0.0f;
   v[3] = -5.0f; v[4] = -5.0f; v[5] = 5.0f;
   v[6] = 5.0f; v[7] = 5.0f; v[8] = 0.0f;
   v[9] = 5.0f; v[10] = 5.0f; v[11] = 5.0f;
   v[12] = -5.0f; v[13] = 5.0f; v[14] = 0.0f;
   v[15] = -5.0f; v[16] = 5.0f; v[17] = 5.0f;
   v[18] = 5.0f; v[19] = -5.0f; v[20] = 0.0f;
   v[21] = 5.0f; v[22] = -5.0f; v[23] = 5.0f;
   object->Lines(LINE_SEGMENTS, mID, v, 8, TRUE);
   v[0] = 0.0f; v[1] = 0.0f; v[2] = 8.0f;
   v[3] = 0.0f; v[4] = 0.0f; v[5] = 0.0f;
   v[6] = 0.0f; v[7] = -8.0f; v[8] = 0.0f;
   object->Lines(OPEN_POLYLINE, mID, v, 3, TRUE);
}


/*-------------------
  load a script file
  C.Wang 0521, 2009
 --------------------*/
BOOL TbLoadScript(char *arg)
{
   // find the new texture path
   char string[256], *key;
   BOOL beOK = FALSE;
   char fileName[256], path[128], name[128];
   FILE *fp;

   if (arg != NULL) {
      strcpy(string, arg);
      key = strtok(string, " ");
   }
   else {
      key = NULL;
   }

   if (key != NULL) {
      strcpy(fileName, key);
      FyCheckFileFormat(fileName, path, name, NULL);
      if (strlen(path) == 0) strcpy(path, ".");
      sprintf(fileName, "%s\\%s.cw2", path, name);
      beOK = TRUE;
   }
   else {
      // get current directory
      char dir[128];
      OPENFILENAME ofn;
      GetCurrentDirectory(128, dir);

      // prepare to open the file browser
      fileName[0] = '\0';
      memset(&ofn, 0, sizeof(OPENFILENAME));
      ofn.lStructSize = sizeof(OPENFILENAME);
      ofn.hwndOwner = NULL;
      ofn.lpstrFilter = "Testbed3 Script {*.cw2}\0*.cw2\0"\
                        "All files {*.*}\0*.*\0\0";
      ofn.lpstrFile = fileName;
      ofn.nMaxFile = 256;
      ofn.Flags = OFN_FILEMUSTEXIST;
      ofn.lpstrDefExt = "cw2";
      ofn.lpstrInitialDir = dir;

      // start to load the model file
      if (GetOpenFileName(&ofn)) {
         // set the extra directory for data loading
         char name[128];
         GetCurrentDirectory(128, name);
         tbWorld.SetExtraDataPath(name);

         // success to get the file name
         beOK = TRUE;
      }

      // restore the current directory
      SetCurrentDirectory(dir);
   }

   if (beOK) {
      int fileSize;
      BYTE *pData;

      // open the file
      fp = fopen(fileName, "rb");
      if (fp != NULL) {
         // check file length
         fseek(fp, 0, SEEK_END);
         fileSize = ftell(fp);
         fseek(fp, 0, SEEK_SET);

         // load the data
         pData = (BYTE *)FyAllocateBuffer(fileSize + 100);
         fread(pData, sizeof(BYTE), fileSize, fp);

         // close the file
         fclose(fp);
         fp = NULL;

         // copy the script to system string
         TbCopySystemString((char *) pData, fileSize);

         // free the buffer
         FyReleaseBuffer(pData);
      }
      else {
         beOK = FALSE;
      }
   }
   
   return beOK;
}


/*------------------
  end a loop
  C.Wang 0521, 2009
 -------------------*/
BOOL TbEndLoopCmd(char *arg)
{
   if (tbCurLoopStackID < 0) {
      return FALSE;
   }

   // check ending
   TBScript *script;
   int incV = tbLoopStack[tbCurLoopStackID].incV;
   int endV = tbLoopStack[tbCurLoopStackID].endV;
   int curV = tbLoopStack[tbCurLoopStackID].curV;
   if (curV >= endV) {
      // end the loop
      tbCurLoopStackID--;
      if (tbCurLoopStackID < 0) tbCurLoopStackID = NONE;
      return TRUE;
   }

   // make the script pointer back to the head of the loop
   script = &tbScript[tbCurScript];
   if (script->beRun) {
      script->length = tbLoopStack[tbCurLoopStackID].loopEntry;
   }

   // increment the counter
   tbLoopStack[tbCurLoopStackID].curV += incV;
   if (incV > 0) {
      if (tbLoopStack[tbCurLoopStackID].curV > endV) {
         tbLoopStack[tbCurLoopStackID].curV = endV;
      }
   }
   else {
      if (tbLoopStack[tbCurLoopStackID].curV < endV) {
         tbLoopStack[tbCurLoopStackID].curV = endV;
      }
   }

   // set the variable
   char value[256];
   sprintf(value, "= %d", tbLoopStack[tbCurLoopStackID].curV);
   TbSetVariableValue(tbLoopStack[tbCurLoopStackID].index, value);

   return TRUE;
}


/*------------------
  begin a loop
  C.Wang 0521, 2009
 -------------------*/
BOOL TbBeginLoopCmd(char *arg)
{
   char string[256], buf[256], *keyV, *keyS, *keyE, *keyI;
   int iS, iE, iI;
   TBScript *script;

   strcpy(string, arg);
   keyV = strtok(string, " ");
   if (keyV != NULL) {
      keyS = strtok(NULL, " ");
      if (keyS != NULL) {
         keyE = strtok(NULL, " ");
         if (keyE != NULL) {
            keyI = strtok(NULL, " ");
            if (keyI != NULL) {
               // check value
               iS = atoi(keyS);
               iE = atoi(keyE);
               iI = atoi(keyI);
               if (iS <= iE && iI < 0) {
                  return FALSE;
               }
               if (iE <= iS && iI > 0) {
                  return FALSE;
               }

               // insert the local variable
               tbBeUseGlobals = FALSE;
               TbSetIntegerCmd(keyV, buf, NULL);
               tbBeUseGlobals = TRUE;

               // set initial value
               char value[256];
               sprintf(value, "= %s", keyS);
               TbSetVariableValue(keyV, value);

               // push the loop to loop stack
               tbCurLoopStackID++;
               tbLoopStack[tbCurLoopStackID].startV = iS;
               tbLoopStack[tbCurLoopStackID].curV = iS;
               tbLoopStack[tbCurLoopStackID].endV = iE;
               tbLoopStack[tbCurLoopStackID].incV = iI;
               strcpy(tbLoopStack[tbCurLoopStackID].index, keyV);

               script = &tbScript[tbCurScript];
               if (script->beRun) {
                  tbLoopStack[tbCurLoopStackID].loopEntry = script->length;
               }
               else {
                  tbLoopStack[tbCurLoopStackID].loopEntry = 0;
               }

               return TRUE;
            }
         }
      }
   }

   return FALSE;
}


/*-----------------------------
  initialize the script system
  C.Wang 0521, 2009
 ------------------------------*/
void TbInitScriptSystem()
{
   tbCurScript = NONE;
   for (int i = 0; i < MAX_SCRIPTS; i++) {
      tbScript[i].beRun = FALSE;
      tbScript[i].length = 0;
      tbScript[i].maxLength = 0;
      tbScript[i].systemString = NULL;
   }
#ifdef MULTICORE
   TbInitializeCriticalSection(&tbScriptCS);
#endif
}


/*--------------------------------
  de-initialize the script system
  C.Wang 0521, 2009
 ---------------------------------*/
void TbDeinitScriptSystem()
{
   tbCurScript = 0;
   for (int i = 0; i < MAX_SCRIPTS; i++) {
      tbScript[i].beRun = FALSE;
      tbScript[i].length = 0;
      tbScript[i].maxLength = 0;
      FyReleaseBuffer(tbScript[i].systemString);
      tbScript[i].systemString = NULL;
   }
#ifdef MULTICORE
   TbDeleteCriticalSection(&tbScriptCS);
#endif
}


/*-------------------------------
  update string in script buffer
  C.Wang 0521, 2009
 --------------------------------*/
void TbCopySystemString(char *string, int maxLen)
{
   TBScript *script;

#ifdef MULTICORE
   TbEnterCriticalSection(&tbScriptCS, 2);
#endif

   // find the available script buffer
   int counter, sID = tbCurScript;

   if (sID < 0) sID = 0;
   script = &tbScript[sID];
   counter = MAX_SCRIPTS;
   while ((script->beRun || script->systemString != NULL) && counter > 0) {
      sID += 1;
      if (sID >= MAX_SCRIPTS) sID = 0;
      script = &tbScript[sID];
      counter--;
   }
   if (counter <= 0) return;

   script->maxLength = maxLen; //(int) strlen(string) + 1;
   script->systemString = (char *)FyAllocateBuffer(sizeof(char)*script->maxLength);

   FyMemCopy(script->systemString, string, maxLen);
   script->length = 0;
   script->beRun = FALSE;

#ifdef MULTICORE
   TbLeaveCriticalSection(&tbScriptCS);
#endif
}


/*-----------------------------------------------
  get a character from a file or a script buffer
  C.Wang 0521, 2009
 ------------------------------------------------*/
int TbGetChar()
{
   TBScript *script;
   if (tbCurScript < 0) return EOF;

   script = &tbScript[tbCurScript];
   if (script->systemString == NULL) {
      script->beRun = FALSE;
      return EOF;
   }

   if (script->length >= script->maxLength) {
      script->beRun = FALSE;
      return EOF;
   }

   int a = (int) script->systemString[script->length];
   script->length++;
   return a;
}


/*----------------------------------
  get a new line from script buffer
  C.Wang 0521, 2009
 -----------------------------------*/
int TbGetNewLine(char *str)
{
   int count, nextC;
   BOOL beContinue;

   count = 0;
   beContinue = FALSE;
   while ((nextC = TbGetChar()) > EOF) {
      if (nextC == '\n' || nextC == '\0') {
         if (beContinue) {
            beContinue = FALSE;
         }
         else {
            break;
         }
      }
      else if (nextC == '\\') {
         beContinue = TRUE;
      }
      else if (nextC == '\t') {
         *(str + count++) = ' ';
      }
      else if (nextC == '/') {
         *(str + count++) = '\\';
      }
      else if (nextC == 13) {
         // nothing
      }
      else {
         *(str + count++) = nextC;
      }
   }

   if (count == 0) {
      if (nextC == '\n') {
         *(str) = '\0';
         return (0);
      }
      else if (nextC == '\0') {
         *(str) = '\0';
         return (0);
      }
      else {
         *(str) = '\0';
         return (-1);
      }
   }
   else {
     *(str + count) = '\0';
     return (count);
   }
}


/*---------------------------
  execute the current script
  C.Wang 0521, 2009
 ----------------------------*/
void TbExecuteScript(int skip)
{
#ifdef MULTICORE
   TbEnterCriticalSection(&tbScriptCS, 2);
#endif

   char s[256];
   int i, len;
   TBScript *script;
   
   if (tbCurScript < 0) {
      for (i = 0; i < MAX_SCRIPTS; i++) {
         script = &tbScript[i];
         if (script->systemString != NULL) {
            tbCurScript = i;
            break;
         }
      }

      if (i >= MAX_SCRIPTS) return;
   }

   script = &tbScript[tbCurScript];

   // activate the script
   if (skip < 0) {
      if (!script->beRun) {
         if (script->length < 0) {
            script->length = 0;
         }
      }
   }

   if (!script->beRun) {
      if (script->length == 0) {
         script->beRun = TRUE;

         // clean local variables
         for (len = 0; len < MAX_LOCALS; len++) {
            tbLocals[len].beUsed = FALSE;
         }
         tbNumLocal = 0;
      }
   }
   else {
      if (script->length >= script->maxLength) {
         script->beRun = FALSE;
         script->length = NONE;

         // clean local variables
         for (len = 0; len < MAX_LOCALS; len++) {
            tbLocals[len].beUsed = FALSE;
         }
         tbNumLocal = 0;
      }
   }

   if (script->beRun) {
      // scan the script and execute the command
      tbRunScript = TRUE;
      tbBeUseGlobals = FALSE;
      while ((len = TbGetNewLine(s)) >= 0) {
         FyTruncateStringSpace(s);
         len = (int) strlen(s);

         if (len > 0) {
            // check the break command
            if (FyCheckWord(s, "break", TRUE)) {
               break;
            }

            // parse the script command
            char *command, *key, argument[256], string[512];
            strcpy(string, s);
            command = strtok(string, " ");
            key = strtok(NULL, " ");
            argument[0] = '\0';
            while (key != NULL) {
               strcat(argument, key);
               key = strtok(NULL, " ");
               if (key != NULL) {
                  strcat(argument, " ");
               }
            }
            TbCommandParser(command, argument, FALSE);
         }
      }
      tbBeUseGlobals = TRUE;

      if (len < 0) {
         script->beRun = FALSE;

         // clear the script
         FyReleaseBuffer(script->systemString);
         script->systemString = NULL;

         // check next script
         int i;
         for (i = 0; i < MAX_SCRIPTS; i++) {
            tbCurScript++;
            if (tbCurScript >= MAX_SCRIPTS) tbCurScript = 0;
            if (tbScript[tbCurScript].systemString != NULL) break;
         }
         if (i >= MAX_SCRIPTS) {
            tbCurScript = NONE;
         }
      }
      tbRunScript = FALSE;
   }

#ifdef MULTICORE
   TbLeaveCriticalSection(&tbScriptCS);
#endif
}


/*------------------
  check real number
  C.Wang 0324, 1992
 -------------------*/
BOOL TbCheckReal(char *string)
{
   int i, len, start;
   int dd;
   char str[256];

   strcpy(str, string);
   FyTruncateStringSpace(str);

   dd = 0;
   len = (int) strlen(str);

   if (*(str) == '-' || *(str) == '+')
      start = 1;
   else
      start = 0;

   for (i = start; i < len && *(str+i) != 13 && *(str+i) != 11; i++) {
      if (*(str+i) < '0' || *(str+i) > '9') {
         if (*(str+i) == '.') {
            if (dd) {
               return FALSE;
            }
            else {
               dd = 1;
            }
         }
         else {
            return FALSE;
         }
      }
   }

   return TRUE;
}


/*---------------------
  check integer number
  C.Wang 1124, 2003
 ----------------------*/
BOOL TbCheckInt(char *string)
{
   int i, len, start;
   int dd;
   char str[256];

   strcpy(str, string);
   FyTruncateStringSpace(str);

   dd = 0;
   len = (int) strlen(str);

   if (*(str) == '-' || *(str) == '+')
      start = 1;
   else
      start = 0;

   for (i = start; i < len && *(str+i) != 13 && *(str+i) != 11; i++)
      if ( *(str+i) < '0' || *(str+i) > '9' )
         return FALSE;

   return TRUE;
}


/*------------------
  set variable
  C.Wang 0521, 2009
 -------------------*/
char *TbSetVariableValue(char *cmd, char *argument)
{
   // extract the value
   char *value, *key, string[256];

   strcpy(string, argument);
   key = strtok(string, " ");
   if (key == NULL) return NULL;
   value = strtok(NULL, " ");
   if (value == NULL) return NULL;

   // check data type
   DWORD type = STRING_VARIABLE;
   BOOL beValue = FALSE;
   if (TbCheckReal(value)) {
      type = FLOAT_VARIABLE;
   }
   else {
      if (TbCheckInt(value)) {
         type = INTEGER_VARIABLE;
      }
      else {
         if (FyCheckWord(value, "TRUE", TRUE)) {
            type = BOOLEAN_VARIABLE;
            beValue = TRUE;
         }
         else if (FyCheckWord(value, "FALSE", FALSE)) {
            type = BOOLEAN_VARIABLE;
            beValue = FALSE;
         }
      }
   }

   // search the variable
   int i;
   for (i = 0; i < tbNumLocal; i++) {
      if (tbLocals[i].beUsed) {
         if (FyCheckWord(tbLocals[i].name, cmd, TRUE)) {
            if (tbLocals[i].type == FLOAT_VARIABLE && (type == FLOAT_VARIABLE || type == INTEGER_VARIABLE)) {
               tbLocals[i].value.f = (float) atof(value);
               strcpy(tbLocals[i].raw, value);
               return tbLocals[i].raw;
            }
            else if (tbLocals[i].type == INTEGER_VARIABLE && type == FLOAT_VARIABLE) {
               tbLocals[i].value.i = (int) atof(value);
               strcpy(tbLocals[i].raw, value);
               return tbLocals[i].raw;
            }
            else if (tbLocals[i].type == INTEGER_VARIABLE && type == INTEGER_VARIABLE) {
               tbLocals[i].value.i = (int) atoi(value);
               strcpy(tbLocals[i].raw, value);
               return tbLocals[i].raw;
            }
            else if (type == BOOLEAN_VARIABLE) {
               tbLocals[i].value.b = beValue;
               strcpy(tbLocals[i].raw, value);
               return tbLocals[i].raw;
            }

            return NULL;
         }
      }
   }

   for (i = 0; i < tbNumGlobal; i++) {
      if (tbGlobals[i].beUsed) {
         if (FyCheckWord(tbGlobals[i].name, cmd, TRUE)) {
            if (tbGlobals[i].type == FLOAT_VARIABLE && (type == FLOAT_VARIABLE || type == INTEGER_VARIABLE)) {
               tbGlobals[i].value.f = (float) atof(value);
               strcpy(tbGlobals[i].raw, value);
               return tbGlobals[i].raw;
            }
            else if (tbGlobals[i].type == INTEGER_VARIABLE && type == FLOAT_VARIABLE) {
               tbGlobals[i].value.i = (int) atof(value);
               sprintf(tbGlobals[i].raw, "%d", tbGlobals[i].value.i);
               return tbGlobals[i].raw;
            }
            else if (tbGlobals[i].type == INTEGER_VARIABLE && type == INTEGER_VARIABLE) {
               tbGlobals[i].value.i = (int) atoi(value);
               strcpy(tbGlobals[i].raw, value);
               return tbGlobals[i].raw;
            }
            else if (tbGlobals[i].type == BOOLEAN_VARIABLE && type == BOOLEAN_VARIABLE) {
               tbGlobals[i].value.b = beValue;
               strcpy(tbGlobals[i].raw, value);
               return tbGlobals[i].raw;
            }

            return NULL;
         }
      }
   }

   return NULL;
}


/*----------------------------------------------
  check variable name is an array or a variable
  C.Wang 0521, 2009
 -----------------------------------------------*/
int TbCheckVariableName(char *v, char *head)
{
   int i, j, len, value;

   len = (int) strlen(v);
   for (i = 0; i < len; i++) {
      if (v[i] == '[') break;
      head[i] = v[i];
   }

   if (i >= len) {
      head[i] = '\0';
      return 0;
   }
   else {
      int k;
      char num[256];
      head[i] = '\0';
      for (j = i + 1, k = 0; j < len; j++, k++) {
         if (v[j] == ']') break;
         num[k] = v[j];
      }
      num[k] = '\0';
      value = atoi(num);
      return value;
   }
}


/*----------------------------
  declare an integer variable
  C.Wang 0521, 2009
 -----------------------------*/
BOOL TbSetIntegerCmd(char *arg, char *log, void *data)
{
   int i, id, nArray;
   char string[256], vName[256], name[256], *key1, *key2, *key3;

   strcpy(string, arg);
   key1 = strtok(string, " ");
   if (key1 != NULL) {
      if (key1[0] == '$') {
         // check if the variable is an array or not
         nArray = TbCheckVariableName(key1, vName);

         // check initial values
         key2 = strtok(NULL, " ");
         if (key2 != NULL) {
            key3 = strtok(NULL, " ");
            if (key3 != NULL) {
               if (nArray == 0) {
                  id = TbRegisterVariable(key1, key3, INTEGER_VARIABLE, 0, tbBeUseGlobals);
                  sprintf(log, "%s = %s", key1, key3);
                  return TRUE;
               }
               else {
                  for (i = 0; i < nArray; i++) {
                     sprintf(name, "%s[%d]", vName, i);
                     id = TbRegisterVariable(name, key3, INTEGER_VARIABLE, 0, tbBeUseGlobals);
                  }
                  sprintf(log, "Create an integer array, %s", vName);
                  return TRUE;
               }
            }
         }
         else {
            if (nArray == 0) {
               id = TbRegisterVariable(key1, "0", INTEGER_VARIABLE, 0, tbBeUseGlobals);
               sprintf(log, "%s = 0", key1);
               return TRUE;
            }
            else {
               for (i = 0; i < nArray; i++) {
                  sprintf(name, "%s[%d]", vName, i);
                  id = TbRegisterVariable(name, "0", INTEGER_VARIABLE, 0, tbBeUseGlobals);
               }
               sprintf(log, "Create an integer array, %s", vName);
               return TRUE;
            }
         }
      }
   }
   strcpy(log, "No integer value to set");
   return TRUE;
}


/*---------------------------
  declare a boolean variable
  C.Wang 0521, 2009
 ----------------------------*/
BOOL TbSetBooleanCmd(char *arg, char *log, void *data)
{
   int i, id, nArray;
   char string[256], vName[256], name[256], *key1, *key2, *key3;

   strcpy(string, arg);
   key1 = strtok(string, " ");
   if (key1 != NULL) {
      if (key1[0] == '$') {
         // check if the variable is an array or not
         nArray = TbCheckVariableName(key1, vName);

         // check initial values
         key2 = strtok(NULL, " ");
         if (key2 != NULL) {
            key3 = strtok(NULL, " ");
            if (key3 != NULL) {
               if (nArray == 0) {
                  id = TbRegisterVariable(key1, key3, BOOLEAN_VARIABLE, 0, TRUE);
                  sprintf(log, "%s = %s", key1, key3);
                  return TRUE;
               }
               else {
                  for (i = 0; i < nArray; i++) {
                     sprintf(name, "%s[%d]", vName, i);
                     id = TbRegisterVariable(name, key3, BOOLEAN_VARIABLE, 0, TRUE);
                  }
                  sprintf(log, "Create a boolean array, %s", vName);
                  return TRUE;
               }
            }
         }
         else {
            if (nArray == 0) {
               id = TbRegisterVariable(key1, "TRUE", BOOLEAN_VARIABLE, 0, TRUE);
               sprintf(log, "%s = TRUE", key1);
               return TRUE;
            }
            else {
               for (i = 0; i < nArray; i++) {
                  sprintf(name, "%s[%d]", vName, i);
                  id = TbRegisterVariable(name, "TRUE", BOOLEAN_VARIABLE, 0, TRUE);
               }
               sprintf(log, "Create a boolean array, %s", vName);
               return TRUE;
            }
         }
      }
   }
   strcpy(log, "No boolean value to set");
   return TRUE;
}


/*----------------------------------
  declare a floating-point variable
  C.Wang 0521, 2009
 -----------------------------------*/
BOOL TbSetFloatCmd(char *arg, char *log, void *data)
{
   int i, id, nArray;
   char string[256], vName[256], name[256], *key1, *key2, *key3;

   strcpy(string, arg);
   key1 = strtok(string, " ");
   if (key1 != NULL) {
      if (key1[0] == '$') {
         // check if the variable is an array or not
         nArray = TbCheckVariableName(key1, vName);

         // check initial values
         key2 = strtok(NULL, " ");
         if (key2 != NULL) {
            key3 = strtok(NULL, " ");
            if (key3 != NULL) {
               if (nArray == 0) {
                  id = TbRegisterVariable(key1, key3, FLOAT_VARIABLE, 0, TRUE);
                  sprintf(log, "%s = %s", key1, key3);
                  return TRUE;
               }
               else {
                  for (i = 0; i < nArray; i++) {
                     sprintf(name, "%s[%d]", vName, i);
                     id = TbRegisterVariable(name, key3, FLOAT_VARIABLE, 0, TRUE);
                  }
                  sprintf(log, "Create a floating-point array, %s", vName);
                  return TRUE;
               }
            }
         }
         else {
            if (nArray == 0) {
               id = TbRegisterVariable(key1, "0.0", FLOAT_VARIABLE, 0, TRUE);
               sprintf(log, "%s = 0.0", key1);
               return TRUE;
            }
            else {
               for (i = 0; i < nArray; i++) {
                  sprintf(name, "%s[%d]", vName, i);
                  id = TbRegisterVariable(name, "0.0", FLOAT_VARIABLE, 0, TRUE);
               }
               sprintf(log, "Create a floating-point array, %s", vName);
               return TRUE;
            }
         }
      }
   }
   strcpy(log, "No floating-point value to set");
   return TRUE;
}


/*--------------------
  register a variable
  C.Wang 0521, 2009
 ---------------------*/
int TbRegisterVariable(char *name, char *raw, DWORD type, int arrayLen, BOOL beGlobal)
{
   int i, iOne, num, maxNum, iGet;
   TBVariable *varTab;

   if (beGlobal) {
      num = tbNumGlobal;
      maxNum = MAX_GLOBALS;
      varTab = tbGlobals;
   }
   else {
      num = tbNumLocal;
      maxNum = MAX_LOCALS;
      varTab = tbLocals;
   }

   iOne = -1;
   for (i = 0; i < num; i++) {
      if (!varTab->beUsed) {
         if (iOne < 0) {
            iOne;
         }
      }
      else {
         if (FyCheckWord(varTab->name, name, TRUE)) return i;
      }

      varTab++;
   }

   if (iOne < 0) {
      if (num >= maxNum) return NONE;

      if (beGlobal) {
         iOne = tbNumGlobal;
         varTab = &tbGlobals[tbNumGlobal];
         tbNumGlobal++;
      }
      else {
         iOne = tbNumLocal;
         varTab = &tbLocals[tbNumLocal];
         tbNumLocal++;
      }
   }

   strcpy(varTab->name, name);
   strcpy(varTab->raw, raw);

   // check data type
   iGet = NONE;
   varTab->type = STRING_VARIABLE;
   if (TbCheckReal(raw) && type == FLOAT_VARIABLE) {
      varTab->type = FLOAT_VARIABLE;
      varTab->value.f = (float) atof(raw);
      varTab->beUsed = TRUE;
      iGet = iOne;
   }
   else {
      if (TbCheckInt(raw) && type == INTEGER_VARIABLE) {
         varTab->type = INTEGER_VARIABLE;
         varTab->value.i = atoi(raw);
         varTab->beUsed = TRUE;
         iGet = iOne;
      }
      else {
         if (FyCheckWord(raw, "TRUE", TRUE) && type == BOOLEAN_VARIABLE) {
            varTab->type = BOOLEAN_VARIABLE;
            varTab->value.b = TRUE;
            varTab->beUsed = TRUE;
            iGet = iOne;
         }
         else if (FyCheckWord(raw, "FALSE", FALSE) && type == BOOLEAN_VARIABLE) {
            varTab->type = BOOLEAN_VARIABLE;
            varTab->value.b = FALSE;
            varTab->beUsed = TRUE;
            iGet = iOne;
         }
      }
   }

   if (iGet == NONE && type == STRING_VARIABLE) {
      iGet = iOne;
   }
   return iGet;
}


/*----------------------
  unregister a variable
  C.Wang 0521, 2009
 -----------------------*/
void TbUnregisterVariable(char *name, BOOL beGlobal)
{
   int i, num;
   TBVariable *varTab;

   if (beGlobal) {
      num = tbNumGlobal;
      varTab = tbGlobals;
   }
   else {
      num = tbNumLocal;
      varTab = tbLocals;
   }

   for (i = 0; i < num; i++) {
      if (varTab->beUsed) {
         if (FyCheckWord(varTab->name, name, TRUE)) {
            varTab->beUsed = FALSE;
            return;
         }
      }
      varTab++;
   }
}


/*-------------------
  the command parser
  C.Wang 0701, 2010
 --------------------*/
BOOL TbCommandParser(char *command, char *argument, BOOL bePasteCommand)
{
   BOOL beOK, beExeOK, beOKInsert = TRUE;
   char log[256], msg[256];

   tbLogOpacity = 255;
   beExeOK = FALSE;
   TbSetLogMessage("");
   if (FyCheckWord(command, "Quit") || FyCheckWord(command, "Q")) {
      // "Quit" command
      TbQuitCommand *quitIt = (TbQuitCommand *) new TbQuitCommand();
      beExeOK = FcDoCommand(quitIt);
   }
   else if (FyCheckWord(command, "Model") || FyCheckWord(command, "M")) {
      // "Model" command
      TbLoadModelCommand *modelIt = (TbLoadModelCommand *) new TbLoadModelCommand();
      beExeOK = FcDoCommand(modelIt, argument);
   }
   else if (FyCheckWord(command, "CurrentModel") || FyCheckWord(command, "CM")) {
      // "CurrentModel" command
      TbCurrentModelCommand *cModel = (TbCurrentModelCommand *) new TbCurrentModelCommand();
      beExeOK = FcDoCommand(cModel, argument);
   }
   else if (FyCheckWord(command, "NextModel") || FyCheckWord(command, "NM")) {
      // "NextModel" command
      TbNextModelCommand *nextModel = (TbNextModelCommand *) new TbNextModelCommand();
      beExeOK = FcDoCommand(nextModel, argument);
   }
   else if (FyCheckWord(command, "PreviousModel") || FyCheckWord(command, "PM")) {
      // "PreviousModel" command
      TbPreviousModelCommand *previousModel = (TbPreviousModelCommand *) new TbPreviousModelCommand();
      beExeOK = FcDoCommand(previousModel, argument);
   }
   else if (FyCheckWord(command, "DeleteModel") || FyCheckWord(command, "DM")) {
      // "DeleteModel" command
      TbDeleteModelCommand *deleteModel = (TbDeleteModelCommand *) new TbDeleteModelCommand();
      beExeOK = FcDoCommand(deleteModel, argument);
   }
   else if (FyCheckWord(command, "SaveModel") || FyCheckWord(command, "SaveM")) {
      // "SaveModel" command
      TbSaveModelCommand *saveIt = (TbSaveModelCommand *) new TbSaveModelCommand();
      beExeOK = FcDoCommand(saveIt, argument);
   }
   else if (FyCheckWord(command, "Rename") || FyCheckWord(command, "R")) {
      // "Rename" command
      // check the type
      char value[256];
      BOOL beOK = TbCheckCommandAttributeS(argument, "-Material", value, 256);
      if (beOK) {
         TbRenameMaterialCommand *rn = (TbRenameMaterialCommand *) new TbRenameMaterialCommand();
         beExeOK = FcDoCommand(rn, value);
      }

      beOK = TbCheckCommandAttributeS(argument, "-ActorAction", value, 256);
      if (beOK) {
         TbRenameActionCommand *aa = (TbRenameActionCommand *) new TbRenameActionCommand();
         beExeOK = FcDoCommand(aa, value);
      }
   }
   else if (FyCheckWord(command, "SetMaterial") || FyCheckWord(command, "Mat")) {
      // "SetMaterial" command
      TbSetMaterialCommand *sm = (TbSetMaterialCommand *) new TbSetMaterialCommand();
      beExeOK = FcDoCommand(sm, argument);
   }
   else if (FyCheckWord(command, "UpdateMaterial") || FyCheckWord(command, "UM")) {
      // "UpdateMaterial" command
      TbUpdateMaterialCommand *um = (TbUpdateMaterialCommand *) new TbUpdateMaterialCommand();
      beExeOK = FcDoCommand(um, argument);
   }
   else if (FyCheckWord(command, "Actor") || FyCheckWord(command, "A")) {
      // "Actor" command
      TbLoadActorCommand *actorIt = (TbLoadActorCommand *) new TbLoadActorCommand();
      beExeOK = FcDoCommand(actorIt, argument);
   }
   else if (FyCheckWord(command, "SaveActor") || FyCheckWord(command, "SaveA")) {
      // "SaveActor" command
      TbSaveActorCommand *saveIt = (TbSaveActorCommand *) new TbSaveActorCommand();
      beExeOK = FcDoCommand(saveIt, argument);
   }
   else if (FyCheckWord(command, "SetActor") || FyCheckWord(command, "SetA")) {
      // "SetActor" command
      TbSetActorCommand *sa = (TbSetActorCommand *) new TbSetActorCommand();
      beExeOK = FcDoCommand(sa, argument);
   }
   else if (FyCheckWord(command, "SetBody") || FyCheckWord(command, "SD")) {
      // "SetBody" command
      TbSetBodyCommand *sd = (TbSetBodyCommand *) new TbSetBodyCommand();
      beExeOK = FcDoCommand(sd, argument);
   }
   else if (FyCheckWord(command, "SelectAction") || FyCheckWord(command, "SelectA")) {
      // "SelectAction" command
      TbSelectActionCommand *sa = (TbSelectActionCommand *) new TbSelectActionCommand();
      beExeOK = FcDoCommand(sa, argument);
   }
   else if (FyCheckWord(command, "DeleteActor") || FyCheckWord(command, "DA")) {
      // "DeleteActor" command
      TbDeleteActorCommand *deleteActor = (TbDeleteActorCommand *) new TbDeleteActorCommand();
      beExeOK = FcDoCommand(deleteActor, argument);
   }
   else if (FyCheckWord(command, "DeleteAction") || FyCheckWord(command, "DAA")) {
      // "DeleteAction" command
      TbDeleteActionCommand *deleteAction = (TbDeleteActionCommand *) new TbDeleteActionCommand();
      beExeOK = FcDoCommand(deleteAction, argument);
   }
   else if (FyCheckWord(command, "NewAction") || FyCheckWord(command, "AA")) {
      // "NewAction" command
      TbNewActionCommand *newAction = (TbNewActionCommand *) new TbNewActionCommand();
      beExeOK = FcDoCommand(newAction, argument);
   }
   else if (FyCheckWord(command, "EditAction") || FyCheckWord(command, "MA")) {
      // "EditAction" command
      TbEditActionCommand *editAction = (TbEditActionCommand *) new TbEditActionCommand();
      beExeOK = FcDoCommand(editAction, argument);
   }
   else if (FyCheckWord(command, "Wireframe") || FyCheckWord(command, "W")) {
      // "Wireframe" command
      TbSetWireframeCommand *wireframeIt = (TbSetWireframeCommand *) new TbSetWireframeCommand();
      beExeOK = FcDoCommand(wireframeIt, argument);
   }
   else if (FyCheckWord(command, "SelectPose") || FyCheckWord(command, "SP")) {
      // select pose for previewing
      beExeOK = TbSelectPoseCmd(argument);
   }
   else if (FyCheckWord(command, "PlayPose") || FyCheckWord(command, "PP")) {
      // play test pose
      beExeOK = TbPlayTestPoseCmd(argument);
   }
   else if (FyCheckWord(command, "Aux") || FyCheckWord(command, "Aux")) {
      // twiddle on/off aux view
      beExeOK = TbShowAuxViewCmd(argument);
   }
   else if (FyCheckWord(command, "Ortho") || FyCheckWord(command, "Ortho")) {
      // twiddle on/off orthogonal view
      beExeOK = TbShowOrthoViewCmd(argument);
   }
   else if (FyCheckWord(command, "Speed") || FyCheckWord(command, "s")) {
      // set play speed
      beExeOK = TbSetPlaySpeedCmd(argument);
   }
   else if (FyCheckWord(command, "SetPath") || FyCheckWord(command, "sp")) {
      // "SetPath" command
      TbSetPathCommand *setPathIt = (TbSetPathCommand *) new TbSetPathCommand();
      beExeOK = FcDoCommand(setPathIt, argument);
   }
   else if (FyCheckWord(command, "MakeBaseMove") || FyCheckWord(command, "mbm")) {
      // make base move command - this command can not undo ...... Orz
      beExeOK = TbMakeBaseMoveCmd(argument);
   }
   else if (FyCheckWord(command, "Load") || FyCheckWord(command, "load")) {
      if (!tbRunScript) {
         // load a script
         beExeOK = TbLoadScript(argument);
      }
      else {
         beExeOK = TRUE;
      }
   }
   else if (FyCheckWord(command, "BeginLoop") || FyCheckWord(command, "bLoop")) {
      // begin the loop
      TbBeginLoopCmd(argument);
      beExeOK = TRUE;
   }
   else if (FyCheckWord(command, "EndLoop") || FyCheckWord(command, "eLoop")) {
      // end loop
      TbEndLoopCmd(argument);
      beExeOK = TRUE;
   }
   else if (FyCheckWord(command, "RestoreWorkDirector") || FyCheckWord(command, "RWD")) {
      if (FyCheckWord(argument, "-OFF")) {
         tbBeRestoreCurrentDir = FALSE;
      }
      else {
         tbBeRestoreCurrentDir = TRUE;
      }
   }
   else if (FyCheckWord(command, "Undo") || FyCheckWord(command, "UD")) {
      // for undo command
      beOK = FcUndoLastCommand(log);
      if (beOK) {
         sprintf(msg, "Undo : %s", log); 
         TbSetLogMessage(msg);
      }
      else {
         TbSetLogMessage("Nothing undo");
      }
      beExeOK = TRUE;
   }
   else if (FyCheckWord(command, "Redo") || FyCheckWord(command, "RD")) {
      // for redo command
      beOK = FcRedoCommand(log);
      if (beOK) {
         sprintf(msg, "Redo : %s", log); 
         TbSetLogMessage(msg);
      }
      else {
         TbSetLogMessage("Nothing re-do");
      }
      beExeOK = TRUE;
   }
   else {
      beOKInsert = FALSE;
      beExeOK = FALSE;
   }

   if (!beExeOK) {
      TbSetLogMessage("Failed to execute commands!");
   }

   // insert the command into the history
   if (beOKInsert) {
      if (bePasteCommand) {
         if (argument != NULL && strlen(argument) > 0) {
            sprintf(tbCommandHistory[tbCurCommand], "%s %s", command, argument);
         }
         else {
            strcpy(tbCommandHistory[tbCurCommand], command);
         }
      }

      if (tbCurCommand == tbNumCommandHistory - 1) {
         // the current command slot is the last one on the history list
         int i;
         for (i = 0; i < tbCurCommand; i++) {
            strcpy(tbCommandHistory[i], tbCommandHistory[i + 1]);
         }
      }
      else {
         tbCurCommand++;
         tbCurCommandPtr = tbCommandHistory[tbCurCommand];
      }
   }

   // check command queue for undo/redo manaagement
   beOK = FcRedoCommand(log, TRUE);
   if (beOK) {
      // enable the redo memu
   }
   else {
      // disable the redo memu
   }

   beOK = FcUndoLastCommand(log, TRUE);
   if (beOK) {
      // enable the undo memu
   }
   else {
      // disable the undo memu
   }

   return beExeOK;
}


/*----------------------------
  command input function keys
  C.Wang 0513, 2009
 -----------------------------*/
void TbCommandFunKey(WORLDid gID, BYTE code, BOOL value)
{
   if (value) {
      if (code == FY_UP) {   // upword
         if (tbCurCommand > 0) {
            tbCurCommand--;
            tbCurCommandPtr = tbCommandHistory[tbCurCommand];
            tbCommandCursor = (int) strlen(tbCurCommandPtr);
         }
      }
      else if (code == FY_DOWN) {   // downword
         if (tbCurCommand < tbNumCommandHistory - 1) {
            if (strlen(tbCommandHistory[tbCurCommand]) > 0) {
               tbCurCommand++;
               tbCurCommandPtr = tbCommandHistory[tbCurCommand];
               tbCommandCursor = (int) strlen(tbCurCommandPtr);
            }
         }
      }
      else if (code == FY_RIGHT) {   // right arrow
         if (tbCommandCursor < (int) strlen(tbCurCommandPtr)) {
            tbCommandCursor++;
         }
      }
      else if (code == FY_LEFT) {   // left arrow
         if (tbCommandCursor > 0) {
            tbCommandCursor--;
         }
      }
   }
}


/*-----------------------
  command input callback
  C.Wang 0920, 2006
 ------------------------*/
void TbCommandInput(WORD code)
{
   if (code == 8) {
      if (tbCommandCursor <= 0) return;

      // BackSpace
      tbCommandCursor--;
      tbCurCommandPtr[tbCommandCursor] = '\0';
   }
   else if (code == 13 || code == 27) {
      // "Return"
      if (code == 13) {
         char *command, *key, argument[256], string[256];

         strcpy(string, tbCurCommandPtr);
         command = strtok(string, " ");
         key = strtok(NULL, " ");
         argument[0] = '\0';
         while (key != NULL) {
            strcat(argument, key);
            key = strtok(NULL, " ");
            if (key != NULL) {
               strcat(argument, " ");
            }
         }
         TbCommandParser(command, argument, FALSE);
      }

      // clear the input command
      tbCommandCursor = 0;
      tbCurCommandPtr[tbCommandCursor] = '\0';
   }
   else {
      if (tbCommandCursor >= MAX_STRING_CHARACTERS - 1) return;
      tbCurCommandPtr[tbCommandCursor] = (char) code;
      tbCommandCursor++;
      tbCurCommandPtr[tbCommandCursor] = '\0';
   }
}


/*--------------------------------
  check and get boolean attribute
  C.Wang 0518, 2009
 ---------------------------------*/
BOOL TbCheckCommandAttributeB(char *string, char *attribute, BOOL *value, int maxLen)
{
   int i;
   char localString[512], *key, *key2;

   strcpy(localString, string);

   key = strtok(localString, " ");
   while (key != NULL) {
      if (FyCheckWord(key, attribute, TRUE)) {
         if (value != NULL) {
            for (i = 0; i < maxLen; i++) {
               key2 = strtok(NULL, " ");
               if (key2 == NULL) return FALSE;
               value[i] = (BOOL) atoi(key2);
            }
         }
         return TRUE;
      }

      // next word
      key = strtok(NULL, " ");
   }

   return FALSE;
}


/*-----------------------------------
  check and get the string attribute
  C.Wang 0518, 2009
 ------------------------------------*/
BOOL TbCheckCommandAttributeS(char *string, char *attribute, char *value, int maxLen)
{
   char localString[512], bufString[256], *key, *key2;

   strcpy(localString, string);

   key = strtok(localString, " ");
   while (key != NULL) {
      if (FyCheckWord(key, attribute, TRUE)) {
         if (value != NULL) {
            key2 = strtok(NULL, " ");
            if (key2 == NULL) return FALSE;

            // check the string buffer length
            strcpy(bufString, key2);
            if ((int) strlen(bufString) >= maxLen - 1) {
               bufString[maxLen - 1] = '\0';
            }
            strcpy(value, bufString);
         }
         return TRUE;
      }

      // next word
      key = strtok(NULL, " ");
   }

   return FALSE;
}


/*--------------------------------
  check and get integer attribute
  C.Wang 0518, 2009
 ---------------------------------*/
BOOL TbCheckCommandAttributeI(char *string, char *attribute, int *value, int maxLen)
{
   int i;
   char localString[512], *key, *key2;

   strcpy(localString, string);

   key = strtok(localString, " ");
   while (key != NULL) {
      if (FyCheckWord(key, attribute, TRUE)) {
         if (value != NULL) {
            for (i = 0; i < maxLen; i++) {
               key2 = strtok(NULL, " ");
               if (key2 == NULL) return FALSE;
               value[i] = atoi(key2);
            }
         }
         return TRUE;
      }

      // next word
      key = strtok(NULL, " ");
   }

   return FALSE;
}


/*------------------------------
  check and get float attribute
  C.Wang 0518, 2009
 -------------------------------*/
BOOL TbCheckCommandAttributeF(char *string, char *attribute, float *value, int maxLen)
{
   int i;
   char localString[512], *key, *key2;

   strcpy(localString, string);

   key = strtok(localString, " ");
   while (key != NULL) {
      if (FyCheckWord(key, attribute, TRUE)) {
         if (value != NULL) {
            for (i = 0; i < maxLen; i++) {
               key2 = strtok(NULL, " ");
               if (key2 == NULL) return FALSE;
               value[i] = (float) atof(key2);
            }
         }
         return TRUE;
      }

      // next word
      key = strtok(NULL, " ");
   }

   return FALSE;
}


/*------------------
  set log message
  C.Wang 0513, 2009
 -------------------*/
void TbSetLogMessage(char *log)
{
   strcpy(tbLogMessage, log);
}


/*---------------------------------------------------------
  create a new scene object and register it to scene table
  C.Wang 0626, 2009
 ----------------------------------------------------------*/
TBScene *TbNewScene(SCENEid sID, OBJECTid cRefID, OBJECTid cID, OBJECTid l1ID, OBJECTid l2ID)
{
   int i;

   // new a TB scene
   TBScene *scene = new TBScene();
   scene->Object(sID);

   // assign default cameras & lights
   scene->AssignCamera(cRefID, cID);
   scene->AssignLights(l1ID, l2ID);

   // create the Ui button

   if (tbSceneList == NULL) {
      tbMaxScene = 32;
      tbSceneList = (TBScene **) FyMalloc(sizeof(TBScene *)*tbMaxScene);
      for (i = 0; i < tbMaxScene; i++) {
         tbSceneList[i] = NULL;
      }
      tbNumScene = 0;
   }

   for (i = 0; i < tbNumScene; i++) {
      if (tbSceneList[i] == NULL) {
         break;
      }
   }
   if (i == tbNumScene) {
      if (tbNumScene + 1 > tbMaxScene) {
         tbMaxScene += 32;
         tbSceneList = (TBScene **) FyRealloc(tbSceneList, sizeof(TBScene *)*tbMaxScene);
         for (int j = tbNumScene; j < tbMaxScene; j++) {
            tbSceneList[j] = NULL;
         }
      }
      tbNumScene++;
   }

   tbSceneList[i] = scene;
   scene->ID(i);
   return scene;
}


/*-------------------------------
  hotkey callback for debug used
  C.Wang 0712, 2010
 --------------------------------*/
void TbDebugKey(WORLDid wID, BYTE code, BOOL value)
{
   if (code == FY_F1) {
      if (value) {
         OBJECTid objID = tbCurScene->GetCurrentObject();

         FnObject model;
         model.Object(objID);
         model.SortGeometryWithID();
      }
   }
}


/*-------------------
  the main program
  C.Wang 0626, 2010
 -------------------*/
void main(int argc, char *argv[])
{
   int i, topWidth, topHeight;

   // initialize the script system
   TbInitScriptSystem();

   // initialize theFly and create the main world
   FyWin32GetDesktopSize(&topWidth, &topHeight);
   if (topWidth > 1440) topWidth = 1440;
   if (topHeight > 900) topHeight = 900;

   strcpy(tbTmpShaderPath, ".");
   tbWorldWidth = topWidth - 6;
   tbWorldHeight = topHeight - 27 - 27;
   tbWorldID = FyWin32CreateWorld("Testbed I (C)2010, Chuan-Chang Wang, All Rights Reserved, Build 2",
                                  0, 0, tbWorldWidth, tbWorldHeight, 32, FALSE);
   if (tbWorldID == FAILED_ID) {
      // failed to create the world, just quit the program
      MessageBox(NULL, "Failed to initialize TestBed I", "Error!", MB_OK);
      return;
   }
   tbWorld.Object(tbWorldID);

   // setup the basic working environment
   FySetSystemShaderPath("Data\\Shader4Fly");
   tbWorld.SetCharacterPath("Data\\Characters");
   tbWorld.SetTexturePath("Data\\Textures");
   tbWorld.SetObjectPath("Data\\Models");
   tbWorld.SetShaderPath("Data\\Shaders");
   tbWorld.SetExtraDataPath(".");

   // load cursors
   tbArrowCursor = LoadCursor(NULL, IDC_ARROW);
   tbWaitCursor = LoadCursor(NULL, IDC_WAIT);

   // calculate the system layout
   TbViewportLayout(tbWorldWidth, tbWorldHeight);

   // create the UIs
   tbNumUISys = 0;
   for (i = 0; i < MAX_UI_SYSTEMS; i++) {
      tbAllUISysID[i] = FAILED_ID;
   }
   TbCreateMenuBarUI();
   TbCreateMenuUI();
   TbCreateSideMenuUI();
   TbCreateDialogues();

   // create the viewports for workspace
   tbWorkSpaceBTID = tbWorld.CreateViewport(tbBottomVPOX, tbBottomVPOY, tbBottomVPWidth, tbBottomVPHeight);
   tbWSBottomVP.Object(tbWorkSpaceBTID);
   tbWSBottomVP.SetBackgroundColor(0.5f, 0.5f, 0.5f);

   tbWorkSpaceID = tbWorld.CreateViewport(tbWorkSpaceVPOX, tbWorkSpaceVPOY, tbWorkSpaceVPWidth, tbWorkSpaceVPHeight);
   tbWorkSpaceVP.Object(tbWorkSpaceID);
   tbWorkSpaceVP.SetBackgroundColor(0.6f, 0.6f, 0.6f);

   // create the main scene
   tbMainSceneID = tbWorld.CreateScene(tbRenderGroup);
   tbScene.Object(tbMainSceneID);
   tbScene.SetEnvironmentLighting(0.84f, 0.84f, 0.84f, 0.44f, 0.44f, 0.44f);

   // create the auxiliary scene
   tbAuxSceneID = tbWorld.CreateScene(1);
   tbAuxScene.Object(tbAuxSceneID);

   // create the dumper scene
   tbDumperSceneID = tbWorld.CreateScene(1);
   tbDumperScene.Object(tbDumperSceneID);

   // create the scene for texture space
   tbOrthoSceneID = tbWorld.CreateScene(2);
   tbOrthoScene.Object(tbOrthoSceneID);

   // create the main camera for 3D
   tbMainCamRefID = tbScene.CreateObject();
   tbCameraRef.Object(tbMainCamRefID);
   tbMainCamID = tbScene.CreateCamera(tbMainCamRefID);
   tbCamera.Object(tbMainCamID);
   tbCamera.Translate(0.0f, 0.0f, 200.0f, LOCAL);
   tbCamera.SetAspect((float)tbWorkSpaceVPWidth/(float)tbWorkSpaceVPHeight);
   tbCamera.SetNear(10.0f);
   tbCamera.SetFar(100000.0f);

   tbCamera.SetDirection(tbCameraFaceDir, tbCameraUpDir);
   tbCamera.SetPosition(tbCameraPosition);
   tbCameraRef.SetDirection(tbCameraRefFaceDir, tbCameraRefUpDir);
   tbCameraRef.SetPosition(tbCameraRefPosition);

   // create the main light
   tbMainLightID = tbScene.CreateLight();
   tbMainLight.Object(tbMainLightID);
   tbMainLight.Rotate(X_AXIS, -30.0f, LOCAL);
   tbMainLight.Translate(60.0, -90.0f, 90.0f, LOCAL);
   tbMainLight.SetColor(0.7f, 0.7f, 0.7f);
   tbMainLight.SetName("MainLight");
   tbMainLight.SetIntensity(1.0f);
   tbMainLight.SetRange(300.0f);

   // create the auxilary light & object
   tbAuxLightID = tbScene.CreateLight(tbMainCamID);
   tbAuxLight.Object(tbAuxLightID);
   tbAuxLight.Rotate(X_AXIS, -30.0f, LOCAL);
   tbAuxLight.SetColor(0.3f, 0.3f, 0.3f);
   tbAuxLight.SetName("AuxLight");
   tbAuxLight.SetIntensity(1.0f);

   // create the auxiliary camera
   tbAuxCamID = tbAuxScene.CreateCamera();
   tbAuxCamera.Object(tbAuxCamID);

   // create the ortho camera
   tbOrthScaleFactor = 3.0f;
   tbOrthoCamID = tbOrthoScene.CreateCamera();
   tbOrthoCamera.Object(tbOrthoCamID);
   tbOrthoCamera.Translate(0.0f, 0.0f, 200.0f, REPLACE);
   tbOrthoCamera.SetProjection(ORTHOGONAL);
   tbOrthoCamera.SetScaleFactor(tbOrthScaleFactor);
   tbOrthoCamera.SetAspect((float)tbWorkSpaceVPWidth/(float)tbWorkSpaceVPHeight);
   tbOrthoCamera.Translate(0.5f, 0.5f, 0.0f, GLOBAL);

   // create the axis
   TbCreateAxis();

   // create a new scene object
   tbCurScene = TbNewScene(tbMainSceneID, tbMainCamRefID, tbMainCamID, tbMainLightID, tbAuxLightID);

   // create fonts
   tbTinyEFontID = tbWorld.CreateShuFa("Trebuchet MS", 18, FALSE, FALSE);
   tbTinyEFont.Object(tbTinyEFontID);

   // create a timer for 30fps
   FyBindTimerFunction(LOOP_TIMER, 30.0f, TbMainLoop, TRUE);
   FyBindTimerFunction(RENDER_TIMER, 30.0f, TbRenderIt, TRUE);
   FyBindTimerFunction(BLINK_TIMER, 2.0f, TbBlinkControl, TRUE, TRUE);

   // bind mouse callbacks
   FyBindMouseFunction(LEFT_MOUSE, TbLeftMousePress, TbLeftMouseDrag, TbLeftMouseRelease, TbLeftMouseDoubleClick);
   FyBindMouseFunction(RIGHT_MOUSE, TbRightMousePress, TbRightMouseDrag, TbRightMouseRelease, NULL);
   FyBindMouseFunction(MIDDLE_MOUSE, TbMiddleMousePress, TbMiddleMouseDrag, TbMiddleMouseRelease, NULL);

   // bind quit function
   FyBindQuitFunction(TbQuitFun);

   // bind keyboard input function
   if (tbShowCommand) {
      FyBindInputFunction(TbCommandInput);
      FyDefineHotKey(FY_LEFT, TbCommandFunKey, TRUE);
      FyDefineHotKey(FY_UP, TbCommandFunKey);
      FyDefineHotKey(FY_RIGHT, TbCommandFunKey, TRUE);
      FyDefineHotKey(FY_DOWN, TbCommandFunKey);
   }

   FyDefineHotKey(FY_F1, TbDebugKey, FALSE);

   // init command queue max 200 commands
   FcSetupCommandQueue(tbNumCommandSlots);

   // create command history buffer
   tbCommandHistory = (char **) FyMalloc(sizeof(char *)*tbNumCommandHistory);
   for (i = 0; i < tbNumCommandHistory; i++) {
      tbCommandHistory[i] = (char *) FyMalloc(MAX_STRING_CHARACTERS);
      *tbCommandHistory[i] = '\0';
   }
   tbCurCommandPtr = tbCommandHistory[0];
   tbCurCommand = 0;

   // initialize the job system to maximum 16 jobs concurrent
   FjUseJobSystem(16);

   // echo the welcome message
   TbSetLogMessage("Welcome to TestBed I Build 2");

   // invoke the Fly
   FyInvokeTheFly(TRUE);
}