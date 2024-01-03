import ctypes
from ctypes import *
import pathlib
import numpy as np

libname = pathlib.Path().absolute() / "lib" / "shared_lib.so"
lib = ctypes.CDLL(libname)

class config(Structure):
    _fields_ = [('model', c_char_p),
                ('vals', POINTER(c_int)),
                ('window', c_double),
                ('dataset', c_char_p),
                ('query', c_char_p),
                ('encoded_dataset', c_char_p),
                ('decoded_dataset', c_char_p)]

class sil_struct:
    pointer = POINTER(c_double)
    val = []

    def __del__(self):
        lib.free_double_array.argtypes = (ctypes.POINTER(ctypes.c_double),)
        lib.free_double_array(self.pointer)

def lsh_test(input, query, queries_num, k, L, table_size, window_size, query_trick, N, int_data=1):
    lib.get_lsh_results.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_double, ctypes.c_bool, ctypes.c_int, ctypes.c_int, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_int))
    average_time = ctypes.c_double()
    aaf = ctypes.c_double()
    min_neighbors = ctypes.c_int()
    lib.get_lsh_results(input, query, queries_num, k, L, table_size, window_size, query_trick, N, int_data, ctypes.byref(average_time), ctypes.byref(aaf), ctypes.byref(min_neighbors))
    return average_time, aaf, min_neighbors

def hypercube_test(input, query, queries_num, k, M, probes, N, window = 1000, int_data=1):
    lib.get_hypercube_results.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_double, ctypes.c_int, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double))
    average_time = ctypes.c_double()
    aaf = ctypes.c_double()
    lib.get_hypercube_results(input, query, queries_num, k, M, probes, N, window, int_data, ctypes.byref(average_time), ctypes.byref(aaf))
    return average_time, aaf

def kmeans_test(conf, int_data=1):
    tmp = config()
    tmp.model = conf['model'] # field for method
    tmp.vals = (ctypes.c_int * len(conf['vals']))(*conf['vals'])
    tmp.dataset = conf['dataset']
    lib.get_kmeans_results.argtypes = (ctypes.POINTER(config), ctypes.c_int, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.POINTER(ctypes.c_double)))
    stotal = ctypes.c_double()
    clustering_time = ctypes.c_double()
    sil = ctypes.POINTER(ctypes.c_double)()
    lib.get_kmeans_results(ctypes.byref(tmp), int_data, ctypes.byref(stotal), ctypes.byref(clustering_time), ctypes.byref(sil))
    silhouette = sil_struct()
    silhouette.pointer = sil
    silhouette.val = [sil[i] for i in range(10)]
    return stotal, clustering_time, silhouette

def gnn_test(input, query, queries_num, k, E, R, N, int_data = 1, load_file=b''):
    lib.get_gnn_results.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_char_p, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double))
    average_time = ctypes.c_double()
    aaf = ctypes.c_double()
    lib.get_gnn_results(input, query, queries_num, k, E, R, N, int_data, load_file, ctypes.byref(average_time), ctypes.byref(aaf))
    return average_time, aaf

def mrng_test(input, query, queries_num, l, N, int_data = 1, load_file=b''):
    lib.get_mrng_results.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_char_p, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double))
    average_time = ctypes.c_double()
    aaf = ctypes.c_double()
    lib.get_mrng_results(input, query, queries_num, l, N, int_data, load_file, ctypes.byref(average_time), ctypes.byref(aaf))
    return average_time, aaf

def nsg_test(input, query, queries_num, m, l, lq, k, N, int_data = 1, load_file=b''):
    lib.get_nsg_results.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_char_p, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double))
    average_time = ctypes.c_double()
    aaf = ctypes.c_double()
    lib.get_nsg_results(input, query, queries_num, m, l, lq, k, N, int_data, load_file, ctypes.byref(average_time), ctypes.byref(aaf))
    return average_time, aaf
 
