# Κάνοντας compile το <foo>_test.cpp με μια υλοποίηση <foo>.cpp του
# συγκεκριμένου τύπου, παράγουμε ένα test για την υλοποίηση αυτή.

graph_search_OBJS =  main.o common/directed_graph.o common/vector_utils.o handle_output.o common/vertex.o\
					 gnn/gnn.o mrng/mrng.o\
					 ../exercise1/A/LSH/lsh.o ../exercise1/A/common/lp_metric.o ../exercise1/A/common/hash_function.o ../exercise1/A/common/brute_force.o\
					 ../exercise1/A/common/handle_binary.o
graph_search_ARGS = -d MNIST/input.dat -o output/output.txt -q MNIST/query.dat

# Ο βασικός κορμός του Makefile
include common.mk