// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "../include/image_ppm.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
  

int nb_max_utilisation = 1;

bool deja_utiliser(std::vector<int> &used,int check){
  if(used[check]>=nb_max_utilisation){
    return false;
  }
  return true;
}

int mediane(int tab[256],int val){
  int acc=0;
  for(int i=0;i<256;i++){
    acc+=tab[i];
    if(acc>=val){
      return i;
    }
  }
  return -1;
}

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgBloc[250], cNomImgMosaique[250], cNomListeMoyenne[250];
  int nH, nW, nTaille, S1;
  
  if (argc != 5) 
  {
    printf("Usage: ImageInitiale.ppm ImageBloc.ppm ImageMosaique.ppm ListeMedianeImagettes \n"); 
    return 1;
  }

  int nbImagette = 41129;
  //int nbImagette = 10000; // Nombre d'imagette disponible, à adapter
  cout<<"nb img : "<<nbImagette<<endl;

  sscanf (argv[1],"%s",cNomImgLue) ;
  sscanf (argv[2],"%s",cNomImgBloc) ;
  sscanf (argv[3],"%s",cNomImgMosaique) ;
  sscanf (argv[4],"%s",cNomListeMoyenne) ;

  OCTET *ImgIn, *ImgOut,*ImgOut2;
   
  lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;
  
  allocation_tableau(ImgIn, OCTET, nTaille*3);
  lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

  int taille_imagette=32; // Taille des imagettes (carrée), à adapter
  int val_mediane=(taille_imagette*taille_imagette)/2;

  int nb=nH/taille_imagette;
  allocation_tableau(ImgOut, OCTET, nb*nb*3);
  allocation_tableau(ImgOut2, OCTET, nTaille*3);

  string nom[nbImagette];
  double med_r[nbImagette];
  double med_g[nbImagette];
  double med_b[nbImagette];
  std::vector<int> used(nbImagette,0);
  


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
    med_r[cpt]=val_r;
    med_g[cpt]=val_g;
    med_b[cpt]=val_b;
    cpt++;

    //affichage pour voir si les infos sont bien récupéré
    //cout<<partieAvantSlash<<val_r<<val_g<<val_b<<endl;
  }
  fichier.close();

  
  for(int i=0;i<nb;i++){
    for(int j=0;j<nb;j++){
      int mediane_r[256]={0};
      int mediane_g[256]={0};
      int mediane_b[256]={0};
      for(int x = 0; x < taille_imagette; x ++) {
            for(int y = 0; y < taille_imagette; y ++) {
                int pixel_index = ((i * taille_imagette) + x) * nW * 3 + ((j * taille_imagette) + y) * 3;
                mediane_r[ImgIn[pixel_index]]++;
                mediane_g[ImgIn[pixel_index+1]]++;
                mediane_b[ImgIn[pixel_index+2]]++;
            }
        }
      int mr=mediane(mediane_r,val_mediane);
      int mg=mediane(mediane_g,val_mediane);
      int mb=mediane(mediane_b,val_mediane);
      ImgOut[(i * nb + j) * 3] = mr;
      ImgOut[(i * nb + j) * 3 + 1] = mg; 
      ImgOut[(i * nb + j) * 3 + 2] = mb;

      int indice= 0;
      char* acc = (char*)nom[0].c_str();;
      double stock=med_r[0]+med_g[0]+med_b[0];
      for(int b=0;b<nbImagette;b++){
        //if(deja_utiliser(used,b)){
          int diff=abs(mr-med_r[b])+abs(mg-med_g[b])+abs(mb-med_b[b]);
          if(diff<stock){
            stock=diff;
            acc=(char*)nom[b].c_str();
            indice=b;
          } 
        //}       
      }
      
      char* acc2 = "../../../../Master_Imagine_local/collection_imagette/couleur/";
      //char* acc2 = "imagettesRedim/";
      char* res = new char[strlen(acc) + strlen(acc2) + 1];
      strcpy(res, acc2);
      strcat(res, acc);

      OCTET * Imgacc;
      allocation_tableau(Imgacc, OCTET,taille_imagette*taille_imagette*3);
      lire_image_ppm(res,Imgacc,taille_imagette*taille_imagette);
      for (int z = 0; z < taille_imagette; z ++ ) {
        for (int k = 0; k < taille_imagette; k ++) {
            int pixel_index_out = ((i * taille_imagette) + z) * nW * 3 + ((j * taille_imagette) + k) * 3;
            int pixel_index_acc = (z * taille_imagette * 3) + k * 3;
            ImgOut2[pixel_index_out] = Imgacc[pixel_index_acc];
            ImgOut2[pixel_index_out + 1] = Imgacc[pixel_index_acc + 1];
            ImgOut2[pixel_index_out + 2] = Imgacc[pixel_index_acc + 2];
        }
      }

      free(Imgacc);
      //cout<<i*nW+j<<endl;
      used[indice]++;
    }
  }    
  
  ecrire_image_ppm(cNomImgBloc,ImgOut,nb,nb);
  ecrire_image_ppm(cNomImgMosaique,ImgOut2,nH,nW);
  free(ImgIn); free(ImgOut);

  return 1;
}
