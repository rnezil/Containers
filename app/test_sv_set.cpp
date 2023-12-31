#define CATCH_CONFIG_MAIN

#include "ra/sv_set.hpp"
#include <iostream>
#include <functional>
#include <cstddef>
#include <catch2/catch.hpp>
#include <memory>

TEST_CASE("Check find member and constructors", "[functions]") {
	ra::container::sv_set<int>::ordered_and_unique_range oaur;
	int values[1000];
	for( int i {0}; i < 1000; ++i )
		values[i] = i;
	ra::container::sv_set<int> buggin(oaur, values, 1000);
	for( int i {0}; i < 1000; ++i )
		CHECK( *buggin.find(values[i]) == values[i] );

	ra::container::sv_set<int>::ordered_and_unique_range oaf;
	int floaties[2500];
	int r;
	for( int i {0}; i < 2500; ++i ){
		r = (int)i;
		floaties[i] = (r + 5)/45;
		ra::container::sv_set<int> covid19(oaf, floaties, i+1);
		CHECK( *covid19.find(floaties[i]) == floaties[i] );
		CHECK( *covid19.find(floaties[i/2]) == floaties[i/2] );
		CHECK( *covid19.find(floaties[i/3]) == floaties[i/3] );
		CHECK( *covid19.find(floaties[i/4]) == floaties[i/4] );
		CHECK( *covid19.find(floaties[i/5]) == floaties[i/5] );
		CHECK( *covid19.find(floaties[i/6]) == floaties[i/6] );
		CHECK( *covid19.find(floaties[i/7]) == floaties[i/7] );
		CHECK( *covid19.find(floaties[i/8]) == floaties[i/8] );
		if( i > 99 && i < 1999 ){
			CHECK( *covid19.find(floaties[i/2 + i/3]) == floaties[i/2 + i/3] );
			CHECK( *covid19.find(floaties[i/2 + i/5]) == floaties[i/2 + i/5] );
			CHECK( *covid19.find(floaties[i/2 + i/7]) == floaties[i/2 + i/7] );
			CHECK( *covid19.find(floaties[i/2 + i/9]) == floaties[i/2 + i/9] );
		}
		CHECK( *covid19.find(floaties[0]) == floaties[0] );
	}
}

TEST_CASE("Check find, insert, and erase  members; check constructors; check size, capacity, reserve, and shrink_to_fit; check begin and end; check assignment operators; check swap and clear", "[functions]"){
	int vals[10] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18};
	ra::container::sv_set<int>::ordered_and_unique_range oaur;
	ra::container::sv_set<int> stickitin(oaur, vals, 10);
	CHECK( *stickitin.insert(-1).first == -1 );
	CHECK( *stickitin.insert(1).first == 1 );
	CHECK( *stickitin.insert(3).first == 3 );
	CHECK( *stickitin.insert(5).first == 5 );
	CHECK( *stickitin.insert(7).first == 7 );
	CHECK( *stickitin.insert(9).first == 9 );
	CHECK( *stickitin.insert(11).first == 11 );
	CHECK( *stickitin.insert(13).first == 13 );
	CHECK( *stickitin.insert(15).first == 15 );
	CHECK( *stickitin.insert(17).first == 17 );
	CHECK( *stickitin.insert(19).first == 19 );
	CHECK( *stickitin.insert(-1).first == -1 );
	CHECK( *stickitin.insert(1).first == 1 );
	CHECK( *stickitin.insert(3).first == 3 );
	CHECK( *stickitin.insert(5).first == 5 );
	CHECK( *stickitin.insert(7).first == 7 );
	CHECK( *stickitin.insert(9).first == 9 );
	CHECK( *stickitin.insert(11).first == 11 );
	CHECK( *stickitin.insert(13).first == 13 );
	CHECK( *stickitin.insert(15).first == 15 );
	CHECK( *stickitin.insert(17).first == 17 );
	CHECK( *stickitin.insert(19).first == 19 );
	ra::container::sv_set<float> foo;
	for( int i = -1000; i < 1000; ++i ){
		CHECK( *foo.insert((float)((float)i/2)).first == (float)((float)i/2) );
		CHECK( *foo.find((float)((float)i/2)) == (float)((float)i/2) );
	}
	for( int i = -1000; i < 1000; ++i ){
		CHECK( *foo.find((float)((float)i/2)) == (float)((float)i/2) );
	}
	ra::container::sv_set<float> bar = foo;
	for( int i = -1000; i < 999; ++i ){
		CHECK( *foo.erase(foo.find((float)i/2)) == (float)(i+1)/2 );
	}
	CHECK( foo.size() == 1 );
	CHECK( foo.capacity() == 2000 );
	CHECK( *foo.begin() == 499.5 );
	CHECK( *(foo.end() - 1) == 499.5 );
	auto baz = foo.erase(foo.find(499.5));
	CHECK( baz == foo.end() );

