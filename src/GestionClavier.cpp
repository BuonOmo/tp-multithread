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
#include "Donnees.h"
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
static int cpt;

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

static void InitialisationClavier
	(int aBalSortie, int aBalEntreeBPP,int aBalEntreeBPA,int aBalEntreeGB)
{
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
            exit(0);
            break;
        case 'P'|'p': // PROF
            /*  - ajouter une voiture à une file d’attente PE
             *
             */
			switch (valeur)
        	{
				Voiture v;
	            case 1:
					v.usagerVoiture = PROF;
					v.numPlaque = (cpt++)%999;
					v.hArrivee = time(NULL);
					msgsnd(balEntreeBPP,&v,0,0);
	                break;
	            case 2:
					v.usagerVoiture = PROF;
					v.numPlaque = (cpt++)%999;
					v.hArrivee = time(NULL);
	                msgsnd(balEntreeGB ,&v,0,0);
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
				Voiture v;
				case 1:
					v.usagerVoiture = AUTRE;
					v.numPlaque = (cpt++)%999;
					v.hArrivee = time(NULL);
					msgsnd(balEntreeBPA,&v,0,0);
					break;
				case 2:
					v.usagerVoiture = AUTRE;
					v.numPlaque = (cpt++)%999;
					v.hArrivee = time(NULL);
					msgsnd(balEntreeGB ,&v,0,0);
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
