#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <conio.h>
#include <windows.h>
#include <fstream>

#include "HEADER/Structure.h"
#include "HEADER/Metadonnee.h"
#include "HEADER/DataBase.h"
#include "HEADER/Utile.h"
#include "HEADER/Affichage.h"
#define TAILLEPAGE 10
#define TAILLEBLOCK 512

using namespace std;
static int IDREL=0; //id auto increment de la meta relation
static int IDATT=0; //id auto increment des attribut
static int NBLISTE = (((TAILLEPAGE - 6) - (TAILLEPAGE - 6)/ 16)/16)+1; // plus 1 pour arrondire a l'entier supperieur

/*
 * Formule du calcul de la TAILLE de la bitMap (N - N/k)/k ou N=512 et k= taille uplet
 * EX: pour la meta rel(512 -512/34)/34 =14
 */
const int TAILLEMETARELATION = 22; //id,nom,nbatt, taille 10+8*3
const int TAILLEATTRIBUT = 18; //id,nom,domaine  10+8*2
const int TAILLERELATT = 12; //idrel,idatt,rang 8*3
const int TAILLELISTE = 8; //  idrel,numblock 8*2

const int TailleBitMapMetaRelation = (TAILLEBLOCK -TAILLEBLOCK/TAILLEMETARELATION)/TAILLEMETARELATION; //14
const int TailleBitMapMetaAttribut = (TAILLEBLOCK -TAILLEBLOCK/TAILLEATTRIBUT)/TAILLEATTRIBUT; //id,nom,domaine  10+8*2
const int TailleBitMapMetaRelAtt = (TAILLEBLOCK -TAILLEBLOCK/TAILLERELATT)/TAILLERELATT; //idrel,idatt,rang 8*3
const int TailleBitMapMetaListe = (TAILLEBLOCK -TAILLEBLOCK/TAILLELISTE)/TAILLELISTE; //  idrel,numblock 8*2

/*
 * 0 POUR Le bitemap de l'allocation de la memoire
 * 1 pour les meta RELATION
 * 2 et 3 pour les meta ATTRIBUT
 * 4 et 5  pour les meta REL ATT
 * 6 et plus Meta liste
 * la suite est calculer par ( TAILLE_DE_MEMOIRE - 5 )/ 16, 24 qui est la taille (idREL,BLOCK,)
 */
char MEMOIRE[TAILLEPAGE][TAILLEBLOCK];
vector<TmpBlock> TEMPMEMOIRE;

string nomfichier = "C:/Users/DIALLO Alpha Abdoura/ClionProjects/ProjetBaseDonnee/AFDataBase/bd2i.txt";


void getAllMetaRelation(vector<Relation>* relations){

    if(MEMOIRE[0][1]=='1'){
        char METARELATION[TAILLEBLOCK];
        Relation  relation;
        copy(METARELATION,0,MEMOIRE[1],TAILLEBLOCK);

        for (int i =0; i < TailleBitMapMetaRelation ; ++i) {
            if(METARELATION[TAILLEBLOCK-TailleBitMapMetaRelation+i] == '1'){
                // i*TAILLEMETARELATION + 8 est l'indice du block decaler de la case du block, on cope dans nom

                char ent[5];

                copyPartie(ent, METARELATION,i*TAILLEMETARELATION,4);
                relation.id = stringToDecimal(ent);

                copyPartie(relation.nom,METARELATION,i*TAILLEMETARELATION + 4 ,9);

                copyPartie(ent,METARELATION,i*TAILLEMETARELATION + 14 ,4);
                relation.nbAtt = stringToDecimal(ent);

                copyPartie(ent,METARELATION,i*TAILLEMETARELATION + 18 ,4);
                relation.taille = stringToDecimal(ent);
                (*relations).push_back(relation);
            }
        }
    }
}

int main() {

    vector<Relation> temporaire;
    getAllMetaRelation(&temporaire);
    IDREL = temporaire.size();
    for (int i = 0; i < temporaire.size(); ++i) {
        IDATT+=temporaire[i].nbAtt;
    }

    ifstream fichier(nomfichier.c_str(),ios::in);
    if (fichier.fail()) {
        cout << "LE FICHER BDI N EXISTE PAS DONC NOUS ALLONS L INITIALISER POUR VOUS SO KEEP CALM AND WATCH :) "<<endl;
        initialisation();

    } else {
        cout << "LE FICHER BDI EXISTE NOUS ALLONS LE CHARGER  :) "<<endl;
        for (int i = 0; i < TAILLEPAGE; ++i) {
            for (int j = 0; j < TAILLEBLOCK; ++j) {
                fichier >> MEMOIRE[i][j];
            }
        }
        cout << "LE FICHER BDI CHARGER AVEC SUCCES :) "<<endl;

    }
    fichier.close();
    getch();
    affichageMenu();
    system("pause");
    return 0;
}



/*
 * FONTION ET PROCEDURE
 */



void initialisation(){
    for (int j = 0; j < TAILLEBLOCK; ++j) {
        MEMOIRE[0][j] ='0';
    }
    MEMOIRE[0][0] ='1';

    for (int j = 0; j < TAILLEBLOCK; ++j) {
        if(j < TAILLEBLOCK-TailleBitMapMetaRelation) {
            MEMOIRE[1][j] = '#';
        }
        else {
            MEMOIRE[1][j] ='0';
        }
    }

    for (int j = 0; j < TAILLEBLOCK; ++j) {
        if (j < TAILLEBLOCK - TailleBitMapMetaAttribut) {
            MEMOIRE[2][j] = '#';
        }else {
            MEMOIRE[2][j] ='0';
        }
    }
    for (int j = 0; j < TAILLEBLOCK; ++j) {
        if (j < TAILLEBLOCK - TailleBitMapMetaAttribut) {
            MEMOIRE[3][j] = '#';
        }else {
            MEMOIRE[3][j] ='0';
        }
    }

    for (int j = 0; j < TAILLEBLOCK; ++j) {
        if(j < TAILLEBLOCK-TailleBitMapMetaRelAtt){
            MEMOIRE[4][j] ='#';
        }
        else {
            MEMOIRE[4][j] ='0';
        }
    }

    for (int j = 0; j < TAILLEBLOCK; ++j) {
        if (j < TAILLEBLOCK - TailleBitMapMetaRelAtt){
            MEMOIRE[5][j] = '#';
        }
        else {
            MEMOIRE[5][j] ='0';
        }
    }

    for (int j = 0; j < TAILLEBLOCK; ++j) {
        if (j < TAILLEBLOCK - TailleBitMapMetaListe){
            MEMOIRE[6][j] = '#';
        }
        else {
            MEMOIRE[6][j] ='0';
        }
    }
    //liste
    for (int i = 7; i < NBLISTE + 7; ++i) {
        for (int j = 0; j < TAILLEBLOCK; ++j) {
            if (j < TAILLEBLOCK - TailleBitMapMetaListe){
                MEMOIRE[i][j] = '#';
            }
            else {
                MEMOIRE[i][j] ='0';
            }
        }
    }

    for (int i = NBLISTE + 7; i < TAILLEPAGE; ++i) {
        for (int j = 0; j < TAILLEBLOCK; ++j) {
            MEMOIRE[i][j] ='#';
        }
    }
}

void afficheBlock(char tab[]){
    cout<<endl;
    cout<<"[ ";
    for (int i = 0; i < TAILLEBLOCK; ++i) {
        // if((i%100)==0) cout<<endl;
        cout<<" "<<tab[i]  ;
    }
    cout<<" ]";
}

void affichePage(){

    cout<< "----> "<<0;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    afficheBlock(MEMOIRE[0]);
    cout<<endl<<endl;

    cout<< "----> "<<1;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    afficheBlock(MEMOIRE[1]);
    cout<<endl<<endl;

    for (int i = 2; i < 4; ++i) {
        cout<< "----> "<<i;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),9 );
        afficheBlock(MEMOIRE[i]);
        cout<<endl<<endl;
    }

    for (int i = 4; i < 6; ++i) {
        cout<< "----> "<<i;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        afficheBlock(MEMOIRE[i]);
        cout<<endl<<endl;
    }

    cout<< "----> "<<6;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
    afficheBlock(MEMOIRE[6]);
    cout<<endl<<endl;

    for (int i = 7; i < NBLISTE+7; ++i) {
        cout<< "----> "<<i;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
        afficheBlock(MEMOIRE[i]);
        cout<<endl<<endl;
    }

    for (int i = NBLISTE+7; i < TAILLEPAGE; ++i) {
        cout<< "----> "<<i;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
        afficheBlock(MEMOIRE[i]);
        cout<<endl<<endl;
    }

//    for (int i = 0; i < TAILLEPAGE; ++i) {
//        cout<< "----> "<<i;
//        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i+5);
//        afficheBlock(MEMOIRE[i]);
//        cout<<endl<<endl;
//    }
}

void upper(char chaine[]) {
    for (char *p = chaine; *p; p++)
        *p = (char) toupper(*p);
}

string decimalToString(int n){
    string string1;
    string1=static_cast<ostringstream*>( &(ostringstream() << n) )->str();
    for (int i = string1.size(); i < 4; ++i) {
        string1.insert(0,"0");
    }
    return  string1;
}

