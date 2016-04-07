/*************************************************************************
                           PorteEntree  -  description
                             -------------------
    début                : 18 mars 2016
    copyright            : (C) PorteEntree par Pierre Bayle et Ulysse Buonomo
    e-mail               : pierre.bayle@insa-lyon.fr
*************************************************************************/

//---------- Interface de la tâche <PorteEntree> (fichier PorteEntree.h) -------
#ifndef PORTE_ENTREE_H
#define PORTE_ENTREE_H

//------------------------------------------------------------------------
// Gère une porte d’entrée (tâche lancée 3 fois)
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisées
#include "Donnees.h"

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void PorteEntree ( int pbalID,
                   int psemID,
                   int pmemIDNbPlace,
                   int pmemIDEtat,
                   int pmemIDRequete,
                   TypeBarriere barType);
// Mode d'emploi :
// Lance la tâche porteEntree

#endif // PORTE_ENTREE_H
