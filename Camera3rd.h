#ifndef _CAMERA3RD_H_
#define _CAMERA3RD_H_
#include "TheFlyWin32.h"
#include "Lyubu.h"
#include "Donzo.h"
#define PI 3.14159265

class Camera3rd
{
	private:
		float fronterPos[3], centerPos[3], higherPos[3];
	public:
		FnObject fronter, center, higher;

		WORLDid gID;
		SCENEid sID;
		OBJECTid terrainID;
		OBJECTid fronterID, centerID, higherID, cameraID;
		VIEWPORTid vID;

		Lyubu *lyubu;

		float disBtwActFro, disBtwFroCen, disBtwCenHig;
		float disBtwActFro_max, disBtwActFro_std, disBtwActFro_min;
		float disBtwCenHig_max, disBtwCenHig_std;
		float disBtwFroCen_std, disBtwFroCen_min;

		Camera3rd(WORLDid , SCENEid, OBJECTid, Lyubu*);
		void SetPosition(float[], ACTIONid);
		OBJECTid getFronterID();
		void update(ACTIONid);
		void Rotate(float degree, float[]);
		void Camera3rd::getCameraPosition(float[]);
		void movement();
		void render();
};
#endif