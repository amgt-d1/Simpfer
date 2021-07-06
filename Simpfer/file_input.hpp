#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <time.h>


// dataset identifier
unsigned int dataset_id = 0;

// data dimensionality
unsigned int dimensionality = 50;

// dataset sampling rate
float sampling_rate = 1.0;

// k_max
const unsigned int k_max = 25;

// top-k
unsigned int k = 0;

// #threads
unsigned int thread_num = 0;


// get current time
void get_current_time() {

	time_t t = time(NULL);
	printf(" %s\n", ctime(&t));
}

// parameter input
void input_parameter() {

	std::ifstream ifs_dataset_id("parameter/dataset_id.txt");
	std::ifstream ifs_sampling_rate("parameter/sampling_rate.txt");
	std::ifstream ifs_thread_num("parameter/thread_num.txt");
	std::ifstream ifs_k("parameter/k.txt");

	if (ifs_dataset_id.fail()) {
		std::cout << " dataset_id.txt does not exist." << std::endl;
		std::exit(0);
	}
	if (ifs_sampling_rate.fail()) {
		std::cout << " sampling_rate.txt does not exist." << std::endl;
		std::exit(0);
	}
	if (ifs_k.fail()) {
		std::cout << " k.txt does not exist." << std::endl;
		std::exit(0);
	}
	if (ifs_thread_num.fail()) {
		std::cout << " thread_num.txt does not exist." << std::endl;
		std::exit(0);
	}

	while (!ifs_dataset_id.eof()) { ifs_dataset_id >> dataset_id; }
	while (!ifs_sampling_rate.eof()) { ifs_sampling_rate >> sampling_rate; }
	while (!ifs_k.eof()) { ifs_k >> k; }
	while (!ifs_thread_num.eof()) { ifs_thread_num >> thread_num; }
}