/*	auto iter = foo.begin() + 1000;
	int rich = 0;
	while( iter != foo.end() ){
		std::cout << *iter << '\t';
		++rich;
		if( rich == 10 ){
			std::cout << '\n';
			rich = 0;
		}
		++iter;
	}std::cout << '\n';*/
	ra::container::sv_set<float>::ordered_and_unique_range oaf;
	ra::container::sv_set<float> qux(oaf, bar.begin(), 2000);
	auto qqq = qux.begin();
	auto bbb = bar.begin();
	while( qqq != qux.end() ){
		CHECK( *qqq == *bbb );
		++qqq;
		++bbb;
	}
	CHECK( qqq == qux.end() );
	CHECK( bbb == bar.end() );
	auto corge = std::move( qux );
	qqq = corge.begin();
	bbb = bar.begin();
	while( qqq != corge.end() ){
		CHECK( *qqq == *bbb );
		++qqq;
		++bbb;
	}
	CHECK( qux.size() == 0 );
	CHECK( qux.capacity() == 0 );
	CHECK( qux.begin() == qux.end() );
	qux = std::move( corge );
	qqq = qux.begin();
	bbb = bar.begin();
	while( qqq != qux.end() ){
		CHECK( *qqq == *bbb );
		++qqq;
		++bbb;
	}
	CHECK( corge.size() == 0 );
	CHECK( corge.capacity() == 0 );
	CHECK( corge.begin() == corge.end() );
	corge = qux;
	auto ccc = corge.begin();
	qqq = qux.begin();
	bbb = bar.begin();
	while( ccc != corge.end() ){
		CHECK( *qqq == *bbb );
		CHECK( *ccc == *qqq );
		CHECK( *bbb == *ccc );
		++ccc;
		++qqq;
		++bbb;
	}
	CHECK( corge.size() == qux.size() );
	CHECK( corge.size() == bar.size() );
	CHECK( qux.size() == bar.size() );
	CHECK( corge.capacity() == qux.capacity() );
	CHECK( corge.capacity() == bar.capacity() );
	CHECK( qux.capacity() == bar.capacity() );
	CHECK( ccc == corge.end() );
	CHECK( bbb == bar.end() );
	CHECK( qqq == qux.end() );
	CHECK( corge.size() == 2000 );
	for( int i = 999; i > -1001; --i ){
		CHECK( bar.end() == bar.erase(bar.find((float)i/2)) );
	}
	CHECK( bar.begin() == bar.end() );
	CHECK( corge.capacity() == 2000 );
	corge.reserve(3000);
	CHECK( corge.capacity() == 3000 );
	CHECK( corge.size() == 2000 );
	CHECK( qux.size() == 2000 );
	CHECK( qux.capacity() == 2000 );
	qqq = qux.begin();
	ccc = corge.begin();
	while( qqq != qux.end() ){
		CHECK( *qqq == *ccc );
		++qqq;
		++ccc;
	}
	corge.shrink_to_fit();
	CHECK( corge.size() == 2000 );
	CHECK( corge.capacity() == 2000 );
	qqq = qux.begin();
	ccc = corge.begin();
	while( qqq != qux.end() ){
		CHECK( *qqq == *ccc );
		++qqq;
		++ccc;
	}
	CHECK( qqq == qux.end() );
	CHECK( ccc == corge.end() );
	float valss[10] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18};
	ra::container::sv_set<float>::ordered_and_unique_range ocra;
	ra::container::sv_set<float> ronch(ocra, valss, 10);
	auto hova = ronch;
	corge.swap(ronch);
	auto v1 = corge.begin();
	auto v2 = hova.begin();
	while( v1 != corge.end() ){
		CHECK( *v1 == *v2 );
		++v1;
		++v2;
	}
	CHECK( v1 == corge.end() );
	CHECK( v2 == hova.end() );
	auto b1 = qux.begin();
	auto b2 = ronch.begin();
	while( b1 != qux.end() ){
		CHECK( *b1 == *b2 );
		++b1;
		++b2;
	}
	CHECK( b1 == qux.end() );
	CHECK( b2 == ronch.end() );
	ronch.clear();
	CHECK( ronch.size() == 0 );
	CHECK( ronch.capacity() == 0 );
	CHECK( ronch.begin() == ronch.end() );
	ronch.swap(qux);
	CHECK( qux.size() == 0 );
	CHECK( qux.capacity() == 0 );
	CHECK( qux.begin() == qux.end() );
	CHECK( ronch.size() == 2000 );
	CHECK( ronch.capacity() == 2000 );
	qux.clear();
}

