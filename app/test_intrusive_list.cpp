#include "ra/intrusive_list.hpp"
#include <iostream>
#include <vector>

namespace ri = ra::intrusive;

struct Widget {
	Widget (int value_) : value(value_) {}
	int value;
	ri::list_hook hook;
};

using iterator = ri::list<Widget, &Widget::hook>::intrusive_list_iterator;
using const_iterator = ri::list<Widget, &Widget::hook>::const_intrusive_list_iterator;

int main()
{
	std::vector<Widget> storage;
	storage.push_back(Widget(42));
	ri::list<Widget, &Widget::hook> values;
	iterator a;
	const_iterator b;
	return 0;
}
