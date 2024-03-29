#pragma comment(lib, "TheFlyLibD.lib")
#include "TheFlyWin32.h"
#include <windows.h>
#include "function.h"
#include "Camera3rd.h"
#include "robber02.h" 
#include "FyMedia.h"
#include "FyFx.h"
#include <math.h>
#include "AllData.h"
#include "FPSview.h"

float debug[6] = {0,0,0,0};

WORLDid gID = FAILED_ID;
SCENEid sID;
OBJECTid nID, lID;
OBJECTid terrainID;
SHUFAid saID;

EnemyTeam *team[10];
int teamCount = 0;
int enemyCount = 0;
Lyubu *lyubu;
Donzo *donzo;
Camera3rd *camera3rd;
int enemy_num = 5;
OurEnemyActor *enemyActor[5];

void QuitGame(WORLDid, BYTE, BOOL);
void GameAI(int);

//FX rock
eF3DFX *fx00 = NULL;
FPSview *fpsview;
void main(int argc, char **argv)
{
	debug[0] = debug[1] = debug[2] = debug[3] = debug[4] = debug[5] = 0;
	FnWorld gw;
	FnScene scene;
	FnTerrain terrain;
	FnViewport vp;
	FnCamera camera;
	bool check;

	// create a new world
	gID = FyWin32CreateWorld("Hello TheFly !", 0, 0, 1300, 600, 32, FALSE);
	gw.Object(gID);

	//load Audio
	::loadAll( gID );

	// create a 3D scene & the 3D entities
	sID = gw.CreateScene(10);
	saID = gw.CreateShuFa("新細明體",12);


	//load scenes
	gw.SetObjectPath("Data\\NTU4\\Scenes");
	gw.SetTexturePath("Data\\NTU4\\Scenes\\Textures");
	gw.SetScenePath("Data\\NTU4\\Scenes");
	gw.SetShaderPath( "Data\\NTU4\\Shaders" );
	scene.Object(sID);
	scene.Load("scene2");


	//load terrain
	terrainID = scene.CreateTerrain(ROOT);
	gw.SetObjectPath("Data\\NTU4\\Scenes");
	gw.SetTexturePath("Data\\NTU4\\Scenes");
	terrain.Object(terrainID);
	terrain.Load( "terrain" );
	terrain.GenerateTerrainData();

	 // play lab.mp3
	FyBeginMedia("Data\\Media", 1);
	HWND hwnd = FyGetWindowHandle(gw.Object());
	MEDIAid mmID = FyCreateMediaPlayer(0, "menu", 0, 0, 0, 0);
	FnMedia mP;
	mP.Object(mmID);

	//Lyubu
	lyubu = new Lyubu( gID, sID );
	//Put an actor on terrain
	lyubu->actor.SetPosition(lyubu->pos_begin);
	check = lyubu->actor.PutOnTerrain(terrainID, false, 0.0f);

	//Donzo
	donzo = new Donzo( gID, sID );
	//Put an actor on terrain
	team[teamCount] = new EnemyTeam(teamCount, 1);
	team[teamCount]->members[0] = donzo;
	donzo->actor.SetPosition(donzo->pos_begin);
	donzo->team = team[teamCount];
	check = donzo->actor.PutOnTerrain(terrainID, false, 0.0f);
	enemyActor[enemyCount++] = donzo;
	teamCount++;

	team[teamCount] = new EnemyTeam(teamCount, 4);
	for(int i = 0; i < 4; i++)
	{
		//Robber02
		Robber02 *robber = new Robber02( gID, sID );
		//Put an actor on terrain
		float pos[3];
		pos[0] = robber->pos_begin[0] + rand()%100;
		pos[1] = robber->pos_begin[1] + rand()%100;
		pos[2] = robber->pos_begin[2];
		robber->actor.SetPosition(pos);
		robber->team = team[teamCount];
		check = robber->actor.PutOnTerrain(terrainID, false, 0.0f);
		enemyActor[enemyCount++] = robber;

		team[teamCount]->members[i] = robber;
	}
	teamCount++;

	/*3rd looker*/
	camera3rd = new Camera3rd(gID, sID, terrainID, lyubu);
	check = camera3rd->fronter.PutOnTerrain(terrainID, false, 20);

	fpsview = new FPSview(gID, sID, terrainID, 800, 0, 1300-800, 600);
	FPSview::setClsNow(fpsview);
	FyBindMouseMoveFunction( FPSview::turn );

	float actPos[3];
	lyubu->actor.GetPosition(actPos);
	camera3rd->SetPosition(actPos, lyubu->aID);


	nID = scene.CreateObject(ROOT);
	lID = scene.CreateLight(ROOT);
	// translate the light
	FnLight light;
	light.Object(lID);
	light.SetName("MainLight");
	//light.Translate(-50.0f, -50.0f, 50.0f, GLOBAL);

	// set Hotkeys
	FyDefineHotKey(FY_ESCAPE, QuitGame, FALSE);

	/* bind a timer, frame rate = 30 fps */
	FyBindTimer(0, 30.0f, GameAI, TRUE);
	// invoke the system
	FyInvokeTheFly(TRUE);
}

