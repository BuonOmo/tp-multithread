/*************************************************************************
                           GestionClavier  -  description
                             -------------------
    début                : 18 mars 2016
    copyright            : (C) GestionClavier par Pierre Bayle et Ulysse
                                                                 Buonomo
    e-mail               :  pierre.bayle@insa-lyon.fr
*************************************************************************/

//----- Interface du module <GestionClavier> (fichier GestionClavier.h) --
#ifndef GESTIONCLAVIER_H
#define GESTIONCLAVIER_H

//------------------------------------------------------------------------
// Rôle du module <GestionClavier>
//
//
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisées

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void SimulationClavier();

void Commande ( char code, unsigned int valeur );


#endif // GESTIONCLAVIER_H
