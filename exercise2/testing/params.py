import ctypes
import pathlib

libname = pathlib.Path().absolute().parent / 'lib' / 'graphsearch.so'
lib = ctypes.CDLL(libname)

def lsh_test(input, query, queries_num, k, L, table_size, window_size, N):
    lib.get_lsh_results.restype = ctypes.POINTER(ctypes.c_double)
    lib.get_lsh_results.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double))
    average_time = ctypes.c_double()
    true_time = ctypes.c_double()
    maf = ctypes.c_double()
    lib.get_lsh_results(input, query, queries_num, k, L, table_size, window_size, N, ctypes.byref(average_time), ctypes.byref(true_time), ctypes.byref(maf))
    return average_time, true_time, maf

def hypercube_test(input, query, queries_num, k, M, probes, N):
    lib.get_hypercube_results.restype = ctypes.POINTER(ctypes.c_double)
    lib.get_hypercube_results.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double))
    average_time = ctypes.c_double()
    true_time = ctypes.c_double()
    maf = ctypes.c_double()
    lib.get_hypercube_results(input, query, queries_num, k, M, probes, N, ctypes.byref(average_time), ctypes.byref(true_time), ctypes.byref(maf))
    return average_time, true_time, maf

def gnn_test(input, query, queries_num, k, E, R, N):
    lib.get_gnn_results.restype = ctypes.POINTER(ctypes.c_double)
    lib.get_gnn_results.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_char_p, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double))
    load_file = b'../graph_files/gnn_graph.bin'
    average_time = ctypes.c_double()
    true_time = ctypes.c_double()
    maf = ctypes.c_double()
    lib.get_gnn_results(input, query, queries_num, k, E, R, N, load_file, ctypes.byref(average_time), ctypes.byref(true_time), ctypes.byref(maf))
    return average_time, true_time, maf

def mrng_test(input, query, queries_num, l, N):
    lib.get_mrng_results.restype = ctypes.POINTER(ctypes.c_double)
    lib.get_mrng_results.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_char_p, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double))
    load_file = b'../graph_files/mrng_graph.bin'
    average_time = ctypes.c_double()
    true_time = ctypes.c_double()
    maf = ctypes.c_double()
    lib.get_mrng_results(input, query, queries_num, l, N, load_file, ctypes.byref(average_time), ctypes.byref(true_time), ctypes.byref(maf))
    return average_time, true_time, maf