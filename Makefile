CFLAGS=-g
CXXFLAGS=-g -std=c++11
CC=gcc
CXX=g++

all: test exe

exe: ./wcrt ./gentasksets

test: ./rb_gap_tree_test

./rb_gap_tree_test: src/rb_gap_tree.h src/rb_gap_tree.c src/rb_gap_tree_test.c
	$(CC) $(CFLAGS) $+ -o $@
./wcrt: src/rb_gap_tree.h src/rb_gap_tree.c src/gap_list.h src/gap_list.c src/gap_enum.h src/gap_enum.c src/gap_enum2.h src/gap_enum2.c src/wcrt.c
	$(CC) $(CFLAGS) $+ -o $@ -lm

./gentasksets: src/gentasksets.cpp
	$(CXX) $(CXXFLAGS) $+ -o $@ -lm

clean:
	@rm -f *.o
	@rm -f *~
	@rm -f ./src/*~
	@rm -f ./wcrt
	@rm -f ./rb_gap_tree_test
	@rm -f ./gentasksets
