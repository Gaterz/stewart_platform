/*
 * reverseKinematics.h
 *
 *  Created on: Mar 7, 2015
 *      Author: julien
 */

#ifndef HEADERS_REVERSEKINEMATICS_H_
#define HEADERS_REVERSEKINEMATICS_H_



//  IN = PositionPlatforme[]= { Posx, Posy, Posz, Rotx, Roty, Rotz }
// OUT AngleServos[]
unsigned char setPos(float PositionPlatforme[], float AngleServos[]);


void initReverseKinematics(void);
float getAngleServo(int *i);
void getmatrix(float pe[]);
void getrxp(float pe[]);
void getT(float pe[]);






#endif /* HEADERS_REVERSEKINEMATICS_H_ */
