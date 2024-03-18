from skimage.metrics import structural_similarity as ssim
import cv2

# Charger les deux images que vous souhaitez comparer
image1 = cv2.imread('../dataImg/squirrel.ppm', cv2.IMREAD_GRAYSCALE)
image2 = cv2.imread('../dataImg/img_mosaique.ppm', cv2.IMREAD_GRAYSCALE)

# Calculer le SSIM entre les deux images
ssim_value, _ = ssim(image1, image2, full=True)

print("SSIM:", ssim_value)
