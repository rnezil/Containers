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
#include <algorithm>

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
		begin_ {static_cast<iterator>(::operator new(sizeof(value_type)))},
		       end_ {begin_}, size_ {0}, capacity_ {0} {};

	// Creates an empty set with no storage allocate and
	// sets the comparison object for the set to comp
	//
	// explicit specifier indicates that this constructor
	// cannot be used for implicit conversions and copy-
	// initialization
	explicit sv_set(const Compare& comp): comp_ {comp},
		 begin_ {static_cast<iterator>(::operator new(sizeof(value_type)))},
		 end_ {begin_}, size_ {0}, capacity_ {0} {};

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
		begin_ {other.begin()}, end_ {other.end()}, size_ {other.size()},
		       capacity_ {other.capacity()}, comp_ {std::move(other.comp_)} {
			       other.begin_ = static_cast<iterator>(
					       ::operator new(sizeof(value_type)));
			       other.end_ = other.begin();
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
		begin_ = other.begin();
		end_ = other.end();
		size_ = other.size();
		capacity_ = other.capacity();
		comp_ = std::move(other.comp_);

		// Leave other set empty
		other.begin_ = static_cast<iterator>(::operator new(sizeof(value_type)));
		other.end_ = other.begin();
		other.size_ = 0;
		other.capacity_ = 0;

		return *this;
	}

	// Copy constructor
	sv_set(const sv_set& other):
		begin_ {static_cast<iterator>(
				::operator new(sizeof(value_type) * (other.size() + 1)))},
		       end_ {&*std::uninitialized_copy<const_iterator>(
				       other.begin(), other.end(), begin_)},
		       size_ {other.size()}, capacity_ {other.capacity()},
		       comp_ {other.key_comp()} {}

	// Copy assignment operator
	sv_set& operator=(const sv_set& other){
		// Destroy contents of this set
		std::destroy<iterator>(begin(), end());
	
		// De-allocate this set
		::operator delete(begin_);

		// Copy other set into this set
		begin_ = static_cast<iterator>(::operator new(
					sizeof(value_type) * (other.size() + 1)));
		end_ = std::uninitialized_copy<const_iterator>(
				other.begin(), other.end(), begin());
		size_ = other.size();
		capacity_ = other.capacity();
		comp_ = other.key_comp();

		return *this;
	}

	~sv_set(){
		// Destroy all elements in set
		std::destroy<iterator>(begin(), end());

		// De-allocate set
		::operator delete(begin_);
	}

	void reserve(size_type n){
		if( capacity() < n ){
			// Move values of this set into temproary set
			auto tmp = std::move(*this);

			// De-allocate this set
			::operator delete(begin_);

			// Allocate space for new set
			begin_ = static_cast<iterator>(
					::operator new(sizeof(value_type) * (n + 1)));

			// Move data from temporary set to new set
			end_ = &*std::uninitialized_move<iterator>(
					tmp.begin(), tmp.end(), begin());

			// Restore size value
			size_ = tmp.size();

			// Restore compare function
			comp_ = std::move(tmp.comp_);

			// Adjust capacity rating
			capacity_ = n;
		}
	}

	void shrink_to_fit(){
		if( size() != capacity() ){
			// Store this set in temporary variable
			auto tmp = std::move(*this);

			// De-allocate this set
			::operator delete(begin_);
	
			// Allocate space for new smalled-down set
			begin_ = static_cast<iterator>(
					::operator new(sizeof(value_type) * (size() + 1)));
	
			// Move data from temporary set to new set
			end_ = &*std::uninitialized_move<iterator>(
					tmp.begin(), tmp.end(), begin());
	
			// Restore size value
			size_ = tmp.size();

			// Restore comparison function
			comp_ = std::move(tmp.comp_);

			// Adjust capacity rating
			capacity_ = size();
		}
	}

	std::pair<iterator, bool> insert(const key_type& x){
		// Already in set protocol
		if( find(x) != end() )
			return std::make_pair<iterator, bool>(find(x), false);

		// Get a mutable iterator for the
		// value to be inserted
		value_type xclone = x;
		iterator val = &xclone;

		// Empty set protocol
		if( begin() == end() ){
			reserve(1);
			end_ = &*std::uninitialized_move_n<
				iterator, size_type, iterator>(val, 1, end()).second;
			++size_;
			return std::make_pair<iterator, bool>(begin(), true);
		}

		// Expand set if needed
		if( size() == capacity() )
			reserve( size() + 1);

		// Variable that will represent where
		// new value is placed in the set
		iterator spot = begin();

		// Find the spot in the array where
		// the new value should be inserted
		while( comp_(*spot, *val) ){
			++spot;
			if( spot == end() ){
				// If insertion to take place at end
				// of set, perform insertion right
				// here and return
				end_ = &*std::uninitialized_move_n<
					iterator, size_type, iterator>(val, 1, end()).second;
				++size_;
				return std::make_pair<iterator, bool>(end()-1, true);
			}
		}

		// Allocate temporary space for
		// objects that need to be bumped
		// down
		iterator temp_begin = static_cast<iterator>(::operator new(
					sizeof(value_type) * (end() - spot + 1)));

		// Move bump-down objects into
		// temporary space
		iterator temp_end = &*std::uninitialized_move<iterator>(
				spot, end(), temp_begin);

		// Insert new object into spot
		std::move_backward(val, val+1, spot+1);

		// Update size
		++size_;

		// Move rest of values back in from
		// temporary space
		std::move_backward(temp_begin, temp_end - 1, end());
		end_ = &*std::uninitialized_move_n<iterator, size_type, iterator>(
				temp_end-1, 1, end() ).second;
	
		// Destroy objects in temporary space
		std::destroy<iterator>(temp_begin, temp_end);

		// De-allocate temporary space
		::operator delete(temp_begin);

		// Return pointer to inserted element
		return std::make_pair<iterator, bool>(std::move(spot), true);
	}

	iterator erase(const_iterator pos){
		// If iterator does not reference an
		// element of the set, return end()
		if( !(pos >= begin() && pos < end()) )
			return end();

		// Get a mutable iterator referencing the
		// same element in the set
		iterator deletionist = begin();
		while( deletionist != pos )
			++deletionist;

		// Move elements down into the place where
		// the erased element used to be
		std::move_backward(deletionist + 1, end(), end() - 1);

		// Reflect changes in data members
		--end_;
		--size_;

		// Delete old data sitting in end()
		std::destroy_at(std::addressof(*end()));

		return deletionist;
	}


	iterator find(const key_type& k){
		if( begin() == end() )
			return end();

		// size() == 2 causes issues for find logic
		// so if size() == 2 then check manually
		if( size() == 2 ){
			if( *begin() == k )
				return begin();
			else if( *(end() - 1) == k )
				return end() - 1;
			else
				return end();
		}

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

		// Variable to keep track of last finder
		// value during loopage
		const_iterator last_go = finder;

		// Iterate until k is found in set
		while( *finder != k ){
			// If bounds of set reached without
			// finding result then return end()
			if( finder == begin() || finder == end() - 1 )
				return end();

			// If stuck in an inifinite loop
			// finding nothing then return end()
			if( (upper - lower == 1)
					&& ((*finder > k && *last_go < k)
						|| (*finder < k && *last_go > k)) )
				return end();

			// Update last go
			last_go = finder;

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
