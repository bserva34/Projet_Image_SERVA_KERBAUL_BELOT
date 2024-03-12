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
    char* directoryPath;
    GtkWidget *window;
    GtkWidget *label; // On associe chaque bouton à un label
} CallbackData;

CallbackData callbackData1;
CallbackData callbackData2;
CallbackData callbackData3;
CallbackData callbackData4;
CallbackData callbackData5;
GtkWidget *entry3;
GtkWidget *entry5;

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

void redimensionImagettes(GtkWidget *button, gpointer data) {
    CallbackData *callbackData = (CallbackData*)data;

    // g_strdup permet de convertir l'entrée saisie en char*
    int tailleRedimension = atoi(g_strdup(gtk_entry_get_text(GTK_ENTRY(entry3)))); // atoi() renvoie 0 si la conversion est impossible
    if (callbackData1.directoryPath != NULL && callbackData2.directoryPath != NULL && tailleRedimension != 0){

        char command[700];
        sprintf(command, "./bash/redimension_image.sh %s %s %d",callbackData1.directoryPath,callbackData2.directoryPath,tailleRedimension);
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
    if (callbackData4.directoryPath != NULL && fileNameOut[0] != '\0'){

        char command[700];
        sprintf(command, "./bash/creation_liste_nom.sh %s ./bash/%s",callbackData4.directoryPath, fileNameOut);
        system(command); // Exécute la commande

        char newLabel[300];
        sprintf(newLabel, "Les imagettes ont été listé dans %s", fileNameOut);
        gtk_label_set_text(GTK_LABEL(callbackData->label),newLabel);
    }else{
        char newLabel[300];
        sprintf(newLabel, "Impossible de lister, vérifier les informations saisies");
        gtk_label_set_text(GTK_LABEL(callbackData->label),newLabel);
    }
}

/*
void printDirectory(GtkWidget *button, gpointer data) {
    CallbackData *callbackData = (CallbackData*)data;
    char* directoryPath = callbackData->directoryPath;

    if (directoryPath != NULL){
        std::cout << "Directory = " << directoryPath << "\n";
    }else{
        std::cout << "Pas de chemin sélectionné\n";
    }
}
*/

int main(int argc, char **argv){
    gtk_init(&argc, &argv);

    // Définition de la fenêtre ----------------------------------------------------------------

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_maximize(GTK_WINDOW(window));
    gtk_window_set_title(GTK_WINDOW(window), "Projet Image - Mosaïque (LK / BS / MB)");
    
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GdkRGBA backgroundColor;
    backgroundColor.red = 33;
    backgroundColor.green = 33;
    backgroundColor.blue = 33;

    gtk_widget_override_background_color(window, GTK_STATE_FLAG_NORMAL, &backgroundColor);



    // Définition des éléments de la fenêtre ---------------------------------------------------

    // Tout les labels auront cette couleur
    GdkRGBA textColor;
    gdk_rgba_parse(&textColor, "white");

    // Element englobant qui permet de placer précisément les widgets
    GtkWidget *fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed);

    GtkWidget *labelStep1 = gtk_label_new("Etape 1 : Redimmensionner les imagettes si nécessaire");
    gtk_widget_override_color(labelStep1, GTK_STATE_FLAG_NORMAL, &textColor);
    gtk_fixed_put(GTK_FIXED(fixed), labelStep1, 50, 50);

    GtkWidget *button1 = gtk_button_new_with_label("Choisir le répertoire initiale des imagettes");
    gtk_fixed_put(GTK_FIXED(fixed), button1, 50, 80);
    GtkWidget *label1 = gtk_label_new("Pas de répertoire sélectionné");
    gtk_widget_override_color(label1, GTK_STATE_FLAG_NORMAL, &textColor);
    gtk_fixed_put(GTK_FIXED(fixed), label1, 50, 110);

    GtkWidget *button2 = gtk_button_new_with_label("Choisir le répertoire des imagettes redimensionnées");
    gtk_fixed_put(GTK_FIXED(fixed), button2, 50, 140);
    GtkWidget *label2 = gtk_label_new("Pas de répertoire sélectionné");
    gtk_widget_override_color(label2, GTK_STATE_FLAG_NORMAL, &textColor);
    gtk_fixed_put(GTK_FIXED(fixed), label2, 50, 170);

    entry3 = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry3), "Entrez la nouvelle taille des imagettes");
    gtk_entry_set_width_chars(GTK_ENTRY(entry3), 35);
    gtk_fixed_put(GTK_FIXED(fixed), entry3, 50, 200);
    GtkWidget *button3 = gtk_button_new_with_label("Redimensionner les imagettes");
    gtk_fixed_put(GTK_FIXED(fixed), button3, 400, 200);
    GtkWidget *label3 = gtk_label_new("");
    gtk_widget_override_color(label3, GTK_STATE_FLAG_NORMAL, &textColor);
    gtk_fixed_put(GTK_FIXED(fixed), label3, 50, 230);




    GtkWidget *labelStep2 = gtk_label_new("Etape 2 : Lister les imagettes dans un fichier texte");
    gtk_widget_override_color(labelStep2, GTK_STATE_FLAG_NORMAL, &textColor);
    gtk_fixed_put(GTK_FIXED(fixed), labelStep2, 50, 260);

    GtkWidget *button4 = gtk_button_new_with_label("Choisir le répertoire des imagettes à lister");
    gtk_fixed_put(GTK_FIXED(fixed), button4, 50, 290);
    GtkWidget *label4 = gtk_label_new("Pas de répertoire sélectionné");
    gtk_widget_override_color(label4, GTK_STATE_FLAG_NORMAL, &textColor);
    gtk_fixed_put(GTK_FIXED(fixed), label4, 50, 320);
    entry5 = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry5), "Entrez le nom du fichier qui va lister les imagettes");
    gtk_entry_set_width_chars(GTK_ENTRY(entry5), 35);
    gtk_fixed_put(GTK_FIXED(fixed), entry5, 50, 350);
    GtkWidget *button5 = gtk_button_new_with_label("Lister les imagettes");
    gtk_fixed_put(GTK_FIXED(fixed), button5, 400, 350);
    GtkWidget *label5 = gtk_label_new("");
    gtk_widget_override_color(label5, GTK_STATE_FLAG_NORMAL, &textColor);
    gtk_fixed_put(GTK_FIXED(fixed), label5, 50, 380);





    // Pour l'instant j'ai pas mieux que de créer un CallbackData différent pour chaque bouton (on pourait à la place utiliser un vector déclaré globalement, et donner juste un indice en paramètre des callback)
    callbackData1.window = window;
    callbackData1.directoryPath = NULL;
    callbackData1.label = label1;

    callbackData2.window = window;
    callbackData2.directoryPath = NULL;
    callbackData2.label = label2;

    callbackData3.window = window;
    callbackData3.directoryPath = NULL;
    callbackData3.label = label3;

    callbackData4.window = window;
    callbackData4.directoryPath = NULL;
    callbackData4.label = label4;

    callbackData5.window = window;
    callbackData5.directoryPath = NULL;
    callbackData5.label = label5;

    // Connecter le signal "clicked" des bouton aux callbacks
    g_signal_connect(button1, "clicked", G_CALLBACK(makeSelectDirectory), &callbackData1);
    g_signal_connect(button2, "clicked", G_CALLBACK(makeSelectDirectory), &callbackData2);
    g_signal_connect(button3, "clicked", G_CALLBACK(redimensionImagettes), &callbackData3);
    g_signal_connect(button4, "clicked", G_CALLBACK(makeSelectDirectory), &callbackData4);
    g_signal_connect(button5, "clicked", G_CALLBACK(listeImagettes), &callbackData5);
    // -----------------------------------------------------------------------------------------

    gtk_widget_show_all(window);
    
    gtk_main();

    return 0;
}