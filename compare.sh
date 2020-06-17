#!/bin/bash

while getopts p: flag
do
    case "${flag}" in
        p) halidePath=${OPTARG};;
    esac
done

printf "<< Executing make command >>\n"
make halidePath=${halidePath}

printf "<< Testing with a width of 1000 and height 500, alpha=4, beta=5 >>\n"
printf "\t<< Executing C version >>\n"
bin/c 0 500 1000 4 5 0 1
printf "\t<< Executing Halide version >>\n"
LD_LIBRARY_PATH=${halidePath}/bin bin/halide  0 500 1000 4 5 0 1

printf "\n<< Checking Results differences >>\n"
differences=`diff -s -q results/c_results.txt results/halide_result.txt`
printf "$differences"