int stringToDecimal(const char *str) {
    return atoi(str);
}

int indicePremierZero(char tab[],int d, int f){
    int ind = 0;
    for (int i = d; i < f; i++) {

        if(tab[i] == '0' || tab[i]=='#') {return ind;}

        ind++;
    }
    return -1;
}

void copy(char t[],int td,char s[],int ss){
    for (int i = 0; i <ss ; ++i) {
        t[td+i]=s[i];
    }
}

void copyPartie(char newTab[], char oldTab[],int od,int of ){
    int j=0;
    for (int i = od; i < od+of ; ++i) {
        newTab[j] = oldTab[i];
        j++;
    }
    newTab[of]='\0';
}

void creationRelation(){
    Relation relation;
    cout<<endl;
    cin.ignore();
    cout<<"Donner le nom de la RELATION : ";
    cin.get(relation.nom,10);
    cin.ignore();

    relation.id = ++IDREL; // id de la relation
    do {
        cout<<"Donner le nombre ATTRIBUT : ";
        cin >>relation.nbAtt;
    } while (relation.nbAtt <0 && relation.nbAtt >15);
    cin.ignore();
    Attribut attribut[relation.nbAtt];
    RelationAttribut relationAttribut[relation.nbAtt];

    for (int i = 0; i < relation.nbAtt; ++i) {
        attribut[i].id = ++IDATT; //id de l'attribut

        cout <<"Donner le NOM attribut N "<<i+1<< " (str): ";
        cin.get(attribut[i].nom,10);
        cin.ignore();
        do{
            cout <<"Donner le type de l'atttribut " <<attribut[i].nom <<" (int 1| str 2): ";
            cin >>attribut[i].type;
        } while (attribut[i].type!=1 && attribut[i].type!=2);

        if(attribut[i].type == 1){
            relation.taille +=4;
        } else{   relation.taille +=10;}

        relationAttribut[i].idRelation = relation.id;
        relationAttribut[i].idAttribut = attribut[i].id;
        relationAttribut[i].rang= i+1;
        cin.ignore();
    }
    //ajjouer a la memoir
    cout<<"LA METARELATION A ETE CREE "<< relation.nom <<endl;
    ajouteMetaRelation(relation);
    ajouteAttribut(attribut,relation.nbAtt);
    ajouteRelAtt(relationAttribut,relation.nbAtt);
}

bool ajouteMetaRelation(Relation relation){
    char METARELATION[TAILLEBLOCK];
    copy(METARELATION,0,MEMOIRE[1],TAILLEBLOCK);

    if(MEMOIRE[0][1] == '0') MEMOIRE[0][1] = '1'; // on met le bit a 1
    int indiceVide = indicePremierZero(METARELATION,TAILLEBLOCK-TailleBitMapMetaRelation,TAILLEBLOCK);
    if(indiceVide != -1){
        int position = indiceVide*TAILLEMETARELATION;
        char ent[5];

        strcpy(ent, decimalToString(relation.id).c_str());
        for (int i = 0; i < 4; ++i) {
            METARELATION[position++] =ent[i];
        }

        for (int i = 0; i < 10; ++i) {
            METARELATION[position++] =relation.nom[i];
        }

        strcpy(ent, decimalToString(relation.nbAtt).c_str());
        for (int i = 0; i < 4; ++i) {
            METARELATION[position++] =ent[i];
        }

        strcpy(ent, decimalToString(relation.taille).c_str());
        for (int i = 0; i < 4; ++i) {
            METARELATION[position++] =ent[i];
        }

        METARELATION[TAILLEBLOCK-TailleBitMapMetaRelation+indiceVide]='1'; //mettre a jour le bipmap
        copy(MEMOIRE[1],0,METARELATION, TAILLEBLOCK); //recrire le tableau

    } else{
        cout<<"PLUS DE PLACE POUR LA CREATION D UNE RELATION ";
        return false;
    }

    return true;
}

bool getMetaRalation(Relation* relation, char nomRel[]){

    if(MEMOIRE[0][1]=='1'){
        char METARELATION[TAILLEBLOCK];
        copy(METARELATION,0,MEMOIRE[1],TAILLEBLOCK);

        for (int i =0; i < TailleBitMapMetaRelation ; ++i) {
            if(METARELATION[TAILLEBLOCK-TailleBitMapMetaRelation+i] == '1'){
                char nomRelation[10];
                // i*TAILLEMETARELATION + 4 est l'indice du block decaler de la case du block, on cope dans nom
                for (int j = 0; j < 10; ++j) {
                    nomRelation[j] = METARELATION[i*TAILLEMETARELATION + 4 + j];
                }

                if(strcmp(nomRel, nomRelation) == 0){
                    char ent[5];

                    copyPartie(ent, METARELATION,i*TAILLEMETARELATION,4);
                    relation->id = stringToDecimal(ent);

                    copyPartie(relation->nom,METARELATION,i*TAILLEMETARELATION + 4 ,9);

                    copyPartie(ent,METARELATION,i*TAILLEMETARELATION + 14 ,4);
                    relation->nbAtt = stringToDecimal(ent);

                    copyPartie(ent,METARELATION,i*TAILLEMETARELATION + 18 ,4);
                    relation->taille = stringToDecimal(ent);

                    return true;
                }

            }
        }
    }
    return false;
}

bool ajouteAttribut(Attribut attribut[], int size){
    char METAATTRIBUT[TAILLEBLOCK];
    bool block2plain =false; // indique si le deuxieme block es plein
    copy(METAATTRIBUT,0,MEMOIRE[2],TAILLEBLOCK);

    if(MEMOIRE[0][2] == '0') MEMOIRE[0][2] = '1'; // on met le bit a 1
    Attribut attribut1;
    int indiceVide=0;

    for (int x = 0; x < size; ++x) { // indice des attributs
        attribut1 = attribut[x];

        indiceVide = indicePremierZero(METAATTRIBUT,TAILLEBLOCK-TailleBitMapMetaAttribut,TAILLEBLOCK);
        if(indiceVide != -1){

        } else{ // si page plaine
            if(x!=1){ // on a jouter au moins un element a METAATTRIBUT  donc on le reecri avant de changer de page
                copy(MEMOIRE[2],0,METAATTRIBUT, TAILLEBLOCK);
            }
            //on alloue le tableau d indice 3 pour les meta attribut
            MEMOIRE[0][3] = '1';
            copy(METAATTRIBUT,0,MEMOIRE[3],TAILLEBLOCK);
            block2plain = true;
            int nombreInsertionPossible=0;
            for (int i = TAILLEBLOCK-TailleBitMapMetaAttribut; i <TAILLEBLOCK ; ++i) {
                if(METAATTRIBUT[i]=='0') nombreInsertionPossible++;
            }
            if(nombreInsertionPossible< size-x){
                cout << "PLUS DE PLACE POSSIBLE POUR LES ATTRIBUTS DE CETTE RELATION DESOLER";
                return false;
            }
            indiceVide = indicePremierZero(METAATTRIBUT,TAILLEBLOCK-TailleBitMapMetaAttribut,TAILLEBLOCK);
        }
        //Memoire disponible
        int position = indiceVide*TAILLEATTRIBUT;

        char ent[4];
        strcpy(ent, decimalToString(attribut1.id).c_str());
        for (int i = 0; i < 4; ++i) {
            METAATTRIBUT[position++] =ent[i];
        }
        for (int i = 0; i < 10; ++i) {
            METAATTRIBUT[position++] =attribut1.nom[i];
        }
        strcpy(ent, decimalToString(attribut1.type).c_str());
        for (int i = 0; i < 4; ++i) {
            METAATTRIBUT[position++] =ent[i];
        }
        METAATTRIBUT[TAILLEBLOCK-TailleBitMapMetaAttribut+indiceVide]='1'; //mettre a jour le bipmap du block
//        afficheBlock(METAATTRIBUT);
    }

    if(block2plain){
        copy(MEMOIRE[3],0,METAATTRIBUT, TAILLEBLOCK);
    } else {
        copy(MEMOIRE[2],0,METAATTRIBUT, TAILLEBLOCK);
    }
    cout << "ATTRIBUT CREE AVEC SUCCES ";
    return  true;
}

