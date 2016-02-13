/**
*  Array class that represents container to store elements
*  and provides a number of useful and handy functions
*  that operate on them.
*
*  @author	Jonatan Klosko
*/

#ifndef FUNCTIONAL_ARRAY_H
#define FUNCTIONAL_ARRAY_H

#include <vector>		// vector
#include <memory>		// allocator
#include <type_traits>	// decay, enable_if
#include <utility>		// declval
#include <numeric>		// accumulate
#include <algorithm>	// sort, reverse, next_permutation, lexicographical_compare
#include <stdexcept>	// invalid_argument, logic_error
#include <iterator>		// back_inserter
#include <sstream>		// stringstream
#include <string>		// stream
#include <random>		// random_device, mt19937, uniform_int_distribution
#include <functional>	// function, comparators
#include <map>			// map

template <class T>
class Array : private std::vector<T> {
public:
	using vector = std::vector<T>;

	using size_type = typename vector::size_type;
	using value_type = T;
	using vector::iterator;
	using vector::const_iterator;
	using vector::reverse_iterator;
	using vector::const_reverse_iterator;

	using vector::begin;
	using vector::rbegin;
	using vector::end;
	using vector::rend;
	using vector::size;

	using vector::operator[];
	using vector::at;

	using vector::push_back;	// must be public in order to use std::back_inserter

	/**
	*  Creates an empty Array.
	*/
	Array()
		: vector() {
	}

	/**
	*  Creates an Array of given size, which contains elements equal to the given one
	*  or construced with default constructor.
	*/
	Array(long long size, const T& value = T())
		: vector(size >= 0 ? size : throw std::invalid_argument("negative array size"), value) {
	}

	/**
	*  Creates an Array by copying.
	*/
	Array(const Array& other)
		: vector(other) {
	}

	/**
	*  Creates an Array by moving other Array.
	*/
	Array(Array&& other)
		: vector(std::move(other)) {
	}

	/**
	*  Creates an Array by copying all elements from given range.
	*
	*  Iterates from @a begin to @a end exclusive, and copies the elements.
	*/
	template <class InputIterator>
	Array(InputIterator begin, InputIterator end)
		: vector(begin, end) {
	}

	/**
	*  Creates an Array out of std::vector object, by copying.
	*/
	Array(const std::vector<T>& other)
		: vector(other) {
	}

	/**
	*  Creates an Array out of std::vector object, by moving.
	*/
	Array(std::vector<T>&& other)
		: vector(std::move(other)) {

	}

	/**
	*  Creates an Array out of std::initializer_list
	*
	*  This is the best way to create a new Array containing specific objects.
	*  It allows to use handy syntax: Array<Type>{objects...};
	*/
	Array(std::initializer_list<T> initializerList)
		: vector(initializerList) {
	}

	/**
	*  Assigns an Array to this one.
	*/
	Array<T>& operator= (Array other) {
		this->swap(other);

		return *this;
	}

	/**
	*  Assigns std::initializer_list elements to this Array.
	*/
	Array<T>& operator= (std::initializer_list<T> initializerList) {
		vector::operator= (initializerList);

		return *this;
	}

	/**
	*  Checks whether at least one of elements in the Array satisfy given condition.
	*
	*  @param function		the function that takes one argument of the type of the elements
	*						and returns bool value
	*  @return				true if @a function returns true for at least one element
	*						and false otherwise
	*/
	template <typename Function>
	bool any(const Function& function) const {
		for (const T& item : *this)
			if (function(item))
				return true;

		return false;
	}

	/**
	*  Checks whether all elements in the Array satisfy given condition.
	*
	*  @param function		the function that takes one argument of the type of the elements
	*						and returns bool value
	*  @return				true if @a function returns true for each element
	*						and false otherwise
	*/
	template <typename Function>
	bool all(const Function& function) const {
		for (const T& item : *this)
			if (function(item) == false)
				return false;

		return true;
	}

	/**
	*  Checks whether all elements in the Array are different.
	*
	*  @return				true if every element is unique
	*						and false otherwise
	*/
	bool allDifferent() const {
		if (!empty()) {
			std::map<T, int> itemCount;
			for (const T& item : *this)
				if (++itemCount[item] > 1)
					return false;
		}

		return true;
	}

	/**
	*  Checks whether all elements in the Array are the same.
	*
	*  @return				true if all items are the same
	*						and false otherwise
	*/
	bool allSame() const {
		if (!empty()) {
			const T& first = front();
			for (auto it = begin() + 1; it != end(); ++it)
				if (*it != first)
					return false;
		}

		return true;
	}

	/**
	*  Clears the Array, that becomes an empty one.
	*
	*  @return				reference to self
	*/
	Array<T>& clear() {
		Array<T> cleaner;
		this->swap(cleaner);

		return *this;
	}

