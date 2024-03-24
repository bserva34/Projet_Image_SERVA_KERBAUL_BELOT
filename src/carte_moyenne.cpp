// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "../include/image_ppm.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int nb_max_utilisation;

bool deja_utiliser(std::vector<int> &used,int check){
  if(used[check] >= nb_max_utilisation){
    return false;
  }
  return true;
}

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgMosaique[250], cNomListeMoyenne[250], repertoireImagette[250];
  int nH, nW, nTaille;
  int taille_imagette, nbImagette;
  
  if (argc != 8) 
	{
		printf("Usage: ImageInitiale.pgm ImageMosaique.pgm ListeMoyenneImagettes TailleImagette NbImagette RepertoireImagettes NbUtilisationMax\n"); 
		return 1;
	}

	sscanf (argv[1],"%s",cNomImgLue) ;
  sscanf (argv[2],"%s",cNomImgMosaique) ;
  sscanf (argv[3],"%s",cNomListeMoyenne) ;
  sscanf (argv[4],"%d",&taille_imagette) ;
  sscanf (argv[5],"%d",&nbImagette) ;
  sscanf (argv[6],"%s",repertoireImagette) ;
  sscanf (argv[7],"%d",&nb_max_utilisation) ;

  OCTET *ImgIn, *ImgOut;
   
  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);

  if (nb_max_utilisation == -1){ // On ne limite pas la réutilisation des imagettes
    nb_max_utilisation = pow(nH/taille_imagette,2); // Nombre d'imagette nécessaire au maximum pour faire l'image mosaïque
  }

  nTaille = nH * nW;
  
  allocation_tableau(ImgIn, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

  int nb=nH/taille_imagette;
  allocation_tableau(ImgOut, OCTET, nTaille);

  string nom[nbImagette];
  double moy[nbImagette];
  std::vector<int> used(nbImagette,0);

  std::fstream fichier(cNomListeMoyenne, std::ios::in);
      
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

    nom[cpt]=partieAvantSlash;
    moy[cpt]=val;
    cpt++;
  }
  fichier.close();

  double moyenne;
  for(int i=0;i<nb;i++){
    for(int j=0;j<nb;j++){
      moyenne=0;
      for (int x = 0; x < taille_imagette; x++) {
        for (int y = 0; y < taille_imagette; y++) {
          moyenne += ImgIn[(i * taille_imagette * nW) + (x * nW) + (j * taille_imagette) + y];
        }
      }
      moyenne=moyenne/(taille_imagette*taille_imagette);

      bool isFound = false;
      int indice;
      char* acc;
      double stock;
      for(int b=0;b<nbImagette;b++){
        if(deja_utiliser(used,b)){
          if (!isFound){
            isFound = true;
            stock=moy[b];
            acc=(char*)nom[b].c_str();
            indice=b;
          }else if(abs(moyenne-moy[b]) <= abs(moyenne-stock)){
            stock=moy[b];
            acc=(char*)nom[b].c_str();
            indice=b;
          } 
        }       
      }
      
      char* res = new char[strlen(acc) + strlen(repertoireImagette) + 2];
      strcpy(res, repertoireImagette);
      strcat(res, "/");
      strcat(res, acc);

      OCTET * Imgacc;
      allocation_tableau(Imgacc, OCTET,taille_imagette*taille_imagette);
      lire_image_pgm(res,Imgacc,taille_imagette*taille_imagette);
      for(int z=0;z<taille_imagette;z++){
        for(int k=0;k<taille_imagette;k++){
          ImgOut[(i * taille_imagette *nW) + (z * nW) + (j * taille_imagette) + k] = Imgacc[z * taille_imagette + k];
        }        
      }
      free(Imgacc);
      used[indice]++;
    }
  }    
	
  ecrire_image_pgm(cNomImgMosaique,ImgOut,nH,nW);
  free(ImgIn); free(ImgOut);

  return 1;
}
