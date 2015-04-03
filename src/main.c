/*
 * main.c
 *
 *  Created on: Mar 7, 2015
 *      Author: julien
 */

#include "main.h"

int main(void)
{
	// PositionPlatforme[]= { Posx, Posy, Posz, Rotx, Roty, Rotz }
	float PositionPlatforme[] = {0,0,0,0.3,0,0};
	float AngleServos[6];
	initReverseKinematics();
	setPos(PositionPlatforme, AngleServos);

	int i;
	for(i=0;i<6;i++)
		printf("Angle %d : %f\n",i,(AngleServos[i]*180.0)/3.14159);



	return 0;
}
