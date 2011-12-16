#ifndef _LYUBU_H_
#define _LYUBU_H_
#include "TheFlyWin32.h"
#include "OurActor.h"
#include "OurAction.h"
#define PI 3.14159265

enum Direction{LEFT,RIGHT,FORWARD,BACK};
class LyubuAction : public Action_type
{
public:
	static char*	ACTION_NORMAL_ATTACK1(){	return "LYUBU_NORMAL_ATTACK1"; }
	static char*	ACTION_NORMAL_ATTACK2(){	return "LYUBU_NORMAL_ATTACK2"; }
	static char*	ACTION_NORMAL_ATTACK3(){	return "LYUBU_NORMAL_ATTACK3"; }
	static char*	ACTION_NORMAL_ATTACK4(){	return "LYUBU_NORMAL_ATTACK4"; }
};

class Lyubu : public OurActor{
	private:
		WORLDid gID;
		SCENEid sID;

		//new
		//暫定紀錄最近5個按鍵
		char pressed_key[5];
		//目前動作，用flag表示
		enum ActFlag{NORMAL_ATTACK1,NORMAL_ATTACK2,NORMAL_ATTACK3,NORMAL_ATTACK4,OTHERS};
		ActFlag actFlag;
		//呂布有的動作

		void dealFlag();
	public:
		FnActor actor;

		OurAction *ourRunAction;
		AttackAction *ourAttack1Action;
		AttackAction *ourAttack2Action;
		AttackAction *ourAttack3Action;
		AttackAction *ourAttack4Action;
		bool sendAction(OurAction*);
		float pos_begin[3];
		float MOVE_SPEED, MOVE_ANGLE;

		Lyubu( WORLDid , SCENEid );
		void dealKey();
		void Rotate(int degree, float cameraPos[]);
		void damaged(int attack_pt){
			return;
		}
};
#endif