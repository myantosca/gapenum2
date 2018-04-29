CFLAGS=-g
CXXFLAGS=-g -std=c++11
CC=gcc
CXX=g++
FNAME_BASE=cosc6384.final.michael-yantosca

all: test exe doc

exe: ./wcrt ./gentasksets

test: ./rb_gap_tree_test

./rb_gap_tree_test: src/rb_gap_tree.h src/rb_gap_tree.c src/rb_gap_tree_test.c
	$(CC) $(CFLAGS) $+ -o $@
./wcrt: src/rb_gap_tree.h src/rb_gap_tree.c src/gap_list.h src/gap_list.c src/gap_enum.h src/gap_enum.c src/gap_enum2.h src/gap_enum2.c src/wcrt.c
	$(CC) $(CFLAGS) $+ -o $@ -lm

./gentasksets: src/gentasksets.cpp
	$(CXX) $(CXXFLAGS) $+ -o $@ -lm

clean: clean-doc-$(FNAME_BASE).report clean-doc-$(FNAME_BASE).slides
	@rm -f *.o
	@rm -f *~
	@rm -f ./src/*~
	@rm -f ./wcrt
	@rm -f ./rb_gap_tree_test
	@rm -f ./gentasksets

doc: $(FNAME_BASE).report.pdf $(FNAME_BASE).slides.pdf

%.pdf: %.tex %.bib
	@lualatex -shell-escape $*.tex
	@biber $*
	@lualatex -shell-escape $*.tex
	@lualatex -shell-escape $*.tex

superclean: clean superclean-doc-$(FNAME_BASE)-report superclean-doc-$(FNAME_BASE).slides

superclean-doc-%:
	rm -f $*.pdf

clean-doc-%: 
	@rm -f $*.aux
	@rm -f $*.bbl
	@rm -f $*.bcf
	@rm -f $*.log
	@rm -f $*.run.xml
	@rm -f $*.dvi
	@rm -f $*.blg
	@rm -f $*.auxlock
	@rm -f $*.pyg
	@rm -f $*-figure*
	@rm -f $*.toc
	@rm -f $*.out
	@rm -f $*.snm
	@rm -f $*.nav
	@rm -rf _minted-$*
