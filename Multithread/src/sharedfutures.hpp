#ifndef SHAREDFUTURES_HPP_
#define SHAREDFUTURES_HPP_

#include <iostream>
#include <thread>
#include <future>
#include <stdexcept>
#include <chrono>
#include <mutex>

void print_result (std::shared_future<int>& fut) {
	std::cout << fut.get() << std::endl;

	/*if (fut.valid()) {
		std::cout << "this is valid future" << std::endl;
		std::cout << fut.get() << std::endl;
	}
	else {
		std::cout << "this is invalid future" << std::endl;
	}
*/
}

void run_code() {

	std::promise<int> prom{};
	std::shared_future<int> fut(prom.get_future());

	std::thread th1(print_result, std::ref(fut));
	std::thread th2(print_result, std::ref(fut));

	prom.set_value(5);

	th1.join();
	th2.join();

}




#endif /* SHAREDFUTURES_HPP_ */
