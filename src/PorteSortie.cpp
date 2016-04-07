/*************************************************************************
                PorteSortie  -  gestion des portes d’entrée
                             -------------------
    début                : 18 mars 2016
    copyright            : (C) PorteSortie par Pierre Bayle
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
#include <vector>
//------------------------------------------------------ Include personnel
#include "Donnees.h"
#include "Mere.h"
#include "PorteSortie.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
// Vecteur qui stocke les pid des voituriers toujours en marche
static std::vector<pid_t> voituriersEnSortie;
static int memIDNbPlace;
static int memIDEtat;
static int memIDRequete;
static int semGene;
static int balSortie;
//------------------------------------------------------ Fonctions privées

static bool operator < (enum TypeUsager typeGauche, enum TypeUsager typeDroit)
// Mode d'emploi :
// 	Redefinition de l'opérateur < pour le type TypesUsager
{
    // On inverse l'ordre car ici on veux
    // AUCUN<AUTRE<PROF
    // Or notre enum nous donne AUCUN<PROF<AUTRE
    if (typeGauche == 2 && typeDroit == 1) return true;
    if (typeGauche == 1 && typeDroit == 2) return false;
    return (int)typeGauche<(int)typeDroit;
}

static short unsigned int definirPriorite(Voiture const & a, Voiture const & b, Voiture const & c)
// Mode d'emploi :
// 	Permet de gerer les prioritées entre prof et autre
{
	Voiture requetePrio = a;

	if (requetePrio.usagerVoiture < b.usagerVoiture)
    {
		requetePrio= b;
	}
    else if (b.usagerVoiture == requetePrio.usagerVoiture)
    {
		if (b.hArrivee < requetePrio.hArrivee)
        {
			requetePrio= b;
		}
	}

	if (requetePrio.usagerVoiture < c.usagerVoiture)
    {
        requetePrio= c;
	}else if (c.usagerVoiture == requetePrio.usagerVoiture)
    {
		if (c.hArrivee < requetePrio.hArrivee)
        {
			requetePrio= c;
		}
	}


    if (requetePrio.usagerVoiture == AUCUN)
    {
        return 0;
    }
    if (   (requetePrio.usagerVoiture == a.usagerVoiture)
       && (requetePrio.hArrivee == a.hArrivee) )
    {
        return 1;
    }
    if (   (requetePrio.usagerVoiture == b.usagerVoiture)
       && (requetePrio.hArrivee == b.hArrivee) )
    {
        return 2;
    }
    if (   (requetePrio.usagerVoiture == c.usagerVoiture)
       && (requetePrio.hArrivee == c.hArrivee) )
    {
        return 3;
    }

    return 0;
}

static void EffacerPlace (unsigned int num) {
	switch(num) {
		case 1:
			Effacer(ETAT_P1);
		case 2:
			Effacer(ETAT_P2);
		case 3:
			Effacer(ETAT_P3);
		case 4:
			Effacer(ETAT_P4);
		case 5:
			Effacer(ETAT_P5);
		case 6:
			Effacer(ETAT_P6);
		case 7:
			Effacer(ETAT_P7);
		case 8:
			Effacer(ETAT_P8);
		default :
			Afficher(MESSAGE, "Erreur de num");

	}
}


static void DestructionPorteSortie(int noSignal);
static void ReceptionMortVoiturier(int noSignal);


static void InitialisationPorteSortie()
// Mode d'emploi :
// 	Phase d'initialisation du processus PorteSortie
{
	// Installation du handler sur SIGUSR2
	struct sigaction action;
	action.sa_handler = DestructionPorteSortie;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0 ;
	sigaction(SIGUSR2,&action,NULL);

	// Installation du handler sur SIGCHLD
	struct sigaction actionMortVoiturier;
	actionMortVoiturier.sa_handler = ReceptionMortVoiturier ;
	sigemptyset(&actionMortVoiturier.sa_mask);
	actionMortVoiturier.sa_flags = 0 ;
	sigaction(SIGCHLD,&actionMortVoiturier,NULL);

}


static void DestructionPorteSortie(int noSignal)
// Mode d'emploi :
// 	Phase de destruction du processus PorteSortie
{
	if (noSignal == SIGUSR2){
		struct sigaction action;
		action.sa_handler = SIG_IGN ;
		sigemptyset(&action.sa_mask);
		action.sa_flags = 0 ;
		sigaction(SIGCHLD,&action,NULL);

        vector<pid_t>::iterator itLE;
		for( itLE = voituriersEnSortie.begin();
             itLE != voituriersEnSortie.end();
             itLE++)
        {
			kill(*itLE, SIGUSR2);
		}
		for( itLE = voituriersEnSortie.begin();
             itLE != voituriersEnSortie.end();
             itLE++)
        {
			waitpid(*itLE,NULL,0);
		}

		exit(0);
	}
}

static void ReceptionMortVoiturier(int noSignal)
// Mode d'emploi :
// Handler pour le signal SIGCHLD
{

	if (noSignal == SIGCHLD){
		// p Operation --> Reservation sur MP NbPlace
		struct sembuf reserverNb 	  = {MutexMPNbPlaces, -1,0};
		// v Operation --> liberation sur MP NbPLace
		struct sembuf libererNb 	  = {MutexMPNbPlaces, 1, 0};
        // p Operation --> Reservation sur MP Etat
		struct sembuf reserverEtat 	  = {MutexMPEtat, -1,0};
        // v Operation --> liberation sur MP Etat
		struct sembuf libererEtat 	  = {MutexMPEtat, 1, 0};
		// p Operation --> Reservation sur MP Requete
		struct sembuf reserverRequete = {MutexMPRequetes, -1,0};
		// v Operation --> liberation sur MP Requete
		struct sembuf libererRequete  = {MutexMPRequetes, 1, 0};

		int status;
		Voiture requetePorteBPPROF;
		Voiture requetePorteBPAUTRE;
		Voiture requetePorteGB;

		pid_t filsFini;
		// Recuperer le fils qui a envoye le SIGCHLD
		while((filsFini = waitpid(-1, &status, 0)) == -1);
		int numPlace = 0;
		if (WIFEXITED(status))
		{
			numPlace = WEXITSTATUS(status);
		}
		Afficher(MESSAGE, "1");
		sleep(5);

		Afficher(MESSAGE, "2");
		sleep(5);

		// Reservation de la memoire pour l'Etat
		while(semop(semGene,&reserverEtat,1)==-1 && errno==EINTR);

		// Recuperer la voiture
		Etat *et = (Etat *) shmat(memIDEtat, NULL, 0);
		Voiture v = et->places[numPlace-1];
		et->places[numPlace-1] = {AUCUN,0,0};
		shmdt(et);
		// Liberation de la memoire Etat
		semop(semGene,&libererEtat,1);

		Afficher(MESSAGE, v.usagerVoiture);
		sleep(5);

		// Recuperer les requêtes en attente afin de déterminer
		// laquelle va être satisfaite en première

		// Reservation de la memoire pour les Requetes
		while(semop(semGene,&reserverRequete,1)==-1 && errno==EINTR);

		Requete *req = (Requete*) shmat(memIDRequete,NULL,0);
		requetePorteBPPROF = req->requetes[PROF_BLAISE_PASCAL -1];
		requetePorteBPAUTRE = req->requetes[AUTRE_BLAISE_PASCAL -1];
		requetePorteGB = req->requetes[ENTREE_GASTON_BERGER -1];
		shmdt(req);

		// Liberation de la memoire Requete
		semop(semGene,&libererRequete,1);

		Afficher(MESSAGE, "4");
		sleep(5);

		EffacerPlace(numPlace);
		Afficher(MESSAGE, "5");
		sleep(5);
		AfficherSortie(v.usagerVoiture,
                       v.numPlaque,
                       v.hArrivee,
                       time(NULL));

		vector<pid_t>::iterator itSorti;
		while(itSorti != voituriersEnSortie.end() && *itSorti != filsFini)
		{
			++itSorti;
		}
		voituriersEnSortie.erase(itSorti);

		// Modifier le nombre de place occupées (décrémentation)

		// Réservation de la mémoire pour le nb de places occupées
		while(semop(semGene,&reserverNb,1)==-1 && errno==EINTR);
		NbPlaceOccupees* nbp =
                            (NbPlaceOccupees*) shmat(memIDNbPlace,NULL,0);
		nbp->nb--;
		shmdt(nbp);
		// Libération de la mémoire NbPlace
		semop(semGene,&libererNb,1);


		short unsigned int prio = definirPriorite(requetePorteBPPROF,
                                                  requetePorteBPAUTRE,
                                                  requetePorteGB);

		if (prio!=0){

			// Reservation de la memoire pour les requêtes afin d'effacer
			// la requête de la mémoire
			while(semop(semGene,&reserverRequete,1)==-1 && errno==EINTR);


			Requete *reqASuppr = (Requete *) shmat(memIDRequete, NULL, 0) ;
            // On efface la requete de la memoire
			reqASuppr->requetes[prio-1] = {AUCUN, 0,0};
			shmdt(reqASuppr);

			// Liberation de la memoire
			semop(semGene,&libererRequete,1);

			struct sembuf pOp = {(short unsigned int)(prio-1),1,0};
			// On relache le bon semaphore de synchronisation (on choisit
			// la bonne porte(1 BPprof, 2 BPAutre, 3GB))
			semop(semGene,&pOp,1);
		}

	}
}

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void PorteSortie( int pmemIDNbPlace,
                  int pmemIDEtat,
                  int pmemIDRequete,
                  int pbalID,
                  int psemID)
// Mode d'emploi
// 	Processus fils Sortie
{
	memIDNbPlace = pmemIDNbPlace;
	memIDEtat = pmemIDEtat;
	memIDRequete = pmemIDRequete;
	balSortie = pbalID;
	semGene = psemID;

	InitialisationPorteSortie();

	for(;;){
		unsigned int numeroPlace;

		// Lecture sur le canal du numero de la place
		while(msgrcv(balSortie,&numeroPlace,sizeof(unsigned int),0,0) == -1);

		// p Operation --> Reservation sur MP NbPlace
		struct sembuf reserverNb = {MutexMPNbPlaces, -1,0};
		// v Operation --> liberation sur MP NbPLace
		struct sembuf libererNb  = {MutexMPNbPlaces, 1, 0};
		// Modifier le nombre de place occupées (décrémentation)
		// Réservation de la mémoire pour le nb de places occupées
		while(semop(semGene,&reserverNb,1)==-1 && errno==EINTR);
		NbPlaceOccupees* nbp = (NbPlaceOccupees*) shmat(memIDNbPlace,NULL,0);
		unsigned int nbPlaceOccupee = nbp->nb;
		shmdt(nbp);
		// Libération de la mémoire NbPlace
		semop(semGene,&libererNb,1);

		if (nbPlaceOccupee == 0)
		{
			continue;
		}
		pid_t voiturierSortie;
		if ((voiturierSortie = SortirVoiture(numeroPlace)) == -1)
		{
			continue;
		}

		// on stocke les voituriers en sortie pour pouvoir les supprimer
		// si on appuie sur E
		voituriersEnSortie.push_back(voiturierSortie);

	}

}
