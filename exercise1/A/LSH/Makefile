# Κάνοντας compile το <foo>_test.cpp με μια υλοποίηση <foo>.cpp του
# συγκεκριμένου τύπου, παράγουμε ένα test για την υλοποίηση αυτή.

lsh_OBJS = main.o lsh.o ../common/lp_metric.o ../common/hash_function.o ../common/handle_binary.o handle_output.o ../common/brute_force.o

lsh_ARGS = -d ../../MNIST/input.dat -q ../../MNIST/query.dat -k 4 -L 5 -o ../../output/output.txt -N 1 -R 10000

# Ο βασικός κορμός του Makefile
include ../../common.mk