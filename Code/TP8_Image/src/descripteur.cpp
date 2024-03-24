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
  char cNomImgLue[250];
  
  if (argc != 2) 
  {
    printf("Usage: ListeNom \n"); 
    return 1;
  }

  sscanf (argv[1],"%s",cNomImgLue) ;

  std::vector<string> nomImagettes;

  std::fstream fichier(cNomImgLue, std::ios::in);

  if (!fichier.is_open()) {
    std::cerr << "Erreur lors de l'ouverture du fichier d'entrée." << std::endl;
    return 1;
  }

  std::string ligne;
  string chemin = "../../../../Master_Imagine_local/collection_imagette/contour_ndg/";
  while (std::getline(fichier, ligne)) {
    string acc = chemin+ligne;
    nomImagettes.push_back(acc);
  }


  fichier.close();


Ptr<SIFT> sift = SIFT::create();

std::vector<Mat> descriptors_imagettes;
for (const auto& nom : nomImagettes) {
    Mat imagette = imread(nom, IMREAD_GRAYSCALE);
    if (imagette.empty()) {
        std::cerr << "Erreur lors du chargement de l'image : " << nom << std::endl;
        continue;
    }
    std::vector<KeyPoint> keypoints;
    Mat descriptors;
    sift->detectAndCompute(imagette, noArray(), keypoints, descriptors);
    descriptors_imagettes.push_back(descriptors);
    cout << "Nombre de points clés détectés dans " << nom << " : " << keypoints.size() << endl;
}





  // Sauvegarder les descripteurs dans un fichier
  std::ofstream outfile("../bash/descriptors.txt", std::ios::out);

  if (outfile.is_open()) {
      for (const auto& descriptors : descriptors_imagettes) {
          for (int i = 0; i < descriptors.rows; ++i) {
              for (int j = 0; j < descriptors.cols; ++j) {
                  outfile << descriptors.at<uchar>(i, j) << " ";
              }
              outfile << "\n";
          }
          outfile << "\n";
      }
      outfile.close();
  } else {
      std::cerr << "Impossible d'ouvrir le fichier pour écrire les descripteurs." << std::endl;
  }


  return 1;
}
