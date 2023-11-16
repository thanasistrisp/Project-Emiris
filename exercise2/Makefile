# Κάνοντας compile το <foo>_test.cpp με μια υλοποίηση <foo>.cpp του
# συγκεκριμένου τύπου, παράγουμε ένα test για την υλοποίηση αυτή.

graph_search_OBJS =  main.o common/directed_graph.o gnn/gnn.o handle_output.o common/vertex.o\
					 ../exercise1/A/RandomProjection/hypercube.o ../exercise1/A/common/lp_metric.o ../exercise1/A/common/hash_function.o ../exercise1/A/common/brute_force.o\
					 ../exercise1/A/common/handle_binary.o ../exercise1/A/RandomProjection/helper_cube.o ../exercise1/A/RandomProjection/binary_string.o
graph_search_ARGS = -d MNIST/input.dat -o output/output.txt -q MNIST/query.dat

# graph_search_ARGS = 

# Ο βασικός κορμός του Makefile
include common.mk