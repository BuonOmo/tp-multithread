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
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/wait.h>

//------------------------------------------------------ Include personnel
#include "Heure.h"
#include "Mere.h"
#include "Donnees.h"
#include "GestionClavier.h"
#include "PorteSortie.h"
#include "PorteEntree.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes
const char* CHEMIN = "./Parking";

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

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

void Initialisation ()
{
	/** Création des clefs pour les IPC **/
	key_t cleBalEntreeBPP = ftok(CHEMIN,1);
	key_t cleBalEntreeBPA = ftok(CHEMIN,2);
	key_t cleBalEntreeGB  = ftok(CHEMIN,3);
	key_t cleBalSortie 	  = ftok(CHEMIN,4);
	key_t cleMPNbPlace 	  = ftok(CHEMIN,5);
	key_t cleMPEtat 	  = ftok(CHEMIN,6);
	key_t cleMPRequete 	  = ftok(CHEMIN,7);
	key_t cleSem 		  = ftok(CHEMIN,8);
	/** Fin création des clefs pour les IPC **/

	/** Création des BaL **/
	if ((baLEntreeBPP = msgget(cleBalEntreeBPP, 0666|IPC_CREAT)) == -1)
	{
		#ifdef MAP
			AFFICHER(MESSAGE,"Erreur pour l'entrée BPP (BAL)");
		#endif
	}
	if ((baLEntreeBPA = msgget(cleBalEntreeBPA, 0666|IPC_CREAT)) == -1)
	{
		#ifdef MAP
			AFFICHER(MESSAGE,"Erreur pour l'entrée BPA (BAL)");
		#endif
	}
	if ((baLEntreeGB = msgget(cleBalEntreeGB, 0666|IPC_CREAT)) == -1)
	{
		#ifdef MAP
			AFFICHER(MESSAGE,"Erreur pour l'entrée GB (BAL)");
		#endif
	}
	if ((baLSortie = msgget(cleBalSortie, 0666|IPC_CREAT)) == -1)
	{
		#ifdef MAP
			AFFICHER(MESSAGE,"Erreur pour l'entrée Sortie (BAL)");
		#endif
	}
	/** Fin création des BaL **/



	/** Création des MP **/

	//Initialisation de la memoire partagee NbPlaceOccupees
	mpNbPlace =
			shmget(cleMPNbPlace,sizeof(NbPlaceOccupees), 0666|IPC_CREAT);
	if (  mpNbPlace == -1)
	{
		#ifdef MAP
			AFFICHER(MESSAGE,"Erreur pour MP nbPlace");
		#endif
	}
	else
	{
		NbPlaceOccupees *nbo =
							(NbPlaceOccupees *) shmat(mpNbPlace, NULL, 0);
		nbo->nb = 0;
		shmdt(nbo);
	}


	//Initialisation de la memoire partagee Etat
	if ((mpEtat = shmget(cleMPEtat,sizeof(Etat), 0666|IPC_CREAT)) == -1)
	{
		#ifdef MAP
			AFFICHER(MESSAGE,"Erreur pour MP Etat");
		#endif
	}
	else
	{
		Etat *et = (Etat *) shmat(mpEtat, NULL, 0);
		for(unsigned int i=0; i < NB_PLACES ; i++)
		{
			et->places[i] = {AUCUN, 0,0};
		}
		shmdt(et);
	}

	//Initialisation de la memoire partagee Requete
	mpRequete = shmget(cleMPRequete,sizeof(Requete), 0666|IPC_CREAT);
	if ( mpRequete == -1)
	{
		#ifdef MAP
			AFFICHER(MESSAGE,"Erreur pour MP Requete");
		#endif
	}
	else
	{
		Requete *req = (Requete *) shmat(mpRequete, NULL, 0);
		for(unsigned int i=0; i < NB_BARRIERES_ENTREE; i++)
		{
			req->requetes[i] = {AUCUN, 0,0};
		}
		shmdt(req);
	}
	/** Fin création des MP **/



	/** Création des sémaphores **/
	if ((semGene = semget(cleSem,NB_SEM,0666|IPC_CREAT)) == -1)
	{
		#ifdef MAP
			AFFICHER(MESSAGE,"Erreur de création de sémaphore");
		#endif
	}
	else
	{
		semctl(semGene,SynchroPorteBPPROF,SETVAL,0);
		semctl(semGene,SynchroPorteBPAUTRE,SETVAL,0);
		semctl(semGene,SynchroPorteGB,SETVAL,0);
		semctl(semGene,MutexMPNbPlaces,SETVAL,1);
		semctl(semGene,MutexMPEtat,SETVAL,1);
		semctl(semGene,MutexMPRequetes,SETVAL,1);
	}
	/** Fin création des sémaphores **/

	heure = ActiverHeure();

	if ((clavier = fork()) == 0)
	{
		SimulationClavier( baLSortie,
						   baLEntreeBPP,
						   baLEntreeBPA,
						   baLEntreeGB);
	}
	else if ((entreeBPP = fork()) == 0)
	{
		PorteEntree( baLEntreeBPP,
			 		 semGene,
					 mpNbPlace,
					 mpEtat,
					 mpRequete,
					 PROF_BLAISE_PASCAL);
	}
	else if ((entreeBPA = fork()) == 0)
	{
		PorteEntree( baLEntreeBPA,
			 		 semGene,
					 mpNbPlace,
					 mpEtat,
					 mpRequete,
					 AUTRE_BLAISE_PASCAL);
	}
	else if ((entreeGB = fork()) == 0)
	{
		PorteEntree( baLEntreeGB ,
			 		 semGene,
					 mpNbPlace,
					 mpEtat,
					 mpRequete,
					 ENTREE_GASTON_BERGER);
	}
	else if ((sortie = fork()) == 0)
	{
		PorteSortie(mpNbPlace, mpEtat, mpRequete, baLSortie, semGene);
	}
}

void Destruction ()
{
	/** Destruction des processus fils **/
	kill(entreeBPP, SIGUSR2);
	waitpid(entreeBPP,NULL,0);
	kill(entreeBPA, SIGUSR2);
	waitpid(entreeBPA,NULL,0);
	kill(entreeGB, SIGUSR2);
	waitpid(entreeGB,NULL,0);
	kill(sortie, SIGUSR2);
	waitpid(sortie,NULL,0);
	kill(heure, SIGUSR2);
	waitpid(heure,NULL,0);
	/** Fin destruction des processus fils **/

	/** Destruction des MP **/
	msgctl(mpNbPlace,IPC_RMID,0);
	msgctl(mpEtat,IPC_RMID,0);
	msgctl(mpRequete,IPC_RMID,0);
	/** Fin destruction des MP **/

	/** Destruction des BAL **/
	shmctl(baLEntreeBPP,IPC_RMID,0);
	shmctl(baLEntreeBPA,IPC_RMID,0);
	shmctl(baLEntreeGB,IPC_RMID,0);
	shmctl(baLSortie,IPC_RMID,0);
	/** Fin destruction des BAL **/

	/** Destruction sémaphore **/
	semctl (semGene, 0, IPC_RMID, 0);
	/** Fin destruction sémaphore **/
	TerminerApplication();

	exit(0);
}

int main ()
{
	
	InitialiserApplication(XTERM);
	Initialisation();
	waitpid(clavier,NULL,0);
	Destruction();
	return 0;

}
