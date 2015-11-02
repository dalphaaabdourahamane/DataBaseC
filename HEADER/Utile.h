//
// Created by DIALLO Alpha Abdoura on 01/11/2015.
//

#ifndef PROJETBASEDONNEE_UTILE_H
#define PROJETBASEDONNEE_UTILE_H

#include <string>

std::string decimalToBinaryString(int n);

int binaireStringToDecimal(char* str);

int indicePremierZero(char tab[],int d, int f);

void copy(char t[],int td,char s[],int ss);

void upper(char chaine[]);

void copyPartie(char newTab[], char oldTab[],int od,int of );

#endif //PROJETBASEDONNEE_UTILE_H
