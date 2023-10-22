# Κάνοντας compile το <foo>_test.cpp με μια υλοποίηση <foo>.cpp του
# συγκεκριμένου τύπου, παράγουμε ένα test για την υλοποίηση αυτή.

cluster_OBJS =  main.o kmeanspp.o kmeans.o helper.o\
			   ../A/RandomProjection/hypercube.o ../A/RandomProjection/helper_cube.o\
			   ../A/common/binary_string.o ../A/common/hash_function.o ../A/common/handle_binary.o\
			   ../A/LSH/lsh.o ../A/common/lp_metric.o

cluster_ARGS = -i ../MNIST/input.dat -c cluster.conf -o ../output/cluster.txt -complete -m Classic

test_output_OBJS =  test_output.o kmeanspp.o ../A/common/metrics.o kmeans.o\
			   ../A/RandomProjection/hypercube.o ../A/RandomProjection/helper_cube.o\
			   ../A/common/binary_string.o ../A/common/hash_function.o ../A/common/handle_binary.o

test_OBJS = test.o kmeanspp.o ../A/common/metrics.o kmeans.o

# Ο βασικός κορμός του Makefile
include ../common.mk