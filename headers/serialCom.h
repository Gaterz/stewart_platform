/*
 * serialCom.h
 *
 *  Created on: Mar 7, 2015
 *      Author: julien
 */

#ifndef HEADERS_SERIALCOM_H_
#define HEADERS_SERIALCOM_H_

int openPort(char link[]);
void closePort(int fd);
void initPort(int fd, int vitesse);
int sendAngles(int fd, unsigned char anglesFormate[]);
void formatDonnees(float nFormate[], unsigned char Formate[]);


#endif /* HEADERS_SERIALCOM_H_ */
