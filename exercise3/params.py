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

class CA(Structure):
    _fields_ = [('model', c_char_p),
                ('enc_vals', POINTER(c_int)),
                ('dataset', c_char_p),
                ('query', c_char_p),
                ('encoded_dataset', c_char_p),
                ('decoded_dataset', c_char_p)]

def get_aaf(queries_num, ca, load_file = b''):
    tmp = CA()
    tmp.model = ca['model']
    tmp.enc_vals = (ctypes.c_int * len(ca['enc_vals']))(*ca['enc_vals'])
    tmp.dataset = ca['dataset']
    tmp.query = ca['query']
    tmp.encoded_dataset = ca['encoded_dataset']
    tmp.decoded_dataset = ca['decoded_dataset']
    lib.get_aaf.argtypes = (ctypes.c_char_p, ctypes.c_int, ctypes.POINTER(CA), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double))
    aaf = ctypes.c_double()
    time = ctypes.c_double()
    lib.get_aaf(load_file, queries_num, ctypes.byref(tmp), ctypes.byref(aaf), ctypes.byref(time))
    return aaf.value, time.value

ca = {
    'model': b'BRUTE',
    'enc_vals': [5,10,10],
    'dataset': b'MNIST/normalized_dataset.dat',
    'query': b'MNIST/normalized_query.dat',
    'encoded_dataset': b'MNIST/output_dataset.dat',
    'decoded_dataset': b'MNIST/decoded_dataset.dat',
}

print(get_aaf(10, ca))