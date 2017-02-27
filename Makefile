# Compiler #
CC 		= g++
CFLAGS	= -std=c++11 -Wall  
OPT 	= -O3 -g

# My directories
HEADERS	= examples/data
library_headers = ${HEADERS}/*.hpp

# 3rd party libraries
BOOSTLIB 	= -lboost_system -lboost_filesystem -lboost_program_options	
HDF5LIB		= -lhdf5 

all: build_examples

build_examples: build build/printRecursively build/storeRecursivelyToHdf5 build/storeConfigAndDataToHdf5

build/printRecursively: examples/printRecursively.cpp ${library_headers} *.hpp
	${CC} ${CFLAGS} ${OPT} ${HDF5LIB} -I${HEADERS} -I./  $< -o $@

build/storeRecursivelyToHdf5: examples/storeRecursivelyToHdf5.cpp ${library_headers} *.hpp
	${CC} ${CFLAGS} ${OPT} ${HDF5LIB} ${BOOSTLIB} -I${HEADERS} -I./  $< -o $@

build/storeConfigAndDataToHdf5: examples/storeConfigAndDataToHdf5.cpp ${library_headers} *.hpp
	${CC} ${CFLAGS} ${OPT} ${HDF5LIB} ${BOOSTLIB} -I${HEADERS} -I./  $< -o $@

build:
	mkdir $@

clean:
	rm -rf build