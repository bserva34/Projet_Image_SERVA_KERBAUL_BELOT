#include <iostream>
#include <fstream>
#include <cstring>
#include "../include/image_ppm.h"

using namespace std;

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

int main(int argc, char* argv[]) {

    char nomListeNom[250], nomListeResultat[250];
    int nH, nW, nTaille, S1;

    if (argc != 3) 
	{
		printf("Usage: ListeNom ListeRésultat \n"); 
		return 1;
	}

	sscanf (argv[1],"%s",nomListeNom) ;
	sscanf (argv[2],"%s",nomListeResultat) ;

    OCTET *ImgIn, *ImgOut;

    std::string liste_nom = nomListeNom;
    std::fstream fichier_entree(liste_nom, std::ios::in);
    
    if (!fichier_entree.is_open()) {
        std::cerr << "Erreur lors de l'ouverture du fichier d'entrée." << std::endl;
        return 1;
    }

    std::string liste_sortie = nomListeResultat;

    std::ofstream fichier_sortie(liste_sortie, std::ios::out);

    if (!fichier_sortie.is_open()) {
        std::cerr << "Erreur lors de l'ouverture du fichier de sortie." << std::endl;
        return 1;
    }

    std::string ligne;
    while (std::getline(fichier_entree, ligne)) {
        char* acc = (char*)ligne.c_str();
        char* acc2 = "../../../../Master_Imagine_local/collection_imagette/couleur/";
        //char* acc2 = "imagettesRedim/";
        
        char* res = new char[strlen(acc) + strlen(acc2) + 1];
        strcpy(res, acc2);
        strcat(res, acc);

        OCTET *Imgacc;
        lire_nb_lignes_colonnes_image_ppm(res, &nH, &nW);
        nTaille = nH * nW;
        allocation_tableau(Imgacc, OCTET, nTaille*3);
        lire_image_ppm(res, Imgacc, nTaille);
        int mediane_r[256]={0};
        int mediane_g[256]={0};
        int mediane_b[256]={0};
        for (int i = 0; i < nTaille*3; i+=3) {
            mediane_r[Imgacc[i]]++;
            mediane_g[Imgacc[i+1]]++;
            mediane_b[Imgacc[i+2]]++;
        }

        fichier_sortie << ligne << "/" << mediane(mediane_r,nTaille/2)<< "/" << mediane(mediane_g,nTaille/2) << "/" << mediane(mediane_b,nTaille/2)<< std::endl;

        free(Imgacc);
    }

    fichier_entree.close();
    fichier_sortie.close();

    return 0;
}
