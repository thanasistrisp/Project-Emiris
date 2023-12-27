import ctypes
from ctypes import *
import pathlib

libname = pathlib.Path().absolute() / "lib" / "shared_lib.so"
lib = ctypes.CDLL(libname)

def lsh_test(input, query, queries_num, k, L, table_size, window_size, query_trick, N):
    lib.get_lsh_results.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_bool, ctypes.c_int, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_int))
    average_time = ctypes.c_double()
    maf = ctypes.c_double()
    min_neighbors = ctypes.c_int()
    lib.get_lsh_results(input, query, queries_num, k, L, table_size, window_size, query_trick, N, ctypes.byref(average_time), ctypes.byref(maf), ctypes.byref(min_neighbors))
    return average_time, maf, min_neighbors

def hypercube_test(input, query, queries_num, k, M, probes, N):
    lib.get_hypercube_results.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double))
    average_time = ctypes.c_double()
    maf = ctypes.c_double()
    lib.get_hypercube_results(input, query, queries_num, k, M, probes, N, ctypes.byref(average_time), ctypes.byref(maf))
    return average_time, maf

def gnn_test(input, query, queries_num, k, E, R, N):
    lib.get_gnn_results.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_char_p, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double))
    # load_file = b'../graph_files/gnns21.bin'
    load_file = b''
    average_time = ctypes.c_double()
    maf = ctypes.c_double()
    lib.get_gnn_results(input, query, queries_num, k, E, R, N, load_file, ctypes.byref(average_time), ctypes.byref(maf))
    return average_time, maf

def mrng_test(input, query, queries_num, l, N):
    lib.get_mrng_results.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_char_p, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double))
    # load_file = b'../graph_files/mrng_graph.bin'
    load_file = b''
    average_time = ctypes.c_double()
    maf = ctypes.c_double()
    lib.get_mrng_results(input, query, queries_num, l, N, load_file, ctypes.byref(average_time), ctypes.byref(maf))
    return average_time, maf

def nsg_test(input, query, queries_num, m, l, lq, k, N):
    lib.get_nsg_results.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_char_p, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double))
    load_file = b''
    average_time = ctypes.c_double()
    maf = ctypes.c_double()
    lib.get_nsg_results(input, query, queries_num, m, l, lq, k, N, load_file, ctypes.byref(average_time), ctypes.byref(maf))
    return average_time, maf

class CA(Structure):
    _fields_ = [('model', c_char_p),
                ('vals', POINTER(c_int))]

def get_nearest_neighbor(input, query, query_index, model, params, load_file = b''):
    tmp = CA()
    tmp.model = model
    tmp.vals = (c_int * len(params))(*params)
    lib.get_nearest_neighbor.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_char_p, ctypes.POINTER(CA), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_int))
    average_time = ctypes.c_double()
    index = ctypes.c_int()
    lib.get_nearest_neighbor(input, query, query_index, load_file, ctypes.byref(tmp), ctypes.byref(average_time), ctypes.byref(index))
    average_time = average_time.value
    index = index.value
    return average_time, index