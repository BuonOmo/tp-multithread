/*************************************************************************
                PorteSortie  -  gestion des portes d’entrée
                             -------------------
    début                : 18 mars 2016
    copyright            : (C) PorteSortie par Pierre Bayle et Ulysse
                                                              Buonomo
    e-mail               : pierre.bayle@insa-lyon.fr
*************************************************************************/

//----- Réalisation de la tâche <PorteSortie> (fichier PorteSortie.cpp) --

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
//------------------------------------------------------ Include personnel
#include "Donnees.h"
#include "Mere.h"
#include "Outils.h"
#include "PorteSortie.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
static vector<pid_t> voituriersEnSortie; //Vecteur qui stocke les pid des
//voituriers toujours en route
static int memIDNbPlace;
static int memIDEtat;
static int memIDRequete;
static int semGene;
static int balSortie;
//------------------------------------------------------ Fonctions privées

static bool operator < (enum TypeUsager typeGauche, enum TypeUsager typeDroit)
//Mode d'emploi :
//	Redefinition de l'opérateur < pour le type TypesUsager
{
    //On inverse l'ordre car ici on veux
    //AUCUN<AUTRE<PROF
    //Or notre enum nous donne AUCUN<PROF<AUTRE
    if(typeGauche == 2 && typeDroit == 1) return true;
    if(typeGauche == 1 && typeDroit == 2) return false;
    return (int)typeGauche<(int)typeDroit;
}

static unsigned int definirPriorite(Voiture const & a, Voiture const & b, Voiture const & c)
//Mode d'emploi :
//	Permet de gerer les prioritées entre prof et autre
{
	Voiture requetePrio = a;

	if(requetePrio.typeUsager < b.typeUsager){
		requetePrio= b;
	}else if(b.typeUsager == requetePrio.typeUsager){
		if(b.instantArrivee < requetePrio.instantArrivee){
			requetePrio= b;
		}
	}


	if(requetePrio.typeUsager < c.typeUsager){
        requetePrio= c;
	}else if(c.typeUsager == requetePrio.typeUsager){
		if(c.instantArrivee < requetePrio.instantArrivee){
			requetePrio= c;
		}
	}


    if(requetePrio.typeUsager == AUCUN){
        return 0;
    }
    if(requetePrio.typeUsager == a.typeUsager && requetePrio.instantArrivee == a.instantArrivee){
        return 1;
    }
    if(requetePrio.typeUsager == b.typeUsager && requetePrio.instantArrivee == b.instantArrivee){
        return 2;
    }
    if(requetePrio.typeUsager == c.typeUsager && requetePrio.instantArrivee == c.instantArrivee){
        return 3;
    }

    return 0;
}


static void destruction(int noSignal);
static void receptionMortVoiturier(int noSignal);


static void InitialisationPorteSortie()
//Mode d'emploi :
//	Phase d'initialisation du processus PorteSortie
{
	//Installation du handler sur SIGUSR2
	struct sigaction action;
	action.sa_handler = DestructionPorteSortie ;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0 ;
	sigaction(SIGUSR2,&action,NULL);

	//Installation du handler sur SIGCHLD
	struct sigaction actionMortVoiturier;
	actionMortVoiturier.sa_handler = ReceptionMortVoiturier ;
	sigemptyset(&actionMortVoiturier.sa_mask);
	actionMortVoiturier.sa_flags = 0 ;
	sigaction(SIGCHLD,&actionMortVoiturier,NULL);

}

static void MoteurPorteSortie()
//Mode d'emploi
//	Phase moteur du processus Sortie
{
	int numeroPlace;

	//Lecture sur le canal du numero de la place
	if(msgrcv(balSortie,&numeroPlace,sizeof(unsigned int),0,MSG_NOERROR) > 0){
		pid_t voiturierSortie = SortirVoiture(numeroPlace);

		//on stocke les voituriers en sortie pour pouvoir les supprimer si on appuie sur Q
		voituriersEnSortie.push_back(voiturierSortie);
	}
}

static void DestructionPorteSortie(int noSignal)
//Mode d'emploi :
//	Phase de destruction du processus PorteSortie
{
	if(noSignal == SIGUSR2){
		struct sigaction action;
		action.sa_handler = SIG_IGN ;
		sigemptyset(&action.sa_mask);
		action.sa_flags = 0 ;
		sigaction(SIGCHLD,&action,NULL);


		for(vector<pid_t>::iterator itLE = voituriersEnSortie.begin(); itLE != voituriersEnSortie.end(); itLE++){
			kill(*itLE, SIGUSR2);
		}
		for(vector<pid_t>::iterator itLE = voituriersEnSortie.begin(); itLE != voituriersEnSortie.end(); itLE++){
			waitpid(*itLE,NULL,0);  
		}

		exit(0);
	}
}

