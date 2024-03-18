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
  
  if (argc != 3) 
	{
		printf("Usage: ImageInitiale.ppm ImageMosaique.ppm \n"); 
		return 1;
	}

 

	sscanf (argv[1],"%s",cNomImgLue) ;
	sscanf (argv[2],"%s",cNomImgLue2) ;

  OCTET *ImgIn, *ImgIn2;
   
  lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;
  
  allocation_tableau(ImgIn, OCTET, nTaille*3);
  lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

  allocation_tableau(ImgIn2, OCTET, nTaille*3);
  lire_image_ppm(cNomImgLue2, ImgIn2, nH * nW);

  
  cout<<"psnr : "<<psnr_color(ImgIn,ImgIn2,nH,nW,nTaille)<<endl;
}