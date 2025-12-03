#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <sys/time.h>
#include <algorithm>
#include <limits>
#include "Colors.hpp"

double getTimeInMicroseconds(struct timeval start, struct timeval end);
void printUsage();

template <typename T>
void check_duplicate(T &stack) {
	for (typename T::iterator it = stack.begin(); it != stack.end(); ++it) {
		for (typename T::iterator it2 = it + 1; it2 != stack.end(); ++it2) {
			if (*it == *it2) {
				throw std::invalid_argument("Duplicate number found");
			}
		}
	}
}

template <typename T>
T fill_stack(int ac, char **av, T &stack) {
	for (int i = 1; i < ac; i++) {
		char *end;
		if (*av[i] == 0)
			throw std::invalid_argument("invalid number found");
		long value = std::strtol(av[i], &end, 10);
		if (*end != 0)
			throw std::invalid_argument("invalid number found");
		if (value > std::numeric_limits<int>::max() || value < 0)
			throw std::invalid_argument("Overflow or negative number found");
		stack.push_back(static_cast<int>(value));
	}
	check_duplicate(stack);
	return stack;
}

template <typename P>
void sort_pairs(P &pairs) {
	typename P::iterator it = pairs.begin();
	for (; it != pairs.end(); it++) {
		if (it->second > it->first)
			std::swap(it->second, it->first);
	}
}

template <typename T, typename P>
P create_pairs(T &stack, P &pairs) {
	for (typename T::iterator it = stack.begin(); it != stack.end(); it++) {
		if (it + 1 != stack.end()) {
			if (*it > *(it + 1))
				pairs.push_back(std::make_pair(*it, *(it + 1)));
			else
				pairs.push_back(std::make_pair(*(it + 1), *it));
			it++;
		}
	}
	sort_pairs(pairs);
	return pairs;
}

template <typename P>
void merge(P &pairs, int left, int mid, int right) {
	int n1 = mid - left + 1;
	int n2 = right - mid;
	P L(n1), R(n2);
	for (int i = 0; i < n1; i++)
		L[i] = pairs[left + i];
	for (int j = 0; j < n2; j++)
		R[j] = pairs[mid + 1 + j];

	int i = 0, j = 0;
	int k = left;
	while (i < n1 && j < n2) {
		if (L[i] <= R[j]) {
			pairs[k] = L[i];
			i++;
		} else {
			pairs[k] = R[j];
			j++;
		}
		k++;
	}
	while (i < n1) {
		pairs[k] = L[i];
		i++;
		k++;
	}
	while (j < n2) {
		pairs[k] = R[j];
		j++;
		k++;
	}
}

template <typename P>
void merge_sort(P &pairs, int left, int right) {
	if (left < right) {
		int mid = left + (right - left) / 2;
		merge_sort(pairs, left, mid);
		merge_sort(pairs, mid + 1, right);
		merge(pairs, left, mid, right);
	}
}

template <typename T, typename P>
T final_stack(P &pairs, T &final) {
	for (size_t i = 0; i < pairs.size(); i++) {
		final.push_back(pairs[i].first);
	}
	return final;
}

template <typename T>
void fill_jacobsthal_array(T &jacobsthalArray) {
	int size = jacobsthalArray.size();

	if (size > 0)
		jacobsthalArray[0] = 3;
	if (size > 1)
		jacobsthalArray[1] = 5;
	for (int i = 2; i < size; i++) {
		jacobsthalArray[i] = jacobsthalArray[i - 1] + 2 * jacobsthalArray[i - 2];
	}
}

template <typename T, typename P>
void jacobsthal_insert(T &jacobsthalArray, P &pairs, T &final) {
	size_t j_index = 0;
	size_t vec_index = 0;

	while (vec_index < pairs.size() && j_index < jacobsthalArray.size() &&
		   (size_t)jacobsthalArray[j_index] < pairs.size()) {
		final.insert(
			std::lower_bound(final.begin(), final.end(), pairs[jacobsthalArray[j_index]].second),
			pairs[jacobsthalArray[j_index]].second);
		while (vec_index < (size_t)jacobsthalArray[j_index]) {
			final.insert(std::lower_bound(final.begin(), final.end(), pairs[vec_index].second),
						 pairs[vec_index].second);
			vec_index++;
		}
		vec_index++;
		j_index++;
	}
	while (vec_index < pairs.size()) {
		final.insert(std::lower_bound(final.begin(), final.end(), pairs[vec_index].second),
					 pairs[vec_index].second);
		vec_index++;
	}
}

template <typename T, typename P>
T PmergeMe(T &stack, T &final) {
	P pairs;
	create_pairs(stack, pairs);
	if (pairs.size() > 0)
		merge_sort(pairs, 0, pairs.size() - 1);
	T jacobsthalArray(stack.size());
	fill_jacobsthal_array(jacobsthalArray);
	final_stack(pairs, final);
	if (stack.size() % 2 != 0)
		final.insert(std::lower_bound(final.begin(), final.end(), *stack.rbegin()),
					 *stack.rbegin());
	jacobsthal_insert(jacobsthalArray, pairs, final);
	return final;
}

#endif