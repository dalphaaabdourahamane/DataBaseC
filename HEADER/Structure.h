//
// Created by DIALLO Alpha Abdoura on 20/10/2015.
//

#ifndef PROJETBASEDONNEE_STRUCTURE_H
#define PROJETBASEDONNEE_STRUCTURE_H

struct Relation {
    int id;
    char nom[10] = {' '};
    int nbAtt;
    int taille=0; //taille d'un enregistrement
};

struct Attribut{
    int id;
    char nom[10] = {' '};
    int type; // 0 entier 1 string
};

struct TmpBlock {
    int bm;
    Relation relation;
    char tmpBlock[512];
};

struct RelationAttribut {
    int idRelation;
    int idAttribut;
    int rang;
};

struct Liste{
    int idRelation;
    int numBlock;
    int idBlockListe;
    int idBipmapListe;
};
#endif //PROJETBASEDONNEE_STRUCTURE_H