def get_stotal(conf, dim, kmeansnew, centroids):
    tmp = config()
    tmp.model = conf['model'] # field for method
    tmp.vals = (ctypes.c_int * len(conf['vals']))(*conf['vals'])
    if 'window' in conf:
        tmp.window = conf['window']
    tmp.encoded_dataset = conf['encoded_dataset']
    lib.get_stotal.argtypes = (ctypes.POINTER(config), ctypes.c_int, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.POINTER(ctypes.c_double)), ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.POINTER(ctypes.c_double)))
    stotal = ctypes.c_double()
    sil = ctypes.POINTER(ctypes.c_double)()
    lib.get_stotal(ctypes.byref(tmp), dim, ctypes.byref(stotal), ctypes.byref(sil), kmeansnew, centroids)
    silhouette = sil_struct()
    silhouette.pointer = sil
    silhouette.val = [sil[i] for i in range(10)]
    return stotal, silhouette

def get_aaf(queries_num, conf, load_file = b''):
    tmp = config()
    tmp.model = conf['model']
    tmp.vals = (ctypes.c_int * len(conf['vals']))(*conf['vals'])
    if 'window' in conf:
        tmp.window = conf['window']
    tmp.dataset = conf['dataset']
    tmp.query = conf['query']
    tmp.encoded_dataset = conf['encoded_dataset']
    tmp.decoded_dataset = conf['decoded_dataset']
    lib.get_aaf.argtypes = (ctypes.c_char_p, ctypes.c_int, ctypes.POINTER(config), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double))
    aaf = ctypes.c_double()
    time = ctypes.c_double()
    lib.get_aaf(load_file, queries_num, ctypes.byref(tmp), ctypes.byref(aaf), ctypes.byref(time))
    return aaf, time

def get_kmeansnew_object(conf):
    tmp = config()
    tmp.model = conf['model'] # field for method
    tmp.vals = (ctypes.c_int * len(conf['vals']))(*conf['vals'])
    if 'window' in conf:
        tmp.window = conf['window']
    tmp.encoded_dataset = conf['encoded_dataset']
    lib.get_kmeans.argtypes = (ctypes.POINTER(config), ctypes.POINTER(ctypes.POINTER(ctypes.c_void_p)))
    kmeans = ctypes.POINTER(ctypes.c_void_p)()
    lib.get_kmeans(ctypes.byref(tmp), ctypes.byref(kmeans))
    return kmeans

def free_centroids(centroids):
    lib.free_centroids.argtypes = (ctypes.POINTER(ctypes.POINTER(ctypes.c_double)),)
    lib.free_centroids(centroids)

def get_centroids(kmeansnew):
    centroids = ctypes.POINTER(ctypes.POINTER(ctypes.c_double))()
    lib.get_centroids.argtypes = (ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.POINTER(ctypes.POINTER(ctypes.c_double))), ctypes.POINTER(ctypes.c_int))
    dim = ctypes.c_int()
    lib.get_centroids(kmeansnew, ctypes.byref(centroids), ctypes.byref(dim))
    centroids_ = np.array([[centroids[i][j] for j in range(dim.value)] for i in range(10)])
    # free_centroids(centroids)
    return centroids_, dim.value, centroids

def free_kmeans(kmeans):
    lib.free_kmeans.argtypes = (ctypes.c_void_p,)
    lib.free_kmeans(kmeans)

conf = {
    'model': b'CLASSIC',
    'vals': [],
    'dataset': b'MNIST/normalized_dataset.dat',
    'encoded_dataset': b'MNIST/output_dataset.dat'
}

from tensorflow.keras.models import save_model, load_model
from helper_funcs import *
from autoencoder import Autoencoder


kmeans = get_kmeansnew_object(conf)

centroids, dim, centroids_ = get_centroids(kmeans)

print(centroids)

print(centroids.shape)

model = 'models/model_conv_12.keras'

autoencoder = load_model(model)
shape = autoencoder.layers[-2].output_shape[1:] # get shape of encoded layer

centroids = deflatten_encoded(centroids, shape)

decoded_centroids = autoencoder.decode(centroids)

print(decoded_centroids)

print(decoded_centroids.shape)

# convert centroids to double array (to already allocated in memory centroid_)
for i in range(10):
    for j in range(dim):
        centroids_[i][j] = decoded_centroids[i][j]

stotal, sil = get_stotal(conf, dim, kmeans, centroids)
print("stotal: ", stotal.value)
print("silhouette: ", sil.val)
del sil

free_kmeans(kmeans)

print('All ok')