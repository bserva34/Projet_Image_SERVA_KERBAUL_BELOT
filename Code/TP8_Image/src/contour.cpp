// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "../include/image_ppm.h"
using namespace std;

int main(int argc, char* argv[])
{
  char cListe[250];
  int nH, nW, nTaille, S1, S2, S3;
  
  if (argc != 2) 
  {
   printf("Usage: ListeNom \n"); 
   exit (1) ;
 }

 sscanf (argv[1],"%s",cListe) ;

 int nbImagette = 10000;


 OCTET *ImgIn, *ImgOut, *ImgOut2, *ImgOut3;
   /*
   lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille*3);
   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille*3);
   allocation_tableau(ImgOut2, OCTET, nTaille*3);
   allocation_tableau(ImgOut3, OCTET, nTaille*3);*/

 string nom[nbImagette];

 std::string liste = cListe;

 std::fstream fichier(liste, std::ios::in);

 if (!fichier.is_open()) {
  std::cerr << "Erreur lors de l'ouverture du fichier d'entrée." << std::endl;
  return 1;
}

std::string ligne;
int cpt=0;
while (std::getline(fichier, ligne)) {
  string acc = ligne.c_str();
  nom[cpt]=acc;
  cpt++;
}
fichier.close();


for(int i=0;i<nbImagette;i++){
  char* acc = (char*)nom[i].c_str();
  char* acc2 = "../../../../Master_Imagine_local/collection_imagette/redimensionner/";  
  char* res = new char[strlen(acc) + strlen(acc2) + 1];
  strcpy(res, acc2);
  strcat(res, acc);

  OCTET *Imgacc;
  lire_nb_lignes_colonnes_image_pgm(res, &nH, &nW);
  nTaille = nH * nW;
  allocation_tableau(Imgacc, OCTET, nTaille);
  lire_image_pgm(res, Imgacc, nTaille);

  cv::Mat image = cv::imread(res,cv::IMREAD_GRAYSCALE);
  cv::Mat contours;
  cv::Canny(image, contours, 100, 200);

  char* ecriture = "../../../../Master_Imagine_local/collection_imagette/contour_ndg/";
  char* res2 = new char[strlen(acc) + strlen(ecriture) + 1];
  strcpy(res2,ecriture);
  strcat(res2,acc);
  cv::imwrite(res2,contours);
}


return 1;
}
