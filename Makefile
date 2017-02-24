# Compiler #
CC 		= g++
CFLAGS	= -std=c++11 -Wall  
OPT 	= -O3 -g

# My directories
HEADERS	= example
library_headers = ${HEADERS}/*.hpp

# 3rd party libraries
BOOSTLIB 	= -lboost_system -lboost_filesystem -lboost_program_options	
HDF5LIB		= -lhdf5 

all: examples

examples: example/printRecursively

example/printRecursively: example/printRecursively.cpp ${library_headers} *.hpp
	${CC} ${CFLAGS} ${OPT} ${HDF5LIB} ${BOOSTLIB} -I${HEADERS} -I./  $< -o $@