/*
TEMPLATE_TEST_CASE("Check += and -=  operator with positive resulting value", "[operators]", short, int, long, long long, unsigned short, unsigned, unsigned long, unsigned long long, int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t, intmax_t, uintmax_t) {
	ra::math::rational frac1 { 1 , 2 };
	ra::math::rational frac2 { 1 , 2 };
	ra::math::rational frac3 { 3 , 8 };
	ra::math::rational frac4 { 7 , 8 };
	ra::math::rational frac5 { 1 , 8 };
	frac1 += frac3;
	frac2 -= frac3;
	CHECK( frac1 == frac4 );
	CHECK( frac2 == frac5 );
}
*/
/*int main(){
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
	ra::container::sv_set<int> move_set(std::move(value_set));
	auto itermove = move_set.begin();
	std::cout << "Move set contains:";
	while(itermove != move_set.end()){
		std::cout << ' ' << *itermove;
		++itermove;
	}std::cout << '\n';
	iterint = value_set.begin();
	std::cout << "Value set contents:";
	while( iterint != value_set.end() ){
		std::cout << ' ' << *iterint;
		++iterint;
	}std::cout << '\n';
	int morevalues[] = {1, 3, 6, 9, 12, 15, 16, 18, 21, 36, 46, 57, 68, 79, 80};
	ra::container::sv_set<int> set_2(oaur, morevalues, 15);
	auto itermore = set_2.begin();
	std::cout << "Set 2 contains:";
	while( itermore != set_2.end() ) {
		std::cout << ' ' << *itermore;
		++itermore;
	}std::cout << '\n';
	std::cout << "Moving set 2 into move set...\n";
	move_set = std::move(set_2);
	itermove = move_set.begin();
	std::cout << "Move set contains:";
	while(itermove != move_set.end()){
		std::cout << ' ' << *itermove;
		++itermove;
	}std::cout << '\n';
	itermore = set_2.begin();
	std::cout << "Set 2 contains:";
	while( itermore != set_2.end() ) {
		std::cout << ' ' << *itermore;
		++itermore;
	}std::cout << '\n';
	std::cout << "Copy constructing bukis from move set...\n";
	ra::container::sv_set<int> bukis(move_set);
	auto iterrata = bukis.begin();
	std::cout << "Bukis contents:";
	while( iterrata != bukis.end() ){
		std::cout << ' ' << *iterrata;
		++iterrata;
	}std::cout << '\n';
	itermore = move_set.begin();
	std::cout << "Move set contains:";
	while( itermore != move_set.end() ) {
		std::cout << ' ' << *itermore;
		++itermore;
	}std::cout << '\n';
	std::cout << "Constructing test set...\n";
	ra::container::sv_set<int> test_set(oaur, randorn, 10);
	auto hata = test_set.begin();
	std::cout << "Test set contains:";
	while( hata != test_set.end() ){
		std::cout << ' ' << *hata;
		++hata;
	}std::cout << '\n';
	std::cout << "Copy assigning bukis to test set...\n";
	bukis = test_set;
	iterrata = bukis.begin();
	std::cout << "Bukis contents:";
	while( iterrata != bukis.end() ){
		std::cout << ' ' << *iterrata;
		++iterrata;
	}std::cout << '\n';
	hata = test_set.begin();
	std::cout << "Test set contains:";
	while( hata != test_set.end() ){
		std::cout << ' ' << *hata;
		++hata;
	}std::cout << '\n';
	test_set.reserve(40);
	hata = test_set.begin();
	std::cout << "Test set contains:";
	while( hata != test_set.end() ){
		std::cout << ' ' << *hata;
		++hata;
	}std::cout << '\n';
	return std::cout ? 0 : 1;
}*/