	/**
	*  Returns an Array with all possible combinations of given length
	*  of the elements from the Array.
	*
	*  Each combination is represented as an Array.
	*  The combinations are not repeated.
	*  They are not sorted unless the Array is sorted.
	*
	*  @param subsetSize	length of each combination
	*  @return				an Array of Arrays, each with single combination
	*/
	Array<Array<T>> combinations(long long subsetSize) const {
		if (subsetSize < 0)
			throw std::invalid_argument("negative array size");
		else if (subsetSize == 0)
			return Array<Array<T>>().push(Array<T>());
		else if (subsetSize == size())
			return Array<Array<T>>().push(Array<T>(*this));
		else if (subsetSize > size())
			return Array<Array<T>>();

		Array<T> thisCopy = Array<T>(*this);
		Array<Array<T>> combinations;

		Array<T> combination;
		std::function<void(size_type)> foo = [&](size_type currentItemIndex) {
			for (size_type i = currentItemIndex; i < thisCopy.size(); ++i) {
				combination.push_back(thisCopy[i]);

				if (combination.size() < subsetSize)
					foo(i + 1);
				else if (combination.size() == subsetSize)
					combinations.push_back(combination);

				combination.pop_back();
			}
		};
		foo(0);

		return combinations;
	}

	/**
	*  Compares the Array to a given one lexicographically.
	*
	*  Comparison is lexicographically, like words in dictionary.
	*
	*  @param other		an Array to be compared
	*  @return			-1 if the Array is lexicographically less than the @a other
	*					if all elements of the Array are equal to corresponding ones in the @a other Array
	*					the comparison result is based on Arrays length (-1 if the Array length is less
	*					than @a other length, 0 if they are the same and 1 otherwise)
	*					 1 if the Array is lexicographically greater than the @a other
	*/
	short int compare(const Array<T>& other) const {
		auto thisIt = this->begin();
		auto otherIt = other.begin();

		while (thisIt != this->end() && otherIt != other.end()) {
			if (*thisIt < *otherIt)
				return -1;
			else if (*thisIt > *otherIt)
				return 1;

			++thisIt; ++otherIt;
		}

		if (this->size() < other.size())
			return -1;
		else if (this->size() > other.size())
			return 1;
		else
			return 0;
	}

	/**
	*  Appends given Arrays to self.
	*
	*  @param arrays		any number of Arrays to be appended to the Array
	*  @return				reference to self
	*/
	template <class... Arrays>
	Array<T>& concat(Arrays... arrays) {
		using expander = int[];
		expander{ 0, (std::move(arrays.begin(), arrays.end(), std::back_inserter(*this)), 0)... };

		return *this;
	}

	/**
	*  Returns the number of elements in the Array.
	*
	*  @return				size of the Array
	*/
	size_type count() const {
		return size();
	}

	/**
	*  Returns the number of elements in the Array equal to given one.
	*
	*  @param object		object, to check how many times does it occur in the Array
	*	@return				number of elements equal to @a object
	*/
	size_type count(const T& object) const {
		size_type count = 0;

		for (const T& item : *this)
			if (item == object)
				++count;

		return count;
	}

	/**
	*  Returns the number of elements in the Array that satisfy given condition.
	*
	*  @param function		the function that takes one argument of the type of the elements
	*						and returns bool value
	*  @return				the number of elements for which @a function returns true
	*/
	template <typename Function>
	size_type count(const Function& function) const {
		size_type count = 0;

		for (const T& item : *this)
			if (function(item))
				++count;

		return count;
	}

	/**
	*  Returns copy of the Array.
	*
	*  @return				copy of self
	*/
	Array<T> copy() const {
		return *this;
	}

	/**
	*  Removes the element at the specified index.
	*
	*  @param index			index of the element to be deleted
	*  @return				reference to self
	*/
	Array<T>& deleteAt(long long index) {
		if (index < 0)
			throw std::invalid_argument("negative array index");

		if (index < size())
			erase(begin() + index);

		return *this;
	}

	/**
	*  Removes all Array elements being in the given range (exclusive).
	*
	*  @param fromIndex		index of the first element to be removed
	*  @param toIndex		index of first element not to be removed
	*  @return				reference to self
	*/
	Array<T>& deleteBetween(long long fromIndex, long long toIndex) {
		if (fromIndex < 0 || toIndex < 0)
			throw std::invalid_argument("negative array index");

		if (toIndex > size())
			toIndex = size();

		if (fromIndex < size() && fromIndex < toIndex)
			erase(begin() + fromIndex, begin() + toIndex);

		return *this;
	}

	/**
	*  Removes the elements that satisfy given condition.
	*
	*  @param function		the function that takes one argument of the type of the elements
	*						and returns bool value
	*  @return				reference to self
	*/
	template <typename Function>
	Array<T>& deleteIf(const Function& function) {
		const auto& newEnd = std::remove_if(begin(), end(), function);
		erase(newEnd, end());

		return *this;
	}

	/**
	*  Removes each element from the Array, that is equal to given one.
	*
	*  @param object		object, to delete all elements equal to it
	*  @return				reference to self
	*/
	Array<T>& deleteObject(const T& object) {
		const auto& newEnd = std::remove(begin(), end(), object);
		erase(newEnd, end());

		return *this;
	}

	/**
	*  Removes each element from the Array, that is equal to one of elements in given Array.
	*
	*  @param objects		Array of objects, to delete all elements in the Array equal to each of them
	*  @return				reference to self
	*/
	Array<T>& deleteObjects(const Array<T>& objects) {
		auto newEnd = end();
		for (const T& object : objects)
			newEnd = std::remove(begin(), newEnd, object);

		erase(newEnd, end());

		return *this;
	}

