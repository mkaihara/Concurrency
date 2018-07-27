#ifndef QUICK_SORT_HPP_
#define QUICK_SORT_HPP_

#include <iostream>
#include <algorithm>
#include <list>
#include <future>

template <typename T>
std::list<T> quick_sort(std::list<T> input)
{
	if (input.size() < 2) {
		return input;
	}

	std::list<T> result{};
	result.splice (result.begin(), input, input.begin());
	T pivot = *result.begin();

	auto divide_point = std::partition(input.begin(), input.end(),
			[&](T const & t)
			{
				return t < pivot;
			});

	std::list<T> lower_list{};
	lower_list.splice(lower_list.end(), input, input.begin(), divide_point);

	auto new_lower(quick_sort(std::move(lower_list)));
	std::future<std::list<T>> new_upper_future
	(std::async(&quick_sort<T>, std::move(input)));

	auto new_upper(quick_sort(std::move(input)));

	result.splice(result.begin(), new_lower);
	result.splice(result.end(), new_upper_future.get());

	return result;

}





#endif /* QUICK_SORT_HPP_ */
