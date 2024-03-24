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


  //creat contour
  cv::Mat image = cv::imread(cNomImgLue,cv::IMREAD_GRAYSCALE);
  cv::Mat contours;
  cv::Canny(image, contours, 100, 200);
  cv::imwrite(cNomImgBloc,contours);




  //lecture et alloc
  OCTET *ImgIn, *ImgOut,*ImgOut2;

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;
  
  allocation_tableau(ImgIn, OCTET, nTaille);
  lire_image_pgm(cNomImgBloc, ImgIn, nH * nW);

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
    nomImagettes.push_back(chemin+ligne);
  }

  fichier.close();

  Ptr<SIFT> sift = SIFT::create();

  std::vector<Mat> descriptors_imagettes;
  for (const auto& nom : nomImagettes) {
    Mat imagette = imread(nom, IMREAD_GRAYSCALE);
    std::vector<KeyPoint> keypoints;
    Mat descriptors;
    sift->detectAndCompute(imagette, noArray(), keypoints, descriptors);
    descriptors_imagettes.push_back(descriptors);
  }


  cout<<"sift imagettes ok de taille  : "<<descriptors_imagettes.size()<<endl;


  for (int i = 0; i < nb; i++) {
    for (int j = 0; j < nb; j++) {
      OCTET* Imgprov;
      allocation_tableau(Imgprov, OCTET, taille_imagette * taille_imagette);
      for (int x = 0; x < taille_imagette; x++) {
        for (int y = 0; y < taille_imagette; y++) {
          Imgprov[x * taille_imagette + y] = ImgIn[(i * taille_imagette * nW) + (x * nW) + (j * taille_imagette) + y];
        }
      }

      ecrire_image_pgm("imgprov.pgm",Imgprov,taille_imagette,taille_imagette);

      // Détecter les points clés et calculer les descripteurs de ImgProv      
      Mat imgprov = imread("imgprov.pgm", IMREAD_GRAYSCALE);
      std::vector<KeyPoint> keypoints_imgprov;
      Mat descriptors_imgprov;
      sift->detectAndCompute(imgprov, noArray(), keypoints_imgprov, descriptors_imgprov);
      //cout<<"calcul descripteur sous zone : "<<i*nb+j<<endl;

      if (keypoints_imgprov.size() > 0) {
        BFMatcher matcher(NORM_HAMMING);
        std::vector<std::vector<DMatch>> matches;
        matcher.knnMatch(descriptors_imgprov, descriptors_imagettes, matches, 2, noArray(), false);

        std::vector<DMatch> good_matches;
        for (size_t i = 0; i < matches.size(); ++i) {
            if (matches[i].size() >= 2 && matches[i][0].distance < 0.75 * matches[i][1].distance) {
                good_matches.push_back(matches[i][0]);
            }
        }
        cout<<good_matches.size()<<endl;

        if (!good_matches.empty()) {
            double min_distance = good_matches[0].distance;
            int best_imagette_index = 0;
            for (size_t i = 1; i < good_matches.size(); ++i) {
                if (good_matches[i].distance < min_distance) {
                    min_distance = good_matches[i].distance;
                    best_imagette_index = i;
                }
            }

            Mat best_imagette = imread(chemin_deux + nomImagettes[best_imagette_index], IMREAD_GRAYSCALE);
            // Écrire la meilleure imagette dans l'image mosaïque à la position (i, j)
            for (int x = 0; x < taille_imagette; x++) {
                for (int y = 0; y < taille_imagette; y++) {
                    ImgOut[(i * taille_imagette * nW) + (x * nW) + (j * taille_imagette) + y] = best_imagette.at<uchar>(x, y);
                }
            }
        }

        free(Imgprov);
    }
    else{
              //faire la technique de la difference

    }
  }
}



ecrire_image_pgm(cNomImgMosaique, ImgOut, nH, nW);
free(ImgIn);
free(ImgOut);

return 1;
}
