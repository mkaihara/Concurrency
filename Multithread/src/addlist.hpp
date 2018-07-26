//============================================================================
// Name        : addlist.hpp
// Author      : Marcelo Kaihara
// Version     :
// Copyright   : 
// Description : Example code to add numbers in a list in parallel
//============================================================================

#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <numeric>
#include <functional>
#include "stack.hpp"

template<typename iterator, typename T>
void accumulate_block (iterator start, iterator end, T & result){
	result = std::accumulate(start, end, result);
	std::cout << std::this_thread::get_id() << ">>" << result << std::endl;
}

template <typename iterator, typename T>
T accumulate_list (iterator start, iterator end, T init) {

	int distance { static_cast<int>(std::distance(start, end)) };
	if (!distance) {
		return init;
	}

	const int min_elements { 500 };
	int max_threads { distance / min_elements };
	std::cout << "Max number of threads to run to avoid heavy overload : " << max_threads << std::endl;

	int allowed_thread { static_cast<int> (std::thread::hardware_concurrency()) };
	std::cout << "Max number of threads to run to avoid oversubscription : " << allowed_thread << std::endl;

	int running_threads { std::min(allowed_thread > 0 ? allowed_thread : 2, max_threads )};
	std::cout << "Running threads : " << running_threads << std::endl;

	std::vector<T> results (running_threads);

	std::vector<std::thread> threads (running_threads -1);

	iterator block_start = start;
	int block_size { distance / running_threads } ;

	for (int i{0}; i < (running_threads - 1); ++i) {
		iterator block_end = block_start;
		std::advance (block_end, block_size);
		threads[i] = std::thread (accumulate_block<iterator, T>, block_start, block_end, std::ref(results[i]));
		block_start = block_end;
	}

	accumulate_block<iterator, T> (block_start, end, results[running_threads - 1]);

	std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

	return std::accumulate(results.begin(), results.end(), init);

}

int run_code() {
	int number_of_elements{500000000};
	std::vector<int> values (number_of_elements);
	int i{0};
	for (auto &x:values) {
		x = i++;
	}
	long val{0};

	long accum {0};
	for (auto &x:values){
		accum+=x;
	}
	std::cout << "Serial accumulation : " << accum << std::endl;

	long addition { accumulate_list(values.begin(), values.end(), val) };
	std::cout << "Summation of list : " << addition << std::endl;

	return 0;
}