	/**
	*  Calls given function once for each element, passing it as an argument by reference.
	*
	*  @param function		the function that takes one argument of the type of the elements
	*  @return				reference to self
	*/
	template <typename Function>
	Array<T>& each(const Function& function) {
		for (T& item : *this)
			function(item);

		return *this;
	}

	template <typename Function>
	const Array<T>& each(const Function& function) const {
		for (const T& item : *this)
			function(item);

		return *this;
	}

	/**
	*  Calls given function once for each element, passing it as an argument by reference
	*  and assigns the function result.
	*
	*  @param function		the function that takes one argument of the type of the elements
	*						and returns object of that type
	*  @return				reference to self
	*/
	template <typename Function>
	Array<T>& eachAssign(const Function& function) {
		for (T& item : *this)
			item = function(item);

		return *this;
	}

	/**
	*  Checks whether the end of the Array is equal to a given Array
	*
	*  @param sequence		an Array containing elements of the same type as the Array
	*  @return				true if the Array ends with the given sequence of elements
	*						and false otherwise
	*/
	bool endsWith(const Array<T>& sequence) const {
		if (size() < sequence.size())
			return false;

		auto sequenceIt = sequence.rbegin();
		for (auto thisIt = rbegin(); sequenceIt != sequence.rend(); ++thisIt, ++sequenceIt)
			if (*thisIt != *sequenceIt)
				return false;

		return true;
	}

	/**
	*  Removes the elements that do not satisfy given condition.
	*
	*  @param function		the function that takes one argument of the type of the elements
	*						and returns bool value
	*  @return				reference to self
	*/
	template <typename Function>
	Array<T>& filter(const Function& function) {
		const auto& newEnd = std::remove_if(begin(), end(), [&](const T& item) {
			return !function(item);
		});
		erase(newEnd, end());

		return *this;
	}

	/**
	*  Returns the first element of the Array.
	*
	*  @return				the first element
	*/
	T first() const {
		if (empty())
			throw std::logic_error("calling first when array is empty");

		return front();
	}

	/**
	*  Returns an Array containing specified number of the first elements
	*
	*  @param count			the number of the first elements to be returned
	*
	*  @return				reference to self
	*/
	Array<T> first(long long count) const {
		if (count < 0)
			throw std::invalid_argument("negative array size");
		else if (count > size())
			count = size();

		Array<T> result;
		result.assign(begin(), begin() + count);

		return result;
	}

	/**
	*  Assigns specified object to each element of the Array in given range (exclusive).
	*
	*  @param object
	*  @param fromIndex		index of the first element to be filled (0 by default)
	*  @param toIndex		index of the first element not to be filled (end of the Array by default)
	*  @return				reference to self
	*/
	Array<T>& fill(const T& object, long long fromIndex, long long toIndex) {
		if (fromIndex < 0 || toIndex < 0)
			throw std::invalid_argument("negative array index");

		size_type size = this->size();
		if (toIndex > size)
			toIndex = size;

		Array<T> result;
		if (fromIndex < size && fromIndex < toIndex)
			std::fill(begin() + fromIndex, begin() + toIndex, object);

		return *this;
	}

	Array<T>& fill(const T& object, long long fromIndex = 0) {
		return fill(object, fromIndex, size());
	}

private:
	template <class C>
	struct is_array : std::false_type {};

	template <class C>
	struct is_array<Array<C>> : std::true_type {};

	// real_type<C>::type	if C is Array then it is type of this Array (the deepest type that is not Array itself)
	//	example: real_type<Array<Array<int>>>::type is int
	template <class C>
	struct real_type {
		using type = C;
	};

	template <class C>
	struct real_type<Array<C>> {
		using type = typename real_type<C>::type;
	};

	template <class ResultArray, class C>
	void flattenAllDimensionsImpl(ResultArray& resultArray, const Array<C>& array) const {
		for (const C& item : array)
			resultArray.push_back(item);
	}

	template <class ResultArray, class C>
	void flattenAllDimensionsImpl(ResultArray& resultArray, const Array<Array<C>>& array) const {
		for (const Array<C>& item : array)
			flattenAllDimensionsImpl(resultArray, item);
	}

public:
	/**
	*  Returns the Array flattened to one-dimensional Array
	*  (regardless of the number of dimensions).
	*
	*  @return				one-dimensional Array
	*/
	template <class RealType = typename real_type<T>::type>
	Array<RealType> flattenAllDimensions() const {
		Array<RealType> result;

		flattenAllDimensionsImpl(result, *this);

		return result;
	}

	/**
	*  Reutrns the array flettened by one dimension.
	*
	*  @return				 copy of the Array with dimension reduced by one
	*/
	template <bool EnableBool = true, class = typename std::enable_if<is_array<T>::value && EnableBool>::type>
	T flatten() const {
		T result;

		for (const T& item : *this)
			std::copy(item.begin(), item.end(), std::back_inserter<T>(result));

		return result;
	}

	/**
	*  Checks whether the Array contains given element.
	*
	*  @param object		element to check
	*  @return				true if @a object is in the Array and false otherwise
	*/
	bool includes(const T& object) const {
		for (const T& item : *this)
			if (item == object)
				return true;

		return false;
	}

