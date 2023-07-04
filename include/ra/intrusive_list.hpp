#include "ra/parent_from_member.hpp"
#include <boost/iterator/iterator_facade.hpp>
#include <cstddef>

namespace ra::intrusive{

// To figure out which specific object owns a list_hook
// instance, use parent_from_member function
class list_hook{
public:
	list_hook(): next_ {nullptr}, prev_ {nullptr} {}

	list_hook(const list_hook&): list_hook() {}

	list_hook& operator=(const list_hook&) { (void)0; }

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
		  list_hook,
		  boost::bidirectional_traversal_tag>
	{
		public:
			intrusive_list_iterator()
				: base_ {nullptr}
			{}

			explicit intrusive_list_iterator( list_hook* le_hook )
				: base_ {le_hook}
			{}

			intrusive_list_iterator( const const_intrusive_list_iterator& other )
				: base_ {other.base_}
			{}

		private:
			friend class boost::iterator_core_access;
			friend class const_intrusive_list_iterator;

			void increment() { base_ = base_->next_; }

			void decrement() { base_ = base_->prev_; }

			bool equal( const intrusive_list_iterator& other ) const
			{
				return (this->base_->next_ == other.base_->next_) &&
					(this->base_->prev_ == other.base_->prev_);
			}
			
			bool equal( const const_intrusive_list_iterator& other ) const
			{
				return (this->base_->next_ == other.base_->next_) &&
					(this->base_->prev_ == other.base_->prev_);
			}
	
			T& dereference() const
			{
				return *ra::util::parent_from_member<list_hook, hook_ptr>(
						base_, hook_ptr);
			}

			list_hook* base_;
	};

	class const_intrusive_list_iterator
		: public boost::iterator_facade<
		  const_intrusive_list_iterator,
		  const list_hook,
		  boost::bidirectional_traversal_tag>
	{
		public:
			const_intrusive_list_iterator()
				: base_ {nullptr}
			{}

			explicit const_intrusive_list_iterator( const list_hook* le_hook )
				: base_ {le_hook}
			{}

			const_intrusive_list_iterator( const intrusive_list_iterator& other )
				: base_ {other.base_}
			{}

		private:
			friend class boost::iterator_core_access;
			friend class intrusive_list_iterator;

			void increment() { base_ = base_->next_; }

			void decrement() { base_ = base_->prev_; }

			bool equal( const const_intrusive_list_iterator& other ) const
			{
				return (this->base_->next_ == other.base_->next_) &&
					(this->base_->prev_ == other.base_->prev_);
			}
			
			bool equal( const intrusive_list_iterator& other ) const
			{
				return (this->base_->next_ == other.base_->next_) &&
					(this->base_->prev_ == other.base_->prev_);
			}
	
			const T& dereference() const
			{
				return *ra::util::parent_from_member<const list_hook, hook_ptr>(
						base_, hook_ptr);
			}

			const list_hook* base_;
	};

	// Default constructor
	list(): size_ {0} {}

	// Destructor
	~list(){
		sentinel_.next_ = nullptr;
		sentinel_.prev_ = nullptr;
	}

	// Move constructor
	list(list&& other);

	// Move assignment operator
	list& operator=(list&& other);

	// Do not allow the copying of lists
	list(const list&) = delete;
	list& operator=(const list&) = delete;

	// Swap function
	void swap(list& x);

	// Returns list size
	size_type size() const;

	// Insert function
	iterator insert(iterator pos, value_type& value);

	// Erase function
	iterator erase(iterator pos);

	// Push back function
	void push_back(value_type& x);

	// Pop back function
	void pop_back();

	// Get reference for last element
	reference back();
	const_reference back() const;

	// Clear list
	void clear();

	// Get begin iterator
	const_iterator begin() const;
	iterator begin();

	// Get end iterator
	const_iterator end() const;
	iterator end();

private:
	size_type size_;
	list_hook sentinel_;
};
}

