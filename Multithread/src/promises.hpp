#ifndef PROMISES_HPP_
#define PROMISES_HPP_

#include <iostream>
#include <functional>
#include <thread>
#include <future>
#include <stdexcept>

void print_int (std::future<int>& fut) {
	std::cout << "waiting for value from print thread" << std::endl;
	std::cout << "value: " << fut.get() << std::endl;
}

void run_code() {
	std::promise<int> prom{};
	std::future<int> fut = prom.get_future();

	std::thread print_thread(print_int, std::ref(fut));

	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	std::cout << "setting the value in main thread" << std::endl;
	prom.set_value(10);

	print_thread.join();
}

#endif /* PROMISES_HPP_ */