bool ajouteRelAtt(RelationAttribut relationAttribut[], int size){
    char METARELATT[TAILLEBLOCK];
    bool block4plain =false; // indique si le 4iem block es plein
    copy(METARELATT,0,MEMOIRE[4],TAILLEBLOCK);

    if(MEMOIRE[0][4] == '0') MEMOIRE[0][4] = '1'; // on met le bit a 1
    RelationAttribut relationAttribut1;
    int indiceVide=0;

    for (int x = 0; x < size; ++x) { // indice des attributs
        relationAttribut1 = relationAttribut[x];

        indiceVide = indicePremierZero(METARELATT,TAILLEBLOCK-TailleBitMapMetaRelAtt,TAILLEBLOCK);
        if(indiceVide != -1){

        } else{ // si page plaine
            if(x!=1){ // on a jouter au moins un element a METAATTRIBUT  donc on le reecri avant de changer de page
                copy(MEMOIRE[4],0, METARELATT, TAILLEBLOCK);
            }
            //on alloue le tableau d indice 3 pour les meta attribut
            MEMOIRE[0][5] = '1';
            copy(METARELATT,0,MEMOIRE[5],TAILLEBLOCK);
            block4plain = true;
            int nombreInsertionPossible=0;
            for (int i = TAILLEBLOCK-TailleBitMapMetaRelAtt; i <TAILLEBLOCK ; ++i) {
                if(METARELATT[i]=='0') nombreInsertionPossible++;
            }
            if(nombreInsertionPossible< size-x){
                cout << "PLUS DE PLACE POSSIBLE POUR LES ATTRIBUTS DE CETTE RELATION DESOLER";
                return false;
            }
            indiceVide = indicePremierZero(METARELATT,TAILLEBLOCK-TailleBitMapMetaRelAtt,TAILLEBLOCK);
        }
        //Memoire disponible
        int position = indiceVide*TAILLERELATT;

        char ent[4];
        strcpy(ent, decimalToString(relationAttribut1.idRelation).c_str());
        for (int i = 0; i < 4; ++i) {
            METARELATT[position++] =ent[i];
        }

        strcpy(ent, decimalToString(relationAttribut1.idAttribut).c_str());
        for (int i = 0; i < 4; ++i) {
            METARELATT[position++] =ent[i];
        }
        strcpy(ent, decimalToString(relationAttribut1.rang).c_str());
        for (int i = 0; i < 4; ++i) {
            METARELATT[position++] =ent[i];
        }
        METARELATT[TAILLEBLOCK-TailleBitMapMetaRelAtt+indiceVide]='1'; //mettre a jour le bipmap du block
    }

    if(block4plain){
        copy(MEMOIRE[5],0, METARELATT, TAILLEBLOCK);
    } else {
        copy(MEMOIRE[4],0, METARELATT, TAILLEBLOCK);
    }
    return  true;
}

void getAttribut(vector<Attribut>* attributs,Relation relation){

    vector<RelationAttribut> relAtts(0);
    getRelAttByRel(&relAtts, relation);

    // on atrouver toute les lignes de la meta rel Att et elle sont dans relAtts
    for (int i = 0; i < relAtts.size() ; ++i) {
        Attribut attribut;
        getAttributById(&attribut,relAtts[i].idAttribut);
        (*attributs).push_back(attribut);
    }
}

void getAttributById(Attribut* attribut, int id){
    char METAATTRIBUT[TAILLEBLOCK];

    if(MEMOIRE[0][2] == '1'){
        copy(METAATTRIBUT, 0, MEMOIRE[2], TAILLEBLOCK);
        for (int i = 0; i < TAILLEBLOCK - TailleBitMapMetaAttribut; ++i) {
            if(METAATTRIBUT[TAILLEBLOCK - TailleBitMapMetaAttribut + i] == '1'){
                char ent[5];
                copyPartie(ent, METAATTRIBUT,i*TAILLEATTRIBUT ,4);

                if(id == stringToDecimal(ent)){
                    attribut->id = stringToDecimal(ent);

                    copyPartie(attribut->nom, METAATTRIBUT,i*TAILLEATTRIBUT + 4 ,9);

                    copyPartie(ent, METAATTRIBUT,i*TAILLEATTRIBUT + 14 ,4);
                    attribut->type = stringToDecimal(ent);
                    return;
                }
            }
        }
    }

    if(MEMOIRE[0][3] == '1'){
        copy(METAATTRIBUT, 0, MEMOIRE[3], TAILLEBLOCK);
        for (int i = 0; i < TAILLEBLOCK - TailleBitMapMetaAttribut; ++i) {
            if(METAATTRIBUT[TAILLEBLOCK - TailleBitMapMetaAttribut + i] == '1'){
                char ent[5];
                copyPartie(ent, METAATTRIBUT,i*TAILLEATTRIBUT ,4);

                if(id == stringToDecimal(ent)){
                    attribut->id = stringToDecimal(ent);

                    copyPartie(attribut->nom, METAATTRIBUT,i*TAILLEATTRIBUT + 4 ,9);

                    copyPartie(ent, METAATTRIBUT,i*TAILLEATTRIBUT + 14 ,4);
                    attribut->type = stringToDecimal(ent);
                    return;
                }
            }
        }
    }
}

void getRelAttByRel(vector<RelationAttribut>* relAtts, Relation relation){
    int nblignetrouve =0; //compte le nombre de ligne trouve pour eviter d'aller jusqu'a la fin OPTIMISATION
    char METARELATT[TAILLEBLOCK];
    RelationAttribut relationAttribut;

    if(MEMOIRE[0][4]=='1') {
        copy(METARELATT, 0, MEMOIRE[4], TAILLEBLOCK);

        for (int i = 0; i < TailleBitMapMetaRelAtt; ++i) {
            if (METARELATT[TAILLEBLOCK - TailleBitMapMetaRelAtt + i] == '1') {
                // i*TAILLEMETARELATION + 84 est l'indice du block decalé de la case du block
                char ent[5];
                copyPartie(ent, METARELATT, i * TAILLERELATT, 4);
                //compare id de la relation et de la metaRelAtt idrelation
                if (relation.id == stringToDecimal(ent)) {
                    relationAttribut.idRelation = stringToDecimal(ent);

                    copyPartie(ent, METARELATT, i * TAILLERELATT + 4, 4);
                    relationAttribut.idAttribut = stringToDecimal(ent);

                    copyPartie(ent, METARELATT, i * TAILLERELATT + 8, 4);
                    relationAttribut.rang = stringToDecimal(ent);

                    (*relAtts).push_back(relationAttribut);

                    nblignetrouve++;
                    if (nblignetrouve == relation.nbAtt) {
                        return;
                    }
                }

            }
        }
    }
    cout<<endl <<"debug nbaat : "<<nblignetrouve<<endl; //debug

    if(nblignetrouve < relation.nbAtt){ // on verifie dans le block 5
        if (MEMOIRE[0][5]=='1') {
            copy(METARELATT,0,MEMOIRE[5],TAILLEBLOCK);

            for (int i =0; i < TailleBitMapMetaRelAtt ; ++i) {
                if(METARELATT[TAILLEBLOCK-TailleBitMapMetaRelAtt+i] == '1'){
                    // i*TAILLEMETARELATION + 8 est l'indice du block decalé de la case du block

                    char ent[5];
                    copyPartie(ent, METARELATT,i*TAILLERELATT ,4);

                    //compare id de la relation et de la metaRelAtt idrelation
                    if(relation.id == stringToDecimal(ent)){

                        relationAttribut.idRelation = stringToDecimal(ent);

                        copyPartie(ent, METARELATT,i*TAILLERELATT + 4 ,4);
                        relationAttribut.idAttribut = stringToDecimal(ent);

                        copyPartie(ent, METARELATT,i*TAILLERELATT + 8,4);
                        relationAttribut.rang = stringToDecimal(ent);

                        (*relAtts).push_back(relationAttribut);

                        nblignetrouve++;
                        if(nblignetrouve == relation.nbAtt){
                            break;
                        }
                    }

                }
            }
        } else{cout <<"PROBLEME AU NIVEAU DES ATTRIBUT: MOINS D ATTRIBUT QUE CEUX DEFINIS" <<endl; }

    }
}

string creationUplet(Relation relation){
    vector<Attribut> attributs(0);
    string uplet;
    char attname[11];
    getAttribut(&attributs,relation);

    for (int i = 0; i < attributs.size(); ++i) {
        strncpy(attname,attributs[i].nom,10); upper(attname);

        cout<<endl<< attname;
        if (attributs[i].type == 1) {
            cout <<" (int <256) : ";
            cin.clear();
            int val; cin >> val;
            cin.sync();
            uplet.append(decimalToString(val));

        } else {
            cout <<" (str <10) : ";
            string string1;
            cin>>string1;

            int taille = string1.size();
            for (int j = 0; j <10 - taille ; ++j) {
                string1.append(" ");
            }
            cout<<endl <<string1<<endl;
            uplet.append(string1);
        }

    }

    return uplet;
}

bool insertionUplet(string uplet,vector<Liste> vector1, int relationId){
    char UPLET[TAILLEBLOCK];
    int tailleBipmap = (TAILLEBLOCK - TAILLEBLOCK/uplet.size())/uplet.size();

    for (int i = 0; i < vector1.size(); ++i) {
        copy(UPLET,0,MEMOIRE[vector1[i].numBlock],TAILLEBLOCK);

        int ind = indicePremierZero(UPLET,TAILLEBLOCK-tailleBipmap,TAILLEBLOCK);
        if(ind == -1) continue; // pas de place dans ce block

//        cout<<"TAILLE : "<<uplet.size()<<" IND : "<<ind<<endl;
//        cout<<"bitMAP : "<<TAILLEBLOCK-tailleBipmap + ind<<endl;

        for (int j = 0; j < uplet.size(); ++j) {
            UPLET[ind * uplet.size() + j] = uplet[j];
        }

        UPLET[TAILLEBLOCK-tailleBipmap + ind] ='1';
        copy(MEMOIRE[vector1[i].numBlock],0,UPLET,TAILLEBLOCK);
        return true;
    }
    //tous les blocks sont plein
    int idNewBlock = creationListe(relationId);
    if (idNewBlock != -1) {
        copy(UPLET,0,MEMOIRE[idNewBlock],TAILLEBLOCK);
        for (int i = 0; i < uplet.size(); ++i) {
            UPLET[i] = uplet[i];
        }
        UPLET[TAILLEBLOCK-tailleBipmap] ='1';
        copy(MEMOIRE[idNewBlock],0,UPLET,TAILLEBLOCK);
        return true;
    } else {
        cout<<endl<<"WARNING : PLUS DE PLACE POUR L INSERTION D UPLET "<<endl;
    }

    return false;
}