static void ReceptionMortVoiturier(int noSignal)
//Mode d'emploi
//	--Handler pour le signal SIGCHLD
{

	if(noSignal == SIGCHLD){
		struct sembuf reserverNb 	  = {MutexMPNbPlaces, -1,0}; //p Operation --> Reservation sur MP NbPlace
		struct sembuf libererNb 	  = {MutexMPNbPlaces, 1, 0}; //v Operation --> liberation sur MP NbPLace
		struct sembuf reserverEtat 	  = {MutexMPEtat, -1,0};	 //p Operation --> Reservation sur MP Etat
		struct sembuf libererEtat 	  = {MutexMPEtat, 1, 0};	 //v Operation --> liberation sur MP Etat
		struct sembuf reserverRequete = {MutexMPRequetes, -1,0}; //p Operation --> Reservation sur MP Requete
		struct sembuf libererRequete  = {MutexMPRequetes, 1, 0}; //v Operation --> liberation sur MP Requete

		int status;
		Voiture requetePorteBPPROF;
		Voiture requetePorteBPAUTRE;
		Voiture requetePorteGB;


		pid_t filsFini = wait(&status); //Recuperer le fils qui a envoye le SIGCHLD


		Effacer((TypeZone)WEXITSTATUS(status)); //Efface la bonne place sur l'ecran


		while(semop(semGene,&reserverEtat,1)==-1 && errno==EINTR); //Reservation de la memoire pour l'Etat

		//Recuperer la voiture
		Etat *et = (Etat *) shmat(memIDEtat, NULL, 0) ;
		Voiture v = et->places[WEXITSTATUS(status)-1] ;
		shmdt(et);
		semop(semGene,&libererEtat,1); //Liberation de la memoire Etat


		//Recuperer les requêtes en attente afin de déterminer laquelle va être satisfaite en première
		while(semop(semGene,&reserverRequete,1)==-1 && errno==EINTR); //Reservation de la memoire pour les Requetes

		Requete *req = (Requete*) shmat(memIDRequete,NULL,0);
		requetePorteBPPROF = a->requetes[PROF_BLAISE_PASCAL -1];
		requetePorteBPAUTRE = a->requetes[AUTRE_BLAISE_PASCAL -1];
		requetePorteGB = a->requetes[ENTREE_GASTON_BERGER -1];
		shmdt(req);

		semop(semGene,&libererRequete,1); //Liberation de la memoire Requete

		AfficherSortie(v.typeUsager,v.numeroPlaque,v.instantArrivee, time(NULL));

		vector<pid_t>::iterator itSorti = std::find(voituriersEnSortie.begin(),voituriersEnSortie.end(),filsFini);
		voituriersEnSortie.erase(itSorti);

		//Modifier le nombre de place occupées (décrémentation)
		while(semop(semGene,&reserverNb,1)==-1 && errno==EINTR); // Réservation de la mémoire pour le nb de places occupées
		NbPlaceOccupees* nbp = (NbPlaceOccupees*) shmat(memIDNbPlace,NULL,0);
		npb->nb--;
		shmdt(nbp);
		semop(semGene,&libererNb,1); //Libération de la mémoire NbPlace


		unsigned int prio = definirPriorite(requetePorteBPPROF, requetePorteBPAUTRE, requetePorteGB);

		if(prio!=0){

			while(semop(semID,&reserverRequete,1)==-1 && errno==EINTR); //Reservation de la memoire pour les requêtes afin d'effacer la requête de la mémoire


			Requete *reqASuppr = (Requete *) shmat(memIDRequete, NULL, 0) ;
			reqASuppr->requetes[prio-1] = {AUCUN, 0,0};	 //On efface la requete de la memoire
			shmdt(reqASuppr);

			semop(semID,&libererRequete,1); //Liberation de la memoire

			struct sembuf pOp = {prio-1,1,0};
			semop(semGene,&pOp,1); //On relache le bon semaphore de synchronisation (on choisit la bonne porte(1 BPprof, 2 BPAutre, 3GB))
		}

	}
}
//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques


void PorteSortie(int pmemIDNbPlace , int pmemIDEtat, int pmemIDRequete, int pbalID, int psemID)
//Mode d'emploi
//	Processus fils Sortie
{
	memIDNbPlace = pmemIDNbPlace;
	memIDEtat = pmemIDEtat;
	memIDRequete = pmemIDRequete;
	balSortie = pbalID;
	semGene = psemID;

	InitialisationPorteSortie();

	for(;;){
		MoteurPorteSortie();
	}

}
