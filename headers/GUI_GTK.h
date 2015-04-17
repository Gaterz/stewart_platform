/*
 * GUI_GTK.h
 *
 *  Created on: 16 avr. 2015
 *      Author: maxime
 */

#ifndef HEADERS_GUI_GTK_H_
#define HEADERS_GUI_GTK_H_

int OpenWindows(int argc,char ** argv);
GtkImage *image_from_ocv(IplImage * img, int dst_w, int dst_h);

#endif /* HEADERS_GUI_GTK_H_ */