bool getUpletByRel(vector<vector<string>> *uplets,Relation relation){
    vector<Liste> listeUpletRel(0);
    vector<Attribut> attributs(0);
//    vector<vector<string>> uplets(0); //liste attribut

    getListebyRel(&listeUpletRel, relation);
    getAttribut(&attributs,relation);

    char UPLET[TAILLEBLOCK];
    int tailleBipmap = (TAILLEBLOCK - TAILLEBLOCK/relation.taille)/relation.taille;

    for (int i = 0; i < listeUpletRel.size(); ++i) {
        copy(UPLET,0,MEMOIRE[listeUpletRel[i].numBlock],TAILLEBLOCK);

        for (int j = 0; j < tailleBipmap; ++j) {
            //parcour tous le block et on cherche les uplets
            if(UPLET[TAILLEBLOCK - tailleBipmap + j] == '0' || UPLET[TAILLEBLOCK - tailleBipmap + j] == '#') continue; //Uplet vide ou supprimé
            int indUplet = j *relation.taille;
            //pour la lecture des données
            vector<string> uplet;

            for (int k = 0; k < attributs.size(); ++k) {
                string champ;
                if(attributs[k].type == 1){
                    //entier
                    string ent;
                    for (int l = 0; l < 4; ++l) {
                        ent.push_back(UPLET[indUplet]);
                        indUplet++;
                    }
                    champ= static_cast<ostringstream*>( &(ostringstream() << stringToDecimal((char *) ent.c_str())) )->str();
//                    cout<<endl<<" int: "<<champ; //debug
                } else{
                    //str
                    for (int l = 0; l < 10; ++l) {
                        champ.push_back(UPLET[indUplet]);
                        indUplet++;
                    }
//                    cout<<endl<<" str: "<<champ; //debug
                }
                uplet.push_back(champ);

            }//forAttribut

            (*uplets).push_back(uplet);

        }//forbitmap
    }//forliste
    return true;
}

bool getUpletByAtt(vector<vector<string>> *uplets, Relation relation, string attr,string val){
    vector<Liste> listeUpletRel(0);
    vector<Attribut> attributs(0);
//    vector<vector<string>> uplets(0); //liste attribut

    getListebyRel(&listeUpletRel, relation);
    getAttribut(&attributs,relation);

    for (int i = 0; i < attributs.size(); ++i) {
        if(attr.compare(attributs[i].nom) == 0) break;
        if(i==attributs.size()-1) return false;
    }

    char UPLET[TAILLEBLOCK];
    int tailleBipmap = (TAILLEBLOCK - TAILLEBLOCK/relation.taille)/relation.taille;

    for (int i = 0; i < listeUpletRel.size(); ++i) {
        copy(UPLET,0,MEMOIRE[listeUpletRel[i].numBlock],TAILLEBLOCK);
//        cout<<endl<<"id block: "<<listeUpletRel[i].numBlock;//debug

        for (int j = 0; j < tailleBipmap; ++j) {
            //parcour tous le block et on cherche les uplets
            if(UPLET[TAILLEBLOCK - tailleBipmap + j] == '0' || UPLET[TAILLEBLOCK - tailleBipmap + j] == '#') continue; //Uplet vide ou supprimé
//            cout<<endl<<"id block: "<<j; //debug

            int indUplet = j *relation.taille;
            //pour la lecture des données
            vector<string> uplet;
            bool upletValide = false;

            for (int k = 0; k < attributs.size(); ++k) {

                string champ;
                if(attributs[k].type == 1){
                    //entier
                    for (int l = 0; l < 4; ++l) {
                        champ.push_back(UPLET[indUplet]);
                        indUplet++;
                    }

                    if((attr.compare(attributs[k].nom) == 0) &&
                       (atoi(val.c_str()) == stringToDecimal((char *) champ.c_str())) ){
                        upletValide = true;
                    }
                } else{
                    //str
                    for (int l = 0; l < 10; ++l) {
                        champ.push_back(UPLET[indUplet]);
                        indUplet++;
                    }
                    champ.erase(remove(champ.begin(), champ.end(), ' '), champ.end());

                    if((attr.compare(attributs[k].nom) == 0) && (val.compare(champ)==0) ){

                        upletValide = true;
                    }
//                    cout<<endl<<" str: "<<champ; //debug
                }


                uplet.push_back(champ);

            }//forAttribut

            if (upletValide) {
                (*uplets).push_back(uplet);
            }

        }//forbitmap
    }//forliste
    return false;
}

bool findAttVAleur(vector<string> attributs,vector<string> valeurs, string att,string val){
    att.erase(remove(att.begin(), att.end(), ' '), att.end());
    val.erase(remove(val.begin(), val.end(), ' '), val.end());

    for (int i = 0; i < attributs.size(); ++i) {
        attributs[i].erase(remove(attributs[i].begin(), attributs[i].end(), ' '), attributs[i].end());

        if((att.compare(attributs[i]) == 0) && (val.compare(valeurs[i])==0) ){
            return true;
        }
    }
    return false;
}

bool getUpletByAttOR(vector<vector<string>> *uplets, Relation relation, vector<string> atts,vector<string> vals){
    vector<Liste> listeUpletRel(0);
    vector<Attribut> attributs(0);
    getListebyRel(&listeUpletRel, relation);
    getAttribut(&attributs,relation);

    char UPLET[TAILLEBLOCK];
    int tailleBipmap = (TAILLEBLOCK - TAILLEBLOCK/relation.taille)/relation.taille;

    for (int i = 0; i < listeUpletRel.size(); ++i) {
        copy(UPLET,0,MEMOIRE[listeUpletRel[i].numBlock],TAILLEBLOCK);

        for (int j = 0; j < tailleBipmap; ++j) {
            //parcour tous le block et on cherche les uplets
            if(UPLET[TAILLEBLOCK - tailleBipmap + j] == '0' || UPLET[TAILLEBLOCK - tailleBipmap + j] == '#') continue; //Uplet vide ou supprimé
            int indUplet = j *relation.taille;
            //pour la lecture des données
            vector<string> uplet;
            bool upletValide = false;

            for (int k = 0; k < attributs.size(); ++k) {

                string champ;
                if(attributs[k].type == 1){
                    //entier
                    for (int l = 0; l < 4; ++l) {
                        champ.push_back(UPLET[indUplet]);
                        indUplet++;
                    }
                    string valeur= static_cast<ostringstream*>( &(ostringstream() <<
                                                                  stringToDecimal((char *) champ.c_str())) )->str();

                    if(findAttVAleur(atts,vals,attributs[k].nom,valeur)){
                        upletValide = true;
                    }
                } else{
                    //str
                    for (int l = 0; l < 10; ++l) {
                        champ.push_back(UPLET[indUplet]);
                        indUplet++;
                    }
                    champ.erase(remove(champ.begin(), champ.end(), ' '), champ.end());
                    if( findAttVAleur(atts,vals,attributs[k].nom,champ) ){
                        upletValide = true;
                    }
                }
                uplet.push_back(champ);

            }//forAttribut

            if (upletValide) {
                (*uplets).push_back(uplet);
            }

        }//forbitmap
    }//forliste
    return false;
}

bool getUpletByAttAND(vector<vector<string>> *uplets, Relation relation, vector<string> atts,vector<string> vals){
    vector<Liste> listeUpletRel(0);
    vector<Attribut> attributs(0);
    getListebyRel(&listeUpletRel, relation);
    getAttribut(&attributs,relation);

    char UPLET[TAILLEBLOCK];
    int tailleBipmap = (TAILLEBLOCK - TAILLEBLOCK/relation.taille)/relation.taille;

    for (int i = 0; i < listeUpletRel.size(); ++i) {
        copy(UPLET,0,MEMOIRE[listeUpletRel[i].numBlock],TAILLEBLOCK);

        for (int j = 0; j < tailleBipmap; ++j) {
            //parcour tous le block et on cherche les uplets
            if(UPLET[TAILLEBLOCK - tailleBipmap + j] == '0' || UPLET[TAILLEBLOCK - tailleBipmap + j] == '#') continue; //Uplet vide ou supprimé
            int indUplet = j *relation.taille;
            //pour la lecture des données
            vector<string> uplet;
            int upletValide = 0;

            for (int k = 0; k < attributs.size(); ++k) {

                string champ;
                if(attributs[k].type == 1){
                    //entier
                    for (int l = 0; l < 4; ++l) {
                        champ.push_back(UPLET[indUplet]);
                        indUplet++;
                    }
                    string valeur= static_cast<ostringstream*>( &(ostringstream() <<
                                                                  stringToDecimal((char *) champ.c_str())) )->str();

                    if(findAttVAleur(atts,vals,attributs[k].nom,valeur)){
                        upletValide ++;
                    }
                } else{
                    //str
                    for (int l = 0; l < 10; ++l) {
                        champ.push_back(UPLET[indUplet]);
                        indUplet++;
                    }
                    champ.erase(remove(champ.begin(), champ.end(), ' '), champ.end());
                    if( findAttVAleur(atts,vals,attributs[k].nom,champ) ){
                        upletValide ++;
                    }
                }
                uplet.push_back(champ);

            }//forAttribut

            if (atts.size() == upletValide) {
                (*uplets).push_back(uplet);
            }

        }//forbitmap
    }//forliste
    return false;
}

