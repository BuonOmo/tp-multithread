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

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC

struct Requete {
	TypeBarriere barriere;
	TypeUsager usager; 
};

typedef struct Requete Requete;

struct Etat {
	TypeUsager usagerEtat;
	int numPlace;
	time_t hArrivee;
}

typedef struct Etat Etat;

// Nombre maximal de requêtes possibles pour l'application
const int MAX_REQUETES = 999;
//---------------------------------------------------- Fonctions publiques


#endif // MERE_H
