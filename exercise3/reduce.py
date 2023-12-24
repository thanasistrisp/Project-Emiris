from autoencoder import Autoencoder
from helper_funcs import *

import os

import tensorflow as tf

import keras
from keras import layers

from tensorflow.keras.models import save_model, load_model

from sklearn.utils import shuffle
import numpy as np

import matplotlib.pyplot as plt

import argparse

# handle command line arguments
parser = argparse.ArgumentParser()
parser.add_argument('-d', '--dataset', help='dataset', type=str, default='MNIST/input.dat')
parser.add_argument('-q', '--query', help='queryset', type=str, default='MNIST/query.dat')
parser.add_argument('-od', '--output_dataset', help='output_dataset_file', type=str, default='output.dat')
parser.add_argument('-oq', '--output_query', help='output_query_file', type=str, default='query.dat')
parser.add_argument('-m', '--model', help='model_file', type=str, default='models/model_conv_46.keras')
parser.add_argument('-t', '--type', help='model_type', type=str, default='conv')

args = parser.parse_args()
dataset = args.dataset
query = args.query
output_dataset = args.output_dataset
output_query = args.output_query
model = args.model

# load model
autoencoder = load_model(model)

# load dataset
x_test = load_dataset(query)
x_test = x_test.astype('float32') / 255.
if args.type == 'conv':
	x_test = np.reshape(x_test, (len(x_test), 28, 28, 1))
else:
	x_test = np.reshape(x_test, (len(x_test), 784))

x_test = x_test[:1000]
x_test1 = x_test[:500]
x_test2 = x_test[500:]

# encode and decode
encoded_imgs = autoencoder.predict(x_test1).numpy()

# plot
decoded_imgs = autoencoder.predict(encoded_imgs).numpy()
print_digits(decoded_imgs, x_test1)