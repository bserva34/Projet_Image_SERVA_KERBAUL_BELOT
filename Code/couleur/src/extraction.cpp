#include <iostream>
#include <fstream>
#include <vector>
#include "../include/image_ppm.h"
#include <filesystem>

using namespace std;

typedef unsigned char OCTET;

void read_binary_file(const string& filename, vector<OCTET>& data) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Erreur lors de l'ouverture du fichier : " << filename << endl;
        exit(EXIT_FAILURE);
    }

    file.seekg(0, ios::end);
    size_t size = file.tellg();
    file.seekg(0, ios::beg);

    data.resize(size);
    file.read(reinterpret_cast<char*>(&data[0]), size);

    file.close();
}

void creer_images_pgm(const string& nomFichierBinaire, const string& nomFichierMeta, const string& repertoireSortie) {
    vector<OCTET> donneesBinaires;
    read_binary_file(nomFichierBinaire, donneesBinaires);

    ifstream fichierMeta(nomFichierMeta);
    if (!fichierMeta) {
        cerr << "Erreur lors de l'ouverture du fichier : " << nomFichierMeta << endl;
        exit(EXIT_FAILURE);
    }

    // Lire les noms de classe
    vector<string> nomsClasses;
    string nomClasse;
    while (getline(fichierMeta, nomClasse)) {
        nomsClasses.push_back(nomClasse);
    }
    fichierMeta.close();

    // Traiter les données binaires
    int nombreImages = donneesBinaires.size() / 3073; // Chaque ligne est de 3073 octets
    int tailleImage = 3072; // 1024 rouges + 1024 verts + 1024 bleus

    for (int i = 0; i < nombreImages; ++i) {
        OCTET etiquette = donneesBinaires[i * 3073];
        vector<OCTET> donneesPixel(tailleImage);
        copy(donneesBinaires.begin() + i * 3073 + 1, donneesBinaires.begin() + (i + 1) * 3073, donneesPixel.begin());

        OCTET * Imgacc;
        allocation_tableau(Imgacc, OCTET,1024*3);
        for(int j=0;j<1024;j++){
            Imgacc[j*3]=donneesPixel[j];
            Imgacc[j*3+1]=donneesPixel[j+1024];
            Imgacc[j*3+1]=donneesPixel[j+2048];
        }

        // Créer un fichier image PPM
        string nomFichierPPM = repertoireSortie + "/image_" + to_string(i) + "_" + nomsClasses[etiquette] + ".ppm";
        ecrire_image_ppm((char*)nomFichierPPM.c_str(), Imgacc, 32, 32); // Supposant que les images sont de taille 32x32

        // En option, vous pouvez créer des images PGM séparées pour chaque canal de couleur
        // Décommentez les lignes suivantes pour créer des images PGM pour chaque canal de couleur
        /*
        ecrire_image_pgm(((char*)(repertoireSortie + "/image_" + to_string(i) + "_rouge.pgm").c_str()), &donneesPixel[0], 32, 32);
        ecrire_image_pgm(((char*)(repertoireSortie + "/image_" + to_string(i) + "_vert.pgm").c_str()), &donneesPixel[1024], 32, 32);
        ecrire_image_pgm(((char*)(repertoireSortie + "/image_" + to_string(i) + "_bleu.pgm").c_str()), &donneesPixel[2048], 32, 32);*/
        
    }
}



int main() {
    // Spécifiez le chemin du répertoire où vous souhaitez enregistrer les images
    string repertoireSortie = "../../../../Master_Imagine_local/collection_imagette/couleur_10000/";

    // Spécifiez le nom du fichier binaire et du fichier meta
    string nomFichierBinaire = "../../../../Master_Imagine_local/collection_imagette/cifar-10-batches-bin/data_batch_1.bin"; // Mettez à jour avec votre nom de fichier réel
    string nomFichierMeta = "../../../../Master_Imagine_local/collection_imagette/cifar-10-batches-bin/batches.meta.txt"; // Mettez à jour avec votre nom de fichier réel

    creer_images_pgm(nomFichierBinaire, nomFichierMeta, repertoireSortie);

    return 0;
}
