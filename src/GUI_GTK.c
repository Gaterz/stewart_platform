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
	return;

}

GtkWidget *fenetre_principale = NULL;
GtkWidget * table1=NULL;
GtkBuilder *builder = NULL;
GError *error = NULL;
gchar *filename = NULL;
IplImage *ocv_image;
CvCapture * cap;
GdkPixbuf* pix;
void presse1()
{

	printf("presse\n");


	GtkImage *imageSansTraitement = NULL;

	ocv_image = cvQueryFrame(cap);

	cvShowImage("hsv",ocv_image);

	//Recuperation du Winget
	imageSansTraitement = GTK_IMAGE(gtk_builder_get_object (builder,"SansTraitement"));
	//Clear de l'image
	gtk_image_clear(imageSansTraitement);


	//Charger image avec OCV
	//ocv_image = cvLoadImage( "test.jpg", CV_LOAD_IMAGE_UNCHANGED);

	// Convert OCV image to GTK image
	//imageSansTraitement = image_from_ocv(ocv_image, 300, 300);

	cvCvtColor( ocv_image, ocv_image, CV_BGR2RGB );
	   pix = gdk_pixbuf_new_from_data(( guchar* )ocv_image->imageData,
	                                             GDK_COLORSPACE_RGB,
	                                             FALSE,
	                                             ocv_image->depth,
	                                             ocv_image->width,
	                                             ocv_image->height,
	                                             ( ocv_image->widthStep ),
	                                             NULL, NULL);

	   gtk_image_set_from_pixbuf(imageSansTraitement,pix);
	   /*gdk_draw_pixbuf( imageSansTraitementwindow,
	                    widget->style->fg_gc[ GTK_WIDGET_STATE( widget )],
	                    pix,
	                    0, 0, 0, 0,
	                    opencvImage->width,
	                    opencvImage->height,
	                    GDK_RGB_DITHER_MAX,
	                    0, 0);*/



	//gtk_image_set_from_file(GTK_IMAGE(imageSansTraitement), "test.jpg");

	gtk_widget_show(GTK_IMAGE(imageSansTraitement));





}


int OpenWindows(int argc,char ** argv)
{
	gtk_init(&argc, &argv);
	//init_imgrecog(0);
	//process_recog();

    // Initialisation de la librairie Gtk.
	cap =cvCaptureFromCAM(0);
				if(cap==NULL)
				{
					 perror("unable to open cam");
					 exit(EXIT_FAILURE);
				}




    // Ouverture du fichier Glade de la fenêtre principale
    builder = gtk_builder_new();


    // Création du chemin complet pour accéder au fichier test.glade.
    // g_build_filename(); construit le chemin complet en fonction du système
    // d'exploitation. ( / pour Linux et \ pour Windows)

    filename =  g_build_filename ("GUI.glade", NULL);


    // Chargement du fichier GUI.glade.
    gtk_builder_add_from_file (builder, filename, &error);
    g_free (filename);

    if (error)
    {
    	gint code = error->code;
        g_printerr("%s\n", error->message);
        g_error_free (error);
        return code;
    }

    gtk_builder_connect_signals (builder, NULL);

      // Récupération du pointeur de la fenêtre principale
    fenetre_principale = GTK_WIDGET(gtk_builder_get_object (builder, "window"));



    // Affichage de la fenêtre principale.
    gtk_widget_show_all (fenetre_principale);

    gtk_main();
    printf("ggggggggggggggggg");
    cvReleaseCapture(&cap);
    return 0;

}




static void destroy(GtkWidget *widget, gpointer data )
{
    gtk_main_quit ();
}


GtkImage *image_from_ocv(IplImage * img, int dst_w, int dst_h)
{
    GdkPixbuf *pix_src, *pix_dst;
    pix_src = gdk_pixbuf_new_from_data((guchar*)img->imageData, GDK_COLORSPACE_RGB, FALSE,8, img->width, img->height, img->widthStep, NULL, NULL);
    pix_dst = gdk_pixbuf_scale_simple(pix_src, dst_w, dst_h, GDK_INTERP_BILINEAR);
    GtkImage *image = (GtkImage*)gtk_image_new_from_pixbuf (pix_dst);
    return image;
}





void ggg(GtkRange *scale, gpointer  data )
{
	   /*-----------------curseur-------------------------*/
	   gdouble val;

	   val = gtk_range_get_value( scale );


	   printf("%f\n",(float)val);
	   //g_print( "%f\n", val );
	   /*-------------------------------------------------*/
}


