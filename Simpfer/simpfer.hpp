#include "simpfer_index.hpp"


// verification by sequential scan
void sequential_scan(data *user, const float ip_) {

    for (unsigned int i = 0; i < item_set.size(); ++i) {

        // guarantee to be top-k
        if (ip_ >= user->norm * item_set[i].norm) break;

        // top-k comp. complete
        if (user->threshold >= user->norm * item_set[i].norm) break;

        // compute ip
        const float ip = compute_ip(*user, item_set[i]);
        ++ip_count;

        // update top-k
        user->update_topk(ip, item_set[i].identifier, k);

        // out from top-k
        if (user->threshold > ip) break;
    }
}

// main operation
void online_processing(const data &query_item) {

    start = std::chrono::system_clock::now();

    // norm computation
    float norm = 0;
    for (unsigned int i = 0; i < dimensionality; ++i) norm += query_item.vec[i] * query_item.vec[i];
    norm = sqrt(norm);

    #pragma omp parallel num_threads(thread_num)
	{
		#pragma omp for schedule(dynamic) reduction(+:ip_count)
        for (unsigned int i = 0; i < block_set.size(); ++i) {

            // compute upper-bound in this block
            float upperbound = block_set[i].member[0]->norm * norm;

            // block-level filtering
            if (upperbound > block_set[i].lowerbound_array[k-1]) {

                for (unsigned int j = 0; j < block_set[i].member.size() - 1; ++j) {

                    // get user-vec
                    data* user = block_set[i].member[j];

                    // compute ip
                    const float ip = compute_ip(query_item, *user);
                    ++ip_count;

                    // comparison with lower-bound
                    if (ip > user->lowerbound_array[k-1]) {

                        upperbound = user->norm * item_set[k-1].norm;

                        // comparison with upper-bound
                        if (upperbound > ip) {

                            // init topk
                            user->update_topk(ip, 0, k);

                            // seq. scan
                            sequential_scan(user, ip);
                        }
                    }
                }
            }
        }
    }

    end = std::chrono::system_clock::now();
	double time_ = (double)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	time_online_processing += time_;
}