#include "TheFlyWin32.h"
#include "function.h"
#include "Camera3rd.h"
#define PI 3.14159265

Camera3rd::Camera3rd(WORLDid gID, SCENEid sID, OBJECTid terrainID, Lyubu *lyubu)
{
	FnWorld gw;
	FnScene scene;
	FnCamera camera;
	FnViewport vp;

	//initial
	gw.Object(gID);
	scene.Object(sID);
	this->gID = gID;
	this->sID = sID;
	this->terrainID = terrainID;
	this->lyubu = lyubu;
	//viewport
	vID = gw.CreateViewport(0, 0, 800, 600);
	vp.Object(vID);
	vp.SetBackgroundColor(0.3f, 0.4f, 0.5f);

	//camera
	cameraID = scene.CreateCamera(ROOT);
	camera.Object(cameraID);
	camera.SetFov(50);  // camera look dis

	//initial value
	disBtwActFro_max = 60;
	disBtwActFro_min = 20;
	disBtwActFro_std = 40;
	disBtwActFro = disBtwActFro_std;

	disBtwFroCen_min = 10;
	disBtwFroCen_std = 200;
	disBtwFroCen = disBtwFroCen_std;
	
	disBtwCenHig = 120;
	disBtwCenHig_max = 350;
	disBtwCenHig_std = 120;


	fronterID = scene.CreateObject();
	centerID = scene.CreateObject(fronterID);
	higherID = scene.CreateObject(centerID);
	fronter.Object(fronterID);
	center.Object(centerID);
	higher.Object(higherID);

	fronterPos[0] = 0;
	fronterPos[1] = 0;
	fronterPos[2] = 0;

	centerPos[0] = 0;
	centerPos[1] = disBtwFroCen;
	centerPos[2] = 0;

	higherPos[0] = 0;
	higherPos[1] = 0;
	higherPos[2] = disBtwCenHig;

	fronter.SetPosition(fronterPos);
	center.SetPosition(centerPos);
	higher.SetPosition(higherPos);

	camera.SetParent(higherID);
}
void Camera3rd::SetPosition(float pos[3], ACTIONid actorID)
{
	update( actorID );
	if( pos != NULL )
	{
	fronterPos[0] = pos[0];
	fronterPos[1] = pos[1];
	fronterPos[2] = pos[2];

	fronter.SetPosition(fronterPos);
	}
	float	cx=0,
			cy=disBtwActFro+disBtwFroCen,
			cz=disBtwCenHig; 

	static float lookpos[]={0,0,80}; 

	float x2=-cx+lookpos[0], y2=-cy+lookpos[1], z2=-cz+lookpos[2], x3=x2, y3=y2, z3=0;
	float v1v2=x2*x3+y2*y3+z2*z3;
	float v1=sqrt(x2*x2+y2*y2+z2*z2);
	float v2=sqrt(x3*x3+y3*y3+z3*z3);
	float down_angle = acos(v1v2/(v1*v2))*180/PI;


	FnCamera camera;
	camera.Object(cameraID);

	camera.Rotate( X_AXIS, 90+down_angle, REPLACE);

	camera.Rotate(Y_AXIS, 180.0f, LOCAL);
}
OBJECTid Camera3rd::getFronterID()
{
	return fronterID;
}

void Camera3rd::update(ACTIONid actorID)
{
	float apos[3], selfpos[3];
	FnActor actor;
	actor.Object( actorID );

	actor.GetWorldPosition( apos );
	fronter.GetWorldPosition( selfpos );
	float x=apos[0]-selfpos[0], y=apos[1]-selfpos[1], z=apos[2]-selfpos[2];
	disBtwActFro = sqrt( x*x + y*y + z*z );

	higherPos[0] = 0;
	higherPos[1] = 0;
	higherPos[2] = disBtwCenHig;
	higher.SetPosition(higherPos);

	centerPos[0] = 0;
	centerPos[1] = disBtwFroCen;
	centerPos[2] = 0;
	center.SetPosition(centerPos );
}