	/**
	*  Finds position of the first element equal to given one.
	*
	*  @param object		element whose index is searched
	*  @param fromIndex		beginning of search range (0 by default)
	*  @return				index of first occurrence of the @a object in the Array
	*						or -1 if element is not found
	*/
	long long indexOf(const T& object, long long fromIndex = 0) const {
		if (fromIndex < 0)
			throw std::invalid_argument("negative array index");

		size_type index = fromIndex;
		if (fromIndex < size()) {
			for (auto it = begin() + fromIndex; it != end(); ++it, ++index)
				if (*it == object)
					return index;
		}

		return -1;
	}

	/**
	*  Finds position of the first element that satisfies given condition.
	*
	*  @param function		the function that takes one argument of the type of the elements
	*						and returns bool value
	*  @param fromIndex		beginning of search range (0 by default)
	*  @return				index of first element for which @a function returns true
	*						or -1 if element is not found
	*/
	template <class Function>
	long long indexWhere(const Function& function, long long fromIndex = 0) const {
		if (fromIndex < 0)
			throw std::invalid_argument("negative array index");

		size_type index = fromIndex;
		if (fromIndex < size()) {
			for (auto it = begin() + fromIndex; it != end(); ++it, ++index)
				if (function(*it))
					return index;
		}

		return -1;
	}

	/**
	*  Inserts the given objects before	the element at the given index.
	*
	*  @param index			position of element before which the given elements are inserted
	*  @param objects...	elements to be inserted to the Array
	*  @return				reference to self
	*/
	template <class ...Objects>
	Array<T>& insert(long long index, Objects&&... objects) {
		if (index < 0)
			throw std::invalid_argument("negative array index");
		if (sizeof...(objects) > 0) {
			if (index >= size())
				resize(index);

			std::vector<T>::insert(begin() + index, { objects... });
		}

		return *this;
	}

	/**
	*  Checks whether the Array have size of zero.
	*
	*  @return				true if the Array is empty end false otherwise
	*/
	bool isEmpty() const {
		return empty();
	}

	/**
	*  Returns string created by converting each element to string
	*  and separates them with the given separator.
	*
	*  @param separator		string which isolates elements in string (empty by default)
	*  @pre					the type of the elements must have overloaded
	*						operator<< that takes std::ostream and this type
	*  @return				std::string made as described above
	*/
	std::string join(const std::string& separator = "") const {
		std::ostringstream result;

		if (!empty()) {
			std::copy(begin(), --end(), std::ostream_iterator<T>(result, separator.c_str()));
			result << back();
		}

		return result.str();
	}
	/**
	*  Returns the last element of the Array.
	*
	*  @return				the last element
	*/
	T last() const {
		if (empty())
			throw std::logic_error("calling last when array is empty");

		return back();
	}

	/**
	*  Returns an Array containing specified number of the last elements
	*
	*  @param count			the number of the last elements to be returned
	*
	*  @return				reference to self
	*/
	Array<T> last(long long count) const {
		if (count < 0)
			throw std::invalid_argument("negative array size");
		else if (count > size())
			count = size();

		Array<T> result;
		result.assign(end() - count, end());

		return result;
	}

	/**
	*  Finds position of the last element equal to given one.
	*
	*  @param object		element whose index is searched
	*  @param fromIndex		beginning of search range (end of Array by default),
	*						searching starts from it and goes to the left
	*  @return				index of last occurrence of the @a object in the Array
	*						or -1 if element is not found
	*/
	long long lastIndexOf(const T& object, long long fromIndex) const {
		if (fromIndex < 0)
			throw std::invalid_argument("negative array index");

		size_type index = fromIndex;
		if (fromIndex < size()) {
			for (auto it = rend() - 1 - fromIndex; it != rend(); ++it, --index)
				if (*it == object)
					return index;
		}

		return -1;
	}

	long long lastIndexOf(const T& object) const {
		if (empty())
			return -1;

		return lastIndexOf(size() - 1);
	}

	/**
	*  Finds position of the last element that satisfies given condition.
	*
	*  @param function		the function that takes one argument of the type of the elements
	*						and returns bool value
	*  @param fromIndex		beginning of search range (end of Array by default),
	*						searching starts from it and goes to the left
	*  @return				index of last element for which @a function returns true
	*						or -1 if element is not found
	*/
	template <class Function>
	long long lastIndexWhere(const Function& function, long long fromIndex) const {
		if (fromIndex < 0)
			throw std::invalid_argument("negative array index");

		size_type index = fromIndex;
		if (fromIndex < size()) {
			for (auto it = rend() - 1 - fromIndex; it != rend(); ++it, --index)
				if (function(*it))
					return index;
		}

		return -1;
	}

	template <class Function>
	long long lastIndexWhere(const Function& function) const {
		if (empty())
			return -1;

		return lastIndexWhere(function, size() - 1);
	}

	/**
	*  Returns the number of elements in the Array.
	*
	*  @return				size of the Array
	*/
	size_type length() {
		return size();
	}

