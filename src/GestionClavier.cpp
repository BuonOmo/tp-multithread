/*************************************************************************
                           GestionClavier  -  description
                             -------------------
    début                : GestionClavier
    copyright            : (C) GestionClavier par Pierre Bayle et
                               Ulysse Buonomo
    e-mail               : pbayle@insa-lyon.fr
*************************************************************************/

//-- Réalisation du module <GestionClavier> (fichier GestionClavier.cpp) -

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système

//------------------------------------------------------ Include personnel
#include "GestionClavier.h"
#include "Menu.h"
#include <unistd.h>
#include <iostream>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/wait.h>
///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
static int balSortie;
static int balEntreeBPP;
static int balEntreeBPA;
static int balEntreeGB;

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

static void SignalDestruction (int noSig)
{
	if(noSig == SIGUSR2)
	{
		exit(0);
	}
}

static void InitialisationClavier
	(int aBalSortie, int aBalEntreeBPP,int aBalEntreeBPA,int aBalEntreeGB)
{
    struct sigaction action1;
    action1.sa_handler = SignalDestruction;
    sigemptyset(&action1.sa_mask);
    action1.sa_flags = 0;
    sigaction(SIGUSR2, &action1, NULL);
    balSortie = aBalSortie;
	balEntreeBPP = aBalEntreeBPP;
	balEntreeBPA = aBalEntreeBPA;
	balEntreeGB = aBalEntreeGB;
}

void SimulationClavier
	(int aBalSortie, int aBalEntreeBPP,int aBalEntreeBPA,int aBalEntreeGB)
{
#ifdef MAP
    cout << "Appel à la méthode SimulationClavier" << endl;
#endif
	InitialisationClavier(aBalSortie, aBalEntreeBPP, aBalEntreeBPA, aBalEntreeGB);
    for ( ; ; )
    {
        Menu ( );
    }
} //----- Fin de SimulationClavier

void Commande ( char code, unsigned int valeur )
{
    switch (code) {
        case 'E'|'e': // EXIT
            /* quitter l’appli :
             *  - envoyer SIGCHILD à mere
             *  - se tuer
             */
             kill(getppid(), SIGUSR2); //_________________________________________  exit(0) ici au lieu de SIGUSR2
             waitpid(getppid(),NULL,0);
            break;
        case 'P'|'p': // PROF
            /*  - ajouter une voiture à une file d’attente PE
             *
             */
			switch (valeur)
        	{
	            case 1:
					msgsnd(balEntreeBPP,NULL,0,0);
	                break;
	            case 2:
	                 msgsnd(balEntreeGB ,NULL,0,0);
	                break;
				default:
					return;
        	}
            break;
        case 'A'|'a': // AUTRE
            /*   - ajouter une voiture à une file d’attente PE
             *
             */
			switch (valeur)
			{
				case 1:
					msgsnd(balEntreeBPA,NULL,0,0);
					break;
				case 2:
					 msgsnd(balEntreeGB ,NULL,0,0);
					break;
				default:
					return;
			}
            break;
        case 'S'|'s': // SORTIE
              /* - ajouter une voiture à la file d’attente PS
               *
               */
               msgsnd(balSortie,&valeur,sizeof(int),0);
            break;
		default:
			std::cerr << "Mauvaise commande : "
					  << code << ' ' << valeur << std::endl;
			return;
    }
}
