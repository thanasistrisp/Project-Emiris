import ctypes
from ctypes import *
import pathlib

libname = pathlib.Path().absolute() / "lib" / "shared_lib.so"
lib = ctypes.CDLL(libname)

def lsh_test(input, query, queries_num, k, L, table_size, window_size, query_trick, N):
    lib.get_lsh_results.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_bool, ctypes.c_int, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_int))
    average_time = ctypes.c_double()
    aaf = ctypes.c_double()
    min_neighbors = ctypes.c_int()
    lib.get_lsh_results(input, query, queries_num, k, L, table_size, window_size, query_trick, N, ctypes.byref(average_time), ctypes.byref(aaf), ctypes.byref(min_neighbors))
    return average_time, aaf, min_neighbors

def hypercube_test(input, query, queries_num, k, M, probes, N):
    lib.get_hypercube_results.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double))
    average_time = ctypes.c_double()
    aaf = ctypes.c_double()
    lib.get_hypercube_results(input, query, queries_num, k, M, probes, N, ctypes.byref(average_time), ctypes.byref(aaf))
    return average_time, aaf

def kmeans_test(input, method, k, L, k_lsh, M, k_hypercube, probes):
    lib.get_kmeans_results.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.int, ctypes.int, ctypes.int, ctypes.int, ctypes.int, ctypes.int)
    clustering_time = ctypes.c_double()
    stotal = ctypes.c_double()
    lib.get_kmeans_results(input, method, k, L, k_lsh, M, k_hypercube, probes)
    return clustering_time, stotal

def gnn_test(input, query, queries_num, k, E, R, N, load_file=b''):
    lib.get_gnn_results.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_char_p, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double))
    average_time = ctypes.c_double()
    aaf = ctypes.c_double()
    lib.get_gnn_results(input, query, queries_num, k, E, R, N, load_file, ctypes.byref(average_time), ctypes.byref(aaf))
    return average_time, aaf

def mrng_test(input, query, queries_num, l, N, load_file=b''):
    lib.get_mrng_results.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_char_p, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double))
    average_time = ctypes.c_double()
    aaf = ctypes.c_double()
    lib.get_mrng_results(input, query, queries_num, l, N, load_file, ctypes.byref(average_time), ctypes.byref(aaf))
    return average_time, aaf

def nsg_test(input, query, queries_num, m, l, lq, k, N, load_file=b''):
    lib.get_nsg_results.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_char_p, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double))
    average_time = ctypes.c_double()
    aaf = ctypes.c_double()
    lib.get_nsg_results(input, query, queries_num, m, l, lq, k, N, load_file, ctypes.byref(average_time), ctypes.byref(aaf))
    return average_time, aaf

class encoded_config(Structure):
    _fields_ = [('model', c_char_p),
                ('enc_vals', POINTER(c_int)),
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
    
def get_stotal(config):
    tmp = encoded_config()
    tmp.model = config['model'] # field for method
    tmp.enc_vals = (ctypes.c_int * len(config['enc_vals']))(*config['enc_vals'])
    tmp.dataset = config['dataset']
    tmp.decoded_dataset = config['decoded_dataset']
    lib.get_stotal.argtypes = (ctypes.POINTER(encoded_config), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double))
    stotal = ctypes.c_double()
    time = ctypes.c_double()
    sil = ctypes.POINTER(ctypes.c_double)()
    lib.get_stotal(ctypes.byref(tmp), ctypes.byref(stotal), ctypes.byref(time), ctypes.byref(sil))
    silhouette = sil_struct()
    silhouette.pointer = sil
    silhouette.val = [sil[i] for i in range(10)]
    return stotal.value, time.value, silhouette

def get_aaf(queries_num, config, load_file = b''):
    tmp = encoded_config()
    tmp.model = config['model']
    tmp.enc_vals = (ctypes.c_int * len(config['enc_vals']))(*config['enc_vals'])
    tmp.dataset = config['dataset']
    tmp.query = config['query']
    tmp.encoded_dataset = config['encoded_dataset']
    tmp.decoded_dataset = config['decoded_dataset']
    lib.get_aaf.argtypes = (ctypes.c_char_p, ctypes.c_int, ctypes.POINTER(encoded_config), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double))
    aaf = ctypes.c_double()
    time = ctypes.c_double()
    lib.get_aaf(load_file, queries_num, ctypes.byref(tmp), ctypes.byref(aaf), ctypes.byref(time))
    return aaf.value, time.value

config = {
    'model': b'CLASSIC',
    'enc_vals': [],
    'dataset': b'MNIST/normalized_dataset.dat',
    'query': b'MNIST/normalized_query.dat',
    'encoded_dataset': b'MNIST/output_dataset.dat',
    'decoded_dataset': b'MNIST/decoded_dataset.dat',
}

stotal, time, sil = get_stotal(config)
print("stotal: ", stotal)
print("time: ", time)
print("silhouette: ", sil.val)
del sil