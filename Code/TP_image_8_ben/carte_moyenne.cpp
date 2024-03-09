// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "../../image_ppm.h"
#include <iostream>
using namespace std;
  
bool deja_utiliser(std::vector<int> &used,int taille,int check){
  for(int i=0;i<taille;i++){
    if(used[i]==check){
      return false;
    }
  }
  return true;
}
int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille, S1;
  
   OCTET *ImgIn, *ImgOut,*ImgOut2;
   
   lire_nb_lignes_colonnes_image_pgm("montagne.pgm", &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm("montagne.pgm", ImgIn, nH * nW);

   int taille_imagette=20;

   int nb=nH/taille_imagette;
   allocation_tableau(ImgOut, OCTET, nb*nb);
   allocation_tableau(ImgOut2, OCTET, nTaille);

   string nom[10000];
   double moy[10000];
   std::vector<int> used;

   std::string liste_nom = "liste_nom.txt";
    std::fstream fichier(liste_nom, std::ios::in);
      
    if (!fichier.is_open()) {
        std::cerr << "Erreur lors de l'ouverture du fichier d'entrée." << std::endl;
        return 1;
    }

   std::string ligne;
    int cpt=0;
    while (std::getline(fichier, ligne)) {
        size_t positionSlash = ligne.find('/');

        std::string partieAvantSlash = ligne.substr(0, positionSlash);
        std::string partieApresSlash = ligne.substr(positionSlash + 1);
        double val = std::stod(partieApresSlash);

        nom[cpt]=partieApresSlash;
        moy[cpt]=val;
        cpt++;
    }
    fichier.close();

   
   double moyenne;
   for(int i=0;i<nb;i++){
    for(int j=0;j<nb;j++){
      moyenne=0;
      for (int y = 0; y < taille_imagette; y++) {
            for (int x = 0; x < taille_imagette; x++) {
                moyenne += ImgIn[i * taille_imagette + y];
            }
      }
      moyenne=moyenne/(taille_imagette*taille_imagette);
      ImgOut[i*nb+j]=moyenne;
      int indice;
      char* acc;
      double stock=0;
      for(int b=0;b<10000;b++){
        if(deja_utiliser(used,used.size(),b)){
          if((moyenne-moy[b])<(moyenne-stock)){
            stock=moy[b];
            acc=(char*)nom[b].c_str();
            indice=b;
          } 
        }       

      }
      
      //char* acc2 = "../../../../Master_Imagine_local/collection_imagette/redimensionner/";
      char* acc2 = "../../../../Master_local/projet_image/BOWS2OrigEp3/redimension/";
      char* res = new char[strlen(acc) + strlen(acc2) + 1];
      strcpy(res, acc2);
      strcat(res, acc);

      OCTET * Imgacc;
      allocation_tableau(Imgacc, OCTET,taille_imagette*taille_imagette);
      lire_image_pgm(res,Imgacc,taille_imagette*taille_imagette);
      for(int z=0;z<taille_imagette;z++){
        for(int k=0;k<taille_imagette;k++){
          ImgOut2[(i * taille_imagette + z) * nW + (j * taille_imagette + k)] = Imgacc[z * taille_imagette + k];
        }        
      }
      free(Imgacc);
      //cout<<i*nW+j<<endl;
      used.push_back(indice);

    }
   }     
	

   ecrire_image_pgm("leopard_bloc.pgm",ImgOut,nb,nb);
   ecrire_image_pgm("mosaique.pgm",ImgOut2,nH,nW);
   free(ImgIn); free(ImgOut);

   return 1;
}
