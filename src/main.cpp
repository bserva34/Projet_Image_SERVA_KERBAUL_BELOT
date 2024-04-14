#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <string>

#include <cstring>
#include <cstdlib> // Pour la fonction system() qui permet d'éxécuter des commandes

#include <gtkmm.h>
#include "../include/image_ppm.h"

// On ne peut passer qu'un seul paramètre dans une fonction de callback
typedef struct {
    char* directoryPath; // J'ai laissé directoryPath, mais on utilise aussi cette structure pour la sélection de fichier
    GtkWidget *window;
    GtkWidget *label; // On associe chaque bouton à un label
} CallbackData;

std::vector<CallbackData> callbacks;
std::vector<GtkWidget*> labels;

GtkWidget *entry3;
GtkWidget *entry5;
GtkWidget *entry6;
GtkWidget *entry7;
GtkWidget *entry8;
GtkWidget *entryNbUtilisation;
GtkWidget *checkboxNbUtilisation;
GtkWidget *entryNbFrameIntacte;
GtkWidget *checkboxFrameIntacte;

GtkWidget* labelPSNR; // Label sous l'image mosaïque qui donne le PSNR

GtkWidget *grid;

void on_checkbox_toggled(GtkToggleButton *togglebutton, gpointer entry) {
    gboolean active = gtk_toggle_button_get_active(togglebutton);
    gtk_widget_set_sensitive(GTK_WIDGET(entry), active);
}

void makeSelectDirectory(GtkWidget *button, gpointer data) {

    CallbackData *callbackData = (CallbackData*)data;

    GtkWidget *dialog = gtk_file_chooser_dialog_new("Sélectionner un répertoire",
                                                     GTK_WINDOW(callbackData->window),
                                                     GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                                                     "Annuler", GTK_RESPONSE_CANCEL,
                                                     "Sélectionner", GTK_RESPONSE_ACCEPT,
                                                     NULL);

    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    
    if (result == GTK_RESPONSE_ACCEPT) {
        char *dp = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        if (dp != NULL) {
            if (callbackData->directoryPath != NULL) {
                g_free(callbackData->directoryPath);
            }
            callbackData->directoryPath = g_strdup(dp);
            char newLabel[300];
            sprintf(newLabel, "Le répertoire sélectionné est : %s", dp);
            gtk_label_set_text(GTK_LABEL(callbackData->label),newLabel);
            g_free(dp);
        } else {
            g_print("Erreur : Impossible de récupérer le chemin du répertoire sélectionné.\n");
        }
    }

    gtk_widget_destroy(dialog);
}

void makeSelectFile(GtkWidget *button, gpointer data) {

    CallbackData *callbackData = (CallbackData*)data;

    GtkWidget *dialog = gtk_file_chooser_dialog_new("Sélectionner un fichier",
                                                     GTK_WINDOW(callbackData->window),
                                                     GTK_FILE_CHOOSER_ACTION_OPEN,
                                                     "Annuler", GTK_RESPONSE_CANCEL,
                                                     "Sélectionner", GTK_RESPONSE_ACCEPT,
                                                     NULL);

    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    
    if (result == GTK_RESPONSE_ACCEPT) {
        char *dp = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        if (dp != NULL) {
            if (callbackData->directoryPath != NULL) {
                g_free(callbackData->directoryPath);
            }
            callbackData->directoryPath = g_strdup(dp);
            char newLabel[300];
            sprintf(newLabel, "Le fichier sélectionné est : %s", dp);
            gtk_label_set_text(GTK_LABEL(callbackData->label),newLabel);
            g_free(dp);
        } else {
            g_print("Erreur : Impossible de récupérer le chemin du fichier sélectionné.\n");
        }
    }

    gtk_widget_destroy(dialog);
}

