#!/bin/bash

# Vérifier si le nombre d'arguments est correct
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 chemin_dossier fichier_sortie"
    exit 1
fi

# Assigner les arguments aux variables
dossier="$1"
fichier_sortie="$2"

# Vérifier si le dossier existe
if [ ! -d "$dossier" ]; then
    echo "Le dossier $dossier n'existe pas."
    exit 1
fi

# Parcourir le dossier et écrire les noms de fichiers dans le fichier de sortie
find "$dossier" -type f -exec basename {} \; > "$fichier_sortie"

echo "Noms de fichiers stockés dans $fichier_sortie."
