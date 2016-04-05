/*************************************************************************
                           Donnees  -  description
                             -------------------
    début                : 21 mars 2016
    copyright            : (C) Mere par Pierre Bayle et Ulysse Buonomo
    e-mail               : pierre.bayle@insa-lyon.fr
*************************************************************************/

//---------- Interface de la tâche <Donnees> (fichier Donnees.h) -------
#ifndef DONNEES_H
#define DONNEES_H

//------------------------------------------------------------------------
// Rôle de la tâche <Donnees>
//
//
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisées
#include "Outils.h"

//------------------------------------------------------------- Constantes


const int NB_SEM = 6;
const int TEMPO = 1;


//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC

struct Voiture {
	TypeUsager usagerVoiture;
	int numPlaque;
	time_t hArrivee;	
};
typedef struct Voiture Voiture;

struct Requete {
	Voiture requetes[NB_BARRIERES_ENTREE]; 
};

typedef struct Requete Requete;

struct NbPlaceOccupees {
	unsigned int nb;
};

typedef struct NbPlaceOccupees NbPlaceOccupees;

struct Etat {
	Voiture places[NB_PLACES];
};

typedef struct Etat Etat;

typedef enum Semaphore
{
	SynchroPorteBPPROF = 0,
	SynchroPorteBPAUTRE = 1,
	SynchroPorteGB = 2,
	MutexMPNbPlaces = 3,
	MutexMPEtat = 4,
	MutexMPRequetes = 5,

}Semaphore;

//---------------------------------------------------- Fonctions publiques


#endif // MERE_H
