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
		std::destroy<iterator>(begin(), end());

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

	// Copy constructor
	sv_set(const sv_set& other):
		begin_ {static_cast<iterator>(::operator new(sizeof(value_type) * (other.size() + 1)))},
		       end_ {&*std::uninitialized_copy<const_iterator>(other.begin(), other.end(), begin_)},
		       size_ {other.size()}, capacity_ {other.capacity()},
		       comp_ {other.key_comp()} {}

	// Copy assignment operator
	sv_set& operator=(const sv_set& other){
		// Destroy contents of this set
		std::destroy<iterator>(begin(), end());

		// De-allocate this set
		::operator delete(begin_);

		// Copy other set into this set
		begin_ = static_cast<iterator>(::operator new(sizeof(value_type) * (other.size() + 1)));
		end_ = std::uninitialized_copy<const_iterator>(other.begin(), other.end(), begin());
		size_ = other.size_;
		capacity_ = other.capacity_;
		comp_ = other.comp_;

		return *this;
	}

	~sv_set(){
		// Destroy all elements in set
		std::destroy<iterator>(begin(), end());

		// De-allocate set
		::operator delete(begin_);

		// Nullify pointers
		begin_ = nullptr;
		end_ = nullptr;
	}

	void reserve(size_type n){
		if( capacity() < n ){
			// Allocate space for array growth
			iterator xtra_space = static_cast<iterator>(
					::operator new(sizeof(value_type) * (n + 1)));

			// Move data from old array to new array
			end_ = &*std::uninitialized_move<iterator>(begin(), end(), xtra_space);

			// De-allocate old array
			::operator delete(begin_);

			// Point to start of new array
			begin_ = xtra_space;

			// Adjust capacity rating
			capacity_ = n;
		}
	}

	void shrink_to_fit(){
		if( size() != capacity() ){
			// Allocate space for new smalled-down array
			iterator shrunken_space = static_cast<iterator>(
					::operator new(sizeof(value_type) * (size() + 1)));

			// Move data from old array to new array
			end_ = &*std::uninitialized_move<iterator>(begin(), end(), shrunken_space);

			// De-allocate old array
			::operator delete(begin_);

			// Point to start of shrunken array
			begin_ = shrunken_space;

			// Adjust capacity rating
			capacity_ = size();
		}
	}

	std::pair<iterator, bool> insert(const key_type& x);

	iterator find(const key_type& k){
		iterator finder = begin() - 1;
		const_iterator upper = end() - 1;
		const_iterator lower = begin();

		if( size() & 0x1 ){
			// Size is odd
			finder += (size() + 1) / 2;
		}else{
			// Size is even
			finder += size() / 2;
		}

		// Iterate until k is found in set
		while( *finder != k ){
			if( finder == begin() || finder == end() - 1 )
				return end();

			if( comp_(*finder, k) ){
				// Finder is less than k, so
				// update lower bound of search
				lower = finder;

				// Increment finder
				if( (upper - finder) & 0x1 ){
					// Odd
					finder += (upper - finder + 1) / 2;
				}else{
					// Even
					finder += (upper - finder) / 2;
				}
			}else{
				// Finder is greater than k, so
				// update upper bound of search
				upper = finder;

				// Increment finder;
				if( (finder - lower) & 0x1 ){
					// Odd
					finder -= (finder - lower + 1) / 2;
				}else{
					// Even
					finder -= (finder - lower) / 2;
				}
			}
		}

		return finder;
	}
			

	// Member functions that serve only to return data members
	// of the set.
	key_compare key_comp() const { return comp_; }
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
