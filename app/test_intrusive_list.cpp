#include "ra/intrusive_list.hpp"
#include <iostream>
#include <vector>

namespace ri = ra::intrusive;

struct Widget {
	Widget (int value_) : value(value_) {}
	int value;
	ri::list_hook hook;
};

std::ostream& operator<<(std::ostream& out, const Widget& w){
	out << w.value;
	return out;
}

using iterator = ri::list<Widget, &Widget::hook>::intrusive_list_iterator;
using const_iterator = ri::list<Widget, &Widget::hook>::const_intrusive_list_iterator;

int main()
{
	std::vector<Widget> storage;
	storage.push_back(Widget(42));
	for( int i = 0; i < 10; ++i )
		storage.push_back(Widget(i*5));
	ri::list<Widget, &Widget::hook> values;
	for(auto&& i : storage) {
		values.push_back(i);
	}
	values.print();
	std::cout << "\n\n";
	auto jank = values.begin();
	std::cout << *jank << "\n\n\n";
	// Point to 3
	std::cout << *++++++++jank << "\n\n\n";
	Widget foo(69);
	std::cout << *values.insert(jank, foo) << "\n\n\n";
	values.print();
	return 0;
}
