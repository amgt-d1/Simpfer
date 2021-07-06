#include "simpfer.hpp"


int main() {

    // input paramter
	input_parameter();

	// file input
	input_matrix();

    // print the current time
    get_current_time();

    std::cout << " --------------------\n";
	std::cout << " dataset id: " << dataset_id << "\n";
	std::cout << " query-set cardinality: " << query_set.size() << "\n";
    std::cout << " item-set cardinality: " << item_set.size() << "\n";
	std::cout << " dataset dimensionality: " << dimensionality << "\n";
    std::cout << " sampling rate: " << sampling_rate << "\n";
    std::cout << " k_max: " << k_max << "\n";
    std::cout << " k: " << k << "\n";
    std::cout << " number of threads: " << thread_num << "\n";
	std::cout << " --------------------\n\n";

    /***** pre-processing *****/
	pre_processing();

    // output result of pre-processing
    output_result_pre();


    /***** prepare for online processing *****/
    
    // get query items
    sample_query_items();


    /***** online processing *****/

    // query item variable
    data query_item;

    for (unsigned int i = 0; i < query_item_cardinality; ++i) {

        // get query item
        const unsigned int idx = query_item_set[i];
        query_item.vec = item_set[idx].vec;

        // reverse MIPS
        online_processing(query_item);

        // init
        for (unsigned int j = 0; j < query_set.size(); ++j) query_set[j].init();
    }

    // output result
    output_result();

    return 0;
}