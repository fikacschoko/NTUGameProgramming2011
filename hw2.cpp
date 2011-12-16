#include "TheFlyWin32.h"
#include <windows.h>
#include "function.h"
#include "Camera3rd.h"

float debug[4] = {0,0,0,0};

WORLDid gID = FAILED_ID;
SCENEid sID;
OBJECTid nID, lID;
OBJECTid terrainID;

Lyubu *lyubu;
Camera3rd *camera3rd;

void QuitGame(WORLDid, BYTE, BOOL);
void GameAI(int);

void main(int argc, char **argv)
{
	FnWorld gw;
	FnScene scene;
	FnTerrain terrain;
	FnViewport vp;
	FnCamera camera;
	bool check;

	// create a new world
	gID = FyWin32CreateWorld("Hello TheFly !", 0, 0, 800, 600, 32, FALSE);
	gw.Object(gID);

	// create a 3D scene & the 3D entities
	sID = gw.CreateScene(10);


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


	//Lyubu
	lyubu = new Lyubu( gID, sID );
	//Put an actor on terrain
	lyubu->actor.SetPosition(lyubu->pos_begin);
	check = lyubu->actor.PutOnTerrain(terrainID, false, 0.0f);

	/*3rd looker*/
	camera3rd = new Camera3rd(gID, sID, terrainID, lyubu);
	check = camera3rd->fronter.PutOnTerrain(terrainID, false, 20);

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
	//int i;
	OurAction *action;
	OurFrame *keyFrame;
	
	//main actor attacks enemy
	action = lyubu->getCurrentAction();
	if( lyubu->current_OurAction->type == LyubuAction::ACTION_ATTACK() ){
		keyFrame = lyubu->getKeyFrame();
		if( keyFrame != NULL ){
			/*
			for( i=0 ; i < enemy_num ; i++ ){
				if( beenHit(lyubu->aID, enemyActor[i]->aid, keyFrame) ){
					enemyActor[i]->damaged( keyFrame->damage_pt, mainActor->aid );
				}
			}
			*/
		}
	}
	//enemy attacks main
	//todo: cut:enemy is aware?
	/*
	for( i=0 ; i<enemy_num ; i++ )
	{
		action = enemyActor[i]->getCurrentAction();
		if( action->type == ACTION_ATTACK ){
			keyFrame = enemyActor[i]->getKeyFrame();
			if( keyFrame != null ){
				if( function::beenHit(enemyActor[i]->aid, mainActor->aid, keyFrame) ){
					mainActor->damaged( keyFrame->damage_pt, enemyActor[i]->aid );
				}
			}
		}
	}
	*/
}
/*
void runAllEnemyAI()
{
	int i;
	for( i=0 ; i<enemy_num ; i++ )
	{
		enemyActor[i]->AI();
	}
}

void playAllAction()
{
	int i;
	mainActor->ourPlayAction();
	for( i=0 ; i<enemy_num ; i++ )
	{
		enemyActor[i]->ourPlayAction();
	}
}
*/
//------------------------------------------------------------------------------------
// timer callback function which will be invoked by TheFly3D System every 1/30 second
// C.Wang 0308, 2004
//------------------------------------------------------------------------------------
void GameAI(int skip)
{
	FnWorld gw;

	camera3rd->movement();
	lyubu->dealKey();
	lyubu->ourPlayAction();
	camera3rd->render();

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