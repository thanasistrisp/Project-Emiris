# Κάνοντας compile το <foo>_test.cpp με μια υλοποίηση <foo>.cpp του
# συγκεκριμένου τύπου, παράγουμε ένα test για την υλοποίηση αυτή.

testcube_OBJS = test.o hypercube.o ../common/metrics.o helper_cube.o ../common/handle_binary.o ../common/handle_output.o ../common/hash_function.o

cube_OBJS = hypercube.o ../common/metrics.o main.o helper_cube.o ../common/handle_binary.o ../common/handle_output.o ../common/hash_function.o

cube_ARGS = -d ../../MNIST/input.dat -q ../../MNIST/query.dat -k 14 -M 50 -probes 20 -o ../../output/output.txt -N 2 -R 10000

# Ο βασικός κορμός του Makefile
include ../../common.mk