	/**
	*  Returns an Array that contains results of
	*  the given function calls for each element.
	*
	*  Pointer to const member function or member variable can be used
	*  instead of function. Then the returned Array contains respectively
	*  the results of the member function calls or the values of member variables.
	*
	*  @param functionOrPointerToMember		function that takes one argument
	*										of type of the elements and does not
	*										return void
	*										or pointer to elements const member function
	*										which does not return void
	*										or pointer to elements member variable
	*  @return								an Array containing corresponding values as described above
	*/
	template <typename FunctionOrPointerToMember>
	auto map(const FunctionOrPointerToMember& functionOrPointerToMember) const {
		using resultType = typename
			std::decay<decltype(extractValue(std::declval<T>(), functionOrPointerToMember))>::type;

		Array<resultType> result;

		for (const T& item : *this)
			result.push_back(extractValue(item, functionOrPointerToMember));

		return result;
	}

	/**
	*  Returns the largest element of the Array.
	*
	*  @return				the maximal element
	*/
	T max() const {
		return *std::max_element(begin(), end());
	}

	/**
	*  Returns the smallest of the elements of the Array.
	*
	*  @return				the minimal element
	*/
	T min() const {
		return *std::min_element(begin(), end());
	}

	/**
	*  Checks whether the Array have size greater then zero.
	*
	*  @return				true if the Array is not empty end false otherwise
	*/
	bool nonEmpty() const {
		return !empty();
	}

	/**
	*  Rearranges the elements of the Array, in such a way that all the elements
	*  which satisfy the given condition precede all those which do not.
	*
	*  The elements order is preserved.
	*
	*  @param function		the function that takes one argument of the type of the elements
	*						and returns bool value
	*  @return				reference to self
	**/
	template <typename Function>
	Array<T>& partition(const Function& function) {
		std::stable_partition(begin(), end(), function);

		return *this;
	}

	/**
	*  Returns an Array containing all sorted permutations
	*  of the Array elements.
	*
	*  Each permutation is represented as an Array.
	*  The permutations are not repeated.
	*  They are sorted.
	*
	*  return				an Array of Array each with single permutation
	*/
	Array<Array<T>> permutations() const {
		Array<Array<T>> permutations;

		Array<T> permutation = Array<T>(*this).sort();
		do {
			permutations.push_back(permutation);
		} while (next_permutation(permutation.begin(), permutation.end()));

		return permutations;
	}

	/**
	*  Removes the last element of the Array and returns it.
	*
	*  @return				the last element
	*/
	T pop() {
		if (empty())
			throw std::logic_error("calling pop when array is empty");

		T popped = back();
		pop_back();

		return popped;
	}

	/**
	*  Removes the specified number of last elements
	*  and returns them as an Array.
	*
	*  @param count			number of elements to remove
	*  @return				an Array with removed elements
	*/
	Array<T> pop(long long count) {
		if (count < 0)
			throw std::invalid_argument("negative array size");
		else if (count > size())
			count = size();

		Array<T> popped;

		std::move(end() - count, end(), std::back_inserter(popped));
		resize(size() - count);

		return popped;
	}

	/**
	*  Append the given objects to the end of the Array.
	*
	*  @param objects...	elements to be appended to the Array
	*  @return				reference to self
	*/
	template <class ...Objects>
	Array<T>& push(Objects&&... objects) {
		/* {} brackets in order to call push_back for each object by expanding,
		then 0 as the elment of initializer list (unnamed, unused, temporary array of zeros
		should be optimized by compiler)*/
		using expander = int[];
		expander{ 0, (push_back(std::forward<Objects>(objects)), 0)... };

		return *this;
	}

	/**
	*  Choose a random element form the Array.
	*
	*  @return				the randomly selected element
	*/
	T random() const {
		if (empty())
			throw std::logic_error("picking random element from an empty Array");

		std::uniform_int_distribution<size_type> randomIndexGenerator(0, size() - 1);
		return operator[](randomIndexGenerator(randomNumberGenerator));
	}

	/**
	*  Choose a specified number of random unique elements form the Array.
	*
	*  The randomly selected elements are unique unless the Array contains
	*  duplicate elements.
	*
	*  @param count			the number of elements to pick, even if it is greater
	*						than the Array size, function returns an Array
	*						with all elements on random position
	*  @return				an Array containing randomly selected elements
	*/
	Array<T> random(long long count) const {
		if (empty())
			throw std::logic_error("picking random element from an empty Array");
		if (count < 0)
			throw std::invalid_argument("negative array size");
		else if (count > size())
			count = size();

		Array<T> randomIndexes;
		std::uniform_int_distribution<size_type> randomIndexGenerator(0, size() - 1);
		while (count > 0) {
			T randomIndex = randomIndexGenerator(randomNumberGenerator);
			if (!randomIndexes.includes(randomIndex)) {
				randomIndexes.push_back(randomIndex);
				--count;
			}
		}

		return randomIndexes.map([&](const size_type& index) { return (*this)[index]; });
	}

	template <typename Function>
	T reduce(const Function& function) const {
		if (size() == 0)
			return T();

		return std::accumulate(++begin(), end(), *begin(), function);
	}

	/**
	*  Uses the specified binary function and calls it for accumulator and each object,
	*  accumulating its result and passing it as a first argument to the function again.
	*
	*  Accumulating goes left to right.
	*  Initial value of accumulation may be specified (first element by default).
	*
	*  @param function		binary function taking arguments of type of the elements
	*						which returning the same type
	*  @param initialValue	initial element of the whole product (first element by default)
	*  @return				product of accumulation
	*/
	template <typename Function>
	T reduce(const Function& function, T initialValue) const {
		return std::accumulate(begin(), end(), initialValue, function);
	}

