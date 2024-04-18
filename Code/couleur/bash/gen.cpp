#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

int main(int argc, char* argv[]) {



    std::string liste_sortie = "occurence.dat";

    std::ofstream fichier_sortie(liste_sortie, std::ios::out);

    if (!fichier_sortie.is_open()) {
        std::cerr << "Erreur lors de l'ouverture du fichier de sortie." << std::endl;
        return 1;
    }

    std::string ligne;
    for(int i=0;i<41129;i++) {
        fichier_sortie << "0" << std::endl;


    }


    fichier_sortie.close();

    return 0;
}
