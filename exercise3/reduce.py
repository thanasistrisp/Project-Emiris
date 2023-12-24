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
parser.add_argument('-d', '--dataset', help='dataset', type=str)
parser.add_argument('-q', '--query', help='queryset', type=str)
parser.add_argument('-od', '--output_dataset', help='output_dataset_file', type=str)
parser.add_argument('-oq', '--output_query', help='output_query_file', type=str)
parser.add_argument('-m', '--model', help='model_file', type=str, default='models/model_conv_46.keras')

args = parser.parse_args()
dataset = args.dataset
query = args.query
output_dataset = args.output_dataset
output_query = args.output_query
model = args.model

# load model
autoencoder = load_model(model)
