//
// Created by DIALLO Alpha Abdoura on 21/10/2015.
//

#ifndef PROJETBASEDONNEE_METADONNEE_H
#define PROJETBASEDONNEE_METADONNEE_H

#include "Structure.h"
#include <vector>

bool ajouteMetaRelation(Relation relation);

int indicePremierZero(char tab[],int d, int f);

void creationRelation();

bool ajouteAttribut(Attribut attribut[], int size);

bool ajouteRelAtt(RelationAttribut relationAttribut[], int size);

void getMetaRalation(Relation* relation,char nomRel[]);

void getAllMetaRalation(std::vector<Relation>* relations);

void getAttributById(Attribut* attribut, int id);

void getRelAttByRel(std::vector<RelationAttribut>* relAtts, Relation relation);

void getAttribut(std::vector<Attribut>* attributs,Relation relation);

void getListebyRel(std::vector<Liste>* listes, Relation relation);

int creationListe(int idRel);

#endif //PROJETBASEDONNEE_METADONNEE_H
