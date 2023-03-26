#merge the images by operlapping on each other give the code
#the path of the images and the path of the output image
#the output image will be the merged image

from PIL import Image

# Open the two images
img1 = Image.open("./FIFO/FIFO1.png")
img2 = Image.open("./LRU/LRU1.png")
img3 = Image.open("./Random/Random1.png")

# Resize the images to have the same dimensions
img1 = img1.resize((800, 600))
img2 = img2.resize((800, 600))
img3=img3.resize((800,600))

# Create a new image with the same dimensions as the resized images
merged_img = Image.new("RGB", (800, 600))

# Paste the first image onto the new image
merged_img.paste(img1, (0, 0))

# Paste the second image onto the new image, with a 50% opacity
merged_img.paste(img2, (0, 0), mask=img2.convert("RGBA").split()[-1])

# Paste the third image onto the new image, with a 50% opacity

merged_img.paste(img3, (0, 0), mask=img3.convert("RGBA").split()[-1])

# Save the merged image
merged_img.save("merged.png")



