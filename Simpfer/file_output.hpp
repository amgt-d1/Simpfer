#include "data.hpp"
#include <chrono>
#include <unordered_set>


/***** pre-processing setting *****/

// variable for time measure
std::chrono::system_clock::time_point start, end, init;

// computation time
double time_pre_processing = 0;
double time_norm_computation = 0;
double time_lower_bound_computation = 0;
double time_blocking = 0;


// result output
void output_result_pre() {

    // file-name
	std::string f_name = "result/index_id(" + std::to_string(dataset_id) + ")_dim(" + std::to_string(dimensionality) + ")_sampling-rate(" + std::to_string(sampling_rate) + ")_thread(" + std::to_string(thread_num) + ").csv";

	std::ofstream file;
	file.open(f_name.c_str(), std::ios::out | std::ios::app);

	if (file.fail()) {
		std::cerr << " cannot open the pre-processing result file." << std::endl;
		file.clear();
		return;
	}

	file << "pre-processing time [msec]" << "," << "norm comp. time [msec]" << "," << "lower-bound comp. time [msec]" << "," << "blocking time [msec]" << "\n";
	file << time_pre_processing / 1000 << "," << time_norm_computation / 1000 << "," << time_lower_bound_computation / 1000 << "," << time_blocking / 1000 << "\n\n";
}


/***** online processing setting *****/

// computation time
double time_online_processing = 0;
long double ip_count = 0;

// query item cardinality
const unsigned int query_item_cardinality = 1000;

// set of query items (idx)
std::vector<unsigned int> query_item_set;

// pick query items
void sample_query_items() {

    std::mt19937_64 mt_(0);
	std::uniform_int_distribution<> rnd(0, item_set.size() - 1);

    std::unordered_set<unsigned int> idx;

    while (1) {

        idx.insert(rnd(mt_));
        if (idx.size() == query_item_cardinality) break;
    }

    auto it = idx.begin();
    while (it != idx.end()) {
        query_item_set.push_back(*it);
        ++it;
    }
}

// result output
void output_result() {

    // file-name
	std::string f_name = "result/simpfer_id(" + std::to_string(dataset_id) + ")_dim(" + std::to_string(dimensionality) + ")_sampling-rate(" + std::to_string(sampling_rate) + ")_k(" + std::to_string(k) + ").csv";

	std::ofstream file;
	file.open(f_name.c_str(), std::ios::out | std::ios::app);

	if (file.fail()) {
		std::cerr << " cannot open the result file." << std::endl;
		file.clear();
		return;
	}

	file << "processing time [msec]" << "," << "#ip comp." << "," << "#results" << "," << "#cores" <<"\n";
	file << time_online_processing / (1000 * query_item_cardinality) << "," << ip_count / query_item_cardinality << "," << thread_num << "\n\n";
}