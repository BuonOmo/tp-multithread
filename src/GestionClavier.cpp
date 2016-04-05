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

static void InitialisationClavier(int balSortieID)

{
    struct sigaction action1;
    action1.sa_handler = SignalDestruction;
    sigemptyset(&action1.sa_mask);
    action1.sa_flags = 0;
    sigaction(SIGUSR2, &action1, NULL);
    balSortie = balSortieID;

}

void SimulationClavier (int BalSortieID )
{
#ifdef MAP
    cout << "Appel à la méthode SimulationClavier" << endl;
#endif
	InitialisationClavier(BalSortieID);
    for ( ; ; )
    {
        Menu ( );
    }
} //----- Fin de SimulationClavier

void Commande ( char code, unsigned int valeur )
{
    switch (code) {
        case 'E': // EXIT
            /* quitter l’appli :
             *  - envoyer SIGCHILD à mere
             *  - se tuer
             */
             kill(getppid(), SIGUSR2);
             waitpid(getppid(),NULL,0);
            break;
        case 'P': // PROF
            /*  - ajouter une voiture à la file d’attente PE
             * 
             */
            break;
        case 'A': // AUTRE
            /*   - ajouter une voiture à la file d’attente PE
             *
             */
            break;
        case 'S': // SORTIE
              /* - ajouter une voiture à la file d’attente PS
               *
               */
               msgsnd(balSortie,&valeur,sizeof(unsigned int),0); // Ecriture du numéro de place choisit
            break;
    }
}
