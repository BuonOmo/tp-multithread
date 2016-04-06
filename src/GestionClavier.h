/*************************************************************************
                           GestionClavier  -  description
                             -------------------
    début                : 18 mars 2016
    copyright            : (C) GestionClavier par Pierre Bayle et
                               Ulysse Buonomo
    e-mail               : ulysse.buonomo@insa-lyon.fr
*************************************************************************/

//----- Interface du module <GestionClavier> (fichier GestionClavier.h) --
#ifndef GESTIONCLAVIER_H
#define GESTIONCLAVIER_H

//------------------------------------------------------------------------
// Permet de controler les entrées clavier de la simulation, fonctionne
// avec l’interface de Menu.
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisées
#include "Menu.h"
#include "Donnees.h"

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

void SimulationClavier
    (int aBalSortie,int aBalEntreeBPP,int aBalEntreeBPA,int aBaLEntreeGB);

void Commande ( char code, unsigned int valeur );


#endif // GESTIONCLAVIER_H
