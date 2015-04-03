/*
 * reverseKinematics.c
 *
 *  Created on: Mar 7, 2015
 *      Author: julien
 */

#include "allHeaders.h"


//constantes pour les calculs
#define pi  3.14159
#define deg2rad 180/pi
#define deg30 pi/6
#define DEG_TO_RAD(X) (X*pi/180.0)

//constantes méchaniques en mm ou deg
#define L1 19.0 		//Longueur du bras de levier des servos
#define L2 128.0 		//Longueur des biellettes
#define Z_HOME 120.0	//Hauteur (axe z) entre le point d'attache sur les servos et les attaches sur le plateau
#define R_PLAT 52.7		//Diamètre du plateau
#define R_BASE 70		//Diamètre de la base
#define THETA_P DEG_TO_RAD(13)	//Angle entre deux attaches du plateau  0.227
#define THETA_B DEG_TO_RAD(41)	//Angle entre deux attache de la base  0.72

//Limite du débattement des servos
#define servo_min DEG_TO_RAD(-80)
#define servo_max DEG_TO_RAD(80)

static float M[3][3];
static float rxp[3][6];
static float T[3];
static float H[3] = {0,0,Z_HOME};
static float theta_a[6]={0.0,0.0,0.0,0.0,0.0,0.0}; //Angle de rotation des servos
float p[2][6]; //p[][]=x y values for servo rotation points
float beta[6];
float re[3][6];

// PositionPlatforme[]= { Posx, Posy, Posz, Rotx, Roty, Rotz }
// OUT AngleServos[]
// Return : Nombre d'erreurs
unsigned char setPos(float PositionPlatforme[], float AngleServos[])
{
	int i;
	unsigned char errorcount = 0;
	    for(i = 0; i < 6; i++)
	    {
	        getT(PositionPlatforme);
	        getmatrix(PositionPlatforme);
	        getrxp(PositionPlatforme);
	        theta_a[i]=getAngleServo(&i);
	        if(i==1||i==3||i==5){
	        	AngleServos[i] = theta_a[i];
	        }
	        else{
	        	AngleServos[i] = theta_a[i];
	        }
	    }

	    //Rajouter la gestion des erreures si l'angle est pas correcte
	    return errorcount;
}

void initReverseKinematics(void)
{
	float ANGLE_THETA=(pi/3-THETA_P)/2;

	//p[][]=x y values for servo rotation points
	p[0][0] = -R_BASE*cos(deg30-ANGLE_THETA);
	p[0][1] = -R_BASE*cos(deg30-ANGLE_THETA);
	p[0][2] = R_BASE*sin(ANGLE_THETA);
	p[0][3] = R_BASE*cos(deg30+ANGLE_THETA);
	p[0][4] = R_BASE*cos(deg30+ANGLE_THETA);
	p[0][5] = R_BASE*sin(ANGLE_THETA);
	p[1][0] = -R_BASE*sin(deg30-ANGLE_THETA);
	p[1][1] = R_BASE*sin(deg30-ANGLE_THETA);
	p[1][2] = R_BASE*cos(ANGLE_THETA);
	p[1][3] = R_BASE*sin(deg30+ANGLE_THETA);
	p[1][4] = -R_BASE*sin(deg30+ANGLE_THETA);
	p[1][5] = -R_BASE*cos(ANGLE_THETA);

	//re[]{}=x y z values of platform attachment points positions
	re[0][0] = -R_PLAT*sin(deg30+THETA_B/2);
	re[0][1] = -R_PLAT*sin(deg30+THETA_B/2);
	re[0][2] = -R_PLAT*sin(deg30-THETA_B/2);
	re[0][3] = R_PLAT*cos(THETA_B/2);
	re[0][4] = R_PLAT*cos(THETA_B/2);
	re[0][5] = -R_PLAT*sin(deg30-THETA_B/2);
	re[1][0] = -R_PLAT*cos(deg30+THETA_B/2);
	re[1][1] = R_PLAT*cos(deg30+THETA_B/2);
	re[1][2] = R_PLAT*cos(deg30-THETA_B/2);
	re[1][3] = R_PLAT*sin(THETA_B/2);
	re[1][4] = -R_PLAT*sin(THETA_B/2);
	re[1][5] = -R_PLAT*cos(deg30-THETA_B/2);
	re[2][0] = 0;
	re[2][1] = 0;
	re[2][2] = 0;
	re[2][3] = 0;
	re[2][4] = 0;
	re[2][5] = 0;

	//rotation of servo arms in respect to axis x
	beta[0] = pi/2;
	beta[1] = -pi/2;
	beta[2] = -pi/6;
	beta[3] = 5*pi/6;
	beta[4] = -5*pi/6;
	beta[5] = pi/6;
}

float getAngleServo(int *i)
{
	static int n;
	   static float th=0;
	   static float q[3], dl[3], dl2;
	   double min=servo_min;
	   double max=servo_max;
	   n=0;
	   th=theta_a[*i];
	   while(n<20){
	    //calculation of position of base attachment point (point on servo arm where is leg connected)
	      q[0] = L1*cos(th)*cos(beta[*i]) + p[0][*i];
	      q[1] = L1*cos(th)*sin(beta[*i]) + p[1][*i];
	      q[2] = L1*sin(th);
	    //calculation of distance between according platform attachment point and base attachment point
	      dl[0] = rxp[0][*i] - q[0];
	      dl[1] = rxp[1][*i] - q[1];
	      dl[2] = rxp[2][*i] - q[2];
	      dl2 = sqrt(dl[0]*dl[0] + dl[1]*dl[1] + dl[2]*dl[2]);
	    //if this distance is the same as leg length, value of theta_a is corrent, we return it
	      if(abs(L2-dl2)<0.01){
	         return th;
	      }
	    //if not, we split the searched space in half, then try next value
	      if(dl2<L2){
	         max=th;
	      }else{
	         min=th;
	      }
	      n+=1;
	      if(max==servo_min || min==servo_max){
	         return th;
	      }
	      th = min+(max-min)/2;
	   }
	   return th;
}

void getmatrix(float pe[])
{
	   float psi=pe[5];
	   float theta=pe[4];
	   float phi=pe[3];
	   M[0][0] = cos(psi)*cos(theta);
	   M[1][0] = -sin(psi)*cos(phi)+cos(psi)*sin(theta)*sin(phi);
	   M[2][0] = sin(psi)*sin(phi)+cos(psi)*cos(phi)*sin(theta);

	   M[0][1] = sin(psi)*cos(theta);
	   M[1][1] = cos(psi)*cos(phi)+sin(psi)*sin(theta)*sin(phi);
	   M[2][1] = cos(theta)*sin(phi);

	   M[0][2] = -sin(theta);
	   M[1][2] = -cos(psi)*sin(phi)+sin(psi)*sin(theta)*cos(phi);
	   M[2][2] = cos(theta)*cos(phi);
}

void getrxp(float pe[])
{
	int i;
	for(i = 0;i<6;i++){
	      rxp[0][i] = T[0]+M[0][0]*(re[0][i]) + M[0][1]*(re[1][i])+M[0][2]*(re[2][i]); ;
	      rxp[1][i] = T[1]+M[1][0]*(re[0][i]) + M[1][1]*(re[1][i])+M[1][2]*(re[2][i]);
	      rxp[2][i] = T[2]+M[2][0]*(re[0][i]) + M[2][1]*(re[1][i])+M[2][2]*(re[2][i]);
	   }
}

void getT(float pe[])
{
	   T[0] = pe[0]+H[0];
	   T[1] = pe[1]+H[1];
	   T[2] = pe[2]+H[2];
}



