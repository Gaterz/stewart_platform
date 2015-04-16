/*
 * main.c
 *
 *  Created on: Mar 7, 2015
 *      Author: julien
 */


#include <fcntl.h>
#include <unistd.h>

#include <gtk/gtk.h>


//#include <gtk/gtktypes.h>
//#include <gtk/gtkmain.h>



#include "main.h"

#define pi  3.14159
#define DEG_TO_RAD(X) (X*pi/180.0)

int main(int argc, char **argv)
{
	float PositionPlatforme[] = {0,0,0,0,0,0};//= { Posx, Posy, Posz, Rotx, Roty, Rotz }
	float AngleServos[6];
	unsigned char AnglesFormate[6];
	int fileDesc;


	GtkWidget *p_window = NULL;

	  /* Initialisation de GTK+ */
	  gtk_init (&argc, &argv);

	  /* Creation de la fenetre principale de notre application */
	  p_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

	  /* Affichage de la fenetre principale */
	  gtk_widget_show (p_window);
	  /* Lancement de la boucle principale */
	  gtk_main ();


	  return EXIT_SUCCESS;




	initReverseKinematics();
	fileDesc = openPort("/dev/ttyACM0");

	if(fileDesc == -1)
	{
		printf("Impossible d'ouvrir le port !! \n");
		return 0;
	}

/*
	initPort(fileDesc, 9600);

	positionPlatforme2Anges(PositionPlatforme, AngleServos);
	formatDonneesGtkWidget(AngleServos, AnglesFormate);
	sendAngles(fileDesc, AnglesFormate);

	usleep(200);

	close(fileDesc);*/














	return 1;
}
