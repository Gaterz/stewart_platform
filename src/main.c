/*
 * main.c
 *
 *  Created on: Mar 7, 2015
 *      Author: julien
 */


#include <fcntl.h>
#include <unistd.h>

#include <gtk/gtk.h>



#include "main.h"

#define pi  3.14159
#define DEG_TO_RAD(X) (X*pi/180.0)

int main(int argc, char **argv)
{
	float PositionPlatforme[] = {0,0,0,0,0,0};//= { Posx, Posy, Posz, Rotx, Roty, Rotz }
	float AngleServos[6];
	unsigned char AnglesFormate[6];
	int fileDesc;


	initReverseKinematics();
	fileDesc = openPort("/dev/ttyACM0");

	if(fileDesc == -1)
	{
		printf("Impossible d'ouvrir le port !! \n");
		return 0;
	}


	initPort(fileDesc, 9600);

	positionPlatforme2Anges(PositionPlatforme, AngleServos);
	formatDonneesGtkWidget(AngleServos, AnglesFormate);
	sendAngles(fileDesc, AnglesFormate);

	usleep(200);

	close(fileDesc);














	return 1;
}
