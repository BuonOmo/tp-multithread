/*************************************************************************
                           GestionClavier  -  Gère les entrées clavier
                             -------------------
    début                : 18/3/2016
    copyright            : (C) 2016 par Pierre et Ulysse
*************************************************************************/

//---------- Réalisation de la classe <GestionClavier> (fichier GestionClavier) --

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
using namespace std;
#include <iostream>

//------------------------------------------------------ Include personnel
#include "GestionClavier.h"

//------------------------------------------------------------- Constantes

//---------------------------------------------------- Variables de classe

//----------------------------------------------------------- Types privés


//----------------------------------------------------------------- PUBLIC
//-------------------------------------------------------- Fonctions amies

//----------------------------------------------------- Méthodes publiques
void GestionClavier::SimulationClavier ()
// Algorithme :
//
{
	for(;;)
	{
		Menu();
	}
} //----- Fin de Méthode


//------------------------------------------------- Surcharge d'opérateurs
GestionClavier & GestionClavier::operator = ( const GestionClavier & aGestionClavier )
// Algorithme :
//
{
    if (this != &aGestionClavier)
    {
        
    }
    return *this;
} //----- Fin de operator =


//-------------------------------------------- Constructeurs - destructeur
GestionClavier::GestionClavier ( const GestionClavier & aGestionClavier )
// Algorithme :
//
{
#ifdef MAP
    cout << "Appel au constructeur de copie de <GestionClavier>" << endl;
#endif
    *this = aGestionClavier;
} //----- Fin de GestionClavier (constructeur de copie)


GestionClavier::GestionClavier (  )
    : ;
// Algorithme :
//
{
#ifdef MAP
    cout << "Appel au constructeur de <GestionClavier>" << endl;
#endif
} //----- Fin de GestionClavier


GestionClavier::~GestionClavier ( )
// Algorithme :
//
{
#ifdef MAP
    cout << "Appel au destructeur de <GestionClavier>" << endl;
#endif
} //----- Fin de ~GestionClavier


//------------------------------------------------------------------ PRIVE

//----------------------------------------------------- Méthodes protégées

//------------------------------------------------------- Méthodes privées
