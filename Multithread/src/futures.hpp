#ifndef FUTURES_HPP_
#define FUTURES_HPP_

#include <iostream>
#include <future>
#include <thread>

int find_answer_how_old_universe_is() {
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	return 50000;
}

void do_other_calculations() {
	std::cout << "Doing other stuff" << std::endl;
}

void run_code() {
	//std::future<int>
	auto the_answer_future = std::async(find_answer_how_old_universe_is);
	do_other_calculations();
	std::cout << "The answer is " << the_answer_future.get() << std::endl;
}


#endif /* FUTURES_HPP_ */
