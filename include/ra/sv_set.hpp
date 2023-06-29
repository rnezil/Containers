// Ryland Nezil
// V00157326
// rylandnezil@gmail.com
//
// Ordered set class template
// based on sorted array

#include <functional>
#include <cstddef>
#include <memory>
#include <iostream>
#include <utility>

namespace ra::container {

// Key: 	type of each element in the set
// Compare:	type of the callable entity used
// 		to test if one key is less than
// 		another (required since set is
// 		ordered)
template <class Key, class Compare = std::less<Key>>
class sv_set {
public:
	// Dummy variable
	struct ordered_and_unique_range {};

	using value_type = Key;
	using key_type = Key;
	using key_compare = Compare;
	using size_type = std::size_t;
	using iterator = Key*;
	using const_iterator = const Key*;
	
	// Default constructor
	// Creates an empty set with no storage allocated
	sv_set() noexcept(std::is_nothrow_default_constructible_v<key_compare>):
		begin_ {nullptr}, end_ {nullptr}, size_ {0}, capacity_ {0} {};

	// Creates an empty set with no storage allocate and
	// sets the comparison object for the set to comp
	//
	// explicit specifier indicates that this constructor
	// cannot be used for implicit conversions and copy-
	// initialization
	explicit sv_set(const Compare& comp): comp_ {comp},
		 begin_ {nullptr}, end_ {nullptr}, size_ {0}, capacity_ {0} {};

	// Allocates space for an array of n objects and fills
	// the array with n objects starting at first
	template <class InputIterator>
	sv_set(ordered_and_unique_range, InputIterator first,
			std::size_t n, const Compare& comp = Compare()):
		begin_ {static_cast<iterator>(::operator new(sizeof(value_type) * (n+1)))},
		end_ {&*std::uninitialized_copy_n<InputIterator, size_type>(first, n, begin_)},
		size_ {n}, capacity_ {n}, comp_ {comp} {}

	// Move construct a set
	sv_set(sv_set&& other) noexcept(std::is_nothrow_move_constructible_v<key_compare>):
		begin_ {other.begin_}, end_ {other.end_}, size_ {other.size_},
		       capacity_ {other.capacity_}, comp_ {std::move(other.comp_)} {
			       other.begin_ = nullptr;
			       other.end_ = nullptr;
			       other.size_ = 0;
			       other.capacity_ = 0;
		       }

	// Move assign a set
	sv_set& operator=(sv_set&& other) noexcept(
			std::is_nothrow_move_assignable_v<key_compare>){
		// Destroy contents of this set
		std::destroy<iterator>(begin_, end_);

		// De-allocate this set
		::operator delete(begin_);

		// Move other set into this set
		begin_ = other.begin_;
		end_ = other.end_;
		size_ = other.size_;
		capacity_ = other.capacity_;
		comp_ = std::move(other.comp_);

		// Leave other set empty
		other.begin_ = nullptr;
		other.end_ = nullptr;
		other.size_ = 0;
		other.capacity_ = 0;

		return *this;
	}

	// Member functions that serve only to return data members
	// of the set.
	const_iterator begin() const noexcept { return begin_; };
	iterator begin() noexcept { return begin_; };
	const_iterator end() const noexcept { return end_; };
	iterator end() noexcept { return end_; };
	size_type size() const noexcept { return size_; }
	size_type capacity() const noexcept { return capacity_; }
private:
	iterator begin_;
	iterator end_;
	size_type size_;
	size_type capacity_;
	key_compare comp_;
};
}
