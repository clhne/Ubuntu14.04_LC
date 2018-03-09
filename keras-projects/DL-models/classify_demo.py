#-*-coding:utf-8-*-
from resnet50 import ResNet50
from keras.preprocessing import image
from imagenet_utils import preprocess_input,decode_predictions

model = ResNet50(weights='imagenet')

img_path = 'cropped_panda.jpg'
img = image.load_img(img_path,target_size = (224,224))
x = image.img_to_array(img)
x = np.expand_dims(x,axis = 0)
x = preprocess_input(x)

pred = model.predict(x)
print('Predicted:',decode_predictions(preds))
