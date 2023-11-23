import ctypes
import pathlib

libname = pathlib.Path().absolute().parent / 'lib' / 'graphsearch.so'
lib = ctypes.CDLL(libname)


# Gnn

lib.get_gnn_results.restype = ctypes.POINTER(ctypes.c_double)
lib.get_gnn_results.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_char_p, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double))
input = b'../MNIST/input.dat'
query = b'../MNIST/query.dat'
queries_num = 10
k = 10
E = 10
R = 10
N = 10
load_file = b'../graph_files/gnn_graph.bin'
average_time = ctypes.c_double()
true_time = ctypes.c_double()
maf = ctypes.c_double()
lib.get_gnn_results(input, query, queries_num, k, E, R, N, load_file, ctypes.byref(average_time), ctypes.byref(true_time), ctypes.byref(maf))
print("average_time: ", average_time.value)
print("true_time: ", true_time.value)
print("maf: ", maf.value)


# Mrng

lib.get_mrng_results.restype = ctypes.POINTER(ctypes.c_double)
lib.get_mrng_results.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_char_p, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double))
input = b'../MNIST/input.dat'
query = b'../MNIST/query.dat'
queries_num = 10
l = 20
N = 10
load_file = b'../graph_files/mrng_graph.bin'
average_time = ctypes.c_double()
true_time = ctypes.c_double()
maf = ctypes.c_double()
lib.get_mrng_results(input, query, queries_num, l, N, load_file, ctypes.byref(average_time), ctypes.byref(true_time), ctypes.byref(maf))
print("average_time: ", average_time.value)
print("true_time: ", true_time.value)
print("maf: ", maf.value)


# Lsh

lib.get_lsh_results.restype = ctypes.POINTER(ctypes.c_double)
lib.get_lsh_results.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double))
input = b'../MNIST/input.dat'
query = b'../MNIST/query.dat'
queries_num = 10
k = 4
L = 5
table_size = 15000
window_size = 1000
N = 10
average_time = ctypes.c_double()
true_time = ctypes.c_double()
maf = ctypes.c_double()
lib.get_lsh_results(input, query, queries_num, k, L, table_size, window_size, N, ctypes.byref(average_time), ctypes.byref(true_time), ctypes.byref(maf))
print("average_time: ", average_time.value)
print("true_time: ", true_time.value)
print("maf: ", maf.value)


# Hypercube

lib.get_hypercube_results.restype = ctypes.POINTER(ctypes.c_double)
lib.get_hypercube_results.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double), ctypes.POINTER(ctypes.c_double))
input = b'../MNIST/input.dat'
query = b'../MNIST/query.dat'
queries_num = 10
k = 14
M = 20
probes = 10
N = 10
average_time = ctypes.c_double()
true_time = ctypes.c_double()
maf = ctypes.c_double()
lib.get_hypercube_results(input, query, queries_num, k, M, probes, N, ctypes.byref(average_time), ctypes.byref(true_time), ctypes.byref(maf))
print("average_time: ", average_time.value)
print("true_time: ", true_time.value)
print("maf: ", maf.value)