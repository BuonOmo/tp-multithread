/*************************************************************************
                           Mere  -  description
                             -------------------
    début                : Mere
    copyright            : (C) Mere par Pierre Bayle et Ulysse Buonomo
    e-mail               : pbayle@insa-lyon.fr
*************************************************************************/

//---------- Réalisation du main <Mere> (fichier Mere.cpp) -----

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>

//------------------------------------------------------ Include personnel
#include "Menu.h"
#include "Heure.h"
#include "Outils.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
static pid_t clavier;
static pid_t entreeBPP;
static pid_t entreeBPA;
static pid_t entreeGB;
static pid_t sortie;

static int baLEntreeBPP;
static int baLEntreeBPA;
static int baLEntreeGB;
static int baLSortie;

static int mpEntreeBPP;
static int mpEntreeBPA;
static int mpEntreeGB;
static int mpSortie;

void Initialisation();
void Moteur();
void Destruction();

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

void Initialisation ()
{
	
	key_t cleEntreeBPP = ftok("balBPP",1);
	key_t cleEntreeBPA = ftok("balBPA",1);
	key_t cleEntreeGB = ftok("balGB",1); 
	key_t cleSortie = ftok("balS",1);
	
	/** Création des BaL **/
	if (baLEntreeBPP = msgget(cleEntreeBPP, IPC_CREAT) == -1)
	{
		cout << "Erreur pour la clé BPP (BAL)" << endl;
	}	
	if (baLEntreeBPA = msgget(cleEntreeBPA, IPC_CREAT)
	{
		cout << "Erreur pour la clé BPA (BAL)" << endl;
	}	
	if (baLEntreeGB = msgget(cleEntreeGB, IPC_CREAT)
	{
		cout << "Erreur pour la clé GB (BAL)" << endl;
	}
	if (baLSortie = msgget(cleSortie, IPC_CREAT)
	{
		cout << "Erreur pour la clé Sortie (BAL)" << endl;
	}
	/** Fin création des BaL **/
	
	/** Création des MP **/
	if (baLEntreeBPP = msgget(cleEntreeBPP, IPC_CREAT) == -1)
	{
		cout << "Erreur pour la clé BPP (MP)" << endl;
	}	
	if (baLEntreeBPA = msgget(cleEntreeBPA, IPC_CREAT)
	{
		cout << "Erreur pour la clé BPA (MP)" << endl;
	}	
	if (baLEntreeGB = msgget(cleEntreeGB, IPC_CREAT)
	{
		cout << "Erreur pour la clé GB (MP)" << endl;
	}
	if (baLSortie = msgget(cleSortie, IPC_CREAT)
	{
		cout << "Erreur pour la clé Sortie (MP)" << endl;
	}
	/** Fin création des MP **/
	
	if ((clavier = fork()) == 0)
	{
		SimulationClavier();
	}
	else
	{
		if ((entreeBPP = fork()) == 0)
		{
			
		}
		if ((entreeBPA = fork()) == 0)
		{
			
		}
		if ((entreeGB = fork()) == 0)
		{
			
		}
		if ((sortie = fork()) == 0)
		{
			
		}		
	}
}

void Moteur ()
{
	
}

void Destruction ()
{
	kill(entreeBPP, SIGUSR2);
	kill(entreeBPA, SIGUSR2);
	kill(entreeGB, SIGUSR2);
	kill(sortie, SIGUSR2);
	kill(clavier, SIGUSR2);
	msgctl(baLEntreeBPP,IPC_RMID,0);
	msgctl(baLEntreeBPA,IPC_RMID,0);
	msgctl(baLEntreeGB,IPC_RMID,0);
	msgctl(baLSortie,IPC_RMID,0);
	shmctl(baLEntreeBPP,IPC_RMID,0);
	shmctl(baLEntreeBPA,IPC_RMID,0);
	shmctl(baLEntreeGB,IPC_RMID,0);
	shmctl(baLSortie,IPC_RMID,0);
}

int main ()
{
	Initialisation();
	Moteur():
	Destruction();
	
	return 0;

}
