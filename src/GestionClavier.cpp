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
///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques

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
type Nom ( liste de paramètres )
// Algorithme :
//
{
} //----- fin de Nom

void SimulationClavier ( )
{
#ifdef MAP
    cout << "Appel à la méthode SimulationClavier" << endl;
#endif
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
            break;
    }
}
