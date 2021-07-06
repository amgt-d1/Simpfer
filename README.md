## Introduction
* This repository provides an implementation of Simpfer.
* This is a fast algorithms for reverse k-MIPS problem.
* As for the details about Simpfer, please read our RecSys2021 paper, [Reverse Maximum Inner Product Search: How to efficiently find users who would like to buy my item?](https://).

## Requirement
* Linux OS (Ubuntu).
   * The others have not been tested.
* `g++ 7.4.0` (or higher version) and `Openmp`.

## How to use
* Parameter configuration can be done via txt files in `parameter` directory.
* Computation time will be stored in `result` directory.
* Compile: `g++ -O3 -o simpfer.out main.cpp -std=c++14 -fopenmp -Wall`
* Run: `./simpfer.out`

### Dataset IDs
- 0: MovieLens
- 1: Netflix
	- Netflix is zipped. Dataset files have to be at "dataset" directory.
- 2: Yahoo!
- 3: Amazon
