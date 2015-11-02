//
// Created by DIALLO Alpha Abdoura on 22/10/2015.
//

#ifndef PROJETBASEDONNEE_DATABASE_H
#define PROJETBASEDONNEE_DATABASE_H

#include <string>

void initialisation();

void creationRelation();

std::string creationUplet(Relation relation);

bool insertionUplet(std::string uplet,std::vector<Liste> vector1);

bool getUpletByRel(std::vector<std::vector<std::string>> *uplets,Relation relation);

bool getUpletByAtt(std::vector<std::vector<std::string>> *uplets,Relation relation, std::string attr,std::string val) ;

bool deleteUpletByAtt(Relation relation,std::string attr,std::string val);

#endif //PROJETBASEDONNEE_DATABASE_H
