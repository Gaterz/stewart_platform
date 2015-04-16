/*
 * imgRecognition.c
 *
 *  Created on: Mar 7, 2015
 *      Author: julien
 */

#include "mainHeaders.h"
int thresh_hue_min=75;
int thresh_hue_max=90;
int thresh_saturation_min=46;
int thresh_saturation_max=148;
int thresh_value_min=12;
int thresh_value_max=42;
int gauss_size=3;
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
	cvCreateTrackbar("thresh_saturation_min", "screen", &thresh_saturation_min, 255,NULL);
	cvCreateTrackbar("thresh_saturation_max", "screen", &thresh_saturation_max, 255,NULL);
	cvCreateTrackbar("thresh_value_min", "screen", &thresh_value_min, 255,NULL);
	cvCreateTrackbar("thresh_value_max", "screen", &thresh_value_max, 255,NULL);
	cvCreateTrackbar("gauss_size", "screen", &gauss_size, 20,NULL);

	#ifdef DEBUG_IMGRECOG
	cvNamedWindow("hsv",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("mat0",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("mat1",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("mat2",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("end_recog",CV_WINDOW_AUTOSIZE);
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
	cvDestroyWindow("end_recog");
	#endif
}
void process_recog()
{
	IplImage* hsv_split[3];
	IplImage* endRecog;
	IplImage* capture=cvQueryFrame(capture_device);
	IplImage* capture_hsv=	cvCreateImage(cvGetSize(capture),capture->depth,capture->nChannels);

	cvSmooth(capture,capture,CV_GAUSSIAN,gauss_size*2+1,gauss_size*2+1,0,0);

	cvCvtColor(capture,capture_hsv,CV_RGB2HSV);

	hsv_split[0]=cvCreateImage(cvGetSize(capture_hsv), IPL_DEPTH_8U, 1);
	hsv_split[1]=cvCreateImage(cvGetSize(capture_hsv), IPL_DEPTH_8U, 1);
	hsv_split[2]=cvCreateImage(cvGetSize(capture_hsv), IPL_DEPTH_8U, 1);
	endRecog=cvCreateImage(cvGetSize(capture_hsv), IPL_DEPTH_8U, 1);

	cvSplit(capture_hsv,hsv_split[0],hsv_split[1],hsv_split[2],0);


	cvInRangeS(hsv_split[0],cvScalar(thresh_hue_min,0,0,0),cvScalar(thresh_hue_max,0,0,0),hsv_split[0]);
	cvInRangeS(hsv_split[1],cvScalar(thresh_saturation_min,0,0,0),cvScalar(thresh_saturation_max,0,0,0),hsv_split[1]);
	cvInRangeS(hsv_split[2],cvScalar(thresh_value_min,0,0,0),cvScalar(thresh_value_max,0,0,0),hsv_split[2]);
	cvAnd(hsv_split[0],hsv_split[1],endRecog,NULL);
	cvAnd(endRecog,hsv_split[2],endRecog,NULL);

	cvShowImage("screen",capture);

#ifdef DEBUG_IMGRECOG
	cvShowImage("hsv",capture_hsv);
	cvShowImage("mat0",hsv_split[0]);
	cvShowImage("mat1",hsv_split[1]);
	cvShowImage("mat2",hsv_split[2]);
	cvShowImage("end_recog",endRecog);
#endif

	cvReleaseImage(&hsv_split[0]);
	cvReleaseImage(&hsv_split[1]);
	cvReleaseImage(&hsv_split[2]);
	cvReleaseImage(&capture_hsv);
}

void test_img_recog()
{
	init_imgrecog(1);

		int c;
		while(c!=1048603) {
			process_recog();
			c=cvWaitKey(10);

		}
		exit_imgrecog();
}
