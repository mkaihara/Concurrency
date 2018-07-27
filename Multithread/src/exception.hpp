#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_

#include <iostream>
#include <thread>
#include <future>
#include <stdexcept>
#include <complex>
#include <mutex>

std::mutex m{};



void throw_exception() {
	throw std::runtime_error("The number cannot be negative");
}

void calculate_square_root (std::promise<int>& prom) {

	std::unique_lock<std::mutex> ul{m};
	std::cout << "calc_sqrt thread id: " << std::this_thread::get_id() << std::endl;
	ul.unlock();
	int x {1};
	ul.lock();
	std::cout << "Please, enter an integer value: ";
	ul.unlock();
	try {
		std::cin >> x;
		if (x < 0) {
			throw_exception();
		}
		prom.set_value(std::sqrt(x));
	}
	catch (std::exception&){
		prom.set_exception (std::current_exception());
	}

}

void print_result (std::future<int>& fut) {
	std::unique_lock<std::mutex> ul{m};
	std::cout << "print_result thread id: " << std::this_thread::get_id() << std::endl;
	ul.unlock();

	try {
		int x = fut.get();
		std::cout << "value: " << x << std::endl;
	}
	catch (std::exception & e) {
		std::cout << "[exception caught: " << e.what() << "]" << std::endl;
	}
}

void run_code() {

	std::unique_lock<std::mutex> ul{m};
	std::cout << "main thread id: " << std::this_thread::get_id() << std::endl;
	ul.unlock();

	std::promise<int> prom{};
	std::future<int> fut = prom.get_future();

	std::thread printing_thread (print_result, std::ref(fut));
	std::thread calculation_thread (calculate_square_root, std::ref(prom));

	printing_thread.join();
	calculation_thread.join();
}

#endif /* EXCEPTION_HPP_ */
