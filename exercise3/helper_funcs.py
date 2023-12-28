import numpy as np
import matplotlib.pyplot as plt
import os
import tensorflow as tf
from tensorflow import keras

def save_encoded_binary(encoded_imgs, output_file):
	with open(output_file, 'wb') as f:
		# flatten encoded images
		encoded_imgs = flatten_encoded(encoded_imgs)
		# write magic number
		f.write((0).to_bytes(4, byteorder='big'))
		# write number of images
		f.write((len(encoded_imgs)).to_bytes(4, byteorder='big'))
		# write number of rows
		f.write((1).to_bytes(4, byteorder='big'))
		# write number of columns
		f.write((encoded_imgs.shape[1]).to_bytes(4, byteorder='big'))
		# write data
		f.write(encoded_imgs.astype('float32').tobytes())

def flatten_encoded(encoded_imgs):
	return encoded_imgs.reshape(len(encoded_imgs), np.prod(encoded_imgs.shape[1:]))

def deflatten_encoded(encoded_imgs, shape):
	return encoded_imgs.reshape(encoded_imgs.shape[0], *shape)

def save_decoded_binary(decoded_imgs, output_file):
	with open(output_file, 'wb') as f:
		# write magic number
		f.write((0).to_bytes(4, byteorder='big'))
		# write number of images
		f.write((len(decoded_imgs)).to_bytes(4, byteorder='big'))
		# write number of rows
		f.write((28).to_bytes(4, byteorder='big'))
		# write number of columns
		f.write((28).to_bytes(4, byteorder='big'))
		# write data
		f.write(decoded_imgs.astype('float32').tobytes())

def load_dataset(dataset, dtype=np.uint8):
	with open(dataset, 'rb') as f:
		magic_number = int.from_bytes(f.read(4), byteorder='big')
		num_images = int.from_bytes(f.read(4), byteorder='big')
		rows = int.from_bytes(f.read(4), byteorder='big')
		cols = int.from_bytes(f.read(4), byteorder='big')
		data = np.fromfile(f, dtype=dtype)
		data = data.reshape(num_images, rows, cols)
	return data

def normalize(x):
	max_value = np.max(x)
	min_value = np.min(x)
	return (x - min_value) / (max_value - min_value)

def print_digits(decoded_imgs, x_test):
    n = 10  # How many digits we will display
    plt.figure(figsize=(20, 4))
    for i in range(n):
        # Display original
        ax = plt.subplot(2, n, i + 1)
        plt.imshow(x_test[i].reshape(28, 28))
        plt.gray()
        ax.get_xaxis().set_visible(False)
        ax.get_yaxis().set_visible(False)

        # Display reconstruction
        ax = plt.subplot(2, n, i + 1 + n)
        plt.imshow(decoded_imgs[i].reshape(28, 28))
        plt.gray()
        ax.get_xaxis().set_visible(False)
        ax.get_yaxis().set_visible(False)
    plt.show()