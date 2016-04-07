/*************************************************************************
                           GestionClavier  -  description
                             -------------------
    début                : 18 mars 2016
    copyright            : (C) GestionClavier par Pierre Bayle et
                               Ulysse Buonomo
    e-mail               : ulysse.buonomo@insa-lyon.fr
*************************************************************************/

//-- Réalisation du module <GestionClavier> (fichier GestionClavier.cpp) -

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système
#include <iostream>
#include <sys/msg.h>

//------------------------------------------------------ Include personnel
#include "GestionClavier.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes
#define MAX_IMMATRICULATION 999

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
static int balSortie;
static int balEntreeBPP;
static int balEntreeBPA;
static int balEntreeGB;
static int immatriculation;

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

void SimulationClavier
	(int aBalSortie, int aBalEntreeBPP,int aBalEntreeBPA,int aBalEntreeGB)
// Algorithme :
// Initialise la tâche gestion clavier. Lance la boucle d’accès au Menu,
// voir Menu.h et la fonction Commande () ci-dessous.
{
#ifdef MAP
    cout << "Appel à la méthode SimulationClavier" << endl;
#endif
	// phase d’initialisation
	balSortie = aBalSortie;
	balEntreeBPP = aBalEntreeBPP;
	balEntreeBPA = aBalEntreeBPA;
	balEntreeGB = aBalEntreeGB;

	// phase moteur
    for ( ; ; )
    {
        Menu ( );
    }
} //----- Fin de SimulationClavier

void Commande ( char code, unsigned int valeur )
// Mode d’emploi :
// Voir mode d’emploi de Menu (Menu.h)
{
	Voiture uneVoiture;
    switch (code) {
        case 'E' :
		case 'e' : // EXIT
            /* quitter l’appli :
             *  - envoyer SIGCHILD à mere
             *  - se tuer
             */
            exit(0);
            break;

        case 'P' :
		case 'p' : // PROF
            /* ajouter une voiture à une file d’attente PE */
			uneVoiture.usagerVoiture = PROF;
			uneVoiture.numPlaque = (immatriculation++)%MAX_IMMATRICULATION;
			uneVoiture.hArrivee = time(NULL);
			switch (valeur)
        	{
	            case 1:
					msgsnd(balEntreeBPP,&uneVoiture,sizeof(Voiture),0);
	                break;
	            case 2:
	                msgsnd(balEntreeGB ,&uneVoiture,sizeof(Voiture),0);
	                break;
        	}
            break;

        case 'A' :
		case 'a' : // AUTRE
            /* ajouter une voiture à une file d’attente PE */

			uneVoiture.usagerVoiture = AUTRE;
			uneVoiture.numPlaque = (immatriculation++)%MAX_IMMATRICULATION;
			uneVoiture.hArrivee = time(NULL);
			switch (valeur)
			{
				case 1:
					msgsnd(balEntreeBPA,&uneVoiture,sizeof(Voiture),0);
					break;
				case 2:
					msgsnd(balEntreeGB ,&uneVoiture,sizeof(Voiture),0);
					break;
			}
            break;

        case 'S' :
		case 's' : // SORTIE
            /* ajouter une voiture à la file d’attente PS */
            msgsnd(balSortie,&valeur,sizeof(unsigned int),0);
            break;
    }
} //----- Fin de Commande
