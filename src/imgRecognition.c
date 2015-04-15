/*
 * imgRecognition.c
 *
 *  Created on: Mar 7, 2015
 *      Author: julien
 */

#include "mainHeaders.h"
int thresh_hue_min=20;
CvCapture* capture_device=NULL;
void init_imgrecog(int device)
{
	CvCapture * cap =cvCaptureFromCAM(device);
	if(cap==NULL)
	{
		perror("unable to open cam");
		exit(EXIT_FAILURE);
	}
	cvNamedWindow("screen",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("mat0",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("mat1",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("mat2",CV_WINDOW_AUTOSIZE);
	capture_device=cap;
	cvCreateTrackbar("thresh1", "screen", &thresh_hue_min, 1024,NULL);
}

void exit_imgrecog()
{
	cvReleaseCapture(&capture_device);
	cvDestroyWindow("screen");
}
void process_recog()
{
	IplImage* capture=cvQueryFrame(capture_device);
	cvCvtColor(capture,capture,CV_RGB2HSV);
	IplImage* arr[4];
	arr[0]=cvCreateImage(cvGetSize(capture), IPL_DEPTH_8U, 1);
	arr[1]=cvCreateImage(cvGetSize(capture), IPL_DEPTH_8U, 1);
	arr[2]=cvCreateImage(cvGetSize(capture), IPL_DEPTH_8U, 1);
	cvSplit(capture,arr[0],arr[1],arr[2],0);
	cvThreshold(arr[0],arr[0],thresh_hue_min,255,CV_THRESH_BINARY);

	cvShowImage("screen",capture);
	cvShowImage("mat0",arr[0]);
	cvShowImage("mat1",arr[1]);
	cvShowImage("mat2",arr[2]);
}
