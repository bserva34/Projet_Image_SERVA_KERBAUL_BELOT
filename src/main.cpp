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

CallbackData callbackData1;
CallbackData callbackData2;
CallbackData callbackData3;
CallbackData callbackData4;
CallbackData callbackData5;
CallbackData callbackData6;
CallbackData callbackData7;
CallbackData callbackData8;
CallbackData callbackData9;
GtkWidget *entry3;
GtkWidget *entry5;
GtkWidget *entry6;
GtkWidget *entry7;
GtkWidget *entry8;

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
        // Créer la liste du nom de toutes les imagettes
        sprintf(command, "./bash/creation_liste_nom.sh %s ./bash/%s.txt",callbackData4.directoryPath, fileNameOut);
        system(command); 

        // Créer la liste contenant les moyennes de toutes les imagettes
        sprintf(command, "make compileMoyenne");
        system(command); 
        sprintf(command, "./moyenne bash/%s.txt bash/%s_moyennes.txt %s/",fileNameOut,fileNameOut,callbackData4.directoryPath);
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
    
    if (nbImagette != 0 && tailleImagette != 0 && fileNameOut[0] != '\0' && callbackData6.directoryPath != NULL && callbackData7.directoryPath != NULL && callbackData8.directoryPath != NULL){
        char command[700];

        sprintf(command, "make compileCarteMoyenne");
        system(command); 

        sprintf(command, "./carteMoyenne %s dataImg/%s.pgm %s %d %d %s/", callbackData6.directoryPath,fileNameOut,callbackData7.directoryPath,tailleImagette,nbImagette,callbackData8.directoryPath);
        system(command); 

        
        char newLabel[300];
        sprintf(newLabel, "L'image mosaïque a été créée dans %s.pgm", fileNameOut);
        gtk_label_set_text(GTK_LABEL(callbackData->label),newLabel);
    }else{
        char newLabel[300];
        sprintf(newLabel, "Impossible de créer la mosaïque, vérifier les informations saisies");
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




    GtkWidget *labelStep3 = gtk_label_new("Etape 3 : Créer l'image mosaïque");
    gtk_widget_override_color(labelStep3, GTK_STATE_FLAG_NORMAL, &textColor);
    gtk_fixed_put(GTK_FIXED(fixed), labelStep3, 50, 410);
    entry6 = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry6), "Nombre d'imagettes");
    gtk_entry_set_width_chars(GTK_ENTRY(entry6), 22);
    gtk_fixed_put(GTK_FIXED(fixed), entry6, 50, 440);
    entry7 = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry7), "Tailles des imagettes");
    gtk_entry_set_width_chars(GTK_ENTRY(entry7), 22);
    gtk_fixed_put(GTK_FIXED(fixed), entry7, 250, 440);
    GtkWidget *button6 = gtk_button_new_with_label("Choisir image initiale");
    gtk_fixed_put(GTK_FIXED(fixed), button6, 50, 480);
    GtkWidget *label6 = gtk_label_new("Aucune image choisie");
    gtk_widget_override_color(label6, GTK_STATE_FLAG_NORMAL, &textColor);
    gtk_fixed_put(GTK_FIXED(fixed), label6, 50, 510);
    entry8 = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry8), "Nom image mosaïque");
    gtk_entry_set_width_chars(GTK_ENTRY(entry8), 22);
    gtk_fixed_put(GTK_FIXED(fixed), entry8, 250, 480);
    GtkWidget *button7 = gtk_button_new_with_label("Fichier moyennes imagettes");
    gtk_fixed_put(GTK_FIXED(fixed), button7, 50, 540);
    GtkWidget *label7 = gtk_label_new("Aucune fichier choisie");
    gtk_widget_override_color(label7, GTK_STATE_FLAG_NORMAL, &textColor);
    gtk_fixed_put(GTK_FIXED(fixed), label7, 50, 570);
    GtkWidget *button8 = gtk_button_new_with_label("Choisir le répertoire des imagettes");
    gtk_fixed_put(GTK_FIXED(fixed), button8, 50, 600);
    GtkWidget *label8 = gtk_label_new("Aucune répertoire choisie");
    gtk_widget_override_color(label8, GTK_STATE_FLAG_NORMAL, &textColor);
    gtk_fixed_put(GTK_FIXED(fixed), label8, 50, 630);

    GtkWidget *button9 = gtk_button_new_with_label("Créer l'image mosaïque");
    gtk_fixed_put(GTK_FIXED(fixed), button9, 225, 700);
    GtkWidget *label9 = gtk_label_new("");
    gtk_widget_override_color(label9, GTK_STATE_FLAG_NORMAL, &textColor);
    gtk_fixed_put(GTK_FIXED(fixed), label9, 225, 730);



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

    callbackData6.window = window;
    callbackData6.directoryPath = NULL;
    callbackData6.label = label6;

    callbackData7.window = window;
    callbackData7.directoryPath = NULL;
    callbackData7.label = label7;

    callbackData8.window = window;
    callbackData8.directoryPath = NULL;
    callbackData8.label = label8;

    callbackData9.window = window;
    callbackData9.directoryPath = NULL;
    callbackData9.label = label9;

    // Connecter le signal "clicked" des bouton aux callbacks
    g_signal_connect(button1, "clicked", G_CALLBACK(makeSelectDirectory), &callbackData1);
    g_signal_connect(button2, "clicked", G_CALLBACK(makeSelectDirectory), &callbackData2);
    g_signal_connect(button3, "clicked", G_CALLBACK(redimensionImagettes), &callbackData3);
    g_signal_connect(button4, "clicked", G_CALLBACK(makeSelectDirectory), &callbackData4);
    g_signal_connect(button5, "clicked", G_CALLBACK(listeImagettes), &callbackData5);
    g_signal_connect(button6, "clicked", G_CALLBACK(makeSelectFile), &callbackData6);
    g_signal_connect(button7, "clicked", G_CALLBACK(makeSelectFile), &callbackData7);
    g_signal_connect(button8, "clicked", G_CALLBACK(makeSelectDirectory), &callbackData8);
    g_signal_connect(button9, "clicked", G_CALLBACK(makeMosaique), &callbackData9);

    // -----------------------------------------------------------------------------------------

    gtk_widget_show_all(window);
    
    gtk_main();

    return 0;
}