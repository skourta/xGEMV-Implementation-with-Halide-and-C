all: bin/halide bin/c
bin/halide: src/halide_gemv.cpp
	g++ src/halide_gemv.cpp -g -I $(halidePath)/include -L $(halidePath)/bin -lHalide -lpthread -ldl -o bin/halide -std=c++11

bin/c: src/c_gemv.c
	gcc src/c_gemv.c -o bin/c