	template <typename Function>
	T reduceRight(const Function& function) const {
		if (size() == 0)
			return T();

		return std::accumulate(++rbegin(), rend(), *rbegin(), function);
	}

	/**
	*  Uses the specified binary function and calls it for accumulator and each object,
	*  accumulating its result and passing it as a first argument to the function again.
	*
	*  Accumulating goes right to left.
	*  Initial value of accumulation may be specified (last element by default).
	*
	*  @param function		binary function taking arguments of type of the elements
	*						which returning the same type
	*  @param initialValue	initial element of the whole product (last element by default)
	*  @return				product of accumulation
	*/
	template <typename Function>
	T reduceRight(const Function& function, T initialValue) const {
		return std::accumulate(rbegin(), rend(), initialValue, function);
	}

	/**
	*  Repleaces all elements of the Array which are equal to the given one
	*  with the specified object.
	*
	*  @param oldObject		object to be replaced
	*  @param newObject		replacement
	*  @return				reference to self
	*/
	Array<T>& replace(const T& oldObject, const T& newObject) {
		std::replace(begin(), end(), oldObject, newObject);

		return *this;
	}

	/**
	*  Repleaces all elements of the Array that satisfy given condition
	*  with the specified object.
	*
	*  @param function		the function that takes one argument of the type of the elements
	*						and returns bool value
	*  @param newObject		replacement
	*  @return				reference to self
	*/
	template <typename Function>
	Array<T>& replaceIf(const Function& function, const T& newObject) {
		std::replace_if(begin(), end(), function, newObject);

		return *this;
	}

	/**
	*  Reverses the order of elements.
	*
	*  @return				reference to self
	*/
	Array<T>& reverse() {
		std::reverse(begin(), end());

		return *this;
	}

	/**
	*  Changes position of each element by the given number.
	*
	*  The elements at extreme positions are moved to the other end of the Array.
	*  If the given number is positive, elements are moved
	*  from rigth to left.
	*  If the given number is negative, elements are moved
	*  from left to right.
	*
	*  @count				factor of moving the elements
	*  @return				reference to self
	**/
	Array<T>& rotate(long long count) {
		size_type thisSize = size();
		if (thisSize) {
			if (count > 0)
				std::rotate(begin(), begin() + (count % thisSize), end());
			else if (count < 0)
				std::rotate(rbegin(), rbegin() + (-count % thisSize), rend());
		}

		return *this;
	}

	/**
	*  Separates the Array elements to two other Arrays, first one
	*  includes all elements which pass the specified condition
	*  and second of the rest of them.
	*
	*  @param function		the function that takes one argument of the type of the elements
	*						and returns bool value
	*  @return				an Array that includes both the Array with elements
	*						for which function returns true and the Array
	*						with the rest of the elements
	**/
	template <typename Function>
	Array<Array<T>> separate(const Function& function) const {
		Array<T> correct, incorrect;
		for (const T& item : *this) {
			if (function(item))
				correct.push_back(item);
			else
				incorrect.push_back(item);
		}

		return Array<Array<T>>{std::move(correct), std::move(incorrect)};
	}

	/**
	*  Removes the first element of the Array and returns it.
	*
	*  It requires reallocation of all elements
	*  so do not use it for large Arrays.
	*
	*  @return				the first element
	*/
	T shift() {
		if (empty())
			throw std::logic_error("calling shift when array is empty");

		T shifted = front();
		erase(begin());

		return shifted;
	}

	/**
	*  Removes the specified number of first elements
	*  and returns them as an Array.
	*
	*  It requires reallocation of all elements
	*  so do not use it for large Arrays.
	*
	*  @param count			number of elements to remove
	*  @return				an Array with removed elements
	*/
	Array<T> shift(long long count) {
		if (count < 0)
			throw std::invalid_argument("negative array size");
		else if (count > size())
			count = size();

		Array<T> shifted;

		std::move(begin(), begin() + count, std::back_inserter(shifted));
		erase(begin(), begin() + count);

		return shifted;
	}

	/**
	*  Shuffles the Array (sets positions of the elements randomly).
	*
	*
	*/
	Array<T>& shuffle() {
		std::shuffle(begin(), end(), randomNumberGenerator);

		return *this;
	}

	/**
	*  Returns a subarray containing the elements of the Array
	*  in given (exclusive) range.
	*
	*  @param fromIndex		beginning of the subarray
	*  @param toIndex		end of the subarray (last element of the Array be default),
	*						it is not a part of the subarray
	*  @return				an Array with selected elements
	*/
	Array<T> slice(long long fromIndex, long long toIndex) const {
		if (fromIndex < 0 || toIndex < 0)
			throw std::invalid_argument("negative array index");

		size_type size = this->size();
		if (toIndex > size)
			toIndex = size;

		Array<T> result;
		if (fromIndex < size && fromIndex < toIndex)
			result.assign(begin() + fromIndex, begin() + toIndex);

		return result;
	}

	Array<T> slice(long long fromIndex) const {
		return slice(fromIndex, size());
	}

