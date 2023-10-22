# Κάνοντας compile το <foo>_test.cpp με μια υλοποίηση <foo>.cpp του
# συγκεκριμένου τύπου, παράγουμε ένα test για την υλοποίηση αυτή.

cluster_OBJS =  main.o kmeanspp.o ../A/common/metrics.o kmeans.o\
			   ../A/RandomProjection/hypercube.o ../A/RandomProjection/helper_cube.o\
			   ../A/common/binary_string.o ../A/common/hash_function.o ../A/common/handle_binary.o\
			   helper.o

test_output_OBJS =  test_output.o kmeanspp.o ../A/common/metrics.o kmeans.o\
			   ../A/RandomProjection/hypercube.o ../A/RandomProjection/helper_cube.o\
			   ../A/common/binary_string.o ../A/common/hash_function.o ../A/common/handle_binary.o

test_OBJS = test.o kmeanspp.o ../A/common/metrics.o kmeans.o

# Ο βασικός κορμός του Makefile
include ../common.mk