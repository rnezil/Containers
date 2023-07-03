#define CATCH_CONFIG_MAIN

#include "ra/sv_set.hpp"
#include <iostream>
#include <functional>
#include <cstddef>
#include <catch2/catch.hpp>
#include <memory>

TEST_CASE("Check find member", "[functions]") {
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

TEST_CASE("Check insert member", "[functions]"){
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
	for( int i = -1000; i < 1000;++i ){
		CHECK( *foo.insert((float)((float)i/2)).first == (float)((float)i/2) );
		CHECK( *foo.find((float)((float)i/2)) == (float)((float)i/2) );
	}
	for( int i = -1000; i < 1000; ++i ){
		CHECK( *foo.find((float)((float)i/2)) == (float)((float)i/2) );
	}
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
