from skimage import io
from skimage.metrics import structural_similarity as ssim
import numpy as np
from scipy.stats import entropy
import sys

def mutual_information(hist2d):
    # Somme des histogrammes pour obtenir la probabilité conjointe
    joint_prob = hist2d / np.sum(hist2d)

    # Probabilité marginale pour chaque image
    x_prob = np.sum(joint_prob, axis=1)
    y_prob = np.sum(joint_prob, axis=0)

    # Entropie conjointe
    joint_entropy = -np.sum(joint_prob * np.log2(joint_prob + 1e-10))

    # Entropie marginale de chaque image
    x_entropy = -np.sum(x_prob * np.log2(x_prob + 1e-10))
    y_entropy = -np.sum(y_prob * np.log2(y_prob + 1e-10))

    # Information mutuelle
    mi = x_entropy + y_entropy - joint_entropy
    return mi





                            # 0 pour couleur, 1 pour ndg
img1 = io.imread(sys.argv[1], as_gray=sys.argv[3])
img2 = io.imread(sys.argv[2], as_gray=sys.argv[3])

data_range = 256.0

# SSIM
scoreSsim, _ = ssim(img1, img2, full=True, data_range=data_range)
print("SSIM : ", scoreSsim)

# MI
hist2d, x_edges, y_edges = np.histogram2d(img1.ravel(), img2.ravel(), bins=256)
mi = mutual_information(hist2d)
print("MI : ", mi)
