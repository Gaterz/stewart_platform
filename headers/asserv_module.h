/*
 * asserv.h
 *
 *  Created on: Mar 30, 2015
 *      Author: julien
 */

#ifndef SRC_ASSERV_H_
#define SRC_ASSERV_H_

enum Asserv_Mode{MODE_STOP,MODE_P,MODE_PI,MODE_PD,MODE_PID};
typedef enum Asserv_Mode Asserv_Mode;
#define AsservVarsType signed long

typedef struct{
	float P_PID;
	float I_PID;
	float D_PID;
	AsservVarsType Integral;
	AsservVarsType Max_integral;
	AsservVarsType Derivative_last_val;
	Asserv_Mode Mode;
}Corrector;

void Gestion_Corrector(Corrector *cor,AsservVarsType conssigne,AsservVarsType valeur,AsservVarsType *commande);
void Integral_boundary(Corrector *cor);
void Mode_Asserv(Asserv_Mode Mode,Corrector *cor);


#endif /* SRC_ASSERV_H_ */
