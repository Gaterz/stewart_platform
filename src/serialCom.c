/*
 * serialCom.c
 *
 *  Created on: Mar 7, 2015
 *      Author: julien
 */
#include "allHeaders.h"

#define pi  3.14159
#define DEG_TO_RAD(X) (X*pi/180.0)

#define VALEUR_OPEN 255
#define VALEUR_CLOSE 255

#define ANGLE_MIN_N_FORMATE -80
#define ANGLE_MAX_N_FORMATE 80

#define ANGLE_MIN_FORMATE 0
#define ANGLE_MAX_FORMATE 254

#include <termios.h>
#include <fcntl.h>
#include <unistd.h>



int openPort(char link[])
{
  int fd; /* File descriptor for the port */


  fd = open(link, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
  if (fd != -1)
    fcntl(fd, F_SETFL, 0);

  return (fd);
}

void closePort(int fd)
{
	close(fd);
}

void initPort(int fd, int vitesse)
{
	int deb;
	struct termios	termios_p;

	switch(vitesse)
		{
		case 2400 :
			deb = 0000013;
			break;
		case 4800 :
			deb = 0000014;
			break;
		case 9600 :
			deb = 0000015;
			break;
		case 19200 :
			deb = 0000016;
			break;
		case 38400 :
			deb = 0000017;
			break;
		default :
			deb = 0000015;
			break;
		}


	/* Lecture des parametres courants */
	tcgetattr(fd,&termios_p);

	/* On ignore les BREAK */
	termios_p.c_iflag = IGNBRK ;

	/* Pas de mode de sortie particulier */
	termios_p.c_oflag = 0;


	/* Liaison a "deb" bps avec 8 bits de donnees*/
	termios_p.c_cflag = deb | CS8;

	/* Mode non-canonique avec echo */
	termios_p.c_lflag = ECHO;

	/* Mode RAW */
	termios_p.c_lflag = ~ICANON;

	/* Caracteres immediatement disponibles */
	termios_p.c_cc[VMIN] = 1;
	termios_p.c_cc[VTIME] = 0;

	/* Sauvegarde des nouveaux parametres */
	tcsetattr(fd,TCSANOW,&termios_p);
}

void formatDonnees(float nFormate[], unsigned char Formate[])
{
	int i;
	for(i=0; i<6;i++)
		Formate[i] = (unsigned char)((nFormate[i] - DEG_TO_RAD(ANGLE_MIN_N_FORMATE)) * (ANGLE_MAX_FORMATE - ANGLE_MIN_FORMATE) / (DEG_TO_RAD(ANGLE_MAX_N_FORMATE) - DEG_TO_RAD(ANGLE_MIN_N_FORMATE)) + ANGLE_MIN_FORMATE);
}

int sendAngles(int fd, unsigned char anglesFormate[])
{
	unsigned char data[8];

	data[0] = VALEUR_OPEN;
	data[1] = anglesFormate[0];
	data[2] = anglesFormate[1];
	data[3] = anglesFormate[2];
	data[4] = anglesFormate[3];
	data[5] = anglesFormate[4];
	data[6] = anglesFormate[5];
	data[7] = VALEUR_CLOSE;

	if(write(fd,data,8) == 8)
		return 1;
	else
		return 0;
}

