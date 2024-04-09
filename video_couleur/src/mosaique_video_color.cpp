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

  VideoWriter videoMosaique(cNomVideoMosaique, VideoWriter::fourcc('X', '2', '6', '4'), fps, Size(frame_width, frame_height), true);
  int nbFrame = 0;


  // Lit le fichier des moyennes
  string nom[nbImagette];
  double moy_r[nbImagette];
  double moy_g[nbImagette];
  double moy_b[nbImagette];
  std::fstream fichier(cNomListeMoyenne, std::ios::in);
      
  if (!fichier.is_open()) {
    std::cerr << "Erreur lors de l'ouverture du fichier d'entrée." << std::endl;
    return 1;
  }

  std::string ligne;
  int cpt=0;
  while (std::getline(fichier, ligne)) {
    size_t positionSlash1 = ligne.find('/');
    size_t positionSlash2 = ligne.find('/', positionSlash1 + 1);
    size_t positionSlash3 = ligne.find('/', positionSlash2 + 1);

    string partieAvantSlash = ligne.substr(0, positionSlash1);
    string partieApresSlash1 = ligne.substr(positionSlash1 + 1, positionSlash2 - positionSlash1 - 1);
    string partieApresSlash2 = ligne.substr(positionSlash2 + 1, positionSlash3 - positionSlash2 - 1);
    string partieApresSlash3 = ligne.substr(positionSlash3 + 1);
    double val_r = std::stod(partieApresSlash1);
    double val_g = std::stod(partieApresSlash2);
    double val_b = std::stod(partieApresSlash3);

    nom[cpt]=partieAvantSlash;
    moy_r[cpt]=val_r;
    moy_g[cpt]=val_g;
    moy_b[cpt]=val_b;
    cpt++;
  }
  fichier.close();

  int nb = frame_width / taille_imagette; // Vidéo de dimension carrée uniquement
  std::vector<int> alreadyUsed;

  int compteurFrame = 0;
  int maxLookFrame = 1; // Frame à partir de laquelle on ne regarde plus toutes les imagettes mais seulement celle déjà utilisées 

  while (true) {
      Mat frame;
      if (!cap.read(frame)){
          break; // Toute les frames de la vidéo initiale ont été lu
      }

      OCTET *ImgIn, *ImgOut;
      allocation_tableau(ImgIn, OCTET, tailleFrame*3); // Récupère les pixels de la frame
      allocation_tableau(ImgOut, OCTET, tailleFrame*3); // Conserve la mosaïque créée à partir de la frame

      // On récupère les pixels de la frame courante que l'on écrit dans ImgIn
      for (int x = 0; x < frame.rows; x++) {
        for (int y = 0; y < frame.cols; y++) {
            Vec3b pixel = frame.at<Vec3b>(x,y);
            // Apparemment c'est au format BGR et non RGB
            uchar blue = pixel[0];
            uchar green = pixel[1];
            uchar red = pixel[2];

            ImgIn[x*frame_width*3 + y*3+0] = red ;
            ImgIn[x*frame_width*3 + y*3+1] = green;
            ImgIn[x*frame_width*3 + y*3+2] = blue;
        }
      }

      // Création de la mosaïque de la frame courante
      Mat frameMosa;
      frameMosa.create(frame_width, frame_height, CV_8UC3);
      double moyenne_r, moyenne_g, moyenne_b;
      for(int i=0;i<nb;i++){
        for(int j=0;j<nb;j++){
          moyenne_r=0;
          moyenne_g=0;
          moyenne_b=0;
          for (int x = 0; x < taille_imagette; x++) {
            for (int y = 0; y < taille_imagette; y++) {
              int pixel_index = (i * taille_imagette * frame_width*3) + (x * frame_width*3) + (j * taille_imagette*3) + y*3;
              moyenne_r += ImgIn[pixel_index + 0];
              moyenne_g += ImgIn[pixel_index + 1];
              moyenne_b += ImgIn[pixel_index + 2];
            }
          }
          moyenne_r=moyenne_r/(taille_imagette*taille_imagette);
          moyenne_g=moyenne_g/(taille_imagette*taille_imagette);
          moyenne_b=moyenne_b/(taille_imagette*taille_imagette);
          char* acc;
          if (compteurFrame < maxLookFrame){
            int indice=0;
            acc = (char*)nom[0].c_str();
            double stock = abs(moyenne_r-moy_r[0])+abs(moyenne_g-moy_g[0])+abs(moyenne_b-moy_b[0]);
            for(int b=1;b<nbImagette;b++){
              if(abs(moyenne_r-moy_r[b])+abs(moyenne_g-moy_g[b])+abs(moyenne_b-moy_b[b]) < stock){
                stock=abs(moyenne_r-moy_r[b])+abs(moyenne_g-moy_g[b])+abs(moyenne_b-moy_b[b]);
                acc=(char*)nom[b].c_str();
                indice=b;
              }       
            }
            if (std::find(alreadyUsed.begin(), alreadyUsed.end(), indice) == alreadyUsed.end()){ // On ajoute si l'élément n'est pas déjà présent
              alreadyUsed.push_back(indice);
            }
          }else{
            int indice=alreadyUsed[0];
            acc = (char*)nom[alreadyUsed[0]].c_str();
            double stock = abs(moyenne_r-moy_r[alreadyUsed[0]])+abs(moyenne_g-moy_g[alreadyUsed[0]])+abs(moyenne_b-moy_b[alreadyUsed[0]]);
            for (int a = 1 ; a < alreadyUsed.size() ; a++){
              int b = alreadyUsed[a];
              if(abs(moyenne_r-moy_r[b])+abs(moyenne_g-moy_g[b])+abs(moyenne_b-moy_b[b]) < stock){
                stock=abs(moyenne_r-moy_r[b])+abs(moyenne_g-moy_g[b])+abs(moyenne_b-moy_b[b]);
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
          allocation_tableau(Imgacc, OCTET,taille_imagette*taille_imagette*3);
          lire_image_ppm(res,Imgacc,taille_imagette*taille_imagette);
          for(int z=0;z<taille_imagette;z++){
            for(int k=0;k<taille_imagette;k++){
              int pixel_index_out = i*taille_imagette*frame_width*3 + z*frame_width*3 + j*taille_imagette*3 + k*3;
              int pixel_index_acc = z*taille_imagette*3 + k*3;
              ImgOut[pixel_index_out+0] = Imgacc[pixel_index_acc+0];
              ImgOut[pixel_index_out+1] = Imgacc[pixel_index_acc+1];
              ImgOut[pixel_index_out+2] = Imgacc[pixel_index_acc+2];
            }        
          }
          free(Imgacc);
        }
      }

      if (compteurFrame < maxLookFrame){
        ++compteurFrame;
        cout<<"nb imagette : " << alreadyUsed.size()<<endl;
      }    

      // Ecrire l'image mosaique dans la nouvelle frame
      for (int n = 0 ; n < frame_height ; n++){
        for (int m = 0 ; m < frame_width ; m++){
          // Attention : Dans les frames on écrit au format BGR et non RGB
          frameMosa.at<Vec3b>(n,m) = Vec3b((uchar)ImgOut[n*frame_width*3 + m*3+2], (uchar)ImgOut[n*frame_width*3 + m*3+1], (uchar)ImgOut[n*frame_width*3 + m*3+0]);
        }
      }

      videoMosaique.write(frameMosa);
      imshow("Frame", frameMosa);
      std::cout << "Une nouvelle frame produite (" << nbFrame << ")\n";
      nbFrame++;

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