void dealAllAttack()
{
	int i;
	OurAction* action;
	OurFrame* keyFrame;
	
	//main actor attacks enemy
	action = lyubu->getCurrentAction();
	if( action->isAttack){
		keyFrame = lyubu->getKeyFrame();
		if( keyFrame != NULL ){
			for( i=0 ; i<enemy_num ; i++ ){
				float angle = twoObjectAngle( lyubu->actor.GetBaseObject(), enemyActor[i]->actor.GetBaseObject() );
				if( ::beenHit(lyubu->aID, enemyActor[i]->aID, *keyFrame, angle) ){

					enemyActor[i]->damaged( keyFrame->damage_pt, lyubu->aID, angle );
				}
			}
		}
	}

	//enemy attacks main
	//todo: cut:enemy is aware?
	for( i=0 ; i<enemy_num ; i++ )
	{
		action = enemyActor[i]->getCurrentAction();
		if( action->isAttack ){
			keyFrame = enemyActor[i]->getKeyFrame();
			if( keyFrame != NULL ){
				float angle = twoObjectAngle( enemyActor[i]->actor.GetBaseObject(), lyubu->actor.GetBaseObject() );
				if( ::beenHit(enemyActor[i]->aID, lyubu->aID, *keyFrame, angle) ){
					lyubu->damaged( keyFrame->damage_pt, enemyActor[i]->aID, angle);
				}
			}
		}
	}
}

void runAllEnemyAI()
{
	int i;
	ACTORid *friends = new ACTORid[enemy_num];
	for( i=0 ; i<enemy_num ; i++ )
	{
		friends[i] = enemyActor[i]->aID;
	}
	for( i=0 ; i<enemy_num ; i++ )
	{
		enemyActor[i]->AI(lyubu->aID, team, teamCount);
	}
}

void playAllAction()
{
	int i;
	lyubu->ourPlayAction();
	for( i=0 ; i<enemy_num ; i++ )
	{
		enemyActor[i]->ourPlayAction();
	}
}

//------------------------------------------------------------------------------------
// timer callback function which will be invoked by TheFly3D System every 1/30 second
// C.Wang 0308, 2004
//------------------------------------------------------------------------------------
void GameAI(int skip)
{
	FnWorld gw;
	static int orz=0;

	FXcenter::playAllFX( skip );
	runAllEnemyAI();
	camera3rd->movement();
	lyubu->dealKey();
	playAllAction();
	dealAllAttack();
	camera3rd->render();
	camera3rd->vp.Render2D(lyubu->s2D, FALSE, TRUE);
	fpsview->move();
	fpsview->render();

	FnShuFa sa;
	sa.Object(saID);
	sa.Begin(camera3rd->vID);
	//sa.Begin(fpsview->vID);
	char debugStr[100];
	sprintf(debugStr, "(x, y, z): (%.2f, %.2f, %.2f)\n%.2f\n%.2f\n%.2f", debug[0],debug[1],debug[2],debug[3],debug[4],debug[5]);
	sa.Write(debugStr, 10, 10, 255,0,0);
	sa.End();

	gw.Object(gID);
	gw.SwapBuffers();
}


//-------------------
// quit the demo
// C.Wang 0304, 2004
//-------------------
void QuitGame(WORLDid gID, BYTE code, BOOL value)
{
   if (code == FY_ESCAPE ) {
      if (value) {
         FyWin32EndWorld(gID);
      }
   }
}