int creationListe(int idRel){

    char METALISTE[TAILLEBLOCK];

    for (int i = 7; i <NBLISTE+7 ; ++i) {
        copy(METALISTE,0,MEMOIRE[i],TAILLEBLOCK);
        int indiceBM = indicePremierZero(METALISTE,TAILLEBLOCK-TailleBitMapMetaListe,TAILLEBLOCK);
        if(indiceBM == -1){
            continue;
        } else{
            int indiceB = indicePremierZero(MEMOIRE[0],7+NBLISTE,TAILLEPAGE); // hors de la taille de la page on ne peu pas ecrire

//            le MEMOIRE[0][0] n'est interresant
            if(indiceB == -1){
                cout <<endl<<"PLUS DE MEMOIRE POUR INSERE UN UPLET "<<endl;
                return -1;
            }

            indiceB+=(7+NBLISTE);
            string ent1;
            string ent2;

            ent1.append(decimalToString(idRel).c_str());
            ent2.append(decimalToString(indiceB).c_str());

            for (int j = 0; j < 4; ++j) {
                METALISTE[indiceBM*TAILLELISTE + j] = ent1[j];
                METALISTE[indiceBM*TAILLELISTE + 4 + j] = ent2[j];
            }

            MEMOIRE[0][i] = '1';
            MEMOIRE[0][indiceB] = '1';
            METALISTE[TAILLEBLOCK - TailleBitMapMetaListe + indiceBM] = '1';
//            cout<<endl<<" idliste : "<<indiceBM<<endl;
            copy(MEMOIRE[i],0,METALISTE,TAILLEBLOCK);

            return indiceB;
        }
    }//for

    return -1;
}

void getListebyRel(vector<Liste>* listes, Relation relation){
    char METALISTE[TAILLEBLOCK];
    Liste lis;
    for (int i = 7; i <NBLISTE+7 ; ++i) {
        if(MEMOIRE[0][i] == '0') continue;
        copy(METALISTE, 0, MEMOIRE[i], TAILLEBLOCK);

//        cout<<endl<<"ID BLOCK LISTE : "<<i; //debug

        for (int j = 0; j < TailleBitMapMetaListe; ++j) {
            if(METALISTE[TAILLEBLOCK-TailleBitMapMetaListe + j] == '0') continue;

//            cout<<"ID BITMAP : "<<i<<endl; //debug

            char ent[5];
            copyPartie(ent, METALISTE,j*TAILLELISTE ,4);

            int id = stringToDecimal(ent);
            if (relation.id == id) {
                lis.idRelation = id;

                copyPartie(ent, METALISTE,j*TAILLELISTE+4 ,4);
                lis.numBlock = stringToDecimal(ent);
                lis.idBlockListe=i;
                lis.idBipmapListe= TAILLEBLOCK - TailleBitMapMetaListe + j;

                (*listes).push_back(lis);
            }
        }
    }
}

bool deleteUpletByAtt(Relation relation,string attr,string val){
    vector<Liste> listeUpletRel(0);
    vector<Attribut> attributs(0);

    getListebyRel(&listeUpletRel, relation);
    getAttribut(&attributs,relation);

    for (int i = 0; i < attributs.size(); ++i) {
        if(attr.compare(attributs[i].nom) == 0) break;
        if(i==attributs.size()-1) return false;
    }

    char UPLET[TAILLEBLOCK];
    int tailleBipmap = (TAILLEBLOCK - TAILLEBLOCK/relation.taille)/relation.taille;

    for (int i = 0; i < listeUpletRel.size(); ++i) {
        copy(UPLET,0,MEMOIRE[listeUpletRel[i].numBlock],TAILLEBLOCK);

        for (int j = 0; j < tailleBipmap; ++j) {
            //parcour tous le block et on cherche les uplets
            if(UPLET[TAILLEBLOCK - tailleBipmap + j] == '0' || UPLET[TAILLEBLOCK - tailleBipmap + j] == '#') continue; //Uplet vide ou supprimé
            int indUplet = j *relation.taille;
            //pour la lecture des données

            for (int k = 0; k < attributs.size(); ++k) {

                string champ;
                if(attributs[k].type == 1){
                    //entier
                    for (int l = 0; l < 4; ++l) {
                        champ.push_back(UPLET[indUplet]);
                        indUplet++;
                    }

                    if((attr.compare(attributs[k].nom) == 0) &&
                       (atoi(val.c_str()) == stringToDecimal((char *) champ.c_str())) ){
                        UPLET[TAILLEBLOCK - tailleBipmap + j] ='0';

//                        cout<<endl<<" AP: "<<UPLET[TAILLEBLOCK - tailleBipmap + j]; //debug
                        break;
                    }
                } else{
                    //str
                    for (int l = 0; l < 10; ++l) {
                        champ.push_back(UPLET[indUplet]);
                        indUplet++;
                    }
                    champ.erase(std::remove(champ.begin(), champ.end(), ' '), champ.end());

                    if((attr.compare(attributs[k].nom) == 0) && (val.compare(champ)==0) ){
                        UPLET[TAILLEBLOCK - tailleBipmap + j] ='0';
                        break;
                    }
//                    cout<<endl<<" str: "<<champ; //debug
                }

            }//forAttribut

        }//forbitmap
        /*
         * au cas il n ya aplus de uplet
         */
        if(indicePremierZero(UPLET,TAILLEBLOCK-tailleBipmap,TAILLEBLOCK) == -1){
            MEMOIRE[0][listeUpletRel[i].numBlock] = '0';
            MEMOIRE[listeUpletRel[i].idBlockListe][listeUpletRel[i].idBipmapListe] = '0';
        }
        copy(MEMOIRE[listeUpletRel[i].numBlock],0,UPLET,TAILLEBLOCK);
    }//forliste

    return true;
}

bool updateUpletByRel(Relation relation,vector<string> attFiltres,vector<string> valfiltres,vector<string> newAtts,vector<string> newVals){

    vector<Liste> listeUpletRel(0);
    vector<Attribut> attributs(0);
    getListebyRel(&listeUpletRel, relation);
    getAttribut(&attributs,relation);

    char UPLET[TAILLEBLOCK];
    int tailleBipmap = (TAILLEBLOCK - TAILLEBLOCK/relation.taille)/relation.taille;

    for (int i = 0; i < listeUpletRel.size(); ++i) {
        copy(UPLET,0,MEMOIRE[listeUpletRel[i].numBlock],TAILLEBLOCK);

        for (int j = 0; j < tailleBipmap; ++j) {
            //parcour tous le block et on cherche les uplets

            if(UPLET[TAILLEBLOCK - tailleBipmap + j] == '0' || UPLET[TAILLEBLOCK - tailleBipmap + j] == '#') continue; //Uplet vide ou supprimé
            int indUplet = j *relation.taille;
            //pour la lecture des données
            vector<string> uplet;
            int upletValide = 0;

            for (int k = 0; k < attributs.size(); ++k) {

                string champ;
                if(attributs[k].type == 1){
                    //entier
                    for (int l = 0; l < 4; ++l) {
                        champ.push_back(UPLET[indUplet]);
                        indUplet++;
                    }
                    string valeur= static_cast<ostringstream*>( &(ostringstream() <<
                                                                  stringToDecimal((char *) champ.c_str())) )->str();

                    if(findAttVAleur(attFiltres,valfiltres,attributs[k].nom,valeur)){
                        upletValide ++;
                    }
                } else{
                    //str
                    for (int l = 0; l < 10; ++l) {
                        champ.push_back(UPLET[indUplet]);
                        indUplet++;
                    }
                    champ.erase(remove(champ.begin(), champ.end(), ' '), champ.end());
                    if( findAttVAleur(attFiltres,valfiltres,attributs[k].nom,champ) ){
                        upletValide ++;
                    }
                }
                uplet.push_back(champ);

            }//forAttribut

            if (attFiltres.size() == upletValide) {
                indUplet = j *relation.taille;

                //parcour attibut et modifier les octets concerné
                for (int k = 0; k < attributs.size(); ++k) {

                    if(attributs[k].type == 1){
                        //entier
                        int pos = find(newAtts.begin(), newAtts.end(), string(attributs[k].nom)) - newAtts.begin(); //uplet k
//                        cout<<endl<<" newAtts.size() "<<newAtts.size(); //debug
                        if(pos >= newAtts.size()){
                            indUplet+=4;
                            continue;
                        }
                        //k < newAtts.size
                        string newIntValeur = decimalToString(atoi(newVals[pos].c_str()));
//                        cout<<endl<<" newIntValeur "<<newIntValeur<<endl; //debug

                        for (int l = 0; l < 4; ++l) {
                            UPLET[indUplet] = newIntValeur[l];
                            indUplet++;
                        }

                    } else{
                        //str
                        int pos = find(newAtts.begin(), newAtts.end(), string(attributs[k].nom)) - newAtts.begin(); //uplet k
                        //k < newAtts.size
                        if(pos >= newAtts.size()) {
                            indUplet+=10;
                            continue;
                        }

                        string newStrtValeur = newVals[pos];

                        for (int l = 0; l < 10; ++l) {
                            UPLET[indUplet] = newStrtValeur[l];
                            indUplet++;
                        }
                        // champ.erase(remove(champ.begin(), champ.end(), ' '), champ.end());
                    }

                }//forAttribut
            }//fin if----------------------------------------------------------------------------------------------
        }//forbitmap
        copy(MEMOIRE[listeUpletRel[i].numBlock],0,UPLET,TAILLEBLOCK);
    }//forliste

    return false;
}
// Sort Container by name function
bool sortByIntVal(vector<string>lhs, vector<string>  rhs) { return atoi(lhs.at(0).c_str()) < atoi(rhs.at(0).c_str()); }
bool sortByStrVal(vector<string>lhs, vector<string>  rhs) { return lhs.at(0) < rhs.at(0); }

