/*
 * asserv.c
 *
 *  	Created on: Mar 30, 2015
 *      Author: Julien MAURAT
 *      Module generique d'asservissement
 *      Cree un systeme boucle avec correcteur PID
 *      Gestion_Corrector doit etre appeler a frequence fixe
 */


#include "asserv_module.h"

void Gestion_Corrector(Corrector *cor,AsservVarsType conssigne,AsservVarsType valeur,AsservVarsType *commande)
{
	//Vars/////////////////////////////////////////////////////////////////
	AsservVarsType Delta;
	AsservVarsType Integral;
	//Si pas en STOP///////////////////////////////////////////////////////
	if(cor->Mode !=MODE_STOP)
	{
	//Erreur///////////////////////////////////////////////////////////////
	AsservVarsType Erreur=conssigne-valeur;
	//Intégration//////////////////////////////////////////////////////////
	if(cor->Mode==MODE_PI || cor->Mode==MODE_PID)
	{
		cor->Integral	+=	Erreur;
		Integral_boundary(cor);
		Integral=cor->Integral;
	}

	//Dérivation///////////////////////////////////////////////////////////
	if(cor->Mode==MODE_PD || cor->Mode==MODE_PID)
	{
		Delta=Erreur-cor->Derivative_last_val;
		cor->Derivative_last_val=Erreur;
	}
	//Application des coefficients/////////////////////////////////////////
	*commande	=		Erreur			* 	cor->P_PID	+
						Integral 		* 	cor->I_PID 	+
						Delta 			* 	cor->D_PID;
	}
}


void Integral_boundary(Corrector *cor)
{
	if(cor->Integral>cor->Max_integral)
	{
		cor->Integral=cor->Max_integral;
	}
	if(cor->Integral<-cor->Max_integral)
	{
		cor->Integral=-cor->Max_integral;
	}
}

void Mode_Asserv(Asserv_Mode Mode,Corrector *cor)
{
	switch(Mode)
	{
		case MODE_STOP:
		case MODE_P:
			cor->Integral=0;
			cor->Derivative_last_val=0;
		break;
		case MODE_PI:
			cor->Derivative_last_val=0;
		break;
		case MODE_PD:
			cor->Derivative_last_val=0;
		break;
		case MODE_PID:
		break;
	}
	cor->Mode=Mode;
}

