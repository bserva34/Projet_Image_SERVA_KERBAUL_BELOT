#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdlib> // Pour la fonction system() qui permet d'éxécuter des commandes

#include <gtkmm.h>

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

GtkWidget* image;

GtkWidget *grid;

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
                                                    GTK_WINDOW(data),
                                                    GTK_FILE_CHOOSER_ACTION_OPEN,
                                                    "Annuler", GTK_RESPONSE_CANCEL,
                                                    "Sélectionner", GTK_RESPONSE_ACCEPT,
                                                    NULL);

    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    
    if (result == GTK_RESPONSE_ACCEPT) {
        char *fp = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        if (fp != NULL) {
            if (callbackData->directoryPath != NULL) {
                g_free(callbackData->directoryPath);
            }
            callbackData->directoryPath = g_strdup(fp);
            char newLabel[300];
            sprintf(newLabel, "Le fichier sélectionné est : %s", fp);
            gtk_label_set_text(GTK_LABEL(callbackData->label),newLabel);
            g_free(fp);
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

        // Créer la liste contenant les moyennes de toutes les imagettes
        sprintf(command, "make compileMoyenne");
        system(command); 
        sprintf(command, "./moyenne bash/%s.txt bash/%s_moyennes.txt %s/",fileNameOut,fileNameOut,callbacks[3].directoryPath);
        system(command); 

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
    
    if (nbImagette != 0 && tailleImagette != 0 && fileNameOut[0] != '\0' && callbacks[5].directoryPath != NULL && callbacks[6].directoryPath != NULL && callbacks[7].directoryPath != NULL){
        char command[700];

        sprintf(command, "make compileCarteMoyenne");
        system(command); 

        sprintf(command, "./carteMoyenne %s dataImg/%s.pgm %s %d %d %s/", callbacks[5].directoryPath,fileNameOut,callbacks[6].directoryPath,tailleImagette,nbImagette,callbacks[7].directoryPath);
        system(command); 

        
        char newLabel[300];
        sprintf(newLabel, "L'image mosaïque a été créée dans %s.pgm", fileNameOut);
        gtk_label_set_text(GTK_LABEL(callbackData->label),newLabel);

        // Convertir l'image mosaïque en png et l'afficher sur la fenêtre (sans redimmensionner donc elle peut sortir de la fenêtre)
        sprintf(command, "convert dataImg/%s.pgm dataImg/%s.png",fileNameOut,fileNameOut);
        system(command); 
        sprintf(command, "dataImg/%s.png",fileNameOut);
        gtk_image_set_from_file(GTK_IMAGE(image), command);

        gtk_grid_attach(GTK_GRID(grid), image, 2, 2, 1,35); // Attention : Pour pas que l'image déplace tout, il faut qu'elle occupe plus de ligne (4è paramètre) que tout le reste réuni
        gtk_widget_show_all(grid);
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

    GdkRGBA backgroundColor;
    backgroundColor.red = 33;
    backgroundColor.green = 33;
    backgroundColor.blue = 33;

    gtk_widget_override_background_color(window, GTK_STATE_FLAG_NORMAL, &backgroundColor);

    // Tout les labels auront cette couleur
    GdkRGBA textColor;
    gdk_rgba_parse(&textColor, "white");

    for (int i = 0 ; i < 9 ; i++){ // Les textes par défaut des labels sont différents (il y en a 3 possible)
        GtkWidget *label;
        if (i==5 || i==6){
            label = gtk_label_new("Aucun fichier choisi");
        }else if (i == 2 || i == 4 || i == 8){
            label = gtk_label_new("");
        }else{
            label = gtk_label_new("Pas de répertoire sélectionné");
        }
        gtk_widget_override_color(label, GTK_STATE_FLAG_NORMAL, &textColor);
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
    gtk_widget_override_color(labelStep1, GTK_STATE_FLAG_NORMAL, &textColor);
    gtk_grid_attach(GTK_GRID(grid), labelStep1, 0, 0, 2, 1);

    GtkWidget *button1 = gtk_button_new_with_label("Choisir le répertoire initiale des imagettes");
    gtk_grid_attach(GTK_GRID(grid), button1, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), labels[0], 0, 2, 2, 1);

    GtkWidget *button2 = gtk_button_new_with_label("Choisir le répertoire des imagettes redimensionnées");
    gtk_grid_attach(GTK_GRID(grid), button2, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), labels[1], 0, 4, 2, 1);

    entry3 = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry3), "Entrez la nouvelle taille des imagettes");
    gtk_entry_set_width_chars(GTK_ENTRY(entry3), 35);
    gtk_grid_attach(GTK_GRID(grid), entry3, 0, 5, 1, 1);
    GtkWidget *button3 = gtk_button_new_with_label("Redimensionner les imagettes");
    gtk_grid_attach(GTK_GRID(grid), button3, 1, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), labels[2], 0, 6, 2, 1);


    GtkWidget *labelStep2 = gtk_label_new("Etape 2 : Lister les imagettes dans un fichier texte");
    gtk_widget_override_color(labelStep2, GTK_STATE_FLAG_NORMAL, &textColor);
    gtk_grid_attach(GTK_GRID(grid), labelStep2, 0, 8, 2, 1);

    GtkWidget *button4 = gtk_button_new_with_label("Choisir le répertoire des imagettes à lister");
    gtk_grid_attach(GTK_GRID(grid), button4, 0, 9, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), labels[3], 0, 10, 2, 1);
    entry5 = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry5), "Entrez le nom du fichier qui va lister les imagettes");
    gtk_entry_set_width_chars(GTK_ENTRY(entry5), 35);
    gtk_grid_attach(GTK_GRID(grid), entry5, 0, 11, 1, 1);
    GtkWidget *button5 = gtk_button_new_with_label("Lister les imagettes");
    gtk_grid_attach(GTK_GRID(grid), button5, 1, 11, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), labels[4], 0, 12, 2, 1);

    GtkWidget *labelStep3 = gtk_label_new("Etape 3 : Créer l'image mosaïque");
    gtk_widget_override_color(labelStep3, GTK_STATE_FLAG_NORMAL, &textColor);
    gtk_grid_attach(GTK_GRID(grid), labelStep3, 0, 14, 2, 1);
    entry6 = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry6), "Nombre d'imagettes");
    gtk_entry_set_width_chars(GTK_ENTRY(entry6), 22);
    gtk_grid_attach(GTK_GRID(grid), entry6, 0, 15, 1, 1);
    entry7 = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry7), "Tailles des imagettes");
    gtk_entry_set_width_chars(GTK_ENTRY(entry7), 22);
    gtk_grid_attach(GTK_GRID(grid), entry7, 1, 15, 1, 1);
    GtkWidget *button6 = gtk_button_new_with_label("Choisir image initiale");
    gtk_grid_attach(GTK_GRID(grid), button6, 0, 16, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), labels[5], 0, 17, 2, 1);
    entry8 = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry8), "Nom image mosaïque");
    gtk_entry_set_width_chars(GTK_ENTRY(entry8), 22);
    gtk_grid_attach(GTK_GRID(grid), entry8, 1, 16, 1, 1);
    GtkWidget *button7 = gtk_button_new_with_label("Fichier moyennes imagettes");
    gtk_grid_attach(GTK_GRID(grid), button7, 0, 18, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), labels[6], 0, 19, 2, 1);
    GtkWidget *button8 = gtk_button_new_with_label("Choisir le répertoire des imagettes");
    gtk_grid_attach(GTK_GRID(grid), button8, 0, 20, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), labels[7], 0, 21, 2, 1);

    GtkWidget *button9 = gtk_button_new_with_label("Créer l'image mosaïque");
    gtk_grid_attach(GTK_GRID(grid), button9, 0, 23, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), labels[8], 0, 24, 2, 1);

    image = gtk_image_new();

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