## Introduction
* This repository provides an implementation of Simpfer.
* This is a fast algorithm for the reverse k-MIPS problem.
* As for the details about Simpfer, please read our RecSys2021 paper, [Reverse Maximum Inner Product Search: How to efficiently find users who would like to buy my item?](https://dl.acm.org/doi/10.1145/3460231.3474229).
* If you want faster online time, visit [here](https://github.com/amgt-d1/Simpfer-plus-plus).
	* This is an extended version of Simpfer.
	* c-Simpfar presented in ACM TWEB is also available by changing c in `file_input.hpp' in this repository.

## Requirement
* Linux OS (Ubuntu).
   * The others have not been tested.
* `g++ 7.5.0` (or higher version) and `Openmp`.

## How to use
* Parameter configuration can be done via txt files in `parameter` directory.
* Dataset should be stored in `dataset` directory.
	* We assign a unique dataset ID for each dataset. You can freely assign it.
	* In `input_matrix()` of `data.hpp`, you can freely write codes for reading your dataset.
* Computation time will be stored in `result` directory.
* Compile: `g++ -O3 -o simpfer.out main.cpp -std=c++14 -fopenmp -Wall`
* Run: `./simpfer.out`


## Citation
If you use our implementation, please cite the following paper.
``` 
@inproceedings{amagata2021reverse,  
    title={Reverse Maximum Inner Product Search: How to efficiently find users who would like to buy my item?},  
    author={Amagata, Daichi and Hara, Takahiro},  
    booktitle={RecSys},  
    pages={273--281},  
    year={2021}  
}
``` 

## License
Copyright (c) 2021 Daichi Amagata  
This software is released under the [MIT license](https://github.com/amgt-d1/Simpfer/blob/main/license.txt).
