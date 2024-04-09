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
  char cNomVideoLue[250], cNomVideoMosaique[250], cNomListeMoyenne[250], repertoireImagette[250];
  int taille_imagette, nbImagette;
  
  if (argc != 7) 
	{
		printf("Usage: VideoInitiale.mp4 VideoMosaique.mp4 ListeMoyenneImagettes TailleImagette NbImagette RepertoireImagettes\n"); 
		return 1;
	}

	sscanf (argv[1],"%s",cNomVideoLue) ;
  sscanf (argv[2],"%s",cNomVideoMosaique) ;
  sscanf (argv[3],"%s",cNomListeMoyenne) ;
  sscanf (argv[4],"%d",&taille_imagette) ;
  sscanf (argv[5],"%d",&nbImagette) ;
  sscanf (argv[6],"%s",repertoireImagette) ;

  VideoCapture cap(cNomVideoLue);
  if (!cap.isOpened()) {
    std::cerr << "Erreur lors de l'ouverture de la vidéo" << std::endl;
    return -1;
  }else{
    std::cout << "La vidéo a bien été ouverte\n";
  }

  int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
  int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
  double fps = cap.get(CAP_PROP_FPS);
  int tailleFrame = frame_width * frame_height;

  VideoWriter videoMosaique(cNomVideoMosaique, VideoWriter::fourcc('X', '2', '6', '4'), fps, Size(frame_width, frame_height), false);
  int nbFrame = 0;


  // Lit le fichier des moyennes
  string nom[nbImagette];
  double moy[nbImagette];
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

  int nb = frame_width / taille_imagette; // Vidéo de dimension carrée uniquement

  int frame_count = 0;
  int frame_read = 4; // On ne lit que une frame sur quatre

  while (true) {
      Mat frame;
      if (!cap.read(frame)){
          break; // Toute les frames de la vidéo initiale ont été lu
      }

      if (frame_count%frame_read==0){
        cvtColor(frame, frame, COLOR_BGR2GRAY); // Convertit la frame en niveau de gris, parceque sinon ça ne lit que 1/3 de la vidéo (une seule des 3 composantes)

        OCTET *ImgIn, *ImgOut;
        allocation_tableau(ImgIn, OCTET, tailleFrame); // Récupère les pixels de la frame
        allocation_tableau(ImgOut, OCTET, tailleFrame); // Conserve la mosaïque créée à partir de la frame

        // On récupère les pixels de la frame courante que l'on écrit dans ImgIn
        for (int x = 0; x < frame.rows; x++) {
          for (int y = 0; y < frame.cols; y++) {
              uchar pixelNDG = frame.at<uchar>(x,y);
              ImgIn[x*frame_width + y] = pixelNDG;
          }
        }

        // Création de la mosaïque de la frame courante
        Mat frameMosa;
        frameMosa.create(frame_width, frame_height, CV_8UC1);
        double moyenne;
        for(int i=0;i<nb;i++){
          for(int j=0;j<nb;j++){
            moyenne=0;
            for (int x = 0; x < taille_imagette; x++) {
              for (int y = 0; y < taille_imagette; y++) {
                moyenne += ImgIn[(i * taille_imagette * frame_width) + (x * frame_width) + (j * taille_imagette) + y];
              }
            }
            moyenne=moyenne/(taille_imagette*taille_imagette);

            int indice=0;
            char* acc = (char*)nom[0].c_str();;
            double stock=moy[0];
            for(int b=1;b<nbImagette;b++){
              if(abs(moyenne-moy[b]) < abs(moyenne-stock)){
                stock=moy[b];
                acc=(char*)nom[b].c_str();
                indice=b;
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
                ImgOut[(i * taille_imagette *frame_width) + (z * frame_width) + (j * taille_imagette) + k] = Imgacc[z * taille_imagette + k];
              }        
            }
            free(Imgacc);
          }
        }    

        // Ecrire l'image mosaique dans la nouvelle frame
        for (int n = 0 ; n < frame_height ; n++){
          for (int m = 0 ; m < frame_width ; m++){
            frameMosa.at<uchar>(n,m) = (uchar)ImgOut[n*frame_width + m];
          }
        }

        videoMosaique.write(frameMosa);
        imshow("Frame", frameMosa);
        std::cout << "Une nouvelle frame produite (" << nbFrame << ")\n";
        nbFrame++;
      }

      ++frame_count;

      // Appuyer sur 'q' pour arrêter
      if (waitKey(10) == 'q'){
        break;
      }
  }

  std::cout << "Un total de " << nbFrame << " frames\n";
  cap.release();
  videoMosaique.release();
  destroyAllWindows();


  return 0;
}