/*
 * imgRecognition.c
 *
 *  Created on: Mar 7, 2015
 *      Author: julien
 */

#include "mainHeaders.h"
int thresh_hue_min=20;
int thresh_hue_max=20;
CvCapture* capture_device=NULL;
void init_imgrecog(int device)
{
	CvCapture * cap =cvCaptureFromCAM(device);
	if(cap==NULL)
	{
		perror("unable to open cam");
		exit(EXIT_FAILURE);
	}
	capture_device=cap;

	cvNamedWindow("screen",CV_WINDOW_AUTOSIZE);
	cvCreateTrackbar("thresh_hue_min", "screen", &thresh_hue_min, 255,NULL);
	cvCreateTrackbar("thresh_hue_max", "screen", &thresh_hue_max, 255,NULL);

	#ifdef DEBUG_IMGRECOG
	cvNamedWindow("hsv",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("mat0",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("mat1",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("mat2",CV_WINDOW_AUTOSIZE);
	#endif
}

void exit_imgrecog()
{
	cvReleaseCapture(&capture_device);
	cvDestroyWindow("screen");

	#ifdef DEBUG_IMGRECOG
	cvDestroyWindow("hsv");
	cvDestroyWindow("mat0");
	cvDestroyWindow("mat1");
	cvDestroyWindow("mat2");
	#endif
}
void process_recog()
{
	IplImage* hsv_split[4];
	IplImage* capture=cvQueryFrame(capture_device);
	IplImage* capture_hsv=	cvCreateImage(cvGetSize(capture),capture->depth,capture->nChannels);
	cvCvtColor(capture,capture_hsv,CV_RGB2HSV);
	hsv_split[0]=cvCreateImage(cvGetSize(capture_hsv), IPL_DEPTH_8U, 1);
	hsv_split[1]=cvCreateImage(cvGetSize(capture_hsv), IPL_DEPTH_8U, 1);
	hsv_split[2]=cvCreateImage(cvGetSize(capture_hsv), IPL_DEPTH_8U, 1);
	cvSplit(capture_hsv,hsv_split[0],hsv_split[1],hsv_split[2],0);


	//cvInRangeS(hsv_split[0],cvScalar(thresh_hue_min,0,0,0),cvScalar(thresh_hue_max,0,0,0),hsv_split[0]);
	cvShowImage("screen",capture);

#ifdef DEBUG_IMGRECOG
	cvShowImage("hsv",capture_hsv);
	cvShowImage("mat0",hsv_split[0]);
	cvShowImage("mat1",hsv_split[1]);
	cvShowImage("mat2",hsv_split[2]);
#endif
}
