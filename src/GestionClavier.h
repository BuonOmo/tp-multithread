/*************************************************************************
                           GestionClavier  -  Gère les entrées clavier
                             -------------------
    début                : 18/3/2016
    copyright            : (C) 2016 par Pierre et Ulysse
*************************************************************************/

//-------------- Interface de la classe <GestionClavier> (fichier GestionClavier) --------------
#ifndef GESTIONCLAVIER_H
#define GESTIONCLAVIER_H

//--------------------------------------------------- Interfaces utilisées

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//------------------------------------------------------------------------
// Rôle de la classe <GestionClavier>
//
//
//------------------------------------------------------------------------

class GestionClavier
{
//----------------------------------------------------------------- PUBLIC

public:
//----------------------------------------------------- Méthodes publiques
    void SimulationClavier ();
    // Mode d'emploi :
    //
    // Contrat :
    //


//------------------------------------------------- Surcharge d'opérateurs
    GestionClavier & operator = ( const GestionClavier & aGestionClavier );
    // Mode d'emploi :
    //
    // Contrat :
    //


//-------------------------------------------- Constructeurs - destructeur
    GestionClavier ( const GestionClavier & aGestionClavier );
    // Mode d'emploi (constructeur de copie) :
    //
    // Contrat :
    //

    GestionClavier (  );
    // Mode d'emploi :
    //
    // Contrat :
    //

    virtual ~GestionClavier ( );
    // Mode d'emploi :
    //
    // Contrat :
    //

//------------------------------------------------------------------ PRIVE

protected:
//----------------------------------------------------- Méthodes protégées

private:
//------------------------------------------------------- Méthodes privées

protected:
//----------------------------------------------------- Attributs protégés

private:
//------------------------------------------------------- Attributs privés

//---------------------------------------------------------- Classes amies

//-------------------------------------------------------- Classes privées

//----------------------------------------------------------- Types privés

};

//----------------------------------------- Types dépendants de <GestionClavier>

#endif // GESTIONCLAVIER_H
