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
#include <map>
// _____________________________________________________________________________________surement trop d’include, à trier TODO
//------------------------------------------------------ Include personnel
#include "PorteEntree.h"
#include "Donnees.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
// Vecteur qui stocke les pid des voituriers toujours en marche
static map<pid_t, Voiture*> voituriersEnEntree;
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
// Mode d'emploi :
// Phase d'initialisation du processus Entree
{
	// Installation du handler destruction
	struct sigaction action;
	action.sa_handler = DestructionPorteEntree ;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0 ;
	// armer SIGUSR2 sur destruction;
	sigaction(SIGUSR2,&action,NULL);


	// Installation du handler actionFinVoiturier
	struct sigaction actionFinVoiturier;
	actionFinVoiturier.sa_handler = ReceptionMortVoiturier ;
	sigemptyset(&actionFinVoiturier.sa_mask);
	actionFinVoiturier.sa_flags = 0 ;
	// armer SIGCHLD sur actionFinVoiturier;
	sigaction(SIGCHLD,&actionFinVoiturier,NULL);

} //----- fin de InitialisationPorteEntree

static void MoteurPorteEntree()
// Mode d'emploi :
// Phase moteur du processus Entree
{
	for(;;)
	{
		Voiture voiture;
		// p Operation --> Reservation sur MP NbPlace
		struct sembuf reserverNb = {MutexMPNbPlaces, -1,0};
		// v Operation --> liberation sur MP NbPLace
		struct sembuf libererNb  = {MutexMPNbPlaces, 1, 0};
		// p Operation --> Reservation sur MP Requete
		struct sembuf reserverRequete = {MutexMPRequetes, -1,0};
		// v Operation --> liberation sur MP Requete
		struct sembuf libererRequete  = {MutexMPRequetes, 1, 0};


		while(msgrcv(balEntree,&voiture,sizeof(Voiture),0,0) == -1);


		DessinerVoitureBarriere(barriereType,voiture.usagerVoiture);

		// Réservation de la mémoire pour le nb de places occupées
		while(semop(semGene,&reserverNb,1)==-1 && errno==EINTR);
		NbPlaceOccupees* nbpcheck = (NbPlaceOccupees*) shmat(memIDNbPlace,NULL,0);
		unsigned int nbPlaceOccupee = nbpcheck->nb;
		shmdt(nbpcheck);
		// Libération de la mémoire NbPlace
		semop(semGene,&libererNb,1);

		if(nbPlaceOccupee >= NB_PLACES)
		{
			AfficherRequete(barriereType, voiture.usagerVoiture, voiture.hArrivee);

			// On ecrit dans la mémoire partagée que l'on a une requete !

			while(semop(semGene,&reserverRequete,1)==-1 && errno==EINTR); // Reservation de la memoire partagee Requete

			Requete *req = (Requete*) shmat(memIDRequete,NULL,0);
			req->requetes[barriereType - 1] = voiture;
			shmdt(req);

			semop(semGene,&libererRequete,1); // Liberation de la memoire Requete

			struct sembuf pOp = {(short unsigned int)(barriereType-1),-1,0};  // p Operation sur le mutex de synchronisation
			while(semop(semGene,&pOp,1)==-1 && errno==EINTR);
			semop(semGene, &pOp,0);

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

		// Réservation de la mémoire pour le nb de places occupées
		while(semop(semGene,&reserverNb,1)==-1 && errno==EINTR);
		NbPlaceOccupees* nbp = (NbPlaceOccupees*) shmat(memIDNbPlace,NULL,0);
		nbp->nb++;
		shmdt(nbp);
		semop(semGene,&libererNb,1); // Libération de la mémoire NbPlace

		// garage voiture ajout du pid voiturier dans la list
		pid_t voiturierEntree;
		if((voiturierEntree = GarerVoiture(barriereType)) == -1)
		{
			continue;
		}
		voituriersEnEntree.insert(make_pair(voiturierEntree, &voiture));

		// sleep 1s
		sleep(TEMPO);

	}
} //----- fin de MoteurPorteEntree

static void DestructionPorteEntree(int noSignal)
// Mode d'emploi :
// 	Phase de destruction du processus Entree
{
	if(noSignal == SIGUSR2){
		struct sigaction action;
		action.sa_handler = SIG_IGN ;
		sigemptyset(&action.sa_mask);
		action.sa_flags = 0 ;
		sigaction(SIGCHLD,&action,NULL);

		map<pid_t, Voiture*>::iterator itLE;

		for( itLE = voituriersEnEntree.begin();
		     itLE != voituriersEnEntree.end();
			 itLE++){
			kill(itLE->first, SIGUSR2);
		}
		for( itLE = voituriersEnEntree.begin();
		     itLE != voituriersEnEntree.end();
			 itLE++){
			waitpid(itLE->first,NULL,0);
		}

		exit(0);
	}
} //----- fin de DestructionPorteEntree

static void ReceptionMortVoiturier(int noSignal)
// Mode d'emploi
// 	Handler pour le signal SIGCHLD
{

	if(noSignal == SIGCHLD){
		// p Operation --> Reservation sur MP Etat
		struct sembuf reserverEtat = {MutexMPEtat, -1,0};
		// v Operation --> liberation sur MP Etat
		struct sembuf libererEtat  = {MutexMPEtat, 1, 0};

		int status;
		// Recuperer le fils qui a envoye le SIGCHLD
		pid_t filsFini;
		// Recuperer le fils qui a envoye le SIGCHLD
		while((filsFini = waitpid(-1, &status, 0)) == -1);
		int numPlace = 0;
		if (WIFEXITED(status))
		{
			numPlace = WEXITSTATUS(status);
		}

		map<pid_t, Voiture*>::iterator itEntree;
		itEntree = voituriersEnEntree.find(filsFini);

		// Recuperer la bonne voiture qui a lancé le signal
		Voiture *v = itEntree->second;

		// Ficher ses caractéristiques dans l'endroit indique
		AfficherPlace(numPlace,v->usagerVoiture,v->numPlaque,v->hArrivee);

		// Reservation de la memoire
		while(semop(semGene,&reserverEtat,1)==-1 && errno==EINTR);
		// Ecrire la voiture sur la mémoire partagée
		Etat *et = (Etat *) shmat(memIDEtat, NULL, 0);
		et->places[numPlace-1] = v;
		shmdt(et);

		// Liberation de la memoire
		semop(semGene,&libererEtat,1);

		// Supprimer la voiture de la map
		voituriersEnEntree.erase(itEntree); //-----------------------------------------Possible de faire avec filsFini / y a un bug par ici (affichage multiple de la meme plaque et meme heure)

	}
} //----- Fin de ReceptionMortVoiturier

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void PorteEntree (int pbalID,
				  int psemID,
				  int pmemIDNbPlace,
				  int pmemIDEtat,
				  int pmemIDRequete,
				  TypeBarriere barType)
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
	MoteurPorteEntree();

} //----- fin de PorteEntre
