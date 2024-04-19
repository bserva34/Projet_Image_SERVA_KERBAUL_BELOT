// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "../include/image_ppm.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
  

int main(int argc, char* argv[])
{
  
  int tab[41129];

  std::string liste_entree = "occurence.dat";

  std::fstream fichier_entree(liste_entree, std::ios::in);

  int cpt_zero=0;
  int cpt_un=0;
  int cpt_deux=0;
  int cpt_cinq=0;
  int cpt_dix=0;
  std::string ligne_entree;
  int cpt_sortie=0;
  while (std::getline(fichier_entree, ligne_entree)) {
    char * acc=(char*) ligne_entree.c_str();
    int val=atoi(acc);
    tab[cpt_sortie] = val;
    if (val==0) cpt_zero++;
    if(val<=1) cpt_un++;
    if(val<=2) cpt_deux++;
    if(val<=5) cpt_cinq++;
    if(val<=10) cpt_dix++;
    cpt_sortie++;
  }
  fichier_entree.close();

  cout<<"zero utilisation : "<<cpt_zero<<endl;
  cout<<"1 utilisation ou moins : "<<cpt_un<<endl;
  cout<<"2 utilisation ou moins : "<<cpt_deux<<endl;
  cout<<"5 utilisation ou moins : "<<cpt_cinq<<endl;
  cout<<"10 utilisation ou moins : "<<cpt_dix<<endl;
  return 1;
}
