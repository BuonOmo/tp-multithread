/*************************************************************************
                           Mere  -  tache principale
                             -------------------
    début                : 18/3/2016
    copyright            : (C) 2016 par Pierre et Ulysse
*************************************************************************/

//---------- Réalisation de la classe <Mere> (fichier Mere) --

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
using namespace std;
#include <iostream>

//------------------------------------------------------ Include personnel
#include "Mere.h"

//------------------------------------------------------------- Constantes

//---------------------------------------------------- Variables de classe

//----------------------------------------------------------- Types privés


//----------------------------------------------------------------- PUBLIC
//-------------------------------------------------------- Fonctions amies

//----------------------------------------------------- Méthodes publiques
// type Mere::Méthode ( liste de paramètres )
// Algorithme :
//
//{
//} //----- Fin de Méthode


//------------------------------------------------- Surcharge d'opérateurs
Mere & Mere::operator = ( const Mere & aMere )
// Algorithme :
//
{
    if (this != &aMere)
    {
        
    }
    return *this;
} //----- Fin de operator =


//-------------------------------------------- Constructeurs - destructeur
Mere::Mere ( const Mere & aMere )
// Algorithme :
//
{
#ifdef MAP
    cout << "Appel au constructeur de copie de <Mere>" << endl;
#endif
    *this = aMere;
} //----- Fin de Mere (constructeur de copie)


Mere::Mere (  )
    : ;
// Algorithme :
//
{
#ifdef MAP
    cout << "Appel au constructeur de <Mere>" << endl;
#endif
} //----- Fin de Mere


Mere::~Mere ( )
// Algorithme :
//
{
#ifdef MAP
    cout << "Appel au destructeur de <Mere>" << endl;
#endif
} //----- Fin de ~Mere


//------------------------------------------------------------------ PRIVE

//----------------------------------------------------- Méthodes protégées

//------------------------------------------------------- Méthodes privées
