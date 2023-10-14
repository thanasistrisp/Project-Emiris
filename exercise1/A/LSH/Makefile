# Κάνοντας compile το <foo>_test.cpp με μια υλοποίηση <foo>.cpp του
# συγκεκριμένου τύπου, παράγουμε ένα test για την υλοποίηση αυτή.

lsh_OBJS = main.o lsh.o lp_metric.o hash_function.o handle_binary.o handle_output.o

lsh_ARGS = -d ../../MNIST/input.dat -q ../../MNIST/query.dat -k 4 -L 5 -o ../../output/output.txt -N 1 -R 10000

# Ο βασικός κορμός του Makefile
include ../../common.mk