int findPosition(vector<Attribut> attributs,string nom){
    for (int i = 0; i < attributs.size(); ++i) {
        if(string(attributs[i].nom).compare(nom) == 0) return i;
    }
    return -1;
}

bool innerJoin(vector<vector<string>>* resultats, Relation relation1,Relation relation2,string att1,string att2){
    vector<vector<string>> uplets1, uplets2;
    vector<Attribut> attributs1, attributs2;

    getUpletByRel(&uplets1,relation1);
    getUpletByRel(&uplets2,relation2);

    getAttribut(&attributs1, relation1);
    getAttribut(&attributs2, relation2);

    int pos1 = findPosition(attributs1, att1); //uplet k
    int pos2 = findPosition(attributs2, att2); //uplet k
    if(pos1 < 0) {
        cout<<endl<<"PROBLEME LE PREMIER ATTRIBUT N HESITE PAS"<<endl;
        return false;
    }
    if(pos2 < 0) {
        cout<<endl<<"PROBLEME LE DEUXIME ATTRIBUT N HESITE PAS"<<endl;
        return false;
    }
    if (attributs2[pos2].type != attributs1[pos1].type){
        cout<<endl<<"PROBLEME LES DEUX ATTRIBUTS SONT DE TYPE DIFFERENTS"<<endl;
        return false;
    }
    cout<<endl<<"ChekPoint 1 "<<pos1<<" "<<pos2; //debug

    for (int i = 0; i < uplets1.size(); ++i) {
        swap( uplets1[i] [0], uplets1[i] [pos1] );
    }
    for (int i = 0; i < uplets2.size(); ++i) {
        swap( uplets2[i] [0], uplets2[i] [pos2] );
    }


    sort(uplets1.begin(), uplets1.end(), attributs1[pos1].type==1 ? sortByIntVal : sortByStrVal);
    sort(uplets2.begin(), uplets2.end(), attributs2[pos2].type==1 ? sortByIntVal : sortByStrVal);

    int i=0,j=0;

    while (i < uplets1.size() && j < uplets2.size()){
        cout<<endl<<"Boucle-> 1 "<<uplets1[i][0]<<"  "<<uplets2[j][0]<<" **i : "<<i<<"**j : "<<j; //debug

        if(uplets1[i][0] > uplets2[j][0]){
            j++;
        } else{
            if (uplets1[i][0] < uplets2[j][0]){
                i++;
            } else{
                //egalité
                cout<<endl<<"egalite 2 "<<uplets1[i][0]<<"  "<<uplets2[j][0]<<" taille"<<uplets1[i].size() + uplets2[j].size(); //debug
                vector<string> vecTmp;

                vecTmp.insert(vecTmp.end(),uplets1[i].begin(),uplets1[i].end());
                vecTmp.insert(vecTmp.end(),uplets2[j].begin(),uplets2[j].end());
                resultats->push_back(vecTmp);
                vecTmp.clear();
                //produire le reste

                while(++j < uplets2.size() && uplets1[i][0] == uplets2[j][0]){

                    vecTmp.insert(vecTmp.end(),uplets1[i].begin(),uplets1[i].end());
                    vecTmp.insert(vecTmp.end(),uplets2[j].begin(),uplets2[j].end());
                    resultats->push_back(vecTmp);
                    cout<<endl<<"parcour t "<<vecTmp.size();
                    vecTmp.clear();
                }

                cout<<endl<<"ChekPoint *** "<<i<<" "<<j;
                if(j==uplets2.size()) j--;
                while(++i < uplets1.size() && uplets1[i][0] == uplets2[j][0]){

                    vecTmp.insert(vecTmp.end(),uplets1[i].begin(),uplets1[i].end());
                    vecTmp.insert(vecTmp.end(),uplets2[j].begin(),uplets2[j].end());
                    resultats->push_back(vecTmp);
                    cout<<endl<<"parcour t "<<vecTmp.size();
                    vecTmp.clear();
                }

            }

        }
    }

    cout<<endl<<"Taille "<<resultats->size()<< " : ";
    for (int k = 0; k < resultats->size(); ++k) {
        cout<<endl<<"UPLET "<<k<<endl;
        cout<<" ->TU : "<<(*resultats)[k].size();
        for (int l = 0; l <(*resultats)[k].size() ; ++l) {
            cout<< "   "<<(*resultats)[k][l];
        }
        cout<<endl;
    }

    return true;
}

bool projetion(vector<vector<string>> uplets, vector<string> atts, Relation relation){

    getUpletByRel(&uplets,relation);
    vector<Attribut> attributs(0);
    getAttribut(&attributs,relation);
    vector<vector<string>> resultats(0);


    for (int i = 0; i < uplets.size(); ++i) {

        vector<string> temps(0);
        int idAtt = 0;
        for (int k = 0; k < atts.size(); ++k) {
            for (int j = 0; j < uplets[i].size(); ++j) { // uplet[i].size == attribut.size
                if(atts[idAtt].compare(attributs[j].nom)==0){
                    temps.push_back(uplets[i][j]);
                }
            }
            idAtt++;
        }
        resultats.push_back(temps);
    }

    cout<<endl<<"Taille "<<resultats.size()<< " : ";
    for (int k = 0; k < resultats.size(); ++k) {
        cout<<endl<<"UPLET "<<k<<endl;
        cout<<" ->TU : "<<resultats[k].size();
        for (int l = 0; l <resultats[k].size() ; ++l) {
            cout<< "   "<<resultats[k][l];
        }
        cout<<endl;
    }
    return true;
}


bool creationBlockTemp(vector<vector<string>> uplets,Relation relation){
    char tmp[TAILLEBLOCK];
    int bm = (TAILLEBLOCK - TAILLEBLOCK/relation.taille)/relation.taille;
    bm=TAILLEBLOCK - bm;

    for (int i = 0; i <TAILLEBLOCK ; ++i) {
        if (i<bm) {
            tmp[i] ='#';
        } else {
            tmp[i] = '0';
        }
    }
    int k=0,p=bm;
    for (int i = 0; i < uplets.size(); ++i) {
        for (int j = 0; j <  uplets[i].size(); ++j) {
            for (int l = 0; l < uplets[i][j].size(); ++l) {
                tmp[k++] = uplets[i][j][l];
            }
        }
        tmp[bm++]='1';
    }

    TEMPMEMOIRE.push_back(TmpBlock{bm,tmp});

}


