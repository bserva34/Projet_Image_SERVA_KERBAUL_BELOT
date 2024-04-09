#!/bin/bash

if [ "$#" -ne 3 ]; then
    echo "Usage: $0 chemin_dossier_source chemin_dossier_destination taille_redimensionnement"
    exit 1
fi

dossier_source="$1"
dossier_destination="$2"
taille_redimension=$3

mkdir -p "$dossier_destination"

for fichier_source in "$dossier_source"/*; do
    if [ -f "$fichier_source" ]; then
        nom_fichier=$(basename "$fichier_source")
        convert "$fichier_source" -resize $taille_redimensionx$taille_redimension "$dossier_destination/$nom_fichier"

    fi
done