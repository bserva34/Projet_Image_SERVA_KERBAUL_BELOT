from skimage.metrics import structural_similarity as ssim
import cv2

# Charger les deux images que vous souhaitez comparer
image1 = cv2.imread('../dataImg/squirrel.ppm', cv2.IMREAD_GRAYSCALE)

image2 = cv2.imread('../dataImg/squirrel_diff.ppm', cv2.IMREAD_GRAYSCALE)
image3 = cv2.imread('../dataImg/squirrel_utilisation_une.ppm', cv2.IMREAD_GRAYSCALE)
image4 = cv2.imread('../dataImg/squirrel_moyenne.ppm', cv2.IMREAD_GRAYSCALE)

# Calculer le SSIM entre les deux images
ssim_value, _ = ssim(image1, image2, full=True)

ssim_value_deux, _ = ssim(image1, image3, full=True)

ssim_value_trois, _ = ssim(image1, image4, full=True)

print("SSIM: différence", ssim_value)

print("SSIM: utilisation unique", ssim_value_deux)

print("SSIM: moyenne classique", ssim_value_trois)
