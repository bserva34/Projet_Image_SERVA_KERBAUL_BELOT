// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "../include/image_ppm.h"
#include <iostream>
#include <fstream>
#include <vector>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;  

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgBloc[250], cNomImgMosaique[250], cNomListeMoyenne[250];
  int nH, nW, nTaille, S1;
  
  if (argc != 5) 
  {
    printf("Usage: ImageInitiale.pgm ImageContour.pgm ImageMosaique.pgm ListeImagettes \n"); 
    return 1;
  }

  int nbImagette = 10000; // Nombre d'imagette disponible, à adapter

  sscanf (argv[1],"%s",cNomImgLue) ;
  sscanf (argv[2],"%s",cNomImgBloc) ;
  sscanf (argv[3],"%s",cNomImgMosaique) ;
  sscanf (argv[4],"%s",cNomListeMoyenne) ;

  string nom[nbImagette];
  double moy[nbImagette];

  std::fstream fichier_un("../bash/listeResultat.txt", std::ios::in);
      
  if (!fichier_un.is_open()) {
    std::cerr << "Erreur lors de l'ouverture du fichier d'entrée." << std::endl;
    return 1;
  }

  std::string ligne_un;
  int cpt=0;
  while (std::getline(fichier_un, ligne_un)) {
    size_t positionSlash = ligne_un.find('/');

    std::string partieAvantSlash = ligne_un.substr(0, positionSlash);
    std::string partieApresSlash = ligne_un.substr(positionSlash + 1);
    double val = std::stod(partieApresSlash);

    nom[cpt]=partieAvantSlash;
    moy[cpt]=val;
    cpt++;
  }
  fichier_un.close();


  //lecture et alloc
  OCTET *ImgIn, *ImgOut,*ImgOut2;

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;
  
  allocation_tableau(ImgIn, OCTET, nTaille);
  //lire_image_pgm(cNomImgBloc, ImgIn, nH * nW);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

  int taille_imagette=20; // Taille des imagettes (carrée), à adapter

  int nb=nH/taille_imagette;
  allocation_tableau(ImgOut, OCTET, nTaille);

  //recup nom fichier dans le tab
  std::vector<string> nomImagettes;

  std::fstream fichier(cNomListeMoyenne, std::ios::in);

  if (!fichier.is_open()) {
    std::cerr << "Erreur lors de l'ouverture du fichier d'entrée." << std::endl;
    return 1;
  }

  std::string ligne;
  string chemin = "../../../../Master_Imagine_local/collection_imagette/contour_ndg/";
  string chemin_deux= "../../../../Master_Imagine_local/collection_imagette/redimensionner/";
  while (std::getline(fichier, ligne)) {
    nomImagettes.push_back(chemin_deux+ligne);
  }

  fichier.close();

  Ptr<SIFT> sift = SIFT::create();

  std::vector<int> descriptors_imagettes;
  for (const auto& nom : nomImagettes) {
    Mat imagette = imread(nom, IMREAD_GRAYSCALE);
    if (imagette.empty()) {
        std::cerr << "Erreur : Impossible de charger l'image " << nom << std::endl;
        continue; // Passer à l'image suivante
    }
    std::vector<KeyPoint> keypoints;
    sift->detect(imagette,keypoints);
    descriptors_imagettes.push_back(keypoints.size());
  }


  for (int i = 0; i < nb; i++) {
    for (int j = 0; j < nb; j++) {
      OCTET* Imgprov;
      allocation_tableau(Imgprov, OCTET, taille_imagette * taille_imagette);
      double moyenne=0;
      for (int x = 0; x < taille_imagette; x++) {
        for (int y = 0; y < taille_imagette; y++) {
          Imgprov[x * taille_imagette + y] = ImgIn[(i * taille_imagette * nW) + (x * nW) + (j * taille_imagette) + y];
          moyenne+=ImgIn[(i * taille_imagette * nW) + (x * nW) + (j * taille_imagette) + y];
        }
      }
      moyenne=moyenne/(taille_imagette*taille_imagette);

      ecrire_image_pgm("imgprov.pgm",Imgprov,taille_imagette,taille_imagette);

      // Détecter les points clés et calculer les descripteurs de ImgProv      
      Mat imgprov = imread("imgprov.pgm", IMREAD_GRAYSCALE);
      std::vector<KeyPoint> keypoints_imgprov;
      sift->detect(imgprov,keypoints_imgprov);

      int nb_sous_zone=keypoints_imgprov.size();
      //if ( nb_sous_zone> 0) {
        int acc=0;
        int stock = 0;
      
        int taille_prov=descriptors_imagettes.size();
        std::vector<int> ecart;
        std::vector<int> indice_equi;
        for(int k=0;k<taille_prov;++k){
          ecart.push_back(abs(nb_sous_zone-descriptors_imagettes[k]));
          if(abs(nb_sous_zone-descriptors_imagettes[k])==0){
            indice_equi.push_back(k);
          }
        }

        if (indice_equi.size()>0) {
            OCTET* best;
            int indice= 0;
            char* acc = (char*)nom[indice_equi[0]].c_str();;
            double stock=moy[indice_equi[0]];
            for(int k=0;k<indice_equi.size();++k){
              if(abs(moy[indice_equi[k]]-moyenne)<abs(stock-moyenne)){
                stock=moy[indice_equi[k]];
                indice=indice_equi[k];
              }
            }
            allocation_tableau(best, OCTET, taille_imagette * taille_imagette);
            //char* acc3 = (char*)nomImagettes[indice].c_str();
            acc = (char*)nom[indice].c_str();
            char* acc2 = (char*)chemin_deux.c_str();
            char* res = new char[strlen(acc) + strlen(acc2) + 1];
            strcpy(res, acc2);
            strcat(res, acc);
            lire_image_pgm(res,best,taille_imagette*taille_imagette);
            for (int x = 0; x < taille_imagette; x++) {
                for (int y = 0; y < taille_imagette; y++) {
                    ImgOut[(i * taille_imagette * nW) + (x * nW) + (j * taille_imagette) + y] = best[x * taille_imagette + y];
                }
            }


        free(Imgprov);
    }
    else{
      int indice= 0;
      char* acc = (char*)nom[0].c_str();
      double stock=moy[0];
      for(int b=1;b<nbImagette;b++){
          if(abs(moyenne-moy[b]) < abs(moyenne-stock)){
            stock=moy[b];
            acc=(char*)nom[b].c_str();
            indice=b;
          }     
      }
      
      char* acc2 = (char*)chemin_deux.c_str();
      char* res = new char[strlen(acc) + strlen(acc2) + 1];
      strcpy(res, acc2);
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

    }
  }
}



ecrire_image_pgm(cNomImgMosaique, ImgOut, nH, nW);
free(ImgIn);
free(ImgOut);

return 1;
}
