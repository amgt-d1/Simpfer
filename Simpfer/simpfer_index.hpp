#include "file_output.hpp"


// set constant for O(k_max)
const float coeff = 2.0;


// ip computation
float compute_ip(const data &q, const data& p) {

	float ip = 0;
	for(unsigned int i = 0; i < dimensionality; ++i) ip += q.vec[i] * p.vec[i];

	return ip;
}

// norm computation
void compute_norm() {

	start = std::chrono::system_clock::now();

	// norm computation
	for (unsigned int i = 0; i < query_set.size(); ++i) query_set[i].norm_computation();
	for (unsigned int i = 0; i < item_set.size(); ++i) item_set[i].norm_computation();

	// sort by norm in descending order
	std::sort(query_set.begin(), query_set.end(), std::greater<data>());
	std::sort(item_set.begin(), item_set.end(), std::greater<data>());

	end = std::chrono::system_clock::now();
	time_norm_computation = (double)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	time_pre_processing += time_norm_computation;
	std::cout << " Norm computation time: " << time_norm_computation / 1000 << "[millisec]\n";
}

// lower-bound computation
void compute_lowerbound() {

	start = std::chrono::system_clock::now();

	const unsigned int size = k_max * coeff;

	for (unsigned int i = 0; i < query_set.size(); ++i) {

		for (unsigned int j = 0; j < size; ++j) {

			// ip comp.
			const float ip = compute_ip(query_set[i], item_set[j]);

			// update top-k
			query_set[i].update_topk(ip, item_set[j].identifier, k_max);
		}

		// convert map to array
		query_set[i].make_lb_array();
	}

	end = std::chrono::system_clock::now();
	time_lower_bound_computation = (double)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	time_pre_processing += time_lower_bound_computation;
	std::cout << " Lower-bound computation time: " << time_lower_bound_computation / 1000 << "[millisec]\n";
}

// blocking
void blocking() {

	start = std::chrono::system_clock::now();

	// determine size
	const unsigned int block_size = (unsigned int)(log2(item_set.size()) * 2);

	// make block
	block blk;

	// block assignment
	for (unsigned int i = 0; i < query_set.size(); ++i) {

		// assign block id
		query_set[i].block_id = blk.identifier;

		// insert into block
		blk.member.push_back(&query_set[i]);

		// init blk
		if (blk.member.size() == block_size) {

			// update lower-bound array
			blk.update_lowerbound_array();

			// insert into set
			block_set.push_back(blk);

			// init
			blk.init();
		}
	}

	end = std::chrono::system_clock::now();
	time_blocking = (double)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	time_pre_processing += time_blocking;
	std::cout << " Blocking time: " << time_blocking / 1000 << "[millisec]\n\n";
}

// pre-processing
void pre_processing() {

	// norm computation
	compute_norm();

	// lower-bound computation
	compute_lowerbound();

	// blocking
	blocking();

	// init
	for (unsigned int i = 0; i < query_set.size(); ++i) query_set[i].init();
}
