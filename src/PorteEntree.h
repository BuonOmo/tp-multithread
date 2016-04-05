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
// Rôle de la tâche <PorteEntree>
//
//
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisées
#include "Donnees.h"

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void PorteEntree (int pbalID, int pmemIDNbPlace, int pmemIDEtat, int pmemIDRequete, TypeBarriere barType);
// Mode d'emploi :
//
// Contrat :
//

#endif // PORTE_ENTREE_H