void redimensionImagettes(GtkWidget *button, gpointer data) {
    CallbackData *callbackData = (CallbackData*)data;

    // g_strdup permet de convertir l'entrée saisie en char*
    int tailleRedimension = atoi(g_strdup(gtk_entry_get_text(GTK_ENTRY(entry3)))); // atoi() renvoie 0 si la conversion est impossible
    if (callbacks[0].directoryPath != NULL && callbacks[1].directoryPath != NULL && tailleRedimension != 0){

        char command[700];
        sprintf(command, "./bash/redimension_image.sh %s %s %d",callbacks[0].directoryPath,callbacks[1].directoryPath,tailleRedimension);
        system(command); // Exécute la commande

        char newLabel[300];
        sprintf(newLabel, "Les imagettes ont été redimensionné en %dx%d", tailleRedimension, tailleRedimension);
        gtk_label_set_text(GTK_LABEL(callbackData->label),newLabel);
    }else{
        char newLabel[300];
        sprintf(newLabel, "Impossible de redimensionner, vérifier les informations saisies");
        gtk_label_set_text(GTK_LABEL(callbackData->label),newLabel);
    }
}

void listeImagettes(GtkWidget *button, gpointer data) {
    CallbackData *callbackData = (CallbackData*)data;

    char* fileNameOut = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry5)));
    if (callbacks[3].directoryPath != NULL && fileNameOut[0] != '\0'){

        char command[700];
        // Créer la liste du nom de toutes les imagettes
        sprintf(command, "./bash/creation_liste_nom.sh %s ./bash/%s.txt",callbacks[3].directoryPath, fileNameOut);
        system(command); 

        // On veut savoir si ce sont des images couleurs ou ndg
        std::string filePath = "./bash/";
        filePath += fileNameOut;
        filePath += ".txt";
        std::fstream inputFile(filePath, std::ios::in);

        std::string ligne;
        std::getline(inputFile, ligne);
        size_t length = ligne.length();
        char* acc = (char*)ligne.c_str();
        acc = acc + length - 3; // 3 derniers caractères
        inputFile.close();

        if (strcmp(acc, "pgm") == 0){ // Images pgm
            // Créer la liste contenant les moyennes de toutes les imagettes
            sprintf(command, "make compileMoyenne");
            system(command); 
            sprintf(command, "./moyenne bash/%s.txt bash/%s_moyennes.txt %s/",fileNameOut,fileNameOut,callbacks[3].directoryPath);
            system(command); 
        }else if (strcmp(acc, "ppm") == 0){ // Image ppm
            sprintf(command, "make compileMoyenneColor");
            system(command); 
            sprintf(command, "./moyenneColor bash/%s.txt bash/%s_moyennes.txt %s/",fileNameOut,fileNameOut,callbacks[3].directoryPath);
            system(command); 
        }else{
            std::cout << "Fichiers non reconnus\n";
            return;
        }

        char newLabel[300];
        sprintf(newLabel, "Les imagettes ont été listé dans %s_moyennes.txt", fileNameOut);
        gtk_label_set_text(GTK_LABEL(callbackData->label),newLabel);
    }else{
        char newLabel[300];
        sprintf(newLabel, "Impossible de lister, vérifier les informations saisies");
        gtk_label_set_text(GTK_LABEL(callbackData->label),newLabel);
    }
}

