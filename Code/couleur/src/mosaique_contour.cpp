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

  int nbImagette = 49129; // Nombre d'imagette disponible, à adapter

  sscanf (argv[1],"%s",cNomImgLue) ;
  sscanf (argv[2],"%s",cNomImgBloc) ;
  sscanf (argv[3],"%s",cNomImgMosaique) ;
  sscanf (argv[4],"%s",cNomListeMoyenne) ;



  string nom[nbImagette];
  double moy_r[nbImagette];
  double moy_g[nbImagette];
  double moy_b[nbImagette];

  std::fstream fichier_un("../bash/listeResultatCouleur.txt", std::ios::in);
      
  if (!fichier_un.is_open()) {
    std::cerr << "Erreur lors de l'ouverture du fichier d'entrée." << std::endl;
    return 1;
  }

  std::string ligne_un;
  int cpt=0;
  while (std::getline(fichier_un, ligne_un)) {
    size_t positionSlash1 = ligne_un.find('/');
    size_t positionSlash2 = ligne_un.find('/', positionSlash1 + 1);
    size_t positionSlash3 = ligne_un.find('/', positionSlash2 + 1);

    string partieAvantSlash = ligne_un.substr(0, positionSlash1);
    string partieApresSlash1 = ligne_un.substr(positionSlash1 + 1, positionSlash2 - positionSlash1 - 1);
    string partieApresSlash2 = ligne_un.substr(positionSlash2 + 1, positionSlash3 - positionSlash2 - 1);
    string partieApresSlash3 = ligne_un.substr(positionSlash3 + 1);
    double val_r = std::stod(partieApresSlash1);
    double val_g = std::stod(partieApresSlash2);
    double val_b = std::stod(partieApresSlash3);

    nom[cpt]=partieAvantSlash;
    moy_r[cpt]=val_r;
    moy_g[cpt]=val_g;
    moy_b[cpt]=val_b;
    cpt++;
  }
  fichier_un.close();

  cout<<"chargement moyenne ok"<<endl;
  //lecture et alloc
  OCTET *ImgIn, *ImgOut,*ImgOut2;

  lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;
  
  allocation_tableau(ImgIn, OCTET, nTaille*3);

  lire_image_ppm(cNomImgLue, ImgIn, nTaille);

  int taille_imagette=32; // Taille des imagettes (carrée), à adapter

  int nb_h=nH/taille_imagette;
  int nb_w=nW/taille_imagette;
  allocation_tableau(ImgOut, OCTET, nTaille*3);

  //recup nom fichier dans le tab
  std::vector<string> nomImagettes;

  std::fstream fichier(cNomListeMoyenne, std::ios::in);

  if (!fichier.is_open()) {
    std::cerr << "Erreur lors de l'ouverture du fichier d'entrée." << std::endl;
    return 1;
  }

  std::string ligne;
  string chemin_deux= "../../../../Master_Imagine_local/collection_imagette/couleur/";
  while (std::getline(fichier, ligne)) {
    nomImagettes.push_back(chemin_deux+ligne);
  }

  fichier.close();

  Ptr<SIFT> sift = SIFT::create();

  std::vector<int> descriptors_imagettes;
  for (const auto& nom : nomImagettes) {
    Mat imagette = imread(nom);
    if (imagette.empty()) {
        std::cerr << "Erreur : Impossible de charger l'image " << nom << std::endl;
        continue; // Passer à l'image suivante
    }
    std::vector<KeyPoint> keypoints;
    sift->detect(imagette,keypoints);
    descriptors_imagettes.push_back(keypoints.size());
  }

  cout<<"calcul points clé ok"<<endl;

  double moyenne_r,moyenne_g,moyenne_b;
  for (int i = 0; i < nb_h; i++) {
    for (int j = 0; j < nb_w; j++) {
      OCTET* Imgprov;
      allocation_tableau(Imgprov, OCTET, taille_imagette * taille_imagette*3);
      moyenne_r=0;
      moyenne_g=0;
      moyenne_b=0;
      for (int x = 0; x < taille_imagette; x++) {
        for (int y = 0; y < taille_imagette; y++) {
          int pixel_index = ((i * taille_imagette) + x) * nW * 3 + ((j * taille_imagette) + y) * 3;
          Imgprov[x * taille_imagette + y] = ImgIn[pixel_index];
          Imgprov[x * taille_imagette + y + 1] = ImgIn[pixel_index + 1];
          Imgprov[x * taille_imagette + y + 2] = ImgIn[pixel_index + 2];
          moyenne_r += ImgIn[pixel_index];
          moyenne_g += ImgIn[pixel_index + 1];
          moyenne_b += ImgIn[pixel_index + 2];
        }
      }
      moyenne_r /= (taille_imagette * taille_imagette);
      moyenne_g /= (taille_imagette * taille_imagette);
      moyenne_b /= (taille_imagette * taille_imagette);

      ecrire_image_ppm("imgprov.pgm",Imgprov,taille_imagette,taille_imagette);

      // Détecter les points clés et calculer les descripteurs de ImgProv      
      Mat imgprov = imread("imgprov.pgm");
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
            double stock=moy_r[indice_equi[0]]+moy_g[indice_equi[0]]+moy_b[indice_equi[0]];
            for(int k=0;k<indice_equi.size();++k){
              int diff=abs(moyenne_r-moy_r[indice_equi[k]])+abs(moyenne_g-moy_g[indice_equi[k]])+abs(moyenne_b-moy_b[indice_equi[k]]);
              if(diff<stock){
                stock=diff;
                indice=indice_equi[k];
              }
            }
            allocation_tableau(best, OCTET, taille_imagette * taille_imagette*3);
            //char* acc3 = (char*)nomImagettes[indice].c_str();
            acc = (char*)nom[indice].c_str();
            char* acc2 = (char*)chemin_deux.c_str();
            char* res = new char[strlen(acc) + strlen(acc2) + 1];
            strcpy(res, acc2);
            strcat(res, acc);
            lire_image_ppm(res,best,taille_imagette*taille_imagette);
            for (int x = 0; x < taille_imagette; x++) {
                for (int y = 0; y < taille_imagette; y++) {
                  int pixel_index_out = ((i * taille_imagette) + x) * nW * 3 + ((j * taille_imagette) + y) * 3;
                  int pixel_index_acc = (x * taille_imagette * 3) + y * 3;
                    ImgOut[pixel_index_out] = best[pixel_index_acc];
                    ImgOut[pixel_index_out + 1] = best[pixel_index_acc + 1];
                    ImgOut[pixel_index_out + 2] = best[pixel_index_acc + 2];
                }
            }


        free(Imgprov);
    }
    else{
      int indice= 0;
      char* acc = (char*)nom[0].c_str();;
      double stock=moy_r[0]+moy_g[0]+moy_b[0];
      for(int b=0;b<nbImagette;b++){
          int diff=abs(moyenne_r-moy_r[b])+abs(moyenne_g-moy_g[b])+abs(moyenne_b-moy_b[b]);
          if(diff<stock){
            stock=diff;
            acc=(char*)nom[b].c_str();
            indice=b;
          }        
      }
      
      char* acc2 = (char*)chemin_deux.c_str();
      char* res = new char[strlen(acc) + strlen(acc2) + 1];
      strcpy(res, acc2);
      strcat(res, acc);

      OCTET * Imgacc;
      allocation_tableau(Imgacc, OCTET,taille_imagette*taille_imagette*3);
      lire_image_ppm(res,Imgacc,taille_imagette*taille_imagette);
      for (int x = 0; x < taille_imagette; x++) {
        for (int y = 0; y < taille_imagette; y++) {
          int pixel_index_out = ((i * taille_imagette) + x) * nW * 3 + ((j * taille_imagette) + y) * 3;
          int pixel_index_acc = (x * taille_imagette * 3) + y * 3;
            ImgOut[pixel_index_out] = Imgacc[pixel_index_acc];
            ImgOut[pixel_index_out + 1] = Imgacc[pixel_index_acc + 1];
            ImgOut[pixel_index_out + 2] = Imgacc[pixel_index_acc + 2];
        }
      }
      free(Imgacc);

    }
  }
}



ecrire_image_ppm(cNomImgMosaique, ImgOut, nH, nW);
free(ImgIn);
free(ImgOut);

return 1;
}
