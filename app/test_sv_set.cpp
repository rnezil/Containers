#include "ra/sv_set.hpp"
#include <iostream>
#include <functional>
#include <cstddef>

int main(){
	ra::container::sv_set<int> default_set;
	std::less<int> compare_obj;
	ra::container::sv_set<int> compare_set(compare_obj);
	int randorn[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	ra::container::sv_set<int>::ordered_and_unique_range oaur;
	ra::container::sv_set<int> value_set(oaur, randorn, 10);
	std::cout << "Default set size:\t" << default_set.size() << '\n'
		<< "Default set capacity:\t" << default_set.capacity() << '\n'
	       << "Compare set size:\t" << compare_set.size() << '\n'
       << "Compare set capacity:\t" << compare_set.capacity() << '\n'
<< "Value set size:\t\t" << value_set.size() << '\n'
<< "Value set capacity:\t" << value_set.capacity() << '\n';
	auto iterint = value_set.begin();
	std::cout << "Value set contents:";
	while( iterint != value_set.end() ){
		std::cout << ' ' << *iterint;
		++iterint;
	}std::cout << '\n';
	return std::cout ? 0 : 1;
}
