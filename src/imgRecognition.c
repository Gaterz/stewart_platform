/*
 * imgRecognition.c
 *
 *  Created on: Mar 7, 2015
 *      Author: julien
 */

#include "allHeaders.h"
int thresh_hue_min = 0;
int thresh_hue_max = 255;
int thresh_saturation_min = 4;
int thresh_saturation_max = 85;
int thresh_value_min = 0;
int thresh_value_max = 79;
int gauss_size = 16;
CvCapture* capture_device = NULL;
void init_imgrecog(int device) {
	CvCapture * cap = cvCaptureFromCAM(device);
	if (cap == NULL) {
		perror("unable to open cam");
		exit(EXIT_FAILURE);
	}
	capture_device = cap;

	cvNamedWindow("screen", CV_WINDOW_AUTOSIZE);
	cvCreateTrackbar("thresh_hue_min", "screen", &thresh_hue_min, 255, NULL);
	cvCreateTrackbar("thresh_hue_max", "screen", &thresh_hue_max, 255, NULL);
	cvCreateTrackbar("thresh_saturation_min", "screen", &thresh_saturation_min,
			255, NULL);
	cvCreateTrackbar("thresh_saturation_max", "screen", &thresh_saturation_max,
			255, NULL);
	cvCreateTrackbar("thresh_value_min", "screen", &thresh_value_min, 255,
	NULL);
	cvCreateTrackbar("thresh_value_max", "screen", &thresh_value_max, 255,
	NULL);
	cvCreateTrackbar("gauss_size", "screen", &gauss_size, 20, NULL);

#ifdef DEBUG_IMGRECOG
	cvNamedWindow("hsv", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("mat0", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("mat1", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("mat2", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("end_recog", CV_WINDOW_AUTOSIZE);
#endif
}

void exit_imgrecog() {
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
void process_recog(int *x, int*y) {
	CvMemStorage *storage;

	storage = cvCreateMemStorage(0);
	CvSeq *contours = 0;

	CvScalar (ext_color);

	IplImage* hsv_split[3];
	IplImage* endRecog;
	IplImage* capture = cvQueryFrame(capture_device);
	cvSetImageROI(capture, cvRect(110, 40, 410,385));
	IplImage* capture_hsv = cvCreateImage(cvGetSize(capture), capture->depth,
			capture->nChannels);

	cvSmooth(capture, capture, CV_GAUSSIAN, gauss_size * 2 + 1,
			gauss_size * 2 + 1, 0, 0);

	cvCvtColor(capture, capture_hsv, CV_RGB2HSV);

	hsv_split[0] = cvCreateImage(cvGetSize(capture_hsv), IPL_DEPTH_8U, 1);
	hsv_split[1] = cvCreateImage(cvGetSize(capture_hsv), IPL_DEPTH_8U, 1);
	hsv_split[2] = cvCreateImage(cvGetSize(capture_hsv), IPL_DEPTH_8U, 1);
	endRecog = cvCreateImage(cvGetSize(capture_hsv), IPL_DEPTH_8U, 1);

	cvSplit(capture_hsv, hsv_split[0], hsv_split[1], hsv_split[2], 0);

	cvInRangeS(hsv_split[0], cvScalar(thresh_hue_min, 0, 0, 0),
			cvScalar(thresh_hue_max, 0, 0, 0), hsv_split[0]);
	cvInRangeS(hsv_split[1], cvScalar(thresh_saturation_min, 0, 0, 0),
			cvScalar(thresh_saturation_max, 0, 0, 0), hsv_split[1]);
	cvInRangeS(hsv_split[2], cvScalar(thresh_value_min, 0, 0, 0),
			cvScalar(thresh_value_max, 0, 0, 0), hsv_split[2]);
	cvAnd(hsv_split[0], hsv_split[1], endRecog, NULL);
	cvAnd(endRecog, hsv_split[2], endRecog, NULL);

	int numb = cvFindContours(endRecog, storage, &contours, sizeof(CvContour),
			CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

	//printf("%d\n",contnumb);
	if (numb != 0) {

		CvSeq* ptr = NULL;
		for (ptr = contours; ptr != NULL; ptr = ptr->h_next) {
			ext_color = CV_RGB(rand() & 255, rand() & 255, rand() & 255); //randomly coloring different contours
			cvDrawContours(capture, ptr, ext_color, CV_RGB(0, 0, 0), -1,
			CV_FILLED, 8, cvPoint(0, 0));
		}
		CvSeq *largestContour = NULL;
		double maxArea = 0;
		for (ptr = contours; ptr != NULL; ptr = ptr->h_next) {
			double area = cvContourArea(ptr, CV_WHOLE_SEQ, 0);
			if (area > maxArea) {
				maxArea = area;
				largestContour = ptr;
			}
		}
		CvMoments mom1;
		if (largestContour != NULL) {

			cvMoments(largestContour, &mom1, 0);
			CvPoint center = cvPoint(mom1.m10 / mom1.m00, mom1.m01 / mom1.m00);
			*x=center.x-110-205+118;
			*y=center.y-40-193+50;
			cvCircle(capture, center, 4, cvScalar(255, 49, 0, 255), 1, 8, 0);
		}
	}
	cvShowImage("screen", capture);

#ifdef DEBUG_IMGRECOG
	cvShowImage("hsv", capture_hsv);
	cvShowImage("mat0", hsv_split[0]);
	cvShowImage("mat1", hsv_split[1]);
	cvShowImage("mat2", hsv_split[2]);
	cvShowImage("end_recog", endRecog);
#endif

	cvReleaseImage(&hsv_split[0]);
	cvReleaseImage(&hsv_split[1]);
	cvReleaseImage(&hsv_split[2]);
	cvReleaseImage(&capture_hsv);
	cvReleaseImage(&endRecog);
}

void test_img_recog() {
	init_imgrecog(1);

	int c,x,y;
	x=y=0;
	Corrector corx;
	Corrector cory;
	Mode_Asserv(MODE_STOP,&corx);
	Mode_Asserv(MODE_STOP,&cory);
	corx.P_PID=4;//6
	corx.I_PID=0;
	corx.D_PID=130;//140
	cory.P_PID=4;//6
	cory.I_PID=0;
	cory.D_PID=130;//140
	AsservVarsType comm_x,comm_y;
	float position[]={0,0,0,0,0,0};
	float angles[6];
	unsigned char tempo[6];
	initReverseKinematics();
	formatDonnees(angles,tempo);
	int serial= openPort("/dev/ttyACM0");
	initPort(serial, 9600);
	Mode_Asserv(MODE_PD,&corx);
		Mode_Asserv(MODE_PD,&cory);
	while (c != 1048603) {
		process_recog(&x,&y);
		Gestion_Corrector(&corx,0,x,&comm_x);
		Gestion_Corrector(&cory,0,y,&comm_y);
		position[4]=(float)(comm_x/10000.0);
		position[3]=-(float)(comm_y/10000.0);
		printf("x : %d y : %d comm_x : %ld comm_y : %ld\n",x,y,comm_x,comm_y);
		positionPlatforme2Anges(position, angles);
		formatDonnees(angles,tempo);
		sendAngles(serial, tempo);
		c = cvWaitKey(10);

	}
	exit_imgrecog();
}
