# Κάνοντας compile το <foo>_test.cpp με μια υλοποίηση <foo>.cpp του
# συγκεκριμένου τύπου, παράγουμε ένα test για την υλοποίηση αυτή.


cube_OBJS = hypercube.o ../common/lp_metric.o main.o helper_cube.o ../common/handle_binary.o\
			../common/hash_function.o binary_string.o handle_output.o ../common/brute_force.o

cube_ARGS = -d ../../../exercise3/MNIST/output_dataset.dat -q ../../../exercise3/MNIST/output_query.dat -k 2 -M 200 -probes 50 -o ../../output/output.txt -N 5 -R 10000

# Ο βασικός κορμός του Makefile
include ../../common.mk