	/**
	*  Sorts the elements in the Array using operator <.
	*
	*  @pre					operator < must be overloaded for the type of the elements
	*  @return				reference to self
	*/
	Array<T>& sort() {
		std::sort(begin(), end());
		return *this;
	}

	/**
	*  Sorts the elements in the Array using the specified comparator.
	*
	*  @function			binary function that takes two arguments of the type
	*						of the elements and returns true if the first argument
	*						is considered to go before the second one
	*  @return				reference to self
	*/
	template <typename Function>
	Array<T>& sort(const Function& function) {
		std::sort(begin(), end(), function);
		return *this;
	}

	/**
	*  Sorts the elements in the Array comparing results of
	*  the given function calls for each element with the operator <.
	*
	*  Pointer to const member function or member variable can be used
	*  instead of function. Then the comparison is based respectively
	*  on the results of the member function calls or the values of member variables.
	*
	*  @param functionOrPointerToMember		function that takes one argument
	*										of type of the elements and returns
	*										a comparable object
	*										or pointer to elements const member function
	*										which returns a comparable object
	*										or pointer to elements member comparable variable
	*  @return								reference to self
	*/
	template <typename FunctionOrPointerToMember>
	Array<T>& sortBy(const FunctionOrPointerToMember& functionOrPointerToMember) {
		std::sort(begin(), end(), [&](const T& a, const T& b) {
			return extractValue(a, functionOrPointerToMember) < extractValue(b, functionOrPointerToMember);
		});
		return *this;
	}

	/**
	*  Checks whether the beginning of the Array is equal to a given Array
	*
	*  @param sequence		an Array containing elements of the same type as the Array
	*  @return				true if the Array begins with the given sequence of elements
	*						and false otherwise
	*/
	bool startsWith(const Array<T>& sequence) const {
		if (size() < sequence.size())
			return false;

		auto sequenceIt = sequence.begin();
		for (auto thisIt = begin(); sequenceIt != sequence.end(); ++thisIt, ++sequenceIt)
			if (*thisIt != *sequenceIt)
				return false;

		return true;
	}

	/**
	*  Swaps content of the Array and the given one
	*  (modifies them both)
	*
	*  @param other			an other Array of the same type
	*						as the Array
	*  @return				reference to self
	*/
	Array<T>& swap(Array<T>& other) {
		vector::swap(other);

		return *this;
	}

	/**
	*  Removes all duplicates of each element of the Array,
	*  making them unique.
	*
	*  The elements order is preserved.
	*  First of the duplicates is not removed.
	*
	*  @return				reference to self
	*/
	Array<T>& unique() {
		auto first = begin();
		auto result = begin();
		auto arrayEnd = end();

		while (first != arrayEnd) {
			if (std::find(begin(), result, *first) == result)
				*result++ = *first;
			++first;
		}

		erase(result, arrayEnd);

		return *this;
	}

	/**
	*  Append the given objects before the beginning of the Array.
	*
	*  It requires reallocation of all elements
	*  so do not use it for large Arrays.
	*
	*  @param objects...	elements to be appended to the Array
	*  @return				reference to self
	*/
	template <class ...Objects>
	Array<T>& unshift(Objects&&... objects) {
		std::vector<T>::insert(begin(), { objects... });

		return *this;
	}

	/**
	*  Append the given object to the end of the Array
	*  (pushing one element).
	*
	*  @param object		element to be appended to the Array
	*  @return				reference to self
	*/
	template <class Type>
	Array<T>& operator<<(Type&& object) {
		push_back(std::forward<Type>(object));

		return *this;
	}

	/**
	*  Appends given Array to self.
	*
	*  @param other			an Array to be appended to the Array
	*  @return				reference to self
	*/
	Array<T>& operator+=(Array<T> other) {
		return this->concat(std::move(other));
	}

	/**
	*  Arrays difference - removes all the elements of the Array
	*  that also belong to the given one.
	*
	*  The order of the elements if preserved.
	*
	*  @param other			an Array of elements to be removed
	*  @return				reference to self
	*/
	Array<T>& operator-=(const Array<T>& other) {
		this->deleteObjects(other);

		return *this;
	}

	/**
	*  Replaces the whole Array with the given number of its copies.
	*
	*  @param numberOfCopies	number of copies to be done
	*  @return					reference to self
	*/
	Array<T>& operator*=(long long numberOfCopies) {
		if (numberOfCopies < 0)
			throw std::invalid_argument("negative number of copies");
		if (numberOfCopies == 0)
			return (*this = Array<T>());

		Array<T> copy = this->copy();
		while (--numberOfCopies)
			this->concat(copy);

		return *this;
	}

private:
	using vector::back;
	using vector::empty;
	using vector::erase;
	using vector::front;
	using vector::pop_back;
	using vector::resize;


	// When item passed:
	// -> if passed function taking item, returns its result
	// -> if passed pointer to const public member function, returns result of applying it on item
	// -> if passed pointer to public member variable, returns this variable for item
	template <typename Function>
	auto extractValue(const T& item, const Function& function) const
		-> decltype(function(item)) {

		return function(item);
	}

	template <typename MemberFunReturnType, class C>
	MemberFunReturnType extractValue(const T& item, MemberFunReturnType(C::*memberFunction)() const) {
		return (item.*memberFunction)();
	}