void makeMosaique(GtkWidget *button, gpointer data) {

    CallbackData *callbackData = (CallbackData*)data;
    
    int nbImagette = atoi(g_strdup(gtk_entry_get_text(GTK_ENTRY(entry6))));
    int tailleImagette = atoi(g_strdup(gtk_entry_get_text(GTK_ENTRY(entry7))));
    char* fileNameOut = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry8)));
    bool isCheckedNbReutilisation = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkboxNbUtilisation));
    int nbUtilisation  = -1;
    if (isCheckedNbReutilisation){
        nbUtilisation = atoi(g_strdup(gtk_entry_get_text(GTK_ENTRY(entryNbUtilisation))));
    }
    bool isCheckedOptimisationVideo = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkboxFrameIntacte));
    int nbFrameIntacte  = -1;
    if (isCheckedOptimisationVideo){
        nbFrameIntacte = atoi(g_strdup(gtk_entry_get_text(GTK_ENTRY(entryNbFrameIntacte))));
    }

    // On veut savoir si c'est une image/vidéo couleur ou ndg
    std::string filePath = callbacks[5].directoryPath;

    // Position du premier '.' rencontré dans le nom du fichier initiale
    size_t pos = filePath.find('.'); 
    char* acc = (char*)(filePath.substr(pos + 1, 3)).c_str(); // 3 premiers caractères suivants ce '.'

    bool isColor = true;
    bool isImage = true;

    char command1[700];
    char command2[700];
    char command3[700];
    char command4[700];
    if (strcmp(acc, "pgm") == 0){ // Images pgm
        isColor = false;
        sprintf(command1, "make compileCarteMoyenne");
        sprintf(command2, "./carteMoyenne %s dataImg/%s.pgm %s %d %d %s %d", callbacks[5].directoryPath,fileNameOut,callbacks[6].directoryPath,tailleImagette,nbImagette,callbacks[7].directoryPath,nbUtilisation);
        sprintf(command3, "eog dataImg/%s.pgm &",fileNameOut);
        sprintf(command4, "eog %s &",callbacks[5].directoryPath);
    }else if (strcmp(acc, "ppm") == 0){ // Image ppm
        sprintf(command1, "make compileCarteMoyenneColor");
        sprintf(command2, "./carteMoyenneColor %s dataImg/%s.ppm %s %d %d %s %d", callbacks[5].directoryPath,fileNameOut,callbacks[6].directoryPath,tailleImagette,nbImagette,callbacks[7].directoryPath,nbUtilisation);
        sprintf(command3, "eog dataImg/%s.ppm &",fileNameOut);
        sprintf(command4, "eog %s &",callbacks[5].directoryPath);
    }else if (strcmp(acc,"mp4") == 0){ // Vidéo mp4 (ndg ou couleur, on ne fait pas la différence)
        isImage = false;
        // On veut savoir si on traite une vidéo couleur ou ndg
        std::string imagettesPath = callbacks[6].directoryPath;
        std::fstream inputFile(imagettesPath, std::ios::in);
        std::string ligne;
        std::getline(inputFile, ligne);
        // Position du premeir '.' rencontré dans la ligne
        size_t typeImagettes = ligne.find('.'); 
        char* acc2 = (char*)(ligne.substr(typeImagettes + 1, 3)).c_str(); // 3 premiers caractères suivants ce '.'

        if (strcmp(acc2,"ppm") == 0){
            sprintf(command1, "make compileVideoCouleur");
            sprintf(command2, "./videoCouleur %s dataVideo/%s.mp4 %s %d %d %s %d", callbacks[5].directoryPath,fileNameOut,callbacks[6].directoryPath,tailleImagette,nbImagette,callbacks[7].directoryPath, nbFrameIntacte);
        }else if(strcmp(acc2,"pgm") == 0){
            sprintf(command1, "make compileVideoNDG");
            sprintf(command2, "./videoNdg %s dataVideo/%s.mp4 %s %d %d %s %d", callbacks[5].directoryPath,fileNameOut,callbacks[6].directoryPath,tailleImagette,nbImagette,callbacks[7].directoryPath, nbFrameIntacte);
        }else{
            std::cout << "Fichiers non reconnus\n";
            return;
        }
        sprintf(command3, "mplayer -loop 0 -fixed-vo dataVideo/%s.mp4 &",fileNameOut);
        sprintf(command4, "mplayer -loop 0 -fixed-vo %s &",callbacks[5].directoryPath);
        
    }else{
        std::cout << "Fichiers non reconnus\n";
        return;
    }
    
    if (nbImagette != 0 && tailleImagette != 0 && fileNameOut[0] != '\0' && callbacks[5].directoryPath != NULL && callbacks[6].directoryPath != NULL && callbacks[7].directoryPath != NULL && ((!isCheckedNbReutilisation) || (isCheckedNbReutilisation && nbUtilisation != 0)) && ((!isCheckedOptimisationVideo) || (isCheckedOptimisationVideo && nbFrameIntacte > 0)) ){
        if (isCheckedNbReutilisation){
            int nW, nH;
            if (!isColor){
                lire_nb_lignes_colonnes_image_pgm(callbacks[5].directoryPath, &nH, &nW); // On lit les dimensions de l'image initiale
            }else{
                lire_nb_lignes_colonnes_image_ppm(callbacks[5].directoryPath, &nH, &nW); // On lit les dimensions de l'image initiale
            }
            if(nbImagette * nbUtilisation < pow(nH/tailleImagette,2)){ // Si le nombre maximale d'imagette que l'on peut utiiser (en tenant compte de la restriction si elle existe) est inférieur au nombre d'imagette nécessaire, alors on ne peut pas faire l'image mosaïque
                char newLabel[300];
                sprintf(newLabel, "Il n'y a pas suffisamment d'imagettes, ou alors il faut augmenter le nombre maximal de réutilisations");
                gtk_label_set_text(GTK_LABEL(callbackData->label),newLabel);
                return;
            }
        }

        system(command1); 
        system(command2); 

        
        char newLabel[300];
        sprintf(newLabel, "L'image/vidéo mosaïque a été créée dans %s", fileNameOut);
        gtk_label_set_text(GTK_LABEL(callbackData->label),newLabel);

        // Afficher l'image initiale l'image mosaïque (grâce au visionneur d'image) l'image mosaïque
        system(command4); 
        system(command3); 

        if (isImage){ // Attention à ne pas calculer le PSNR pour les vidéos

            // Calcul du PSNR
            OCTET *ImgIn, *ImgOut;
            char cheminImgOut[200];
            int tailleImg;
            int height, width;
            if (!isColor){
                lire_nb_lignes_colonnes_image_pgm(callbacks[5].directoryPath, &height, &width);
                tailleImg = height * width;
                sprintf(cheminImgOut, "dataImg/%s.pgm",fileNameOut);
            }else{
                lire_nb_lignes_colonnes_image_ppm(callbacks[5].directoryPath, &height, &width);
                tailleImg = height * width;
                sprintf(cheminImgOut, "dataImg/%s.ppm",fileNameOut);
            }


            double psnr;
            if (isColor){
                allocation_tableau(ImgIn, OCTET, tailleImg*3);
                lire_image_ppm(callbacks[5].directoryPath, ImgIn, tailleImg);
                allocation_tableau(ImgOut, OCTET, tailleImg*3);
                lire_image_ppm(cheminImgOut, ImgOut, tailleImg);
                psnr = psnr_color(ImgIn,ImgOut,width,height,width*height);
            }else{
                allocation_tableau(ImgIn, OCTET, tailleImg);
                lire_image_pgm(callbacks[5].directoryPath, ImgIn, tailleImg);
                allocation_tableau(ImgOut, OCTET, tailleImg);
                lire_image_pgm(cheminImgOut, ImgOut, tailleImg);
                psnr = psnr_ndg(ImgIn,ImgOut,width,height);
            }
            char indicationPSNR[300];
            sprintf(indicationPSNR, "PSNR entre l'image initiale et l'image mosaïque = %f dB", psnr);
            gtk_label_set_text(GTK_LABEL(labelPSNR),indicationPSNR);
            gtk_grid_attach(GTK_GRID(grid), labelPSNR, 2, 0, 2, 1);
            gtk_widget_show_all(grid);
        }

    }else{
        char newLabel[300];
        sprintf(newLabel, "Impossible de créer la mosaïque, vérifier les informations saisies");
        gtk_label_set_text(GTK_LABEL(callbackData->label),newLabel);
    }
}

