#include "ra/parent_from_member.hpp"
#include <boost/iterator/iterator_facade.hpp>
#include <cstddef>
#include <utility>
#include <iostream>

namespace ra::intrusive{

// To figure out which specific object owns a list_hook
// instance, use parent_from_member function
class list_hook{
public:
	list_hook(): next_ {this}, prev_ {this} {}

	list_hook(const list_hook&): list_hook() {}

	list_hook& operator=(const list_hook&) { return *this; }

	~list_hook() {
		next_ = nullptr;
		prev_ = nullptr;
	}

private:
	list_hook* next_;
	list_hook* prev_;

	template<class T, list_hook T::* Hook> friend class list;
};

template<class T, list_hook T::* Hook>
class list {
public:
	class intrusive_list_iterator;
	class const_intrusive_list_iterator;

	// The type of the elements in the list
	using value_type = T;

	// The pointer-to-member associated with the list hook object
	static constexpr list_hook T::* hook_ptr = Hook;

	// The type of a mutatin reference to a node in the list
	using reference = T&;

	// The type of a non-mutating reference to a node in the list
	using const_reference = const T&;
	
	// An unsigned integral type used to represent sizes
	using size_type = std::size_t;

	// The mutating (bidirectional) iterator type for the list. This
	// type must provide all of the functionality of a bidirectional
	// iterator. If desried, the Boost Iterator library may be used
	// to implement this type.
	using iterator = intrusive_list_iterator;

	// The non-mutating (bidirectional) iterator type for the list. This
	// type must provide all of the functionality of a bidirectional
	// iterator. If desried, the Boost Iterator library may be used
	// to implement this type.
	using const_iterator = const_intrusive_list_iterator;

	class intrusive_list_iterator
		: public boost::iterator_facade<
		  intrusive_list_iterator,
		  value_type,
		  boost::bidirectional_traversal_tag>
	{
		public:
			intrusive_list_iterator()
				: base_(nullptr)
			{}

			explicit intrusive_list_iterator( value_type* element )
				: base_(element)
			{}

		private:
			friend class boost::iterator_core_access;
			friend class const_intrusive_list_iterator;

			void increment() {
				base_ = ra::util::parent_from_member<
					value_type, list_hook>(
							(base_->*hook_ptr).next_, hook_ptr);
			}

			void decrement() {
				base_ = ra::util::parent_from_member<
					value_type, list_hook>(
							(base_->*hook_ptr).prev_, hook_ptr);
			}

			bool equal( const intrusive_list_iterator& other ) const
			{
				return base_ == other.base_;
			}
			
			bool equal( const const_intrusive_list_iterator& other ) const
			{
				return base_ == other.base_;
			}

			bool equal( const value_type* p ) const
			{
				return base_ == p;
			}
	
			value_type& dereference() const
			{
				return *base_;
			}

			value_type* base_;
	};

	class const_intrusive_list_iterator
		: public boost::iterator_facade<
		  const_intrusive_list_iterator,
		  const value_type,
		  boost::bidirectional_traversal_tag>
	{
		public:
			const_intrusive_list_iterator()
				: base_(nullptr)
			{}

			explicit const_intrusive_list_iterator( value_type* element )
				: base_(element)
			{}

			const_intrusive_list_iterator( const intrusive_list_iterator& other )
				: base_(other.base_)
			{}

		private:
			friend class boost::iterator_core_access;
			friend class intrusive_list_iterator;

			void increment() {
				base_ = ra::util::parent_from_member<
					value_type, list_hook>(
							(base_->*hook_ptr).next_, hook_ptr);
			}

			void decrement() {
				base_ = ra::util::parent_from_member<
					value_type, list_hook>(
							(base_->*hook_ptr).prev_, hook_ptr);
			}

			bool equal( const const_intrusive_list_iterator& other ) const
			{
				return base_ == other.base_;
			}
			
			bool equal( const intrusive_list_iterator& other ) const
			{
				return base_ == other.base_;
			}

			bool equal( const value_type* p ) const
			{
				return base_ == p;
			}
	
			const value_type& dereference() const
			{
				return *base_;
			}

			const value_type* base_;
	};

	// Default constructor
	list(): size_ {0} {
		sentinel_.next_ = &(end_.*hook_ptr);
		sentinel_.prev_ = &(end_.*hook_ptr);
		(end_.*hook_ptr).next_ = nullptr;
		(end_.*hook_ptr).prev_ = sentinel_.prev_;
	}

	// Destructor
	~list(){
		sentinel_.next_ = nullptr;
		sentinel_.prev_ = nullptr;
		(end_.*hook_ptr).prev_ = nullptr;
	}

	// Move constructor
	list(list&& other){
		sentinel_.next_ = other.sentinel_.next_;
		sentinel_.prev_ = other.sentinel_.prev_;
		(end_.*hook_ptr).prev_ = (other.end_.*hook_ptr).prev_;
		size_ = other.size_;
		other.sentinel_.next_ = &(other.end_.*hook_ptr);
		other.sentinel_.prev_ = &(other.end_.*hook_ptr);
		(other.end_.*hook_ptr).prev_ = other.sentinel_.prev_;
	}

	// Move assignment operator
	list& operator=(list&& other){
		sentinel_.next_ = other.sentinel_.next_;
		sentinel_.prev_ = other.sentinel_.prev_;
		(end_.*hook_ptr).prev_ = (other.end_.*hook_ptr).prev_;
		size_ = other.size_;
		other.sentinel_.next_ = &(other.end_.*hook_ptr);
		other.sentinel_.prev_ = &(other.end_.*hook_ptr);
		(other.end_.*hook_ptr).prev_ = other.sentinel_.prev_;

		return *this;
	}	

