#include <iostream>
#include <fstream>
#include <cstring>
#include "../include/image_ppm.h"

using namespace std;

int main(int argc, char* argv[]) {

    char nomListeNom[250], nomListeResultat[250];
    char cheminImagettes[250];

    int nH, nW, nTaille, S1;

    if (argc != 4) 
	{
		printf("Usage: ListeNom ListeRésultat CheminImagettes \n"); 
		return 1;
	}

	sscanf (argv[1],"%s",nomListeNom) ;
	sscanf (argv[2],"%s",nomListeResultat) ;
    sscanf (argv[3],"%s",cheminImagettes) ;

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

        if (strcmp(acc, ".gitkeep") == 0) {
            continue;
        } 
        
        char* res = new char[strlen(acc) + strlen(cheminImagettes) + 1];
        strcpy(res, cheminImagettes);
        strcat(res, acc);

        OCTET *Imgacc;
        lire_nb_lignes_colonnes_image_pgm(res, &nH, &nW);
        nTaille = nH * nW;
        allocation_tableau(Imgacc, OCTET, nTaille);
        lire_image_pgm(res, Imgacc, nTaille);
        double moyenne_imagette = 0;
        for (int i = 0; i < nTaille; i++) {
            moyenne_imagette += Imgacc[i];
        }
        moyenne_imagette = moyenne_imagette / nTaille;

        fichier_sortie << ligne << "/" << moyenne_imagette << std::endl;

        free(Imgacc);
    }

    fichier_entree.close();
    fichier_sortie.close();

    return 0;
}