import numpy as np
import cv2


def compute_histogram(image):
    # Calculer l'histogramme de chaque canal de l'image
    histograms = [np.histogram(image[:,:,i].flatten(), bins=256, range=[0,256])[0] for i in range(image.shape[2])]
    # Normaliser les histogrammes pour obtenir des distributions de probabilité
    histograms = [histogram / float(np.sum(histogram)) for histogram in histograms]
    return histograms

def compute_mi(image1, image2):
    # Calculer les histogrammes normalisés de chaque canal des deux images
    histograms1 = compute_histogram(image1)
    histograms2 = compute_histogram(image2)
    
    # Calculer les histogrammes conjoints normalisés de chaque paire de canaux des deux images
    joint_histograms = [np.histogram2d(image1[:,:,i].flatten(), image2[:,:,i].flatten(), bins=256, range=[[0, 256], [0, 256]])[0] 
                        for i in range(image1.shape[2])]
    joint_histograms = [joint_histogram / float(np.sum(joint_histogram)) for joint_histogram in joint_histograms]
    
    # Calculer le mutual information
    mi = 0
    for i in range(image1.shape[2]):
        for j in range(image2.shape[2]):
            for k in range(256):
                for l in range(256):
                    if joint_histograms[i][k, l] > 0 and histograms1[i][k] > 0 and histograms2[j][l] > 0:
                        mi += joint_histograms[i][k, l] * np.log(joint_histograms[i][k, l] / (histograms1[i][k] * histograms2[j][l]))
    return mi

# Charger les deux images que vous souhaitez comparer
image1 = cv2.imread('../dataImg/squirrel/squirrel.ppm')
image2 = cv2.imread('../dataImg/squirrel/squirrel_diff.ppm')
image3 = cv2.imread('../dataImg/squirrel/squirrel_utilisation_une.ppm')
image4 = cv2.imread('../dataImg/squirrel/squirrel_moyenne.ppm')
image5 = cv2.imread('../dataImg/squirrel/squirrel_mediane.ppm')

# Assurez-vous que les images ont la même taille
image1 = cv2.resize(image1, (image2.shape[1], image2.shape[0]))

# Calculer le MI entre les deux images
mi_value = compute_mi(image1, image2)
print("Mutual Information (MI): difference", mi_value)

mi_value_deux = compute_mi(image1, image3)
print("Mutual Information (MI): utilisation unique ", mi_value_deux)

mi_value_trois = compute_mi(image1, image4)
print("Mutual Information (MI): moyenne classique", mi_value_trois)

mi_value_quatre = compute_mi(image1, image5)
print("Mutual Information (MI): mediane", mi_value_quatre)