void Camera3rd::Rotate(float degree, float distanceToOrigin[])
{
	float rad = degree*2*PI/360;
	/*
	fronter.Translate(-distanceToOrigin[0],-distanceToOrigin[1],-distanceToOrigin[2], GLOBAL);
	fronter.GetPosition(temp);

	x[0] = temp[0]-centerPos[1]*cos(theta);
	y[0] = temp[1]-centerPos[1]*sin(theta);

	fronter.Rotate(Z_AXIS, degree, GLOBAL);
	fronter.GetPosition(temp);

	x[1] = temp[0]-centerPos[1]*cos(theta);
	y[1] = temp[1]-centerPos[1]*sin(theta);

	fronter.Translate(distanceToOrigin[0],distanceToOrigin[1],distanceToOrigin[2], GLOBAL);
	fronter.GetPosition(fronterPos);
	*/
	float M[12] = {	cos(rad), sin(rad), 0,
					-sin(rad), cos(rad), 0,
					0,	0,	1,
					-distanceToOrigin[0]*cos(rad)+distanceToOrigin[1]*sin(rad)+distanceToOrigin[0],-distanceToOrigin[0]*sin(rad)-distanceToOrigin[1]*cos(rad)+distanceToOrigin[1], 0
	};
	fronter.SetMatrix(M, GLOBAL);
	fronter.GetPosition(fronterPos);
}


void Camera3rd::getCameraPosition(float pos[])
{
	float fdir[3], udir[3];
	fronter.GetDirection(fdir, udir);
	float theta = atan(fdir[1]/fdir[0]);
	if(fdir[0] < 0)
		theta += PI;
	pos[0] = fronterPos[0]-centerPos[1]*cos(theta);
	pos[1] = fronterPos[1]-centerPos[1]*sin(theta);
	pos[2] = fronterPos[2]+higherPos[2];
}

