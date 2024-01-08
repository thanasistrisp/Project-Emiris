import ctypes
from ctypes import *
import pathlib
import numpy as np

libname = pathlib.Path().absolute() / "lib" / "shared_lib.so"
lib = ctypes.CDLL(libname)

class config(Structure):
    _fields_ = [('model', c_char_p), # algorithm name ("BRUTE", "LSH", "CUBE", "GNNS", "MRNG", "NSG" for ann / "CLASSIC", "LSH", "CUBE" for kmeans)
                ('vals', POINTER(c_int)), # array of int parameters given with the following order for each method:
                                          # (K, L, table_size, query_trick for LSH)
                                          # (K, M, probes for CUBE)
                                          # (K, E, R for GNNS)
                                          # (l for MRNG)
                                          # (l, m, k, lq for NSG)
                                          # (L, K, limit_queries for reversed LSH)
                                          # (M, K, probes for reversed CUBE)
                ('window', c_double),
                ('dataset', c_char_p), # initial dataset
                ('query', c_char_p),
                ('encoded_query', c_char_p),
                ('encoded_dataset', c_char_p)]

class sil_struct: # struct for silhouette (double array of size 10 for each cluster)
    pointer = POINTER(c_double)
    val = []

    def __del__(self):
        lib.free_double_array.argtypes = (ctypes.POINTER(ctypes.c_double),)
        lib.free_double_array(self.pointer)

# AAF: Average Approximation Factor

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

def get_aaf(queries_num, conf, load_file = b''): # calculates the fraction {P_approx_new_projected - q_init} / {P_true_init - q_init}
    tmp = config()
    tmp.model = conf['model']
    tmp.vals = (ctypes.c_int * len(conf['vals']))(*conf['vals'])
    if 'window' in conf:
        tmp.window = conf['window']
    tmp.dataset = conf['dataset']
    tmp.query = conf['query']
    tmp.encoded_query = conf['encoded_query']
    tmp.encoded_dataset = conf['encoded_dataset']
    lib.get_aaf.argtypes = (ctypes.c_char_p, ctypes.c_int, ctypes.POINTER(config), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double))
    aaf = ctypes.c_double()
    time = ctypes.c_double()
    lib.get_aaf(load_file, queries_num, ctypes.byref(tmp), ctypes.byref(aaf), ctypes.byref(time))
    return aaf, time

# Kmeans

def kmeans_test(conf, int_data=1): # returns kmeans results for a given dataset (can be encoded or not)
    tmp = config()
    tmp.model = conf['model']
    tmp.vals = (ctypes.c_int * len(conf['vals']))(*conf['vals'])
    if 'window' in conf:
        tmp.window = conf['window']
    tmp.dataset = conf['dataset']
    lib.get_kmeans_results.argtypes = (ctypes.POINTER(config), ctypes.c_int, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.POINTER(ctypes.c_double)), ctypes.POINTER(ctypes.c_double))
    stotal = ctypes.c_double()
    clustering_time = ctypes.c_double()
    sil = ctypes.POINTER(ctypes.c_double)()
    obj_func = ctypes.c_double()
    lib.get_kmeans_results(ctypes.byref(tmp), int_data, ctypes.byref(stotal), ctypes.byref(clustering_time), ctypes.byref(sil), ctypes.byref(obj_func))
    silhouette = sil_struct()
    silhouette.pointer = sil
    silhouette.val = [sil[i] for i in range(10)]
    return stotal, clustering_time, silhouette, obj_func
 
def get_stotal(conf, kmeans_eval, centroids): # takes encoded centroids and pointer of KmeansEval class and returns projected silhouette
    tmp = config()
    tmp.model = conf['model']
    tmp.vals = (ctypes.c_int * len(conf['vals']))(*conf['vals'])
    if 'window' in conf:
        tmp.window = conf['window']
    tmp.dataset = conf['dataset']
    lib.get_stotal.argtypes = (ctypes.POINTER(config), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.POINTER(ctypes.c_double)), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.POINTER(ctypes.c_double)))
    stotal = ctypes.c_double()
    sil = ctypes.POINTER(ctypes.c_double)()
    obj_func = ctypes.c_double()
    lib.get_stotal(ctypes.byref(tmp), ctypes.byref(stotal), ctypes.byref(sil), ctypes.byref(obj_func), kmeans_eval, centroids)
    silhouette = sil_struct()
    silhouette.pointer = sil
    silhouette.val = [sil[i] for i in range(10)]
    return stotal, silhouette, obj_func


def get_kmeans_eval_object(conf): # returns pointer of KmeansEval class
    tmp = config()
    tmp.model = conf['model']
    tmp.vals = (ctypes.c_int * len(conf['vals']))(*conf['vals'])
    if 'window' in conf:
        tmp.window = conf['window']
    tmp.encoded_dataset = conf['encoded_dataset']
    lib.get_kmeans.argtypes = (ctypes.POINTER(config), ctypes.POINTER(ctypes.POINTER(ctypes.c_void_p)))
    kmeans = ctypes.POINTER(ctypes.c_void_p)()
    lib.get_kmeans(ctypes.byref(tmp), ctypes.byref(kmeans))
    return kmeans

def get_centroids(kmeans_eval): # returns centroids as numpy array and dimension of centroids, shape: (10, dim)
    centroids = ctypes.POINTER(ctypes.POINTER(ctypes.c_double))()
    lib.get_centroids.argtypes = (ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.POINTER(ctypes.POINTER(ctypes.c_double))), ctypes.POINTER(ctypes.c_int))
    dim = ctypes.c_int()
    lib.get_centroids(kmeans_eval, ctypes.byref(centroids), ctypes.byref(dim))
    centroids_ = np.array([[centroids[i][j] for j in range(dim.value)] for i in range(10)])
    free_centroids(centroids)
    return centroids_, dim.value

def convert_to_2d_array(array, dim): # converts 1d flatten c array to 2d array
    lib.convert_1d_to_2d.argtypes = (ctypes.POINTER(ctypes.c_double), ctypes.c_int, ctypes.POINTER(ctypes.POINTER(ctypes.POINTER(ctypes.c_double))))
    array2d = ctypes.POINTER(ctypes.POINTER(ctypes.c_double))()
    lib.convert_1d_to_2d(array, dim, ctypes.byref(array2d))
    return array2d

def free_centroids(centroids):
    lib.free_centroids.argtypes = (ctypes.POINTER(ctypes.POINTER(ctypes.c_double)),)
    lib.free_centroids(centroids)

def free_kmeans(kmeans):
    lib.free_kmeans.argtypes = (ctypes.c_void_p,)
    lib.free_kmeans(kmeans)