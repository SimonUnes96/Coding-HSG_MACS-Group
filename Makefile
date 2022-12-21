CXXFLAGS=-Wall -g -O2 -std=c++11
.PHONY: all
SHELL=/bin/bash

PROGRAM=dbfiles

TIMEOUT=8



.PHONY: all
all: compile-program check

.PHONY: compile-program

compile-program: $(PROGRAM)


