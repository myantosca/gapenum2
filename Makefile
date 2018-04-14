CFLAGS=-g
CC=gcc

all: test exe

exe: ./wcrt

test: ./rb_gap_tree_test

./rb_gap_tree_test: src/rb_gap_tree.h src/rb_gap_tree.c src/rb_gap_tree_test.c
	$(CC) $(CFLAGS) $+ -o $@
./wcrt: src/rb_gap_tree.h src/rb_gap_tree.c src/gap_enum.h src/gap_enum.c src/wcrt.c
	$(CC) $(CFLAGS) $+ -o $@ -lm

clean:
	rm *.o
	rm *~
	rm ./wcrt
	rm ./rb_gap_tree_test
