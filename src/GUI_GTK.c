/*
 * GUI_GTK.c
 *
 *  Created on: 16 avr. 2015
 *      Author: maxime
 */

#include "allHeaders.h"

#include "mainHeaders.h"

void gtk_main_quit()
{
	gtk_main_quit ();
}

typedef struct Window
{
	GtkWidget *fenetre_principale;
	GtkBuilder *builder;
	GError *error;
	gchar *filename;
} s_Window;

s_Window Windows;





CvCapture * cap;


void refreshVideo()
{
	GtkImage *gtkImg1 = NULL;
	GtkImage *gtkImg2 = NULL;
	GdkPixbuf* pix_Video1;
	GdkPixbuf* pix_Video2;
	IplImage *ocv_image_Video1;
	IplImage *ocv_image_Video2;

	//Image utilisées
	ocv_image_Video1 = cvQueryFrame(cap);
	ocv_image_Video2 = cvCloneImage(ocv_image_Video1);

	//Recuperation du Winget
	gtkImg1 = GTK_IMAGE(gtk_builder_get_object (Windows.builder,"Video1"));
	gtkImg2 = GTK_IMAGE(gtk_builder_get_object (Windows.builder,"Video2"));

	//Clear de l'image
	gtk_image_clear(gtkImg1);
	gtk_image_clear(gtkImg2);

	//OCV image to GTK image
	cvCvtColor( ocv_image_Video1, ocv_image_Video1, CV_BGR2RGB );
	pix_Video1 = gdk_pixbuf_new_from_data(( guchar* )ocv_image_Video1->imageData,
	                                             GDK_COLORSPACE_RGB,
	                                             FALSE,
												 ocv_image_Video1->depth,
												 ocv_image_Video1->width,
												 ocv_image_Video1->height,
	                                             ( ocv_image_Video1->widthStep ),
	                                             NULL, NULL);

	cvCvtColor( ocv_image_Video2, ocv_image_Video2, CV_BGR2RGB );
	pix_Video2 = gdk_pixbuf_new_from_data(( guchar* )ocv_image_Video2->imageData,
			                                     GDK_COLORSPACE_RGB,
			                                     FALSE,
												 ocv_image_Video2->depth,
												 ocv_image_Video2->width,
												 ocv_image_Video2->height,
			                                     ( ocv_image_Video2->widthStep ),
			                                     NULL, NULL);

	//Maj des buffer videos
	gtk_image_set_from_pixbuf(gtkImg1,pix_Video1);
	gtk_image_set_from_pixbuf(gtkImg2,pix_Video2);

	//Affichage des nouvelles images
	gtk_widget_show(GTK_IMAGE(gtkImg1));
	gtk_widget_show(GTK_IMAGE(gtkImg2));

	usleep(0); //Ne fonctionne pas sans ...
}


int OpenWindows(int argc,char ** argv)
{

	gtk_init(&argc, &argv);

    // Initialisation de la librairie Gtk.
	cap =cvCaptureFromCAM(0);
				if(cap==NULL)
				{
					 perror("unable to open cam");
					 exit(EXIT_FAILURE);
				}


    // Ouverture du fichier Glade de la fenêtre principale
    Windows.builder = gtk_builder_new();


    // Création du chemin complet pour accéder au fichier test.glade.
    // g_build_filename(); construit le chemin complet en fonction du système
    // d'exploitation. ( / pour Linux et \ pour Windows)
    Windows.filename =  g_build_filename ("GUI.glade", NULL);


    // Chargement du fichier GUI.glade.
    gtk_builder_add_from_file (Windows.builder, Windows.filename, &(Windows.error));
    g_free (Windows.filename);

    if (Windows.error)
    {
    	gint code = (Windows.error)->code;
        g_printerr("%s\n", (Windows.error)->message);
        g_error_free (Windows.error);
        return code;
    }

    gtk_builder_connect_signals (Windows.builder, NULL);

     // Récupération du pointeur de la fenêtre principale
    Windows.fenetre_principale = GTK_WIDGET(gtk_builder_get_object (Windows.builder, "window"));
    //fenetre_principale = GTK_WIDGET(gtk_builder_get_object (builder, "window"));

    // Affichage de la fenêtre principale.
    gtk_widget_show_all (Windows.fenetre_principale);


    gtk_main();

    //Close webcam
    cvReleaseCapture(&cap);
    return 0;

}






void ggg(GtkRange *scale, gpointer  data )
{
	   /*-----------------curseur-------------------------*/
	   gdouble val;

	   val = gtk_range_get_value( scale );


	   printf("%f\n",(float)val);
	   g_print( "%f\n", val );
	   /*-------------------------------------------------*/
}


