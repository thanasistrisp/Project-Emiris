# Κάνοντας compile το <foo>_test.cpp με μια υλοποίηση <foo>.cpp του
# συγκεκριμένου τύπου, παράγουμε ένα test για την υλοποίηση αυτή.

graph_search_OBJS =  $(MODULES)/main.o $(LIB)/graphsearch.so
graph_search_ARGS = -d MNIST/input.dat -o output/output.txt -q MNIST/query.dat -k 50 -E 30 -R 1 -N 5 -l 500 -m 3

# Ο βασικός κορμός του Makefile
include common.mk

.PHONY: $(LIB)/graphsearch.so
$(LIB)/graphsearch.so:
	$(MAKE) -C $(LIB) graphsearch.so