/*
 *
 */

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int affichageMenuOperation(){

    LabelMenu: //lable
    system("cls");
    int choixMenu;

    cout<<" _______________MENU OPERATION_______________"<<endl;
    cout<<"|                                            |"<<endl;
    cout<<"|            1.INSERTION                     |"<<endl;
    cout<<"|            2.SELECTION                     |"<<endl;
    cout<<"|            3.SUPRESSION                    |"<<endl;
    cout<<"|            4.MISE A JOUR                   |"<<endl;
    cout<<"|            5.JOINTURE                      |"<<endl;
    cout<<"|            6.PROJECTION                    |"<<endl;
    cout<<"|            7.RETOUR                        |"<<endl;
    cout<<"|____________________________________________|"<<endl;

    LabelchoixMenu:   cout<<"\nVotre choix?"<<endl;
    cin>>choixMenu;
    switch (choixMenu)
    {
        case 1: {

            string nomRelation;
            int cpt=0;
            Relation relation;
            vector<Liste> listes(0);
            LableInsertion: system("cls");

            cout << "_________________INSERTION__________________";
            cout<<endl<<"DONNER LE NOM DE LA RELATION : ";
            cin.ignore(); cin>>nomRelation;
            if(! getMetaRalation(&relation, (char *) nomRelation.c_str()) ){
                cout<<endl<<"ERREUR CETTE RELATION N HESITE PAS"<<endl;
                if(4 == cpt++)goto LabelMenu;
                goto LableInsertion;
            }
//            creationListe(relation.id);//!! averifier
            getListebyRel(&listes, relation);
            insertionUplet(creationUplet(relation), listes,relation.id);
            getch();
            goto LabelMenu;

            break;
        }
        case 2:
            system("cls");
            cout<<"_______________SELECTION____________________"<<endl;
            affichageMenuOperationSelection();
            break;
        case 3:{

            string nomRelation,attribut,valeur;
            int cpt=0;
            Relation relation;
            LableSuppression: system("cls");

            cout << "_________________SUPPRESSION__________________";
            cout<<endl<<"DONNER LE NOM DE LA RELATION : ";
            cin.ignore(); cin>>nomRelation;
            if(! getMetaRalation(&relation, (char *) nomRelation.c_str()) ){
                cout<<endl<<"ERREUR CETTE RELATION N HESITE PAS"<<endl;
                if(4 == cpt++)goto LabelMenu;
                goto LableSuppression;
            }

            cout<<endl<<"DONNER CHAMP DE LA RELATION A SELECTIONNER : ";
            cin.ignore();
            cin>>attribut;
            cout<<endl<<"DONNER LA VALEUR DU CHAMP : ";
            cin.ignore();
            cin>>valeur;

            deleteUpletByAtt(relation,attribut,valeur);
            getch();
            goto LabelMenu;

            break;
        }
        case 4:{
            string nomRelation,attribut,valeur;
            int cpt = 0;
            Relation relation;
            vector<string> listeAttrs,newListeAttrs;
            vector<string> listeValeurs,newListeValeurs;

            LableMAJ:system("cls");

            cout << "______________MISE A JOUR________________  ";
            cout<<endl<<"DONNER LE NOM DE LA RELATION : ";
            cin.ignore(); cin>>nomRelation;
            if(! getMetaRalation(&relation, (char *) nomRelation.c_str()) ){
                cout<<endl<<"ERREUR CETTE RELATION N HESITE PAS"<<endl;
                if(4 == cpt++)goto LabelMenu;
                goto LableMAJ;
            }

            string rep;
            do {
                cout<<endl<<"DONNER CHAMP DE LA RELATION A SELECTIONNER : ";
                cin.ignore();
                cin>>attribut;
                listeAttrs.push_back(attribut);
                cout<<endl<<"DONNER LA VALEUR DU CHAMP : ";
                cin.ignore();
                cin>>valeur;
                listeValeurs.push_back(valeur);
                cout<<endl<<"VOULEZ VOUS CONTINUEZ (o/n):  ";
                cin.ignore();
                cin>>rep;
            } while (rep.compare("n")!=0);
            do {
                cout<<endl<<"DONNER CHAMP DE LA RELATION A MODIFIER : ";
                cin.ignore();
                cin>>attribut;
                newListeAttrs.push_back(attribut);
                cout<<endl<<"DONNER NOUVELLE LA VALEUR DU CHAMP : ";
                cin.ignore();
                cin>>valeur;
                newListeValeurs.push_back(valeur);
                cout<<endl<<"VOULEZ VOUS CONTINUEZ (o/n):  ";
                cin.ignore();
                cin>>rep;
            } while (rep.compare("n")!=0);

            updateUpletByRel(relation,listeAttrs,listeValeurs,newListeAttrs,newListeValeurs);

            goto LabelMenu;
            break;
        }
        case 5: {
            string nomRelation1,nomRelation2,attribut1, attribut2;
            int cpt = 0;
            Relation relation1, relation2;
            vector<vector<string>> resultats;

            LableJointure:system("cls");

            cout << "_________________JOINTURE___________________";
            cout<<endl<<"DONNER LE NOM DE LA RELATION : ";
            cin.ignore(); cin>>nomRelation1;
            if(! getMetaRalation(&relation1, (char *) nomRelation1.c_str()) ){
                cout<<endl<<"ERREUR CETTE RELATION N HESITE PAS"<<endl;
                if(4 == cpt++)goto LabelMenu;
                goto LableJointure;
            }
            cout<<endl<<"DONNER LE NOM DE LA DEUXIEME RELATION : ";
            cin.ignore(); cin>>nomRelation2;
            if(! getMetaRalation(&relation2, (char *) nomRelation2.c_str()) ){
                cout<<endl<<"ERREUR CETTE RELATION N HESITE PAS"<<endl;
                if(4 == cpt++)goto LabelMenu;
                goto LableJointure;
            }
            cout<<endl<<"DONNER LE NOM DU PREMIER ATTRIBUT : ";
            cin.ignore(); cin>>attribut1;
            cout<<endl<<"DONNER LE NOM DU DEUXIEME ATTRIBUT : ";
            cin.ignore(); cin>>attribut2;
            innerJoin(&resultats, relation1,relation2,"age","age");
            break;
        }
        case 6: {
            string nomRelation,tmp,rep;
            int cpt = 0;
            Relation relation;
            vector<vector<string>> resultats;
            vector<string> attributs;

            LableProjection:system("cls");

            cout << "________________PROJECTION__________________";
            cout<<endl<<"DONNER LE NOM DE LA PREMIERE RELATION : ";
            cin.ignore(); cin>>nomRelation;
            if(! getMetaRalation(&relation, (char *) nomRelation.c_str()) ){
                cout<<endl<<"ERREUR CETTE RELATION N HESITE PAS"<<endl;
                if(4 == cpt++)goto LabelMenu;
                goto LableProjection;
            }

            cpt = 0;
            do {
                cout<<endl<<"DONNER LE NOM DE L ATTRIBUT : ";
                cin.ignore();
                cin>>tmp;
                attributs.push_back(tmp);
                cout<<endl<<"VOULEZ VOUS CONTINUEZ (o/n) : ";
                cin.ignore();
                cin>>rep;
            } while (cpt!= relation.nbAtt && rep.compare("n") !=0);

            projetion(resultats,attributs,relation);
            getch();
            goto LabelMenu;
            break;
        }
        case 7:
            cout<<"RETOUR"<<endl;
            affichageMenu();
            break;
        default:
            cout<<"Vous ne ferez rien du tout!";
            goto LabelchoixMenu;
            break;
    }
    cout<<endl;
    return choixMenu;

}

int affichageMenuAffichage(){

    LabelMenu: system("cls");
    int choixMenu;

    cout<<" _______________MENU AFFICHAGE_______________"<<endl;
    cout<<"|                                            |"<<endl;
    cout<<"|            1.RELATIONS                     |"<<endl;
    cout<<"|            2.UPLETS RELATION               |"<<endl;
    cout<<"|            3.BLOCK                         |"<<endl;
    cout<<"|            4.MEMOIRE                       |"<<endl;
    cout<<"|            5.RETOUR                        |"<<endl;
    cout<<"|____________________________________________|"<<endl;

    LabelchoixMenu:   cout<<"\nVotre choix?"<<endl;
    cin>>choixMenu;
    switch (choixMenu)
    {
        case 1: {
            system("cls");
            vector<Relation> relations;
            cout << "_________________RELATIONS__________________";
            getAllMetaRelation(&relations);
            if(relations.empty()){
                cout<<endl<<"PAS DE RELATION";
            }
            for (int i = 0; i < relations.size(); ++i) {
                cout<<endl<<"--> ID : "<<relations[i].id<<" NOM : "<<relations[i].nom<<" NB ATT : "<<relations[i].nbAtt<<" TAILLE : "<<relations[i].taille<<endl;
            }
            getch();
            goto  LabelMenu;
            break;
        }
        case 2: {
            string nomRelation;
            Relation relation;
            vector<Attribut> attributs(0);
            vector<vector<string>> uplets;

            LableUpletRelation:
            system("cls");
            cout << "_______________UPLETS RELATION_____________" << endl;
            cout << endl << "DONNER LE NOM DE LA RELATION : ";
            cin.ignore();
            cin >> nomRelation;
            if (!getMetaRalation(&relation, (char *) nomRelation.c_str())) {
                cout << endl << "ERREUR CETTE RELATION N HESITE PAS" << endl;
                goto LableUpletRelation;
            }
            getAttribut(&attributs, relation);
            getUpletByRel(&uplets, relation);
            if(uplets.empty()){
                cout<<endl<<"PAS DE UPLETS POUR CETTE RELATION";
                getch();
                goto LabelMenu;
            }
            char nomAtt[11];
            for (int i = 0; i < uplets.size(); ++i) {
                cout << endl << "---> ";
                for (int j = 0; j < uplets[i].size(); ++j) {
                    strncpy(nomAtt,attributs[j].nom,10);
                    upper(nomAtt);
                    cout <<nomAtt<<" "<<uplets[i][j]<<" ";
                }
            }
            getch();
            goto LabelMenu;
            break;
        }
        case 3: {
            system("cls");
            cout << "__________________BLOCK_____________________";
            int idBlock = 0;
            do {
                cout << endl << "DONNER LE NUMERO DU BLOCK [0 - " << TAILLEPAGE << "] : ";
                cin >> idBlock;
            } while (idBlock > TAILLEPAGE || idBlock < 0);
            afficheBlock(MEMOIRE[idBlock]);
            getch();
            goto LabelMenu;
            break;
        }
        case 4: {
            system("cls");
            cout << "_________________MEMOIRE___________________"<<endl<<endl;
            affichePage();
            getch();
            goto LabelMenu;
            break;
        }
        case 5:
            cout<<"RETOUR"<<endl;
            affichageMenu();
            break;
        default:
            cout<<"Vous ne ferez rien du tout!";
            goto LabelchoixMenu;
            break;
    }
    cout<<endl;
    return choixMenu;

}

