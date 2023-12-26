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
parser.add_argument('-od', '--output_dataset', help='output_dataset_file', type=str, default='MNIST/output_dataset.dat')
parser.add_argument('-oq', '--output_query', help='output_query_file', type=str, default='MNIST/output_query.dat')
parser.add_argument('-m', '--model', help='model_file', type=str, default='models/model_conv_12.keras')

args = parser.parse_args()
dataset = args.dataset
query = args.query
output_dataset = args.output_dataset
output_query = args.output_query
model = args.model

# load model
autoencoder = load_model(model)
shape = autoencoder.layers[-2].output_shape[1:] # get shape of encoded layer

# load dataset
x_train = load_dataset(dataset)
x_train = x_train.astype('float32') / 255.
x_test = load_dataset(query)
x_test = x_test.astype('float32') / 255.
if len(shape) == 3: # if model type is convolutional
	x_train = np.reshape(x_train, (len(x_train), 28, 28, 1))
	x_test = np.reshape(x_test, (len(x_test), 28, 28, 1))
else:
	x_train = np.reshape(x_train, (len(x_train), 784))
	x_test = np.reshape(x_test, (len(x_test), 784))

encoded_train = autoencoder.encode(x_train)
encoded_test = autoencoder.encode(x_test)

# save encoded datasets
save_encoded_binary(encoded_train, output_dataset)
save_encoded_binary(encoded_test, output_query)

# # load encoded datasets (already normalized)
# encoded_train = load_dataset(output_dataset, dtype=np.float32)
# encoded_test = load_dataset(output_query, dtype=np.float32)

# # deflatten encoded datasets
# encoded_train = deflatten_encoded(encoded_train, shape)
# encoded_test = deflatten_encoded(encoded_test, shape)

# # decode encoded datasets
# decoded_train = autoencoder.decode(encoded_train)
# decoded_test = autoencoder.decode(encoded_test)

# print_digits(decoded_train, x_train)