	// Do not allow the copying of lists
	list(const list&) = delete;
	list& operator=(const list&) = delete;

	// Swap function
	void swap(list& x){
		auto tmpn = sentinel_.next_;
		auto tmpp = sentinel_.next_;
		auto tmps = size_;
		auto tmpe = (end_.*hook_ptr).prev_;
		sentinel_.next_ = x.sentinel_.next_;
		sentinel_.prev_ = x.sentinel_.prev_;
		size_ = x.size_;
		(end_.*hook_ptr).prev_ = (x.end_.*hook_ptr).prev_;
		x.sentinel_.next_ = tmpn;
		x.sentinel_.prev_ = tmpp;
		x.size_ = tmps;
		(x.end_.*hook_ptr).prev_ = tmpe;
	}

	// Returns list size
	size_type size() const { return size_; }

	// Insert function
	iterator insert(iterator pos, value_type& value){
		if( pos == end() ){
			// If insertion taking place at
			// end of list, can simply use
			// push_back. This clause also
			// accounts for the size equals
			// zero case.
			push_back( value );
		}else{
			// Hook new element between existing elements
			(value.*hook_ptr).next_ = &((*pos).*hook_ptr);
			(value.*hook_ptr).prev_ = ((*pos).*hook_ptr).prev_;

			// Hook bump-down element to new element
			((value.*hook_ptr).next_)->prev_ = &(value.*hook_ptr);

			if( pos == begin() ){
				// If inserting at beginning, point sentinel
				// to new element
				sentinel_.next_ = &(value.*hook_ptr);
			}else{
				// If not inserting at beginning, hook preceding
				// element to new element
				((value.*hook_ptr).prev_)->next_ = &(value.*hook_ptr);
			}
		}

		// Incremenet size
		++size_;

		// Return iterator referring to new element
		return iterator( ra::util::parent_from_member<value_type, list_hook>(
				&(value.*hook_ptr), hook_ptr) );
	}

	// Erase function
	iterator erase(iterator pos){
		if( pos == end() ){
			if( size() ){
				// Size is not 0
				sentinel_.prev_ = ((*pos).*hook_ptr).prev_;
			}else{
				// Size is 0
				sentinel_.prev_ = &(end_.*hook_ptr);
			}
		}

		if( pos == begin() )
			sentinel_.next_ = ((*pos).*hook_ptr).next_;

		// Front hook item in front of pos
		// to item behind pos
		(((*pos).*hook_ptr).prev_)->next_ = ((*pos).*hook_ptr).next_;

		// Rear hook item behind pos to item
		// in front of pos
		(((*pos).*hook_ptr).next_)->prev_ = ((*pos).*hook_ptr).prev_;

		// Decrement size
		--size_;
		
		return iterator(ra::util::parent_from_member<value_type, list_hook>(
				((*pos).*hook_ptr).next_, hook_ptr));
	}

	// Push back function
	void push_back(value_type& x){
		if( size() ){
			// Size is not 0
			(sentinel_.prev_)->next_ = &(x.*hook_ptr);
			(x.*hook_ptr).prev_ = sentinel_.prev_;
		}else{
			// Size is 0
			sentinel_.next_ = &(x.*hook_ptr);
			(x.*hook_ptr).prev_ = nullptr;
		}

		sentinel_.prev_ = &(x.*hook_ptr);
		(x.*hook_ptr).next_ = &(end_.*hook_ptr);
		(end_.*hook_ptr).prev_ = sentinel_.prev_;
		++size_;
	}

	// Pop back function
	void pop_back(){
		erase(end()--);
		--size_;
	}

	// Get reference for last element
	reference back(){
		iterator getref(ra::util::parent_from_member<value_type, list_hook>(
					sentinel_.prev_, hook_ptr));
		return *getref;
	}

	const_reference back() const{
		const_iterator getref(ra::util::parent_from_member<value_type, list_hook>(
					sentinel_.prev_, hook_ptr));

		return *getref;
	}

	// Clear list
	void clear(){
		sentinel_.next_ = &(end_.*hook_ptr);
		sentinel_.prev_ = &(end_.*hook_ptr);
		(end_.*hook_ptr).prev_ = sentinel_.prev_;
		size_ = 0;
	}

	// Get const begin iterator
	const_iterator begin() const { 
		return const_iterator(ra::util::parent_from_member<value_type, list_hook>(
				sentinel_.next_, hook_ptr));
	}

	// Get begin iterator
	iterator begin() {
		return iterator(ra::util::parent_from_member<value_type, list_hook>(
				sentinel_.next_, hook_ptr));
	}

	// Get const end iterator
	const_iterator end() const {
		return const_iterator(&end_);
		//return const_iterator(ra::util::parent_from_member<value_type, list_hook>(
		//		sentinel_.prev_, hook_ptr));
	}

	// Get end iterator
	iterator end() {
		return iterator(&end_);
		//return iterator(ra::util::parent_from_member<value_type, list_hook>(
		//		sentinel_.prev_, hook_ptr));
	}

	// Debugging function
/*	void print() const {
		const_iterator iter = begin();
		std::cout << *iter << '\n';
		++iter;
		while( iter != begin() ){
			std::cout << *iter << '\n';
			++iter;
		}
	}
*/
private:
	size_type size_;
	list_hook sentinel_;
	value_type end_;
};
}

