/*
 * imgRecognition.h
 *
 *  Created on: Mar 7, 2015
 *      Author: julien
 */

#ifndef HEADERS_IMGRECOGNITION_H_
#define HEADERS_IMGRECOGNITION_H_

#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc_c.h>

typedef struct coords{
	int x;
	int y;
}coords;

typedef struct recog_param{
	int thresh_hue_min;
	int thresh_hue_max;
	int thresh_saturation_min;
	int thresh_saturation_max;
	int thresh_value_min;
	int thresh_value_max;
	int gauss_size;
}recog_param;

extern recog_param recog_params;
void init_imgrecog(int device);
void exit_imgrecog();
void process_recog(int *x, int*y);
void test_img_recog();
#endif /* HEADERS_IMGRECOGNITION_H_ */
