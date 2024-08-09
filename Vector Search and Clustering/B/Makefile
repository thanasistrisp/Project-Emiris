cluster_OBJS =  main.o kmeanspp.o kmeans.o helper.o\
			   ../A/RandomProjection/hypercube.o ../A/RandomProjection/helper_cube.o\
			   ../A/common/handle_binary.o ../A/RandomProjection/binary_string.o ../A/common/hash_function.o\
			   ../A/LSH/lsh.o ../A/common/lp_metric.o\
			   vector_utils.o

cluster_ARGS = -i ../MNIST/input.dat -c cluster.conf -o ../output/cluster.txt -complete -m Classic

include ../common.mk