/*************************************************************************
                           PorteSortie  -  description
                             -------------------
    début                : 18 mars 2016
    copyright            : (C) PorteSortie par Pierre Bayle
    e-mail               : pierre.bayle@insa-lyon.fr
*************************************************************************/

//-------- Interface de la tâche <PorteSortie> (fichier PorteSortie.h) ---
#ifndef PORTE_SORTIE_H
#define PORTE_SORTIE_H

//------------------------------------------------------------------------
// Rôle de la tâche <PorteSortie>
//
//
//------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void PorteSortie( int pmemIDNbPlace,
                  int pmemIDEtat,
                  int pmemIDRequete,
                  int pbalID,
                  int psemID);
// Mode d'emploi :
//	Processus fils sortie.

#endif // PORTE_SORTIE_H
