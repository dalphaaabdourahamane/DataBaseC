//
// Created by DIALLO Alpha Abdoura on 01/11/2015.
//

#ifndef PROJETBASEDONNEE_UTILE_H
#define PROJETBASEDONNEE_UTILE_H

#include <string>

std::string decimalToString(int n);

int stringToDecimal(const char *str);

int indicePremierZero(char tab[],int d, int f);

void copy(char t[],int td,char s[],int ss);

void upper(char chaine[]);

void copyPartie(char newTab[], char oldTab[],int od,int of );

bool findAttVAleur(std::vector<std::string> attributs,std::vector<std::string> valeurs,std::string att,std::string val);

bool sortByIntVal(std::vector<std::string>lhs, std::vector<std::string>  rhs);

bool sortByStrVal(std::vector<std::string>lhs, std::vector<std::string>  rhs);

int findPosition(std::vector<Attribut> attributs,std::string nom);

void gotoxy(int x, int y);

#endif //PROJETBASEDONNEE_UTILE_H
