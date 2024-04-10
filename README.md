# Actuellement :
**Image mosaïque en niveau de gris uniquement**

Depuis *./Code/TP8_Image* :
**Pour redimensionner les imagettes** (dans répertoire *bash*) : ./redimension_image.sh repertoireEntree repertoireSortie tailleRedimension
> (Exemple : ./redimension.sh imagettes imagettesRedim 20)
	
**Pour lister toutes les imagettes redimensionnées dans un fichier** (dans répertoire *bash*): ./creation_liste_nom.sh repertoireEntree listeDesNoms
> (Exemple : ./creation_liste_nom.sh imagettesRedim listeNom.txt)
> Le fichier est créé dans le répertoire *bash*
		
**Pour calculer la moyenne de toutes les imagettes redimensionnées** (dans *src*) : moyenne_imagette.cpp produit le fichier listeResultat.txt
> (Cible du Makefile : 'make compileMoyenne' puis 'make moyenneImagette')
> Le fichier est créé dans le répertoire *bash*
		
**Pour créer l'image mosaïque** (dans *src*) : carte_moyenne.cpp produit l'image mosaïque + une image où chaque pixel est la moyenne d'une région (couverte par l'imagette) de l'image initiale
> (Cible du Makefile : 'make compileCarteMoyenne' puis 'make carteMoyenne', changer les paramètres dans le Makefile si nécéssaire)
> Attention à bien spécifier le nombre d'imagette dans le code, ainsi que leur taille
