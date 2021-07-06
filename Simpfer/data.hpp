#include "file_input.hpp"
#include <vector>
#include <functional>
#include <algorithm>
#include <map>
#include <cfloat>


// definition of data
class data {
public:

	unsigned int identifier = 0;
	std::vector<float> vec;
	float norm = 0;
	
	std::map<float, unsigned int, std::greater<float>> topk;
	float threshold = 0;
	std::vector<float> lowerbound_array;

	unsigned int block_id = 0;

	// constructor
	data() { }

	// norm computation
	void norm_computation() {

		for (unsigned int i = 0; i < vec.size(); ++i) norm += vec[i] * vec[i];
		norm = sqrt(norm);
	}

	// k-MIPS update
	void update_topk(const float ip, const unsigned int id, const unsigned int k_) {

		if (ip > threshold) topk.insert({ip,id});

		if (topk.size() > k_) {
			auto it = topk.end();
			--it;
			topk.erase(it);
		}

		if (topk.size() == k_) {
			auto it = topk.end();
			--it;
			threshold = it->first;
		}
	}

	// make lower-bound array
	void make_lb_array() {
		
		auto it = topk.begin();
		while(it != topk.end()) {
			lowerbound_array.push_back(it->first);
			++it;
		}
	}

	// init
	void init() {

		// clear top-k in pre-processing
		topk.clear();

		// clear threshold in pre-processing
		threshold = 0;
	}

	bool operator <(const data &d) const { return norm < d.norm; }
	bool operator >(const data &d) const { return norm > d.norm; }
};

// set of query & item vectors
std::vector<data> query_set, item_set;


// definition of block
class block{
public:

	unsigned int identifier = 0;
	std::vector<data*> member;
	std::vector<float> lowerbound_array;


	// constructor
	block() {

		lowerbound_array.resize(k_max);
		for (unsigned int i = 0; i < k_max; ++i) lowerbound_array[i] = FLT_MAX;
	}

	// init
	void init() {

		// increment identifier
		++identifier;

		// clear member
		member.clear();

		// init array
		for (unsigned int i = 0; i < k_max; ++i) lowerbound_array[i] = FLT_MAX;
	}

	// make lower-bound array
	void update_lowerbound_array() {

		for (unsigned int i = 0; i < k_max; ++i) {
			for (unsigned int j = 0; j < member.size(); ++j) {
				if (lowerbound_array[i] > member[j]->lowerbound_array[i]) lowerbound_array[i] = member[j]->lowerbound_array[i];
			}
		}
	}
};

// set of blocks
std::vector<block> block_set;


// input matrix
void input_matrix() {

	// file-name
	std::string f_name;

	data query, item;

	std::mt19937_64 mt(1);
	std::uniform_real_distribution<> uni_rnd(0,1);

	if (dataset_id == 0 || dataset_id == 1 || dataset_id == 2 || dataset_id == 3) {

		if (dataset_id == 0) f_name = "dataset/movielens_mf-50.txt";
		if (dataset_id == 1) f_name = "dataset/netflix_mf-50.txt";
		if (dataset_id == 2) f_name = "dataset/yahoosong_mf-50.txt";
		if (dataset_id == 3) f_name = "dataset/amazon_mf-50.txt";

		// file input
		std::ifstream ifs_file(f_name);
		std::string full_data;

		// error detection
		if (ifs_file.fail()) {
			std::cout << " file does not exist." << std::endl;
			std::exit(0);
		}

		while (std::getline(ifs_file, full_data)) {

			std::string meta_info;
			std::istringstream stream(full_data);
			std::string type = "";
			std::string flag = "";

			std::vector<float> vector_query, vector_item;

			for (unsigned int i = 0; i < dimensionality + 3; ++i) {

				std::getline(stream, meta_info, ' ');

				std::istringstream stream_(meta_info);
				double val = 0;

				switch (i) {

				case 0:	// obtain user or item

					type = meta_info;

					break;

				case 1:	// obtain T or F

					flag = meta_info;

					break;

				default:	// obtain value

					if (meta_info != "") {

						val = std::stof(meta_info);
						if (type[0] == 'p') vector_query.push_back(val);
						if (type[0] == 'q') vector_item.push_back(val);
					}

					break;
				}

				if (flag == "F") break;
			}

			// store vector
			if (flag == "T") {

				if (type[0] == 'p') {

					if (uni_rnd(mt) <= sampling_rate) {

						query.vec = vector_query;
						query_set.push_back(query);

						++query.identifier;
					}
				}

				if (type[0] == 'q') {

					item.vec = vector_item;
					item_set.push_back(item);

					++item.identifier;
				}
			}
		}
	}
}
