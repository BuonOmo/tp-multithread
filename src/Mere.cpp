/*************************************************************************
                           Mere  -  description
                             -------------------
    début                : Mere
    copyright            : (C) Mere par Pierre Bayle et Ulysse Buonomo
    e-mail               : pbayle@insa-lyon.fr
*************************************************************************/

//---------- Réalisation du module <Mere> (fichier Mere.cpp) -----

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système
#include <unistd.h>

//------------------------------------------------------ Include personnel
#include "Menu.h"
#include "Heure.h"
#include "Outils.h"


///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques

//------------------------------------------------------ Fonctions privées
//static type nom ( liste de paramètres )
// Mode d'emploi :
//
// Contrat :
//
// Algorithme :
//
//{
//} //----- fin de nom

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
int main ()
{
	pid_t nofils;
	pid_t noE1;
	pid_t noE2;
	pid_t noE3;
	pid_t noS;
	
	if((nofils = fork()) == 0)
	{
		SimulationClavier();
	}
	else if((noE1 = fork()) == 0)
	{
		BarriereEntree();
	}
	else if((noE2 = fork()) == 0)
	{
		BarriereEntree();
	}
	else if((noE3 = fork()) == 0)
	{
		BarriereEntree();
	}
	else if((noS = fork()) == 0)
	{
		BarriereSortie();
	}
	else
	{
		InitialiserApplication(XTERM);
		sleep(10);
		TerminerApplication();
	}
	
	InitialiserApplication(XTERM);
	sleep(10);
	TerminerApplication();
	return 0;
	
}
