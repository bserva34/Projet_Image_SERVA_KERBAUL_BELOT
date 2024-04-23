// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "../include/image_ppm.h"
#include <iostream>
#include <fstream>
#include <vector>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

std::vector<string> nom;
std::vector<double> moy_r;
std::vector<double> moy_g;
std::vector<double> moy_b;

void lireFichierMoyenne(char* cNomListeMoyenne){
  std::fstream fichier(cNomListeMoyenne, std::ios::in);
      
  if (!fichier.is_open()) {
    std::cerr << "Erreur lors de l'ouverture du fichier d'entrée." << std::endl;
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

    nom.push_back(partieAvantSlash);
    moy_r.push_back(val_r);
    moy_g.push_back(val_g);
    moy_b.push_back(val_b);
    cpt++;
  }
  fichier.close();
}

Mat imgToFrame(OCTET *ImgOut, int frame_width, int frame_height){
  // Ecrire l'image mosaique dans la nouvelle frame
  Mat res;
  res.create(frame_height, frame_width, CV_8UC3);
  for (int n = 0 ; n < frame_height ; n++){
    for (int m = 0 ; m < frame_width ; m++){
      // Attention : Dans les frames on écrit au format BGR et non RGB
      res.at<Vec3b>(n,m) = Vec3b((uchar)ImgOut[n*frame_width*3 + m*3+2], (uchar)ImgOut[n*frame_width*3 + m*3+1], (uchar)ImgOut[n*frame_width*3 + m*3+0]);
    }
  }
  return res;
}

OCTET* frameToImg(Mat frame, int frame_width, int frame_height){
  int tailleFrame = frame_width*frame_height;
  OCTET* res;
  allocation_tableau(res, OCTET, tailleFrame*3); // Récupère les pixels de la frame

  // On récupère les pixels de la frame courante que l'on écrit dans ImgIn
  for (int x = 0; x < frame.rows; x++) {
    for (int y = 0; y < frame.cols; y++) {
        Vec3b pixel = frame.at<Vec3b>(x,y);
        // Apparemment c'est au format BGR et non RGB
        uchar blue = pixel[0];
        uchar green = pixel[1];
        uchar red = pixel[2];

        res[x*frame_width*3 + y*3+0] = red ;
        res[x*frame_width*3 + y*3+1] = green;
        res[x*frame_width*3 + y*3+2] = blue;
    }
  }
  return res;
}

OCTET* createColorMosaique(OCTET* ImgIn, int frame_width, int frame_height, int taille_imagette, char* repertoireImagette, int nbImagette){
  // Création de la mosaïque de la frame courante

  OCTET* ImgOut;
  allocation_tableau(ImgOut, OCTET, frame_width*frame_height*3); 

  int nb_w = frame_width/taille_imagette; 
  int nb_h = frame_height/taille_imagette; 

  double moyenne_r, moyenne_g, moyenne_b;
  for(int i=0;i<nb_h;i++){
    for(int j=0;j<nb_w;j++){
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

      int indice=0;
      char* acc = (char*)nom[0].c_str();
      double stock = abs(moyenne_r-moy_r[0])+abs(moyenne_g-moy_g[0])+abs(moyenne_b-moy_b[0]);
      for(int b=1;b<nbImagette;b++){
        if(abs(moyenne_r-moy_r[b])+abs(moyenne_g-moy_g[b])+abs(moyenne_b-moy_b[b]) < stock){
          stock=abs(moyenne_r-moy_r[b])+abs(moyenne_g-moy_g[b])+abs(moyenne_b-moy_b[b]);
          acc=(char*)nom[b].c_str();
          indice=b;
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
  return ImgOut;  
}

int main(int argc, char* argv[])
{
  char cNomVideoLue[250], cNomVideoMosaique[250], cNomListeMoyenne[250], repertoireImagette[250];
  int taille_imagette, nbImagette, frame_skip;
  
  if (argc != 8) 
	{
		printf("Usage: VideoInitiale.mp4 VideoMosaique.mp4 ListeMoyenneImagettes TailleImagette NbImagette RepertoireImagettes FrameSkip \n"); 
		return 1;
	}

	sscanf (argv[1],"%s",cNomVideoLue) ;
  sscanf (argv[2],"%s",cNomVideoMosaique) ;
  sscanf (argv[3],"%s",cNomListeMoyenne) ;
  sscanf (argv[4],"%d",&taille_imagette) ;
  sscanf (argv[5],"%d",&nbImagette) ;
  sscanf (argv[6],"%s",repertoireImagette) ;
  sscanf (argv[7],"%d",&frame_skip) ;

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

  lireFichierMoyenne(cNomListeMoyenne);

  VideoWriter videoMosaique(cNomVideoMosaique, VideoWriter::fourcc('X', '2', '6', '4'), fps, Size(frame_width, frame_height), true);
  int nbFrame = 0;

  int frame_count = 0;

  OCTET* previousKeyMosaique;

  while (true) {
      Mat frame;
      if (!cap.read(frame)){
          break; // Toute les frames de la vidéo initiale ont été lu
      }
      if (frame_count%frame_skip==0){

        // previousKeyFrame = frame;

        OCTET *ImgIn = frameToImg(frame, frame_width, frame_height);
    
        OCTET* ImgOut = createColorMosaique(ImgIn, frame_width, frame_height, taille_imagette, repertoireImagette, nbImagette);
        
        if (frame_count != 0){
          // On fait l'interpolation entre previousKeyMosaique et ImgOut en frame_skip étapes, on génére donc frame_skip-1 images intermédiaires
          float pasEntreFrame = 1./frame_skip;
          for (int i = 1 ; i < frame_skip ; i++){ // frame_skip-1 images intermédiaires
            OCTET* imgIntermediaire;
            allocation_tableau(imgIntermediaire, OCTET, tailleFrame*3);
            float u = i*pasEntreFrame;
            for (int l = 0 ; l < frame_height ; l++){
              for (int c = 0 ; c < frame_width ; c++){
                imgIntermediaire[l*frame_width*3+c*3+ 0] = (1.0-u)*previousKeyMosaique[l*frame_width*3+c*3 + 0] + u*ImgOut[l*frame_width*3+c*3 + 0] ;
                imgIntermediaire[l*frame_width*3+c*3+ 1] = (1.0-u)*previousKeyMosaique[l*frame_width*3+c*3 + 1] + u*ImgOut[l*frame_width*3+c*3 + 1] ;
                imgIntermediaire[l*frame_width*3+c*3+ 2] = (1.0-u)*previousKeyMosaique[l*frame_width*3+c*3 + 2] + u*ImgOut[l*frame_width*3+c*3 + 2] ;
              }
            }
            Mat frameIntermediaire = imgToFrame(imgIntermediaire, frame_width, frame_height);
            videoMosaique.write(frameIntermediaire);
            std::cout << "Une nouvelle frame intermédiaire produite (" << nbFrame << ")\n";
            nbFrame++;
            
          }
        }

        previousKeyMosaique = ImgOut;

        Mat frameMosa = imgToFrame(ImgOut, frame_width, frame_height);

        videoMosaique.write(frameMosa);
        imshow("Frame", frameMosa);
        std::cout << "Une nouvelle frame clé produite (" << nbFrame << ")\n";
        nbFrame++;
      }

      frame_count++;

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