int main(int argc, char **argv){
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_maximize(GTK_WINDOW(window));
    gtk_window_set_title(GTK_WINDOW(window), "Projet Image - Mosaïque (LK / BS / MB)");
    
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // On utilise du CSS pour la couleur de fond de la fenêtre et la couleur des labels
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "style/styles.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);

    for (int i = 0 ; i < 9 ; i++){ // Les textes par défaut des labels sont différents (il y en a 3 possible)
        GtkWidget *label;
        if (i==5 || i==6){
            label = gtk_label_new("Aucun fichier choisi");
        }else if (i == 2 || i == 4 || i == 8){
            label = gtk_label_new("");
        }else{
            label = gtk_label_new("Pas de répertoire sélectionné");
        }
        gtk_widget_set_name(label, "labelSimple");
        labels.push_back(label);
    }

    for (int i = 0 ; i < 9 ; i++){
        CallbackData c;
        c.window = window;
        c.directoryPath = NULL;
        c.label = labels[i];
        callbacks.push_back(c);
    }

    // Element englobant qui permet de placer selon des numéros de ligne et de colonne
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    gtk_grid_set_column_spacing(GTK_GRID(grid), 15);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);

    // Définition des éléments de la fenêtre ---------------------------------------------------

    GtkWidget *labelStep1 = gtk_label_new("Etape 1 : Redimmensionner les imagettes si nécessaire");
    gtk_widget_set_name(labelStep1, "labelStep");
    gtk_grid_attach(GTK_GRID(grid), labelStep1, 0, 0, 2, 1);

    GtkWidget *button1 = gtk_button_new_with_label("Choisir le répertoire initiale des imagettes");
    gtk_widget_set_name(button1, "bID");
    gtk_grid_attach(GTK_GRID(grid), button1, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), labels[0], 0, 2, 2, 1);

    GtkWidget *button2 = gtk_button_new_with_label("Choisir le répertoire des imagettes redimensionnées");
    gtk_widget_set_name(button2, "bID");
    gtk_grid_attach(GTK_GRID(grid), button2, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), labels[1], 0, 4, 2, 1);

    entry3 = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry3), "Entrez la nouvelle taille des imagettes");
    gtk_entry_set_width_chars(GTK_ENTRY(entry3), 35);
    gtk_grid_attach(GTK_GRID(grid), entry3, 0, 5, 1, 1);
    GtkWidget *button3 = gtk_button_new_with_label("Redimensionner les imagettes");
    gtk_widget_set_name(button3, "bID");
    gtk_grid_attach(GTK_GRID(grid), button3, 1, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), labels[2], 0, 6, 2, 1);


    GtkWidget *labelStep2 = gtk_label_new("Etape 2 : Lister les imagettes dans un fichier texte");
    gtk_widget_set_name(labelStep2, "labelStep");
    gtk_grid_attach(GTK_GRID(grid), labelStep2, 0, 8, 2, 1);

    GtkWidget *button4 = gtk_button_new_with_label("Choisir le répertoire des imagettes à lister");
    gtk_widget_set_name(button4, "bID");
    gtk_grid_attach(GTK_GRID(grid), button4, 0, 9, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), labels[3], 0, 10, 2, 1);
    entry5 = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry5), "Entrez le nom du fichier qui va lister les imagettes");
    gtk_entry_set_width_chars(GTK_ENTRY(entry5), 35);
    gtk_grid_attach(GTK_GRID(grid), entry5, 0, 11, 1, 1);
    GtkWidget *button5 = gtk_button_new_with_label("Lister les imagettes");
    gtk_widget_set_name(button5, "bID");
    gtk_grid_attach(GTK_GRID(grid), button5, 1, 11, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), labels[4], 0, 12, 2, 1);

    GtkWidget *labelStep3 = gtk_label_new("Etape 3 : Créer l'image/la vidéo mosaïque");
    gtk_widget_set_name(labelStep3, "labelStep");
    gtk_grid_attach(GTK_GRID(grid), labelStep3, 0, 14, 2, 1);
    entry6 = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry6), "Nombre d'imagettes");
    gtk_entry_set_width_chars(GTK_ENTRY(entry6), 22);
    gtk_grid_attach(GTK_GRID(grid), entry6, 0, 15, 1, 1);
    entry7 = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry7), "Tailles des imagettes");
    gtk_entry_set_width_chars(GTK_ENTRY(entry7), 22);
    gtk_grid_attach(GTK_GRID(grid), entry7, 1, 15, 1, 1);
    GtkWidget *button6 = gtk_button_new_with_label("Choisir image/vidéo initiale");
    gtk_widget_set_name(button6, "bID");
    gtk_grid_attach(GTK_GRID(grid), button6, 0, 16, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), labels[5], 0, 17, 2, 1);
    entry8 = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry8), "Nom image/vidéo mosaïque");
    gtk_entry_set_width_chars(GTK_ENTRY(entry8), 22);
    gtk_grid_attach(GTK_GRID(grid), entry8, 1, 16, 1, 1);
    GtkWidget *button7 = gtk_button_new_with_label("Fichier moyennes imagettes");
    gtk_widget_set_name(button7, "bID");
    gtk_grid_attach(GTK_GRID(grid), button7, 0, 18, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), labels[6], 0, 19, 2, 1);
    GtkWidget *button8 = gtk_button_new_with_label("Choisir le répertoire des imagettes");
    gtk_widget_set_name(button8, "bID");
    gtk_grid_attach(GTK_GRID(grid), button8, 0, 20, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), labels[7], 0, 21, 2, 1);

    entryNbUtilisation = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entryNbUtilisation), "Cochez pour modifier");
    gtk_widget_set_sensitive(GTK_WIDGET(entryNbUtilisation), false); // Désactive initialement l'entry comme la checkbox n'est pas coché
    gtk_grid_attach(GTK_GRID(grid), entryNbUtilisation, 0, 23, 1, 1);
    checkboxNbUtilisation = gtk_check_button_new_with_label("Limiter le nombre de réutilisation (pour les images uniquement)");
    g_signal_connect(checkboxNbUtilisation, "toggled", G_CALLBACK(on_checkbox_toggled), entryNbUtilisation);
    gtk_grid_attach(GTK_GRID(grid), checkboxNbUtilisation, 0, 22, 1, 1);

    entryNbFrameIntacte = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entryNbFrameIntacte), "Cochez pour modifier");
    gtk_widget_set_sensitive(GTK_WIDGET(entryNbFrameIntacte), false); // Désactive initialement l'entry comme la checkbox n'est pas coché
    gtk_grid_attach(GTK_GRID(grid), entryNbFrameIntacte, 1, 23, 1, 1);
    checkboxFrameIntacte = gtk_check_button_new_with_label("Activer la production rapide (pour les vidéos uniquement)");
    g_signal_connect(checkboxFrameIntacte, "toggled", G_CALLBACK(on_checkbox_toggled), entryNbFrameIntacte);
    gtk_grid_attach(GTK_GRID(grid), checkboxFrameIntacte, 1, 22, 1, 1);

    GtkWidget *button9 = gtk_button_new_with_label("Créer l'image mosaïque");
    gtk_widget_set_name(button9, "bID");
    gtk_grid_attach(GTK_GRID(grid), button9, 0, 24, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), labels[8], 0, 25, 1, 1);

    labelPSNR = gtk_label_new("");
    gtk_widget_set_name(labelPSNR, "labelSimple");

    // Connecter le signal "clicked" des bouton aux callbacks
    g_signal_connect(button1, "clicked", G_CALLBACK(makeSelectDirectory),&(callbacks[0]));
    g_signal_connect(button2, "clicked", G_CALLBACK(makeSelectDirectory),&(callbacks[1]));
    g_signal_connect(button3, "clicked", G_CALLBACK(redimensionImagettes),&(callbacks[2]));
    g_signal_connect(button4, "clicked", G_CALLBACK(makeSelectDirectory),&(callbacks[3]));
    g_signal_connect(button5, "clicked", G_CALLBACK(listeImagettes),&(callbacks[4]));
    g_signal_connect(button6, "clicked", G_CALLBACK(makeSelectFile),&(callbacks[5]) );
    g_signal_connect(button7, "clicked", G_CALLBACK(makeSelectFile),&(callbacks[6]));
    g_signal_connect(button8, "clicked", G_CALLBACK(makeSelectDirectory),&(callbacks[7]));
    g_signal_connect(button9, "clicked", G_CALLBACK(makeMosaique),&(callbacks[8]));

    // -----------------------------------------------------------------------------------------

    gtk_widget_show_all(window);
    
    gtk_main();

    return 0;
}