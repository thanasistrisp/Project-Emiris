import tensorflow as tf

import keras
from keras import layers
from keras import saving

from tensorflow.keras.models import save_model, load_model

from sklearn.utils import shuffle
import numpy as np

INITIAL_DIM = 784

@keras.saving.register_keras_serializable()
class Autoencoder(keras.Model):
    def __init__(self, model_type, dim_list, activation, optimizer, loss, patience):
        super(Autoencoder, self).__init__()
        self.es = keras.callbacks.EarlyStopping(monitor='loss', verbose=1, patience=patience, min_delta=0.0001)
        self.dim_list = dim_list
        self.activation = activation
        self.encoder = keras.Sequential()
        self.decoder = keras.Sequential()
        self.loss = loss
        if model_type == 'dense':
            self.build_dense()
        elif model_type == 'conv':
            self.build_conv()
        self.compile(optimizer=optimizer, loss=loss)

    def build_dense(self):
        self.encoder.add(layers.Flatten(input_shape=(INITIAL_DIM,)))
        for dim in self.dim_list:
            self.encoder.add(layers.Dense(dim, activation=self.activation))
        for dim in reversed(self.dim_list):
            self.decoder.add(layers.Dense(dim, activation=self.activation))
        self.decoder.add(layers.Dense(INITIAL_DIM, activation='sigmoid'))
        
    def build_conv(self):
        for filters, kernel_size, pool_size, padding in self.dim_list[:-1]:
            self.encoder.add(layers.Conv2D(filters, kernel_size, activation=self.activation, padding=padding))
            self.encoder.add(layers.MaxPooling2D(pool_size, padding='same'))
        for i, config in enumerate(reversed(self.dim_list[:-1])):
            filters, kernel_size, pool_size, padding = config
            if i == len(self.dim_list[:-1]) - 1:
                padding = 'valid'
            self.decoder.add(layers.Conv2D(filters, kernel_size, activation=self.activation, padding=padding))
            self.decoder.add(layers.UpSampling2D(pool_size))
        if self.loss == 'binary_crossentropy':
            activation = 'sigmoid'
        elif self.loss == 'mean_squared_error':
            activation = 'linear'
        self.decoder.add(layers.Conv2D(1, self.dim_list[-1][0], activation=activation, padding=self.dim_list[-1][1]))

    def encode(self, x):
        return self.encoder(x)

    def decode(self, x):
        return self.decoder(x)    
    
    def call(self, x):
        encoded = self.encoder(x)
        decoded = self.decoder(encoded)
        return decoded

    def predict(self, x):
        encoded = self.encoder(x)
        decoded = self.decoder(encoded)
        return decoded