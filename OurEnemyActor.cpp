#include "OurEnemyActor.h"
#include "function.h"

void OurEnemyActor::bloodAdjust()
{
	float size[2];
	size[0] = blood_length*HP/HP_MAX;
	size[1] = blood_width;
	blood.SetSize(size);
}


OurEnemyActor::OurEnemyActor()
{
	justRun = false;
}
void OurEnemyActor::detectEnemy(ACTORid enemy)
{
	if(twoObjectDis(this->aID, enemy) < AWARE_DISTANCE)
		this->team->aware = true;
	else
		this->team->aware = false;
}

void OurEnemyActor::walkingAgent(ACTORid enemyID, EnemyTeam **team, int teamCount)
{
	float origin[3] = {0,0,0};

	//敵人位置
	float enemyPos[3];
	FnActor enemy;
	enemy.Object(enemyID);
	enemy.GetPosition(enemyPos);
	//自己位置
	float selfPos[3];
	FnActor self;
	self.Object(aID);
	self.GetPosition(selfPos);
	float selfFDir[3], selfUDir[3];
	actor.GetDirection(selfFDir,selfUDir);

	float distance = twoPointDis(selfPos, enemyPos);

	float flockingPos[3];
	flockingPosition(flockingPos, selfPos, enemyPos, team, teamCount);
	float flockingDis = twoPointDis(flockingPos, origin);

	float newPos[3];
	newPos[0] = flockingPos[0]+selfPos[0];
	newPos[1] = flockingPos[1]+selfPos[1];
	newPos[2] = flockingPos[2]+selfPos[2];

	//先轉向再移動，再轉回來
	actorFaceTo(aID, newPos[0], newPos[1], newPos[2]);
	actor.MoveForward(flockingDis,true);
	//actor.SetWorldDirection(selfFDir,selfUDir);
	actorFaceTo(aID, enemyPos[0], enemyPos[1], enemyPos[2]);
	//actor.SetPosition(newPos);

	if(distance < AWARE_DISTANCE)
	{
		
		if(distance <= COMBAT_DISTANCE)
		{
			sendAction(ourCombatIdleAction);
		}
		else
		{
			sendAction(ourRunAction);
			actor.MoveForward(SPEED, TRUE, FALSE, 0.0f, TRUE);
		}
	}
	else
	{
		sendAction(ourIdleAction);
	}
}
//計算在這個群體中適當的位置
void OurEnemyActor::flockingPosition(float *newPos, float *selfPos, float *targetPos, EnemyTeam **team, int teamCount)
{
	float attraction[3] = {0,0,0};

	//共有多少隊友離你夠近，會影響到調整距離
	int member_num = 0;
	for(int i = 0; i < teamCount; i++)
	{
		//驚動狀態的隊伍才考慮
		if(team[i]->aware)
		{
			//separation & cohesion
			for(int mi = 0; mi < team[i]->member_num; mi++)
			{
				//只檢查沒掛的人，且不要把自己算進去
				if(team[i]->members[mi]->HP > 0 && team[i]->members[mi]->aID != this->aID)
				{
					float friPos[3];
					FnActor fri;
					fri.Object(team[i]->members[mi]->aID);
					fri.GetPosition(friPos);

					float dis = sqrt(pow(friPos[0]-selfPos[0],2)
							+pow(friPos[1]-selfPos[1],2)
							+pow(friPos[2]-selfPos[2],2));

					if(dis < OVERLAP_DISTANCE
					||(team[i] == this->team && dis < CROWDED_DISTANCE))
					{
						//separation
						if(dis == 0)
						{
							attraction[0] += 1;
							attraction[1] += 1;
						}
						else
						{
							attraction[0] += -(friPos[0]-selfPos[0])*pow(1-dis/CROWDED_DISTANCE,2);
							attraction[1] += -(friPos[1]-selfPos[1])*pow(1-dis/CROWDED_DISTANCE,2);
							attraction[2] += -(friPos[2]-selfPos[2])*pow(1-dis/CROWDED_DISTANCE,2);
						}
					}
					else if(team[i] == this->team)
					{
						//cohesion
						attraction[0] += (friPos[0]-selfPos[0])*pow((dis-CROWDED_DISTANCE)/(AWARE_DISTANCE-CROWDED_DISTANCE),2);
						attraction[1] += (friPos[1]-selfPos[1])*pow((dis-CROWDED_DISTANCE)/(AWARE_DISTANCE-CROWDED_DISTANCE),2);
						attraction[2] += (friPos[2]-selfPos[2])*pow((dis-CROWDED_DISTANCE)/(AWARE_DISTANCE-CROWDED_DISTANCE),2);
					}
					member_num++;
				}
			}
		}
	}

	//若與呂布重疊就分離
	float dis = sqrt(pow(targetPos[0]-selfPos[0],2)
			+pow(targetPos[1]-selfPos[1],2)
			+pow(targetPos[2]-selfPos[2],2));

	if(dis < OVERLAP_DISTANCE)
	{
		//separation
		if(dis == 0)
		{
			attraction[0] += 1;
			attraction[1] += 1;
		}
		else
		{
			attraction[0] += -(targetPos[0]-selfPos[0])*pow(1-dis/CROWDED_DISTANCE,2);
			attraction[1] += -(targetPos[1]-selfPos[1])*pow(1-dis/CROWDED_DISTANCE,2);
			attraction[2] += -(targetPos[2]-selfPos[2])*pow(1-dis/CROWDED_DISTANCE,2);
		}
		member_num++;
	}

	if(member_num > 0)
	{
		attraction[0] /= member_num;
		attraction[1] /= member_num;
		attraction[2] /= member_num;
	}

	/*
	//cohesion
	float sum[3] = {0,0,0};
	for(int i = 0; i < member_num; i++)
	{
		if(team->members[i]->aID != aID)
		{
			float friPos[3];
			FnActor fri;
			fri.Object(team->members[i]->aID);
			fri.GetPosition(friPos);

			sum[0] += friPos[0];
			sum[1] += friPos[1];
			sum[2] += friPos[2];
		}
	}
	float avg[3] = {sum[0]/member_num,
					sum[1]/member_num,
					sum[2]/member_num};
	float avgDis = sqrt((avg[0]-selfPos[0])*(avg[0]-selfPos[0])+
						(avg[1]-selfPos[1])*(avg[1]-selfPos[1])+
						(avg[2]-selfPos[2])*(avg[2]-selfPos[2]));

	float cohesion[3] = {(avg[0]-selfPos[0])*pow((avgDis-crowdedDistance)/(awareDistance-crowdedDistance),2),
						 (avg[1]-selfPos[1])*pow((avgDis-crowdedDistance)/(awareDistance-crowdedDistance),2),
						 (avg[2]-selfPos[2])*pow((avgDis-crowdedDistance)/(awareDistance-crowdedDistance),2)};
						 */

	//align
	float align[3] = {0,0,0};
	/*
	float sumV[3] = {0,0,0};
	for(int i = 0; i < member_num; i++)
	{
		if(team->members[i]->aID != aID && team->members[i]->HP > 0)
		{
			float friPos[3];
			FnActor fri;
			fri.Object(team->members[i]->aID);
			fri.GetPosition(friPos);

			float vx = (targetPos[0]-friPos[0]);
			float vy = (targetPos[1]-friPos[1]);
			float vz = (targetPos[2]-friPos[2]);
			float dis = sqrt(sumV[0]*sumV[0]+sumV[1]*sumV[1]+sumV[2]*sumV[2]);
			if(dis != 0)
			{
				sumV[0] += vx/dis;
				sumV[1] += vy/dis;
				sumV[2] += vz/dis;
			}
		}
	}
	if(member_num > 0)
	{
		align[0] = sumV[0]/member_num;
		align[1] = sumV[1]/member_num;
		align[1] = sumV[2]/member_num;
	}
	*/
	newPos[0] = attraction[0]+align[0];
	newPos[1] = attraction[1]+align[1];
	newPos[2] = attraction[2]+align[2];
}