int affichageMenuOperationSelection(){

    LabelMenu: system("cls");
    int choixMenu;

    cout<<" __________MENU OPERATION SELECTION__________"<<endl;
    cout<<"|                                            |"<<endl;
    cout<<"|            1.SELECTION SUR UN ATTIBUT      |"<<endl;
    cout<<"|            2.SELECTION OU                  |"<<endl;
    cout<<"|            3.SELECTION ET                  |"<<endl;
    cout<<"|            4.RETOUR                        |"<<endl;
    cout<<"|____________________________________________|"<<endl;

    LabelchoixMenu:  cout<<"\nVotre choix?"<<endl;
    cin>>choixMenu;
    switch (choixMenu)
    {
        case 1: {
            string nomRelation,attribut,valeur;
            int cpt = 0;
            Relation relation;
            vector<Attribut> attributs(0);
            vector<vector<string>> uplets;

            LableSelectionAtt:system("cls");

            cout << "___________SELECTION SUR UN ATTIBUT___________ ";
            cout<<endl<<"DONNER LE NOM DE LA RELATION : ";
            cin.ignore(); cin>>nomRelation;
            if(! getMetaRalation(&relation, (char *) nomRelation.c_str()) ){
                cout<<endl<<"ERREUR CETTE RELATION N HESITE PAS"<<endl;
                if(4 == cpt++)goto LabelMenu;
                goto LableSelectionAtt;
            }
            cout<<endl<<"DONNER CHAMP DE LA RELATION : ";
            cin.ignore(); cin>>attribut;
            cout<<endl<<"DONNER CHAMP DE LA VALEUR : ";
            cin.ignore(); cin>>valeur;
            getUpletByAtt(&uplets,relation,attribut,valeur);

            getAttribut(&attributs, relation);
            if(uplets.empty()){
                cout<<endl<<"PAS DE UPLETS";
                getch();
                goto LabelMenu;
            }
            char nomAtt[11];
            for (int i = 0; i < uplets.size(); ++i) {
                cout << endl << "---> ";
                for (int j = 0; j < uplets[i].size(); ++j) {
                    strncpy(nomAtt,attributs[j].nom,10);
                    upper(nomAtt);
                    cout <<nomAtt<<" "<<uplets[i][j]<<" ";
                }
            }
            getch();
            goto LabelMenu;

            break;
        }
        case 2: {
            string nomRelation,attribut,valeur;
            int cpt = 0;
            Relation relation;
            vector<Attribut> attributs(0);
            vector<string> listeAttrs;
            vector<string> listeValeurs;
            vector<vector<string>> uplets;

            LableSelectionOR:system("cls");

            cout << "______________SELECTION OU__________________  ";
            cout<<endl<<"DONNER LE NOM DE LA RELATION : ";
            cin.ignore(); cin>>nomRelation;
            if(! getMetaRalation(&relation, (char *) nomRelation.c_str()) ){
                cout<<endl<<"ERREUR CETTE RELATION N HESITE PAS"<<endl;
                if(4 == cpt++)goto LabelMenu;
                goto LableSelectionOR;
            }

            string rep;
            do {
                cout<<endl<<"DONNER CHAMP DE LA RELATION : ";
                cin.ignore();
                cin>>attribut;
                listeAttrs.push_back(attribut);
                cout<<endl<<"DONNER CHAMP DE LA VALEUR : ";
                cin.ignore();
                cin>>valeur;
                listeValeurs.push_back(valeur);
                cout<<endl<<"VOULEZ VOUS CONTINUEZ (o/n):  ";
                cin.ignore();
                cin>>rep;
            } while (rep.compare("n")!=0);

            getUpletByAttOR(&uplets,relation,listeAttrs,listeValeurs);

            getAttribut(&attributs, relation);
            if(uplets.empty()){
                cout<<endl<<"PAS DE UPLETS";
                getch();
                goto LabelMenu;
            }
            char nomAtt[11];
            for (int i = 0; i < uplets.size(); ++i) {
                cout << endl << "---> ";
                for (int j = 0; j < uplets[i].size(); ++j) {
                    strncpy(nomAtt,attributs[j].nom,10);
                    upper(nomAtt);
                    cout <<nomAtt<<" "<<uplets[i][j]<<" ";
                }
            }
            getch();
            goto LabelMenu;
            break;
        }
        case 3: {
            string nomRelation,attribut,valeur;
            int cpt = 0;
            Relation relation;
            vector<Attribut> attributs(0);
            vector<string> listeAttrs;
            vector<string> listeValeurs;
            vector<vector<string>> uplets;

            LableSelectionAND:system("cls");

            cout << "______________SELECTION ET__________________  ";
            cout<<endl<<"DONNER LE NOM DE LA RELATION : ";
            cin.ignore(); cin>>nomRelation;
            if(! getMetaRalation(&relation, (char *) nomRelation.c_str()) ){
                cout<<endl<<"ERREUR CETTE RELATION N HESITE PAS"<<endl;
                if(4 == cpt++)goto LabelMenu;
                goto LableSelectionAND;
            }

            string rep;
            do {
                cout<<endl<<"DONNER CHAMP DE LA RELATION : ";
                cin.ignore();
                cin>>attribut;
                listeAttrs.push_back(attribut);
                cout<<endl<<"DONNER CHAMP DE LA VALEUR : ";
                cin.ignore();
                cin>>valeur;
                listeValeurs.push_back(valeur);
                cout<<endl<<"VOULEZ VOUS CONTINUEZ (o/n):  ";
                cin.ignore();
                cin>>rep;
            } while (rep.compare("n")!=0);

            getUpletByAttAND(&uplets,relation,listeAttrs,listeValeurs);

            getAttribut(&attributs, relation);
            if(uplets.empty()){
                cout<<endl<<"PAS DE UPLETS";
                getch();
                goto LabelMenu;
            }
            char nomAtt[11];
            for (int i = 0; i < uplets.size(); ++i) {
                cout << endl << "---> ";
                for (int j = 0; j < uplets[i].size(); ++j) {
                    strncpy(nomAtt,attributs[j].nom,10);
                    upper(nomAtt);
                    cout <<nomAtt<<" "<<uplets[i][j]<<" ";
                }
            }
            getch();
            goto LabelMenu;
            break;
        }
        case 4:
            cout<<"RETOUR"<<endl;
            affichageMenuOperation();
            break;
        default:
            cout<<"Vous ne ferez rien du tout!";
            goto LabelchoixMenu;
            break;
    }
    cout<<endl;
    return choixMenu;

}

int affichageMenu(){

    LabelMenu: system("cls");

    int choixMenu;

    cout<<" ___________________MENU_____________________"<<endl;
    cout<<"|                                            |"<<endl;
    cout<<"|            1.CREATION RELATION             |"<<endl;
    cout<<"|            2.OPERATION                     |"<<endl;
    cout<<"|            3.AFFICHAGE                     |"<<endl;
    cout<<"|            4.EXIT!                         |"<<endl;
    cout<<"|____________________________________________|"<<endl;

    LabelChoixMenu: cout<<"\nVotre choix?"<<endl;
    cin>>choixMenu;
    switch (choixMenu)
    {
        case 1: {
            system("cls");
            cout << "________________CREATION RELATION__________ ";
            creationRelation();
            goto LabelMenu;
            break;
        }
        case 2:
            system("cls");
            cout<<"OPERATION"<<endl;
            affichageMenuOperation();
            break;
        case 3:
            system("cls");
            cout<<"AFFICHAGE";
            affichageMenuAffichage();
            break;
        case 4: {
            cout << "exit";
            cout<<endl<<"OUH LA LA VOUS PARTEZ DEJA !!! BON C EST PAS GRAVE ON VA ENREGISTRER QUAND MEME POUR  "<<endl;
            const char separateur(' ');

            ofstream ofstream1(nomfichier,ios::out|ios::ate);
            for (int k = 0; k < TAILLEPAGE; ++k) {
                for (int i = 0; i < TAILLEBLOCK; ++i) {
                    ofstream1 << MEMOIRE[k][i] << separateur;
                }
            }

            ofstream1.close();
            cout<<endl<<"C EST OK BEY BEY "<<endl;

            break;
        }
        default:
            cout<<"Vous ne ferez rien du tout!";
            goto LabelChoixMenu;
            break;
    }
    cout<<endl;
    return choixMenu;

}