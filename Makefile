CFLAGS=-g
CC=gcc

all: test

test: ./rb_gap_tree_test

./rb_gap_tree_test: src/rb_gap_tree.h src/rb_gap_tree.c src/rb_gap_tree_test.c
	$(CC) $(CFLAGS) $+ -o $@

