/*************************************************************************
                PorteEntree  -  gestion des portes d’entrée
                             -------------------
    début                : 18 mars 2016
    copyright            : (C) PorteEntree par Pierre Bayle et Ulysse
                                                              Buonomo
    e-mail               : pierre.bayle@insa-lyon.fr
*************************************************************************/

//----- Réalisation de la tâche <PorteEntree> (fichier PorteEntree.cpp) --

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <iostream>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <vector>
// _____________________________________________________________________________________surement trop d’include, à trier
//------------------------------------------------------ Include personnel
#include "PorteEntree.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
static vector<pid_t> voituriersEnEntree; //Vecteur qui stocke les pid des
//voituriers toujours en route
static int memIDNbPlace;
static int memIDEtat;
static int memIDRequete;
static int semGene;
static int balEntree;
static TypeBarriere barriereType;
//------------------------------------------------------ Fonctions privées
static void DestructionPorteEntree(int noSignal);
static void ReceptionMortVoiturier(int noSignal);


static void InitialisationPorteEntree()
//Mode d'emploi :
//	Phase d'initialisation du processus Entree
{
	//Installation du handler destruction
	struct sigaction action;
	action.sa_handler = DestructionPorteEntree ;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0 ;
	sigaction(SIGUSR2,&action,NULL); //armer SIGUSR2 sur destruction;


	//Installation du handler actionFinVoiturier
	struct sigaction actionFinVoiturier;
	actionFinVoiturier.sa_handler = ReceptionMortVoiturier ;
	sigemptyset(&actionFinVoiturier.sa_mask);
	actionFinVoiturier.sa_flags = 0 ;
	sigaction(SIGCHLD,&actionFinVoiturier,NULL);  //armer SIGCHLD sur actionFinVoiturier;

}

static void MoteurPorteEntree()
//Mode d'emploi :
//	Phase moteur du processus Entree
{
	for(;;)
	{
		Voiture voiture;
		struct sembuf reserverNb = {MutexMPNbPlaces, -1,0}; //p Operation --> Reservation sur MP NbPlace
		struct sembuf libererNb  = {MutexMPNbPlaces, 1, 0}; //v Operation --> liberation sur MP NbPLace
		struct sembuf reserverRequete = {MutexMPRequetes, -1,0}; //p Operation --> Reservation sur MP Requete
		struct sembuf libererRequete  = {MutexMPRequetes, 1, 0}; //v Operation --> liberation sur MP Requete

		while(msgrcv(balEntree,&voiture,sizeof(Voiture),0,0) == -1);


		DessinerVoitureBarriere(barriereType,voiture.usagerVoiture);

		while(semop(semGene,&reserverNb,1)==-1 && errno==EINTR); // Réservation de la mémoire pour le nb de places occupées
		NbPlaceOccupees* nbp = (NbPlaceOccupees*) shmat(memIDNbPlace,NULL,0);
		unsigned int nbPlaceOccupee = nbp->nb;
		shmdt(nbp);
		semop(semGene,&libererNb,1); //Libération de la mémoire NbPlace

		if(nbPlaceOccupee == NB_PLACES)
		{
			AfficherRequete(barriereType, voiture.usagerVoiture, voiture.hArrivee);

			//On ecrit dans la mémoire partagée que l'on a une requete !

			while(semop(semGene,&reserverRequete,1)==-1 && errno==EINTR); //Reservation de la memoire partagee Requete

			Requete *req = (Requete*) shmat(memIDRequete,NULL,0);
			req->requetes[barriereType - 1] = voiture;
			shmdt(req);

			semop(semGene,&libererRequete,1); //Liberation de la memoire Requete

			struct sembuf pOp = {(short unsigned int)barriereType-1,-1,0};  //p Operation sur le mutex de synchronisation
			while(semop(semGene,&pOp,1)==-1 && errno==EINTR);

			TypeZone zone;
			if(barriereType == PROF_BLAISE_PASCAL)
			{
				zone = REQUETE_R1;
			}
			if(barriereType == AUTRE_BLAISE_PASCAL)
			{
				zone = REQUETE_R2;
			}
			if(barriereType == ENTREE_GASTON_BERGER)
			{
				zone = REQUETE_R3;
			}
			Effacer(zone);
		}

		while(semop(semGene,&reserverNb,1)==-1 && errno==EINTR); // Réservation de la mémoire pour le nb de places occupées
		nbp = (NbPlaceOccupees*) shmat(memIDNbPlace,NULL,0);
		nbp->nb++;
		shmdt(nbp);
		semop(semGene,&libererNb,1); //Libération de la mémoire NbPlace

		// garage voiture ajout du pid voiturier dans la list
		pid_t voiturierEntree;
		if((voiturierEntree = GarerVoiture(barriereType)) == -1)
		{
			continue;
		}
		voituriersEnEntree.push_back(voiturierEntree);

		//sleep 1s
		sleep(TEMPO);

	}
}

static void DestructionPorteEntree(int noSignal)
//Mode d'emploi :
//	Phase de destruction du processus Entree
{
	if(noSignal == SIGUSR2){
		struct sigaction action;
		action.sa_handler = SIG_IGN ;
		sigemptyset(&action.sa_mask);
		action.sa_flags = 0 ;
		sigaction(SIGCHLD,&action,NULL);


		for(vector<pid_t>::iterator itLE = voituriersEnEntree.begin(); itLE != voituriersEnEntree.end(); itLE++){
			kill(*itLE, SIGUSR2);
		}
		for(vector<pid_t>::iterator itLE = voituriersEnEntree.begin(); itLE != voituriersEnEntree.end(); itLE++){
			waitpid(*itLE,NULL,0);
		}

		exit(0);
	}
}

static void ReceptionMortVoiturier(int noSignal)
//Mode d'emploi
//	Handler pour le signal SIGCHLD
{

	if(noSignal == SIGCHLD){
		struct sembuf reserverEtat 	  = {MutexMPEtat, -1,0};	 //p Operation --> Reservation sur MP Etat
		struct sembuf libererEtat 	  = {MutexMPEtat, 1, 0};	 //v Operation --> liberation sur MP Etat

		int status;
		//Recuperer le fils qui a envoye le SIGCHLD
		pid_t filsFini = wait(&status);

		vector<pid_t>::iterator itEntree;
		while(itEntree != voituriersEnEntree.end() && *itEntree != filsFini)
		{
			++itEntree;
		}

		//Recuperer la bonne voiture qui a lancé le signal
		Voiture v = *itEntree;

		//Afficher ses caractéristiques dans l'endroit indique
		AfficherPlace(WEXITSTATUS(status),v.usagerVoiture,v.numPlaque,v.hArrivee);

		while(semop(semID,&reserverEtat,1)==-1 && errno==EINTR); //Reservation de la memoire


		//Ecrire la voiture sur la mémoire partagée
		Etat *et = (Etat *) shmat(memIDEtat, NULL, 0);
		a->placesParking[WEXITSTATUS(status)-1] = v;
		shmdt(a);

		semop(semGene,&libererEtat,1); //Liberation de la memoire

		//Supprimer le bon voiturier
		voituriersEnSortie.erase(itSorti);

	}
}

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void PorteEntree (int pbalID, int psemID, int pmemIDNbPlace , int pmemIDEtat, int pmemIDRequete, TypeBarriere barType)
// Algorithme :
//
{
	balEntree = pbalID;
	semGene = psemID;
	memIDNbPlace = pmemIDNbPlace;
	memIDEtat = pmemIDEtat;
	memIDRequete = pmemIDRequete;
	barriereType = barType;

	InitialisationPorteEntree();

} //----- fin de Nom
