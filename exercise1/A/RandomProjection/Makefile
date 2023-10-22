# Κάνοντας compile το <foo>_test.cpp με μια υλοποίηση <foo>.cpp του
# συγκεκριμένου τύπου, παράγουμε ένα test για την υλοποίηση αυτή.


cube_OBJS = hypercube.o ../common/lp_metric.o main.o helper_cube.o ../common/handle_binary.o ../common/hash_function.o ../common/binary_string.o ../common/handle_output.o

cube_ARGS = -d ../../MNIST/input.dat -q ../../MNIST/query.dat -k 14 -M 200 -probes 50 -o ../../output/output.txt -N 5 -R 10000

testcube_OBJS = test.o hypercube.o ../common/metrics.o helper_cube.o ../common/handle_binary.o ../common/hash_function.o ../common/binary_string.o

# Ο βασικός κορμός του Makefile
include ../../common.mk