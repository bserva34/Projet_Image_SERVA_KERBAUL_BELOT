#!/bin/bash

# Vérifier si le nombre d'arguments est correct
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 chemin_dossier_source chemin_dossier_destination"
    exit 1
fi

# Assigner les chemins aux variables
dossier_source="$1"
dossier_destination="$2"

# Créer le dossier de destination s'il n'existe pas
mkdir -p "$dossier_destination"

# Boucler à travers les fichiers du dossier source
for fichier_source in "$dossier_source"/*; do
    # Vérifier si le fichier est une image
    if [ -f "$fichier_source" ]; then
        # Extraire le nom du fichier sans le chemin
        nom_fichier=$(basename "$fichier_source")

        # Utiliser ImageMagick pour redimensionner l'image
        convert "$fichier_source" -resize 4x4 "$dossier_destination/$nom_fichier"

        echo "Image redimensionnée : $nom_fichier"
    fi
done

echo "Redimensionnement terminé."