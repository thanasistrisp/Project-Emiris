from params import lsh_test, hypercube_test, gnn_test, mrng_test, nsg_test

import ctypes

input = b'../MNIST/input.dat'
query = b'../MNIST/query.dat'
queries_num = 100
m = 10
l = 100
N = 10

average_time, maf, min_neighbors = nsg_test(input, query, queries_num, m, l, N)