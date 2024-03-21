#include <stdio.h>
#include "../include/image_ppm.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgLue2[250];
  int nH, nW, nTaille;

 

  OCTET *ImgIn, *ImgIn2,*ImgIn3,*ImgIn4,*ImgIn5;
   
  lire_nb_lignes_colonnes_image_ppm("../dataimg/squirrel/squirrel.ppm", &nH, &nW);
  nTaille = nH * nW;
  
  allocation_tableau(ImgIn, OCTET, nTaille*3);
  lire_image_ppm("../dataimg/squirrel/squirrel.ppm", ImgIn, nH * nW);

  allocation_tableau(ImgIn2, OCTET, nTaille*3);
  lire_image_ppm("../dataimg/squirrel/squirrel_diff.ppm", ImgIn2, nH * nW);

  allocation_tableau(ImgIn3, OCTET, nTaille*3);
  lire_image_ppm("../dataimg/squirrel/squirrel_utilisation_une.ppm", ImgIn3, nH * nW);

  allocation_tableau(ImgIn4, OCTET, nTaille*3);
  lire_image_ppm("../dataimg/squirrel/squirrel_moyenne.ppm", ImgIn4, nH * nW);

  allocation_tableau(ImgIn5, OCTET, nTaille*3);
  lire_image_ppm("../dataimg/squirrel/squirrel_mediane.ppm", ImgIn5, nH * nW);

  
  cout<<"psnr diff : "<<psnr_color(ImgIn,ImgIn2,nH,nW,nTaille)<<endl;

  cout<<"psnr utilisation unique : "<<psnr_color(ImgIn,ImgIn3,nH,nW,nTaille)<<endl;

  cout<<"psnr moyenne classique : "<<psnr_color(ImgIn,ImgIn4,nH,nW,nTaille)<<endl;

  cout<<"psnr mediane : "<<psnr_color(ImgIn,ImgIn5,nH,nW,nTaille)<<endl;
}