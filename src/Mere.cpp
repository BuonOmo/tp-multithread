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
#include <sys/sem.h>

//------------------------------------------------------ Include personnel
#include "Menu.h"
#include "Heure.h"
#include "Outils.h"
#include "Mere.h"
#include "Donnees.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
static pid_t clavier;
static pid_t entreeBPP;
static pid_t entreeBPA;
static pid_t entreeGB;
static pid_t sortie;
static pid_t heure;

static int baLEntreeBPP;
static int baLEntreeBPA;
static int baLEntreeGB;
static int baLSortie;

static int mpNbPlace;
static int mpEtat;
static int mpRequete;

static int semGene;
/*static int semEntreeBPA;
static int semEntreeGB;
static int semMPNbPlace;
static int semMPEtat;
static int semMPRequete;*/

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
	
	key_t cle = ftok("parking",1);
	
	/** Création des BaL **/
	if ((baLEntreeBPP = msgget(cle, IPC_CREAT)) == -1)
	{
		cout << "Erreur pour l'entrée BPP (BAL)" << endl;
	}	
	if ((baLEntreeBPA = msgget(cle, IPC_CREAT)) == -1)
	{
		cout << "Erreur pour l'entrée BPA (BAL)" << endl;
	}	
	if ((baLEntreeGB = msgget(cle, IPC_CREAT)) == -1)
	{
		cout << "Erreur pour l'entrée GB (BAL)" << endl;
	}
	if ((baLSortie = msgget(cle, IPC_CREAT)) == -1)
	{
		cout << "Erreur pour l'entrée Sortie (BAL)" << endl;
	}
	/** Fin création des BaL **/
	
	/** Création des MP **/
	if ((mpNbPlace = shmget(cle,sizeof(unsigned int), IPC_CREAT)) == -1)
	{
		cout << "Erreur pour MP nbPlace" << endl;
	}	
	if ((mpEtat = shmget(cle,sizeof(Etat)*NB_PLACES, IPC_CREAT)) == -1)
	{
		cout << "Erreur pour MP Etat" << endl;
	}	
	if ((mpRequete = shmget(cle,sizeof(Requete)*MAX_REQUETES, IPC_CREAT)) == -1)
	{
		cout << "Erreur pour MP Requete" << endl;
	}
	/** Fin création des MP **/
	
	/** Création des sémaphores **/
	if ((semGene = semget(cle,6,IPC_CREAT)) == -1)
	{
			cout << "Erreur de création de sémaphore (Entree BPP)" << endl;
	}
	/*if ((semEntreeBPA = semget(cle,,IPC_CREAT)) == -1)
	{
			cout << "Erreur de création de sémaphore (Entree BPA)" << endl;
	}
	if ((semEntreeGB = semget(cle,,IPC_CREAT)) == -1)
	{
			cout << "Erreur de création de sémaphore (Entree GB)" << endl;
	}
	if ((semMPNbPlace = semget(cle,,IPC_CREAT)) == -1)
	{
			cout << "Erreur de création de sémaphore (MPNbPLaces)" << endl;
	}
	if ((semMPEtat = semget(cle,,IPC_CREAT)) == -1)
	{
			cout << "Erreur de création de sémaphore (MPEtat)" << endl;
	}
	if ((semMPRequete = semget(cle,,IPC_CREAT)) == -1)
	{
			cout << "Erreur de création de sémaphore (MPRequete)" << endl;
	}*/
	/** Fin création des sémaphores **/
	
	heure = ActiverHeure();
	
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
	waitpid(entreeBPP,NULL,0);
	kill(entreeBPA, SIGUSR2);
	waitpid(entreeBPA,NULL,0);
	kill(entreeGB, SIGUSR2);
	waitpid(entreeGB,NULL,0);
	kill(sortie, SIGUSR2);
	waitpid(sortie,NULL,0);
	kill(clavier, SIGUSR2);
	waitpid(clavier,NULL,0);
	kill(heure, SIGUSR2);
	waitpid(heure,NULL,0);
	
	msgctl(mpNbPlace,IPC_RMID,0);
	msgctl(mpEtat,IPC_RMID,0);
	msgctl(mpRequete,IPC_RMID,0);
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
