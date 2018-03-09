from PIL import Image
img =  Image.open("tang.jpg")
new_img = img.resize((100,100),Image.BILINEAR)
new_img.save("resize_tang.jpg")
