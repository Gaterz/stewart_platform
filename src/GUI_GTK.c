/*
 * GUI_GTK.c
 *
 *  Created on: 16 avr. 2015
 *      Author: maxime
 */

#include "allHeaders.h"

void OpenWindows(int argc,char ** argv)
{
	GtkWidget *p_window = NULL;

	/* Initialisation de GTK+ */
	gtk_init (&argc, &argv);

	/* Creation de la fenetre principale de notre application */
	p_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

	/* Affichage de la fenetre principale */
	gtk_widget_show (p_window);
	/* Lancement de la boucle principale */
	gtk_main ();
}