void Camera3rd::movement()
{
	float MOVE_SPEED=10.0;
	float CAM_SPEED=3;
	float ANGLE_SPEED=3;
	float cameraPos[3];
	float actPos[3];
	float fdir[3], udir[3];
	int result;

	getCameraPosition(cameraPos);
	lyubu->actor.GetPosition(actPos);

	update(lyubu->aID);

	if (FyCheckHotKeyStatus(FY_UP)) {
		if(lyubu->sendAction(lyubu->ourRunAction))
		{
			result = lyubu->actor.MoveForward( MOVE_SPEED, TRUE, FALSE, 0.0f, TRUE);
			fronter.GetWorldDirection( fdir, udir );
			lyubu->actor.SetWorldDirection( fdir, udir );

			if (result != BOUNDARY) {
				if( disBtwActFro >= disBtwActFro_max )
				{	
					fronter.MoveForward(MOVE_SPEED, true, FALSE, 20);
					if( disBtwFroCen < disBtwFroCen_std )
						disBtwFroCen += MOVE_SPEED;
				}
				else
				{
					fronter.MoveForward(CAM_SPEED, true, FALSE, 20);
					if( disBtwFroCen < disBtwFroCen_std )
						disBtwFroCen += CAM_SPEED;
				}
			}
		}

		if( disBtwCenHig > disBtwCenHig_std )
			disBtwCenHig-=10;
	}
	else if(FyCheckHotKeyStatus(FY_LEFT)){	
		if(lyubu->sendAction(lyubu->ourRunAction))
		{
			float pos[3];
			fronter.GetWorldDirection( fdir, udir );
			lyubu->actor.SetWorldDirection( fdir, udir );
			lyubu->actor.TurnRight(-90-ANGLE_SPEED);
			lyubu->actor.GetWorldPosition(pos);
			SetPosition( pos, lyubu->aID );

			lyubu->actor.MoveForward( ::twoObjectDis(lyubu->actor.GetBaseObject(),centerID)*PI/180 , TRUE);
			fronter.TurnRight(-ANGLE_SPEED);
		}
	}
	else if (FyCheckHotKeyStatus(FY_RIGHT) ){
		if(lyubu->sendAction(lyubu->ourRunAction))
		{
			float pos[3];
			fronter.GetWorldDirection( fdir, udir );
			lyubu->actor.SetWorldDirection( fdir, udir );
			lyubu->actor.TurnRight(90+ANGLE_SPEED);
			lyubu->actor.GetWorldPosition(pos);
			SetPosition( pos, lyubu->aID );

			lyubu->actor.MoveForward( ::twoObjectDis(lyubu->actor.GetBaseObject(),centerID)*PI/180 , TRUE);
			fronter.TurnRight(ANGLE_SPEED);
		}
	}
	else if (FyCheckHotKeyStatus(FY_DOWN)) {
		if(lyubu->sendAction(lyubu->ourRunAction))
		{
			float c2pos[3];
			fronter.GetWorldDirection( fdir, udir );
			lyubu->actor.SetWorldDirection( fdir, udir );
			lyubu->actor.TurnRight(180);

			lyubu->actor.GetWorldPosition(c2pos);
			float pos[3];
			center.HitTest(c2pos, fdir, pos, FALSE);
			if( ::posObjectDis( pos, centerID ) > MOVE_SPEED )
			{
				result = lyubu->actor.MoveForward( MOVE_SPEED, TRUE, FALSE, 0.0f, TRUE);
			}
		
			if (result != BOUNDARY) {
				//hittest
				FnObject cam;
				cam.Object( cameraID );
				cam.GetWorldDirection( fdir, udir );
				fdir[0] *= -1;
				fdir[1] *= -1;
				fdir[2] *= 0;
				//actor.GetWorldDirection( fdir, NULL );
				cam.GetWorldPosition(c2pos);
				c2pos[2]=40;
				FnTerrain terrain;
				terrain.Object(terrainID);
				float pos[3];
				if( terrain.HitTest(c2pos, fdir, pos, FALSE) ){
					//QuitGame(gID, FY_ESCAPE, true);
					if( ::posObjectDis( pos, centerID ) > 40 )
					{
						if( disBtwActFro <= disBtwActFro_min )
						{	
							fronter.MoveForward(-MOVE_SPEED, true, FALSE, 20);
						}
						else
						{
							fronter.MoveForward(-CAM_SPEED, true, FALSE, 20);
						}
					}
					else if(disBtwFroCen > disBtwFroCen_min )
					{
						if( disBtwActFro <= disBtwActFro_min )
						{	
							fronter.MoveForward(-MOVE_SPEED, true, FALSE, 20);
							disBtwFroCen -= MOVE_SPEED;
						}
						else
						{
							fronter.MoveForward(-CAM_SPEED, true, FALSE, 20);
							disBtwFroCen -= CAM_SPEED;
						}
						if( disBtwCenHig < disBtwCenHig_max )
							disBtwCenHig+=10;
					}
				}
				else
				{
					if( disBtwActFro <= disBtwActFro_min )
					{	
						fronter.MoveForward(-MOVE_SPEED, true, FALSE, 20);
					}
					else
					{
						fronter.MoveForward(-CAM_SPEED, true, FALSE, 20);
					}
				
				}
			}
			else
			{
				if( disBtwCenHig < disBtwCenHig_max )
					disBtwCenHig+=10;
			}
		}
	}
	else{  //IDLE  ,  no move
		if(lyubu->sendAction(lyubu->ourIdleAction))
		{
		//::actorChangePose( lyubu->aID, lyubu->ourIdleAction->actID );
		
			if( disBtwActFro > disBtwActFro_std + CAM_SPEED*3 )
			{
				fronter.MoveForward(CAM_SPEED*3, true, FALSE, 20);
			}
			else if(  disBtwActFro < disBtwActFro_std - CAM_SPEED*2 )
			{
				if( disBtwCenHig_std == disBtwCenHig )
				{
					fronter.MoveForward(-CAM_SPEED*2, true, FALSE, 20);
				}
			}
		}
	}

	//just let camera look lyubu
	SetPosition(NULL, lyubu->aID);
}


void Camera3rd::render()
{
	FnViewport vp;
	vp.Object(vID);
	vp.Render(cameraID, TRUE, TRUE);
}