# Notre interface graphique :
**Application C++ avec la librairie GTK**

Production d'image mosaïque en niveau de gris et en couleur, de même pour les vidéos.  

Pour lancer l'application (dans le répertoire *Interface*) :
> make  
> make run

3 étapes pour créer une mosaïque :  

**Redimensionner les imagettes (si nécessaire)**
Choisir le répertoire source (qui contient toutes les imagettes) et le répertoire cible (celui qui contiendra les imagettes redimensionnées). Indiquer aussi la taille du redimensionnement.  


**Lister les imagettes et calculer leurs moyennes :**
Choisir le répertoire des imagettes à lister, et saisir le nom du fichier. Si par exemple on saisit *liste*, alors 2 fichiers seront crées (dans le répertoire *bash*) :  
*liste.txt* qui liste simplement les imagettes  
*liste_moyennes.txt* qui donne pour chaques imagettes sa moyenne  

**Produire la mosaïque :**
Pour cette dernière étape, il faut :  
Renseigner le nombre d'imagettes qui seront disponible au total  
Donner la taille des imagettes  
Chosir l'image ou la vidéo à transformer en mosaïque  
Ecrire le nom du fichier qui sera produit
Définir le chemin vers le fichier des moyennes des imagettes (obtenu à l'étape précédente), ainsi que le chemin vers le répertoire des imagettes qui seront utilisées  
Pour les images uniquement, on peut mettre une restriction sur le nombre d'utilisation d'une même imagette (si l'option est cochée alors il faut saisir une valeur)  
Pour les vidéos uniquement, choisir l'une des 2 options suivantes (une à la fois seulement, ou aucune) :  
&emsp; Activer la production rapide. Il faut alors renseigner une valeur N, ainsi seul les N premières frames de la vidéo seront produites avec toutes les imagettes disponibles, les frames suivantes n'utiliseront ensuite que les imagettes déjà utilisées.  
&emsp; Utiliser la technique de l'interpolation. Il faut avoir renseigner une valeur N, ainsi entre 2 frames clés il y aura N-1 frames intermédiaires qui seront générées sans les imagettes  

Une fois terminé, appuyer sur le bouton tout en bas de l'application pour commencer la production de la mosaïque :  
&emsp; Pour une image, le résultat ainsi que l'image initiale s'afficheront à la fin du processus (il est nécesaire d'exécuter l'application sur une machine Linux où *Eye of Gnome* est disponible)  
&emsp; Pour une vidéo, les frames mosaïques générées seront affichées au fur et à mesure. Une fois le processus terminé, le résultat et la vidéo initiale se lanceront automatiquement (il est nécessaire d'exécuter l'application sur une machine Linux où MPlayer est disponible)  