	template <typename MemberVariableType, class C>
	auto extractValue(const T& item, const MemberVariableType C::*memberVariable) const
		-> typename std::enable_if<std::is_member_object_pointer<decltype(memberVariable)>::value, MemberVariableType>::type {

		return item.*memberVariable;
	}

	static std::random_device randomDevice;
	static std::mt19937 randomNumberGenerator;
};

template <class T>
std::random_device Array<T>::randomDevice;
template <class T>
std::mt19937 Array<T>::randomNumberGenerator(randomDevice());


// addictional operators //

/**
*  Creates an Array by concatenating the two given Arrays.
*
*  @param first			first Array
*  @param second		second Array
*  @return				a third Array as result of concatenation
*/
template <class T>
Array<T> operator+(Array<T> first, const Array<T>& second) {
	return first += second;
}

/**
*  Creates an Array with all the elements of first Array
*  that do not belong to the second one.
*
*  The order of the elements if preserved.
*
*  @param other			an Array of elements to be removed
*  @return				reference to self
*/
template <class T>
Array<T> operator-(Array<T> first, const Array<T>& second) {
	return first -= second;
}

/**
*  Creates an Array of the given number of its copies.
*
*  @param array				an Array to multiply
*  @param numberOfCopies	number of copies to be done
*  @return					the result Array
*/
template <class T>
Array<T> operator*(Array<T> array, long long numberOfCopies) {
	return array *= numberOfCopies;
}

/**
*  Set intersection - creates an Array that contains
*  all elements of first Array that also belongs to the second one,
*  excluding any duplicates.
*
*  @param first				first Array
*  @param second			second Array
*  @return					intersection of the Arrays
*/
template <class T>
Array<T> operator&(Array<T> first, Array<T> second) {
	first.unique();
	second.unique();
	first.deleteIf([&](const T& item) { return !second.includes(item); });

	return first;
}

/**
*  Checks whether two given Arrays are equal.
*
*  The Arrays are equal if they contain the same number of elements
*  and each element of one Array is equal to the corresponding
*  element in the other one.
*
*  @param first				first Array to be compared
*  @param second			second Array to be compared
*  @return					true if the Arrays are equal and false otherwise
*/
template <class T>
bool operator==(const Array<T>& first, const Array<T>& second) {
	return std::equal(first.begin(), first.end(), second.begin(), second.end());
}

/**
*  Checks whether two given Arrays are not equal.
*
*  The Arrays are not equal if they do not contain the same
*  number of elements or any element of one Array is not
*  equal to the corresponding element in the other one.
*
*  @param first				first Array to be compared
*  @param second			second Array to be compared
*  @return					true if the Arrays are not equal and false otherwise
*/
template <class T>
bool operator!=(const Array<T>& first, const Array<T>& second) {
	return !(first == second);
}

/**
*  Checks whether one Array is lexicographically less than another.
*
*  @param first				first Array to be compared
*  @param second			second Array to be compared
*  @return					true if @first Array is less than @second Array
*/
template <class T>
bool operator<(const Array<T>& first, const Array<T>& second) {
	return std::lexicographical_compare(first.begin(), first.end(), second.begin(), second.end(), std::less<T>());
}

/**
*  Checks whether one Array is lexicographically greater than another.
*
*  @param first				first Array to be compared
*  @param second			second Array to be compared
*  @return					true if @first Array is greater than @second Array
*/
template <class T>
bool operator>(const Array<T>& first, const Array<T>& second) {
	return std::lexicographical_compare(first.begin(), first.end(), second.begin(), second.end(), std::greater<T>());
}

/**
*  Checks whether one Array is lexicographically less than or equal to another./
*
*  @param first				first Array to be compared
*  @param second			second Array to be compared
*  @return					true if @first Array is less than or equal to @second Array
*/
template <class T>
bool operator<=(const Array<T>& first, const Array<T>& second) {
	return std::lexicographical_compare(first.begin(), first.end(), second.begin(), second.end(), std::less<T>());
}

/**
*  Checks whether one Array is lexicographically greater than or equal to another./
*
*  @param first				first Array to be compared
*  @param second			second Array to be compared
*  @return					true if @first Array is greater than or equal to @second Array
*/
template <class T>
bool operator>=(const Array<T>& first, const Array<T>& second) {
	return std::lexicographical_compare(first.begin(), first.end(), second.begin(), second.end(), std::greater_equal<T>());
}

// helpers //

/**
*  Alternative Array creator.
*
*  This function is a handy way to create an Array
*  that includes specified elements.
*  It also allows you not to type the elements type.
*
*  @param objects...		one or more objects of the same type as the first
*							of them (or implicitly convertible to it)
*  @return					an Array of the type of the first given object
*							containing all the arguments
*/
template <class... Objects, class ElementsType = typename std::tuple_element<0, std::tuple<Objects...>>::type>
Array<ElementsType> MakeArray(Objects&&... objects) {
	return Array<ElementsType>{ objects... };
}



#include <iostream>

/**
*  Passes an Array to the output as a string.
*
*  @param os				any std::ostrem
*  @param array				an Array
*  @return					reference to the @a os
*/
template <typename T>
std::ostream& operator<<(std::ostream &os, const Array<T>& array) {
	os << "[" << array.join(", ") << "]";

	return os;
}

#endif // FUNCTIONAL_ARRAY_H
