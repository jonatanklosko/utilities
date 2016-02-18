#include "catch.hpp"

#include "functional_array.h"

#include "functors.h"

// BEGIN Tests resources

class Person {
public:
	Person(std::string name, int age)
		: name(name), age(age) { }

	int getAge() const { return age; }

	std::string name;
private:
	int age;
};

// Catch needs a way to print Array<Person> and operator<< for Array<Person> needs operator<< for Person
std::ostream& operator<<(std::ostream& os, const Person& person) {
	os << person.name;
	return os;
}

// necessary for comparing objects of the class Array<People> in tests
bool operator==(const Person& first, const Person& second) {
	return (first.name == second.name) && (first.getAge() == second.getAge());
}

// END tests resources

TEST_CASE("Array constructors", "[Array]") {

	SECTION("should be empty when created without arguments")
		REQUIRE( Array<int>().isEmpty() );

	SECTION("should contains the given number of default objects when the size is given") {
		Array<int> array(5);
		REQUIRE( array.size() == 5 );
		for (int item : array)
			REQUIRE( item == 0 );
	}

	SECTION("should contains the given number of given object when both are specified") {
		Array<int> array(5, 10);
		REQUIRE( array.size() == 5 );
		for (int item : array)
			REQUIRE( item == 10 );
	}

	SECTION("should be copy of other array when it is specified") {
		Array<int> array{1, 2, 3};
		Array<int> copy(array);
		REQUIRE( array == copy );
		copy.push(4);
		REQUIRE( array != copy );
	}

	SECTION("should contains items in the given range when constructed with iterators") {
		std::vector<int> v{1, 2, 3};
		Array<int> array(v.begin(), v.end());
		Array<int> expected{ 1, 2, 3 };
		REQUIRE( array == expected );
	}

	SECTION("should contains copies of std::vector elements when std::vector is passed") {
		std::vector<int> v{1, 2, 3};
		Array<int> array(v);
		Array<int> expected{ 1, 2, 3 };
		REQUIRE( array == expected );
	}

	SECTION("should be able to be constructed with initializer list") {
		Array<int> array{1, 1, 1, 1, 1};
		REQUIRE( array == Array<int>(5, 1) );
	}
}

TEST_CASE("assignment operator", "[Array]") {

	SECTION("should be assigned correctly") {
		Array<int> array{1, 2, 3};
		Array<int> copy;
		copy = array;
		REQUIRE( copy == array );
		REQUIRE( copy.push(1) != array );
	}

	SECTION("should be able to assign initializer list to it") {
		Array<int> array;
		array = { 1, 1, 1 };
		REQUIRE( array == Array<int>(3, 1) );
	}
}

const Array<int> oneToFive{ 1, 2, 3, 4, 5 };
const Array<int> emptyArray;

TEST_CASE("any method", "[Array]") {

	SECTION("should return true if one element satisfies the given condition")
		REQUIRE( oneToFive.any(greaterThan(3)) );

	SECTION("should return false if no element satisfies the given condition")
		REQUIRE_FALSE( oneToFive.any(greaterThan(5)) );

	SECTION("should return false if Array is empty regardless of the given condition") {
		REQUIRE_FALSE( emptyArray.any(greaterThan(0)) );
	}
}

TEST_CASE("all method", "[Array]") {

	SECTION("should return true if every element satisfies the given condition")
		REQUIRE( oneToFive.all(greaterThan(0)) );

	SECTION("should return false if at least one element does not satisfies the given condition")
		REQUIRE_FALSE( oneToFive.all(greaterThan(1)) );
	
	SECTION("should return true if an Array is empty regardeless of the given condition")
		REQUIRE( emptyArray.all(greaterThan(0)) );
}

TEST_CASE("allDifferent method", "[Array]") {

	SECTION("should return true if every element is unique")
		REQUIRE( oneToFive.allDifferent() );

	SECTION("should return false if at least two elements are equal")
		REQUIRE_FALSE( Array<int>(2).allDifferent() );

	SECTION("should return true if an Array is empty")
		REQUIRE( emptyArray.allDifferent() );
}

TEST_CASE("allSame method", "[Array]") {

	SECTION("should return true if all elements are equal to each other")
		REQUIRE( Array<int>(5, 10).allSame() );

	SECTION("should return false if at least two elements are different")
		REQUIRE_FALSE( oneToFive.allSame() );

	SECTION("should return true if an Array is empty")
		REQUIRE( emptyArray.allSame() );
}

TEST_CASE("clear method", "[Array]") {

	SECTION("after call an Array should be empty", "[Array]") {
		Array<int> array{1, 2, 3};
		REQUIRE( array.clear() == emptyArray );
		REQUIRE( array.isEmpty() );
	}
}

TEST_CASE("combinations method", "[Array]") {

	SECTION("should return an Array containing Arrays of size n, each with one combination of n elements") {
		Array<int> array{1, 2, 3};
		Array<Array<int>> twoNumsCombinations{ {1, 2}, {1, 3}, {2, 3} };
		REQUIRE( array.combinations(2) == twoNumsCombinations );

		Array<Array<int>> oneNumCombinations{ {1}, {2}, {3} };
		REQUIRE( array.combinations(1) == oneNumCombinations );
	}

	SECTION("should return an Array containing an empty Array when 0 passed as combinations size") {
		Array<Array<int>> arrayWithEmptyArray{ {} };
		REQUIRE( oneToFive.combinations(0) == arrayWithEmptyArray );
	}

	SECTION("should return an empty Array when combination size argument is greater than an Array size")
		REQUIRE( oneToFive.combinations(10) == Array<Array<int>>() );
}

TEST_CASE("compare method", "[Array]") {

	Array<int> twoToSix{ 2, 3, 4, 5, 6 };
	Array<int> oneToSix{ 1, 2, 3, 4, 5, 6 };

	SECTION("should return -1 when first element of an Array is smaller than first one of the given Array")
		REQUIRE( oneToFive.compare(twoToSix) == -1 );

	SECTION("should return 0 when every element in an Array is equal to the corresponding one in the given Array")
		REQUIRE( oneToFive.compare(oneToFive) == 0 );

	SECTION("should return 1 when first element of an Array is greater than the first one of the given Array")
		REQUIRE(twoToSix.compare(oneToFive) == 1);

	SECTION("should return -1 when corresponding elements in both Arrays are equal to each other but the first Array have less elements")
		REQUIRE( oneToFive.compare(oneToSix) == -1 );

	SECTION("should return 1 then corresponding elements in both Arrays are equal to teach otehr but the first Array have more elements")
		REQUIRE( oneToSix.compare(oneToFive) == 1 );
}

TEST_CASE("concat method", "[Array]") {

	Array<int> oneToThree{ 1, 2, 3 };
	Array<int> fourToSix{ 4, 5, 6 };
	Array<int> sevenToNine{ 7, 8, 9 };
	Array<int> oneToSix{ 1, 2, 3, 4, 5, 6 };
	Array<int> oneToNine{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	SECTION("should append the given Array to self saving elements order")
		REQUIRE( oneToThree.concat(fourToSix) == oneToSix );

	SECTION("should be able append any number of Arrays to self")
		REQUIRE( oneToThree.concat(fourToSix, sevenToNine) == oneToNine );
}

TEST_CASE("count method", "[Array]") {
	
	SECTION("should return size of an Array when no argument is given")
		REQUIRE( oneToFive.count() == 5 );

	SECTION("when object given should return number of elements equal to it") {
		REQUIRE( oneToFive.count(3) == 1 );
		REQUIRE( oneToFive.count(6) == 0 );
		REQUIRE( Array<int>(5, 10).count(10) == 5 );
	}

	SECTION("when condition given should return number of elements, which satisfy it") {
		REQUIRE( oneToFive.count(greaterThan(2)) == 3 );
		REQUIRE( oneToFive.count(greaterThan(5)) == 0 );
	}
}

TEST_CASE("copy method", "[Array]") {

	SECTION("should return copy of an Array")
		REQUIRE( oneToFive.copy() == oneToFive );

	SECTION("modifying copy should not modify the Array") {
		Array<int> copy = oneToFive.copy();
		copy.push(6);
		REQUIRE( copy != oneToFive );
	}
}

TEST_CASE("deleteAt method", "[Array]") {

	Array<int> oneToFour{ 1, 2, 3, 4 };

	SECTION("should remove element at the given position")
		REQUIRE( oneToFour.deleteAt(2) == MakeArray(1, 2, 4) );

	SECTION("deleting an element should decrease size of an Array by one") {
		oneToFour.deleteAt(2);
		REQUIRE( oneToFour.size() == 3 );
	}
}

TEST_CASE("deleteBetween method", "[Array]") {
	
	Array<int> oneToFour{ 1, 2, 3, 4 };

	SECTION("should remove all elements at positions greater or equal to the given beginning and less than given end")
		REQUIRE( oneToFour.deleteBetween(1, 3) == MakeArray(1, 4) );

	SECTION("deleting n elements should decrease the size of an Array by n")
		REQUIRE( oneToFour.deleteBetween(1, 3).size() == 2 );
}

TEST_CASE("deleteIf method", "[Array]") {

	Array<int> oneToSix{ 1, 2, 3, 4, 5, 6 };

	SECTION("should remove all elements satisfying the given condition") {
		REQUIRE( oneToSix.deleteIf(odd) == MakeArray(2, 4, 6) );
		REQUIRE( oneToSix.size() == 3 );
	}
}

TEST_CASE("deleteObject method", "[Array]") {

	SECTION("should remove all elements equal to the given one") {
		Array<int> array{1, 2, 2, 3, 2};
		REQUIRE( array.deleteObject(2) == MakeArray(1, 3) );
		REQUIRE( array.size() == 2 );
	}
}

TEST_CASE("deleteObjects method", "[Array]") {

	SECTION("should remove all elements equal to one of the elements in the given Array") {
		Array<int> oneAndTwo{ 1, 2 };
		Array<int> array{1, 2, 3, 4, 1, 2};
		REQUIRE( array.deleteObjects(oneAndTwo) == MakeArray(3, 4) );
		REQUIRE( array.size() == 2 );
	}
}

TEST_CASE("each method", "[Array]") {

	SECTION("should perform the given function for each element") {
		int sum = 0;
		oneToFive.each([&](int n) { sum += n; });
		REQUIRE( sum == 15 );
	}
}

TEST_CASE("eachAssign method", "[Array]") {

	SECTION("should assign to each element result of passing it as an argument to the given function") {
		Array<int> oneToThree{ 1, 2, 3 };
		Array<int> threeToFive{ 3, 4, 5 };
		
		REQUIRE(oneToThree.eachAssign(add(2)) == threeToFive );
	}
}

TEST_CASE("endsWith method", "[Array]") {

	SECTION("should return true if the given Array is an ending of an Array") {
		REQUIRE( oneToFive.endsWith(MakeArray(3, 4, 5)) );
		REQUIRE( oneToFive.endsWith(MakeArray(5)) );
	}

	SECTION("should return true if the given Array is empty")
		REQUIRE( oneToFive.endsWith(emptyArray) );

	SECTION("should return false if the given Array is not an ending of an Array")
		REQUIRE_FALSE( oneToFive.endsWith(MakeArray(4, 5, 6)) );
}

TEST_CASE("filter method", "[Array]") {

	Array<int> oneToSix{ 1, 2, 3, 4, 5, 6 };

	SECTION("should keep in an Array only these elements that satisfies the given condition") {
		REQUIRE( oneToSix.filter(even) == MakeArray(2, 4, 6) );
		REQUIRE( oneToSix.size() == 3 );
	}

	SECTION("if every element satisfies the given condition, an Array should not change")
		REQUIRE( oneToSix.filter(greaterThan(0)) == MakeArray(1, 2, 3, 4, 5, 6) );

	SECTION("if no element setisfies the given condition, an Array should become empty")
		REQUIRE( oneToSix.filter(lessThan(0)) == emptyArray );
}

TEST_CASE("first method", "[Array]") {

	SECTION("when no argument given should return the first element of an Array")
		REQUIRE( oneToFive.first() == 1 );

	SECTION("when a number is given should return new Array containing specified number of the first elements")
		REQUIRE( oneToFive.first(3) == MakeArray(1, 2, 3) );
}

TEST_CASE("fill method", "[Array]") {

	Array<int> sixZeros(6);

	SECTION("when only an object is given, should assign it to every element in an Array")
		REQUIRE( sixZeros.fill(1) == Array<int>(6, 1) );

	SECTION("when beginning position is specified, should assign the given object to elements at positions greater or equal to the given one")
		REQUIRE( sixZeros.fill(1, 2) == MakeArray(0, 0, 1, 1, 1, 1) );

	SECTION("when both beginning and end positions are specified, should assign the given object to elements at positions between them (including the beginning and excluding the end)")
		REQUIRE( sixZeros.fill(1, 2, 4) == MakeArray(0, 0, 1, 1, 0, 0) );
}

TEST_CASE("flatten method", "[Array]") {

	SECTION("when an Array is multi-dimensional, should return a one-dimensional Array") {
		Array<Array<Array<int>>> threeDimensional{ { {1, 2}, {3, 4} }, { {5, 6}, {7, 8} } };
		Array<int> asOneDimensional{ 1, 2, 3, 4, 5, 6, 7, 8 };
		REQUIRE( threeDimensional.flatten() == asOneDimensional );
	}
}

TEST_CASE("flattenByOne method", "[Array]") {

	SECTION("when an Array is two-dimensional, should return a one-dimensional Array") {
		Array<Array<int>> twoDimensional{ { 1, 2 }, { 3, 4 }, { 5, 6 } };
		Array<int> asOneDimensional{ 1, 2, 3, 4, 5, 6 };
		REQUIRE( twoDimensional.flattenByOne() == asOneDimensional );
	}

	SECTION("when an Array is three-dimensional, should return a two-dimensional Array") {
		Array<Array<Array<int>>> threeDimensional{ { { 1, 2 },{ 3, 4 } },{ { 5, 6 },{ 7, 8 } } };
		Array<Array<int>> asTwoDimensional{ { 1, 2 }, { 3, 4 }, { 5, 6 }, { 7, 8 } };
		REQUIRE( threeDimensional.flattenByOne() == asTwoDimensional );
	}
}

TEST_CASE("includes method", "[Array]") {

	SECTION("should return true if an Array contains at least one element equal to the given one") {
		REQUIRE( oneToFive.includes(3) );
		REQUIRE( Array<int>(5, 10).includes(10) );
	}

	SECTION("should return false if no element in an Array is equal to the given one")
		REQUIRE_FALSE( oneToFive.includes(0) );

	SECTION("should return false if an Array is empty")
		REQUIRE_FALSE( emptyArray.includes(0) );
}

TEST_CASE("indexOf method", "[Array]") {

	SECTION("should return the first index of an Array where an element is equal to the given one") {
		REQUIRE( oneToFive.indexOf(2) == 1 );
		REQUIRE( Array<int>(5, 10).indexOf(10) == 0 );
	}

	SECTION("should return -1 if an Array does not contain the given object")
		REQUIRE( oneToFive.indexOf(10) == -1 );

	SECTION("when an initial index is specified should start searching from it") {
		REQUIRE( oneToFive.indexOf(1, 2) == -1 );
		REQUIRE( Array<int>(10, 10).indexOf(10, 5) == 5 );
	}
}

TEST_CASE("indexWhere method", "[Array]") {

	SECTION("should return the first index of an Array where an element setisfies the given condition") {
		REQUIRE( oneToFive.indexWhere(even) == 1 );
		REQUIRE( Array<int>(5, 10).indexWhere(even) == 0 );
	}

	SECTION("should return -1 if no element in an Array satisfies the given condition")
		REQUIRE( oneToFive.indexWhere(greaterThan(10)) == -1 );

	SECTION("when an initial index is specified should start searching from it") {
		REQUIRE( oneToFive.indexWhere(even, 3) == 3 );
		REQUIRE( Array<int>(10, 10).indexWhere(even, 5) == 5 );
	}
}

TEST_CASE("insert method", "[Array]") {

	Array<int> oneToThree{ 1, 2, 3 };

	SECTION("should insert the given object before an element at the given position")
		REQUIRE( oneToThree.insert(1, 2) == MakeArray(1, 2, 2, 3) );

	SECTION("should insert any number of objects (saving the order)")
		REQUIRE( oneToThree.insert(2, 5, 6, 7, 8) == MakeArray(1, 2, 5, 6, 7, 8, 3) );

	SECTION("when the given index is equal to the size of an Array, should append given elements to the end")
		REQUIRE( oneToThree.insert(3, 4) == MakeArray(1, 2, 3, 4) );

	SECTION("when the given index greater than the size of an Array, should resize the Array with the default values and then append the given objects to the end") {
		REQUIRE( oneToThree.insert(6, 10) == MakeArray(1, 2, 3, 0, 0, 0, 10) );
		REQUIRE( Array<int>().insert(4, 1, 2, 3) == MakeArray(0, 0, 0, 0, 1, 2, 3) );
	}
}

TEST_CASE("isEmpty method", "[Array]") {

	SECTION("should return true if an Array have 0 size")
		REQUIRE( emptyArray.isEmpty() );

	SECTION("should return false if an Array have size greater than 0") {
		REQUIRE_FALSE( oneToFive.isEmpty() );
		REQUIRE_FALSE( Array<int>(2).isEmpty() );
	}
}

TEST_CASE("join method", "[Array]") {

	SECTION("should return empty string then an Array is empty") {
		REQUIRE( emptyArray.join() == "" );
		REQUIRE( emptyArray.join("separator") == "" );
	}

	SECTION("when no argument is given should return a string created by sticking all elements together as strings") {
		REQUIRE( oneToFive.join() == "12345" );
		REQUIRE( Array<int>(5, 10).join() == "1010101010" );
	}

	SECTION("when separator is specified should insert it between each two elements") {
		REQUIRE( oneToFive.join(", ") == "1, 2, 3, 4, 5" );
		REQUIRE( oneToFive.join("word") == "1word2word3word4word5" );
	}
}

TEST_CASE("last method", "[Array]") {

	SECTION("when no argument given should return the last element of an Array")
		REQUIRE( oneToFive.last() == 5 );

	SECTION("when a number is given should return new Array containing specified number of the last elements")
		REQUIRE( oneToFive.last(3) == MakeArray(3, 4, 5) );
}

TEST_CASE("lastIndexOf method", "[Array]") {

	SECTION("should return the last index of an Array where an element is equal to the given one") {
		REQUIRE( oneToFive.lastIndexOf(2) == 1 );
		REQUIRE( Array<int>(5, 10).lastIndexOf(10) == 4 );
	}

	SECTION("should return -1 if an Array does not contain the given object")
		REQUIRE( oneToFive.lastIndexOf(10) == -1 );

	SECTION("when an initial index is specified should start searching from it (going to the left)") {
		REQUIRE( oneToFive.lastIndexOf(4, 1) == -1 );
		REQUIRE( Array<int>(10, 10).lastIndexOf(10, 5) == 5 );
		REQUIRE( MakeArray(1, 2, 1, 2, 1, 2).lastIndexOf(1, 3) == 2 );
	}
}

TEST_CASE("lastIndexWhere method", "[Array]") {

	SECTION("should return the last index of an Array where an element setisfies the given condition") {
		REQUIRE( oneToFive.lastIndexWhere(even) == 3 );
		REQUIRE( Array<int>(5, 10).lastIndexWhere(even) == 4 );
	}

	SECTION("should return -1 if no element in an Array satisfies the given condition")
		REQUIRE( oneToFive.lastIndexWhere(greaterThan(10)) == -1 );

	SECTION("when an initial index is specified should start searching from it (going to the left)") {
		REQUIRE( oneToFive.lastIndexWhere(even, 3) == 3 );
		REQUIRE( Array<int>(10, 10).lastIndexWhere(even, 5) == 5 );
		REQUIRE( MakeArray(1, 2, 1, 2, 1, 2).lastIndexWhere(even, 4) == 3 );
	}
}

TEST_CASE("length method", "[Array]") {

	SECTION("should return size of an Array") {
		REQUIRE( oneToFive.length() == 5 );
		REQUIRE( Array<int>(10).length() == 10 );
	}
}

TEST_CASE("map method", "[Array]") {

	SECTION("should return a new Array with results of calling the given function on each element") {
		REQUIRE( oneToFive.map(add(10)) == MakeArray(11, 12, 13, 14, 15) );
		REQUIRE( oneToFive.map(multiplyBy(-1.0)) == MakeArray(-1.0, -2.0, -3.0, -4.0, -5.0) );
	}

	SECTION("should infer types correctly")
		REQUIRE( oneToFive.map([](int n) { return std::to_string(n) + "!"; }) == (Array<std::string>{"1!", "2!", "3!", "4!", "5!"}) );

	SECTION("when an Array is empty, should return a new Array of correct type") {
		std::string (*intToString)(int) = &std::to_string;
		REQUIRE( emptyArray.map(intToString) == Array<std::string>() );
				// other possibility: emptyArray.map<std::string(int)>(std::to_string);
	}

	SECTION("should support both public member variable pointer and const member function pointer") {
		const Array<Person> people{ Person("Harry", 18),
									Person("John", 85),
									Person("Albert", 33),
									Person("Jennifer", 35) };

		REQUIRE( people.map(&Person::name) == MakeArray(std::string("Harry"), "John", "Albert", "Jennifer") );
		REQUIRE( people.map(&Person::getAge) == MakeArray(18, 85, 33, 35) );
	}
}

TEST_CASE("max method", "[Array]") {

	SECTION("should return the largets element of an Array") {
		REQUIRE( oneToFive.max() == 5 );
		REQUIRE( Array<int>(5, 10).max() == 10 );
	}
}

TEST_CASE("min method", "[Array]") {

	SECTION("shoul return the smallest element of an Array") {
		REQUIRE( oneToFive.min() == 1 );
		REQUIRE( Array<int>(5, 10).min() == 10 );
	}
}

TEST_CASE("nonEmpty method", "[Array]") {

	SECTION("should return false if an Array have 0 size")
		REQUIRE_FALSE( emptyArray.nonEmpty() );

	SECTION("should return true if an Array have size greater than 0") {
		REQUIRE( oneToFive.nonEmpty() );
		REQUIRE( Array<int>(2).nonEmpty() );
	}
}

TEST_CASE("partition method", "[Array]") {

	SECTION("should reaarrange the elements in an Array basing on the given condition, saving the order at the same time") {
		Array<int> oneToSix{ 1, 2, 3, 4, 5, 6 };
		REQUIRE( oneToSix.partition(odd) == MakeArray(1, 3, 5, 2, 4, 6) );
		REQUIRE( oneToSix.partition(even) == MakeArray(2, 4, 6, 1, 3, 5) );
		REQUIRE( Array<int>(5, 10).partition(odd) == Array<int>(5, 10) );
	}

	SECTION("when an Array is empty, should return the Array and it still should be empty")
		REQUIRE( Array<int>().partition(even) == emptyArray );
}

TEST_CASE("permutations method", "[Array]") {

	SECTION("should return new Array containng Arrays, each with single permutation of all elements") {
		Array<Array<int>> oneTwoThreePermutations{ {1, 2, 3}, { 1, 3, 2 }, { 2, 1, 3}, {2, 3, 1}, {3, 1, 2}, {3, 2, 1} };
		REQUIRE( MakeArray(1, 2, 3).permutations() == oneTwoThreePermutations );

		REQUIRE( MakeArray(1, 2, 3, 4, 5).permutations().size() == 120 );
		REQUIRE( MakeArray(1, 2, 3, 4, 5, 6, 7, 8).permutations().size() == 40320 );
	}

	SECTION("should a sorted Array of sorted Arrays")
		REQUIRE( MakeArray(1, 2, 3).permutations() == MakeArray(3, 1, 2).permutations() );

	SECTION("when an Array is empty, should return a new empty Array containing one empty Array")
		REQUIRE( emptyArray.permutations() == Array<Array<int>>(1) );
}

TEST_CASE("pop method", "[Array]") {

	Array<int> oneToSix{ 1, 2, 3, 4, 5, 6 };

	SECTION("should remove the last element from an Array and return it") {
		REQUIRE( oneToSix.pop() == 6 );
		REQUIRE( oneToSix.size() == 5 );
		REQUIRE_FALSE( oneToSix.includes(6) );
	}

	SECTION("when a number given, should remove the number of last elements and return them as new Array") {
		REQUIRE( oneToSix.pop(3) == MakeArray(4, 5, 6) );
		REQUIRE( oneToSix.size() == 3 );
		REQUIRE( oneToSix == MakeArray(1, 2, 3) );
	}

	SECTION("when the given number is greater than the size of an Array, should clear the Array and return new Array with all elements") {
		REQUIRE( oneToSix.pop(10) == MakeArray(1, 2, 3, 4, 5, 6) );
		REQUIRE( oneToSix.empty() );
	}
}

TEST_CASE("push method", "[Array]") {

	SECTION("should take any number of elements and append them to the end of an Array") {
		REQUIRE( MakeArray(1).push(2) == MakeArray(1, 2) );
		REQUIRE( MakeArray(1, 2, 3).push(4, 5, 6) == MakeArray(1, 2, 3, 4, 5, 6) );

		Array<int> oneToThree{ 1, 2, 3 };
		REQUIRE( oneToThree.push(1, 2).size() == 5 );
		REQUIRE( oneToThree.endsWith(MakeArray(1, 2)) );
	}
}

TEST_CASE("random method", "[Array]") {

	SECTION("should return an element from an Array") {
		REQUIRE( oneToFive.includes(oneToFive.random()) );
	}

	SECTION("when a number is given, should return new Array with unique elements of an Array") {
		REQUIRE( oneToFive.random(3).allDifferent() );
		REQUIRE( oneToFive.random(3).all([](int n){ return oneToFive.includes(n); }) );
	}

	SECTION("when the given number is greater than the size of an Array, should return new Array containing all elements of the Array")
		REQUIRE( oneToFive.random(10).size() == 5 );
}

TEST_CASE("reduce method", "[Array]") {

	Array<std::string> abcd{"a", "b", "c", "d"};

	SECTION("should return result of calling the given function with the given acumulator and each element, storing the result in the accumulator after each call") {
		REQUIRE( oneToFive.reduce(std::plus<int>(), 5) == 20 );
		REQUIRE( oneToFive.reduce(std::multiplies<int>(), 10) == 1200 );
		REQUIRE( abcd.reduce(std::plus<std::string>(), "_") == "_abcd" );
	}

	SECTION("when no accumulator is specified, should start by passing the given function with first two elements and so on") {
		REQUIRE( oneToFive.reduce(std::plus<int>()) == 15 );
		REQUIRE( oneToFive.reduce(std::multiplies<int>()) == 120 );
		REQUIRE( oneToFive.reduce(std::minus<int>()) == -13 ); // 1 - 2 - 3 - 4 - 5
		REQUIRE( abcd.reduce(std::plus<std::string>()) == "abcd");
	}

	SECTION("when an Array is empty, should return the initial value") {
		REQUIRE( Array<std::string>().reduce(std::plus<std::string>(), "init") == "init" );
		REQUIRE( emptyArray.reduce(std::plus<int>(), 101) == 101 );
	}

	SECTION("when an Array is empty and no initial value is given, should the default value of the type of its elements") {
		REQUIRE( Array<std::string>().reduce(std::plus<std::string>()) == "" );
		REQUIRE( emptyArray.reduce(std::plus<int>()) == 0 );
	}
}

TEST_CASE("reduceRight method", "[Array]") {

	Array<std::string> abcd{ "a", "b", "c", "d" };

	SECTION("should return result of calling the given function with the given acumulator and each element (in reversed order), storing the result in the accumulator after each call") {
		REQUIRE( oneToFive.reduceRight(std::plus<int>(), 5) == 20 );
		REQUIRE( oneToFive.reduceRight(std::multiplies<int>(), 10) == 1200 );
		REQUIRE( abcd.reduceRight(std::plus<std::string>(), "_") == "_dcba" );
	}

	SECTION("when no accumulator is specified, should start by passing the given function with last two elements and so on") {
		REQUIRE( oneToFive.reduceRight(std::plus<int>()) == 15 );
		REQUIRE( oneToFive.reduceRight(std::multiplies<int>()) == 120 );
		REQUIRE( oneToFive.reduceRight(std::minus<int>()) == -5 ); // 5 - 4 - 3 - 2 - 1
		REQUIRE( abcd.reduceRight(std::plus<std::string>()) == "dcba" );
	}

	SECTION("when an Array is empty, should return the initial value") {
		REQUIRE(Array<std::string>().reduceRight(std::plus<std::string>(), "init") == "init");
		REQUIRE(emptyArray.reduceRight(std::plus<int>(), 101) == 101);
	}

	SECTION("when an Array is empty and no initial value is given, should the default value of the type of its elements") {
		REQUIRE(Array<std::string>().reduceRight(std::plus<std::string>()) == "");
		REQUIRE(emptyArray.reduceRight(std::plus<int>()) == 0);
	}
}

TEST_CASE("replace method", "[Array]") {

	Array<int> oneToThree{ 1, 2, 3 };

	SECTION("should replace all occurrences of the first given object with the second one") {
		REQUIRE( oneToThree.replace(3, 10) == MakeArray(1, 2, 10) );
		REQUIRE( Array<int>(5, 10).replace(10, 100) == Array<int>(5, 100) );
	}

	SECTION("if an Array does not include the given object, should not change the Array")
		REQUIRE( oneToThree.replace(10, 0) == MakeArray(1, 2, 3) );

	SECTION("if an Array is empty, should sill be empty")
		REQUIRE( Array<int>().replace(0, 0) == emptyArray );
}

TEST_CASE("replaceIf method", "[Array]") {

	Array<int> oneToSix{ 1, 2, 3, 4, 5, 6 };

	SECTION("should replace all the elements satisfying the given condition with the given object") {
		REQUIRE( oneToSix.replaceIf(even, 11) == MakeArray(1, 11, 3, 11, 5, 11) );
		REQUIRE( Array<int>(5, 10).replaceIf(even, 100) == Array<int>(5, 100) );
	}

	SECTION("if no element in an Array satisfies the given condition, should not change the Array")
		REQUIRE( oneToSix.replaceIf(greaterThan(10), 0) == MakeArray(1, 2, 3, 4, 5, 6) );

	SECTION("if an Array is empty, should sill be empty")
		REQUIRE( Array<int>().replaceIf(even, 0) == emptyArray );
}

TEST_CASE("reverse method", "[Array]") {

	Array<std::string> abcd{ "a", "b", "c", "d" };
	Array<std::string> dcba{ "d", "c", "b", "a" };
	Array<int> oneToSix{ 1, 2, 3, 4, 5, 6 };

	SECTION("should reverse the order of the elements in an Array") {
		REQUIRE( abcd.reverse() == dcba );
		REQUIRE( oneToSix.reverse() == MakeArray(6, 5, 4, 3, 2, 1) );
	}

	SECTION("if an Array is empty, should sill be empty")
		REQUIRE( Array<int>().reverse() == emptyArray );
}

TEST_CASE("rotate method", "[Array]") {

	SECTION("should move all elements in an Array by the given number") {
		REQUIRE( MakeArray(1, 2, 3, 4).rotate(1) == MakeArray(2, 3, 4, 1) );
		REQUIRE( MakeArray(1, 2, 3, 4).rotate(2) == MakeArray(3, 4, 1, 2) );
		REQUIRE( MakeArray(1, 2, 3, 4).rotate(3) == MakeArray(4, 1, 2, 3) );
	}

	SECTION("when the given number is negative, should move the elements in the opposite direction") {
		REQUIRE( MakeArray(1, 2, 3, 4).rotate(-1) == MakeArray(4, 1, 2, 3) );
		REQUIRE( MakeArray(1, 2, 3, 4).rotate(-2) == MakeArray(3, 4, 1, 2) );
		REQUIRE( MakeArray(1, 2, 3, 4).rotate(-3) == MakeArray(2, 3, 4, 1) );
	}

	SECTION("when an Array is empty, should still be empty")
		REQUIRE( Array<int>().rotate(2) == emptyArray );

	SECTION("when the absolute value of the given numebr is greater than the size of an Array, should work correctly") {
		REQUIRE( MakeArray(1, 2, 3, 4).rotate(5) == MakeArray(2, 3, 4, 1) );
		REQUIRE( MakeArray(1, 2, 3).rotate(100) == MakeArray(1, 2, 3).rotate(1) );
		REQUIRE( MakeArray(1, 2, 3, 4).rotate(-98) == MakeArray(1, 2, 3, 4).rotate(-2) );
	}

	SECTION("when the given number is congruent to 0 modulo the size of an Array, the Array should not change") {
		REQUIRE( MakeArray(1, 2).rotate(0) == MakeArray(1, 2) );
		REQUIRE( MakeArray(1, 2, 3, 4).rotate(88) == MakeArray(1, 2, 3, 4 ) );
	}

	SECTION("if an Array is empty, should sill be empty")
		REQUIRE( Array<int>().rotate(10) == emptyArray );
}

TEST_CASE("separate method", "[Array]") {

	SECTION("should return an Array with two Arrays, the first one containing all the elements that satisfy the given condition, and the second one with the rest of them") {
		REQUIRE( oneToFive.separate(even) == MakeArray(MakeArray(2, 4), MakeArray(1, 3, 5)) );
		REQUIRE( oneToFive.separate(greaterThan(3)) == MakeArray(MakeArray(4, 5), MakeArray(1, 2, 3)) );
	}

	SECTION("should return an Array with two Arrays even if one of them is empty") {
		REQUIRE( oneToFive.separate(greaterThan(0)) == MakeArray(oneToFive, emptyArray) );
		REQUIRE( oneToFive.separate(greaterThan(10)) == MakeArray(emptyArray, oneToFive) );
	}

	SECTION("should return an Array with two empty Arrays if there is no elements")
		REQUIRE( emptyArray.separate(even) == MakeArray(emptyArray, emptyArray) );
}

TEST_CASE("shift method", "[Array]") {

	Array<int> oneToSix{ 1, 2, 3, 4, 5, 6 };

	SECTION("should remove the first element from an Array and return it") {
		REQUIRE( oneToSix.shift() == 1 );
		REQUIRE( oneToSix.size() == 5 );
		REQUIRE_FALSE( oneToSix.includes(1) );
	}

	SECTION("when a number given, should remove the number of first elements and return them as new Array") {
		REQUIRE( oneToSix.shift(3) == MakeArray(1, 2, 3) );
		REQUIRE( oneToSix.size() == 3 );
		REQUIRE( oneToSix == MakeArray(4, 5, 6) );
	}

	SECTION("when the given number is greater than the size of an Array, should clear the Array and return new Array with all elements") {
		REQUIRE(oneToSix.shift(10) == MakeArray(1, 2, 3, 4, 5, 6));
		REQUIRE(oneToSix.empty());
	}
}

TEST_CASE("shuffle method", "[Array]") {

	Array<int> oneToSix{ 1, 2, 3, 4, 5, 6 };

	SECTION("shuffling an Array should not change its size")
		REQUIRE( oneToSix.shuffle().size() == 6 );

	SECTION("a shuffled Array should still contains each of its elements") {
		oneToSix.shuffle();
		for (int i = 1; i <= 6; ++i)
			REQUIRE( oneToSix.includes(i) );
	}
}

TEST_CASE("slice method", "[Array]") {

	SECTION("should return a new Array containing all elements in the specified range (excluding the end)") {
		REQUIRE( oneToFive.slice(2, 4) == MakeArray(3, 4) );
		REQUIRE( oneToFive.slice(1, 5) == MakeArray(2, 3, 4, 5) );
		REQUIRE( oneToFive.slice(0, 5) == oneToFive );
	}

	SECTION("if only the beginning of the range is given, should return a new Array with all elements starting from it up to the end") {
		REQUIRE( oneToFive.slice(2) == MakeArray(3, 4, 5) );
		REQUIRE( oneToFive.slice(0) == oneToFive );
		REQUIRE( oneToFive.slice(3) == MakeArray(4, 5) );
	}

	SECTION("when the second given index is greater than the size of an Array, should omit it") {
		REQUIRE( oneToFive.slice(2, 10) == MakeArray(3, 4, 5) );
		REQUIRE( oneToFive.slice(3, 100) == MakeArray(4, 5) );
	}

	SECTION("when the first given index is greater than the size of an Array, should return an empty Array") {
		REQUIRE( oneToFive.slice(6, 10) == emptyArray );
		REQUIRE( oneToFive.slice(5, 100) == emptyArray );
	}

	SECTION("when the first given intex is greater then the second one, should return an empty Array") {
		REQUIRE( oneToFive.slice(2, 1) == emptyArray );
		REQUIRE( oneToFive.slice(3, 3) == emptyArray );
		REQUIRE( oneToFive.slice(6, 0) == emptyArray );
	}
}

TEST_CASE("sort method", "[Array]") {

	Array<int> mixedOneToFive{ 3, 5, 1, 4, 2 };

	SECTION("when no function is specified, should sort elements in the ascending order from left to right") {
		REQUIRE( mixedOneToFive.sort() == oneToFive );
		REQUIRE( Array<int>(5, 10).sort() == Array<int>(5, 10) );
	}
	
	SECTION("when the function is given, should sort elements using it to compare them")
		REQUIRE( mixedOneToFive.sort(std::greater<int>()) == MakeArray(5, 4, 3, 2, 1) );


	SECTION("when an Array is empty, should still be empty")
		REQUIRE(Array<int>().sort() == emptyArray);
}

TEST_CASE("sortBy method", "[Array]") {

	SECTION("should sort the elements by the results of calling the given function for each of them (in the ascending order)") {
		Array<int> mixedOneToFive{ 3, 5, 1, 4, 2 };
		REQUIRE( mixedOneToFive.sortBy([](int n){ return -n; }) == MakeArray(5, 4, 3, 2, 1) );
	}

	SECTION("should support both public member variable pointer and const member function pointer") {
		Array<Person> people{ Person("Harry", 18),
							  Person("John", 85),
							  Person("Albert", 33),
							  Person("Jennifer", 35) };

		REQUIRE( people.sortBy(&Person::name) == MakeArray(Person("Albert", 33),
														   Person("Harry", 18),
														   Person("Jennifer", 35),
														   Person("John", 85)) );

		REQUIRE( people.sortBy(&Person::getAge) == MakeArray(Person("Harry", 18),
															 Person("Albert", 33),
															 Person("Jennifer", 35),
															 Person("John", 85)) );
	}

	SECTION("when an Array is empty, should still be empty")
		REQUIRE(Array<Person>().sortBy(&Person::name) == Array<Person>());
}

TEST_CASE("startsWith method", "[Array]") {

	SECTION("should return true if the given Array is a beginning of an Array") {
		REQUIRE( oneToFive.startsWith(MakeArray(1, 2, 3)) );
		REQUIRE( oneToFive.startsWith(MakeArray(1)) );
	}

	SECTION("should return true if the given Array is empty")
		REQUIRE( oneToFive.startsWith(emptyArray) );

	SECTION("should return false if the given Array is not an ending of an Array")
		REQUIRE_FALSE( oneToFive.startsWith(MakeArray(2, 3, 4)) );
}

TEST_CASE("swap method", "[Array]") {

	SECTION("should swap the contents of an Array and the given one") {
		Array<int> oneToThree{ 1, 2, 3 };
		Array<int> fourToSix{ 4, 5, 6 };
		REQUIRE( oneToThree.swap(fourToSix) == MakeArray(4, 5, 6) );
		REQUIRE( fourToSix == MakeArray(1, 2, 3) );

		REQUIRE( Array<int>().swap(Array<int>(5, 10)) == Array<int>(5, 10) );
	}
}

TEST_CASE("unique method", "[Array]") {

	SECTION("should remove all occurrences of the same element except the first one (saving the order)") {
		REQUIRE( MakeArray(1, 3, 4, 1, 1, 5, 3, 4, 2).unique() == MakeArray(1, 3, 4, 5, 2) );
		REQUIRE( Array<int>(5, 10).unique() == Array<int>(1, 10) );
	}

	SECTION("when each element is unique, should not change an Array") {
		Array<int> oneToFour{ 1, 2, 3, 4 };
		REQUIRE( oneToFour.unique() == MakeArray(1, 2, 3, 4) );
	}
}

TEST_CASE("unshift method", "[Array]") {

	SECTION("should take any number of elements and append them to the beginning of an Array") {
		REQUIRE( MakeArray(2).unshift(1) == MakeArray(1, 2) );
		REQUIRE( MakeArray(4, 5, 6).unshift(1, 2, 3) == MakeArray(1, 2, 3, 4, 5, 6) );

		Array<int> oneToThree{ 1, 2, 3 };
		REQUIRE( oneToThree.unshift(-1, 0).size() == 5 );
		REQUIRE( oneToThree.startsWith(MakeArray(-1, 0)) );
	}
}


TEST_CASE("operator<<", "[Array]") {

	SECTION("should take an element and append it to the end of an Array") {
		REQUIRE( (MakeArray(1) << 2) == MakeArray(1, 2));

		Array<int> oneToThree{ 1, 2, 3 };
		REQUIRE( (oneToThree << 4).size() == 4 );
		REQUIRE( oneToThree.endsWith(MakeArray(4)) );
	}

	SECTION("should allow chaining")
		REQUIRE( (MakeArray(1, 2, 3) << 4 << 5 << 6) == MakeArray(1, 2, 3, 4, 5, 6) );
}

TEST_CASE("operator*=", "[Array]") {

	SECTION("should repleace an Array with the given number of its copies") {
		REQUIRE( (MakeArray(1, 2) *= 2) == MakeArray(1, 2, 1, 2) );

		Array<int> oneToThreeTimesFour{ 1, 2, 3 };
		oneToThreeTimesFour *= 4;
		REQUIRE( oneToThreeTimesFour == MakeArray(1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3) );
	}

	SECTION("when an Array is empty, should still be empty")
		REQUIRE( (Array<int>() *= 2) == emptyArray );
}

TEST_CASE("operator&", "[Array]") {

	SECTION("should return set intersection of two Arrays as a new Array") {
		REQUIRE( (MakeArray(4, 2, 1, 3) & MakeArray(5, 2, 3, 7, 5, 4)) == MakeArray(4, 2, 3) );
		REQUIRE( (oneToFive & Array<int>(5, 3)) == MakeArray(3) );
	}

	SECTION("should return a new Array with unique elements")
		REQUIRE( (Array<int>(5, 10) & Array<int>(5, 10)) == MakeArray(10) );

	SECTION("if one of the given Arrays is empty, should return an empty Array")
		REQUIRE((oneToFive & Array<int>()) == emptyArray);
}

TEST_CASE("operator==", "[Array]") {

	SECTION("should return true if and only if the two given Arrays contain the same number of elements and every element in the first Array is equal to the corresponding one in the second Array") {
		REQUIRE( Array<int>(5, 10) == Array<int>(5, 10) );
		REQUIRE( oneToFive == MakeArray(1, 2, 3, 4, 5) );
		REQUIRE( MakeArray(1, 10) == MakeArray(1, 10) );

		REQUIRE_FALSE( oneToFive == MakeArray(2, 1, 3, 4, 5) );
		REQUIRE_FALSE( Array<int>(5, 10) == Array<int>(6, 10) );
		REQUIRE_FALSE( MakeArray(2, 3, 4) == MakeArray(4, 3, 2) );
		REQUIRE_FALSE( MakeArray(1) == MakeArray(1, 2) );
	}

	SECTION("should return true if both the given Arrays are empty")
		REQUIRE( emptyArray == Array<int>() );
}

TEST_CASE("operator!=", "[Array]") {

	SECTION("should return true if and only if the two given Arrays do not contain the same number of elements or at least one element in the first Array is not equal to the corresponding one in the second Array") {
		REQUIRE_FALSE( Array<int>(5, 10) != Array<int>(5, 10) );
		REQUIRE_FALSE( oneToFive != MakeArray(1, 2, 3, 4, 5) );
		REQUIRE_FALSE( MakeArray(1, 10) != MakeArray(1, 10) );

		REQUIRE( oneToFive != MakeArray(2, 1, 3, 4, 5) );
		REQUIRE( Array<int>(5, 10) != Array<int>(6, 10) );
		REQUIRE( MakeArray(2, 3, 4) != MakeArray(4, 3, 2) );
		REQUIRE( MakeArray(1) != MakeArray(1, 2) );
	}

	SECTION("should return false if both the given Arrays are empty")
		REQUIRE_FALSE(emptyArray != Array<int>());
}

TEST_CASE("operator<", "[Array]") {

	SECTION("should return result of comparing two Arrays lexicographically") {
		REQUIRE( MakeArray(1, 2) < MakeArray(1, 2, 3) );
		REQUIRE( MakeArray(1, 2) < MakeArray(2) );
		REQUIRE( MakeArray(1, 2, 3) < MakeArray(1, 3, 2) );
		REQUIRE( MakeArray(6, 3, 2, 5) < MakeArray(6, 4, 1, 0) );

		REQUIRE_FALSE( MakeArray(1, 2, 3) < MakeArray(1, 2) );
		REQUIRE_FALSE( MakeArray(1) < emptyArray );
		REQUIRE_FALSE( Array<int>(5, 10) < Array<int>(4, 10) );
	}

	SECTION("should return false if the Arrays are equal")
		REQUIRE_FALSE( oneToFive < MakeArray(1, 2, 3, 4, 5) );
}

TEST_CASE("operator>", "[Array]") {

	SECTION("should return result of comparing two Arrays lexicographically") {
		REQUIRE( MakeArray(1, 2, 3) > MakeArray(1, 2) );
		REQUIRE( MakeArray(2) > MakeArray(1, 2) );
		REQUIRE( MakeArray(1, 3, 2) > MakeArray(1, 2, 3) );
		REQUIRE( MakeArray(6, 4, 1, 0) > MakeArray(6, 3, 2, 5) );

		REQUIRE_FALSE( emptyArray > MakeArray(1) );
		REQUIRE_FALSE( Array<int>(4, 10) > Array<int>(5, 10) );
	}

	SECTION("should return false if the Arrays are equal")
		REQUIRE_FALSE( oneToFive > MakeArray(1, 2, 3, 4, 5) );
}

TEST_CASE("operator<=", "[Array]") {

	SECTION("should return result of comparing two Arrays lexicographically") {
		REQUIRE( MakeArray(1, 2) <= MakeArray(1, 2, 3) );
		REQUIRE( MakeArray(1, 2) <= MakeArray(2) );
		REQUIRE( MakeArray(1, 2, 3) <= MakeArray(1, 3, 2) );
		REQUIRE( MakeArray(6, 3, 2, 5) <= MakeArray(6, 4, 1, 0) );

		REQUIRE_FALSE( MakeArray(1, 2, 3) <= MakeArray(1, 2) );
		REQUIRE_FALSE( MakeArray(1) <= emptyArray );
		REQUIRE_FALSE( Array<int>(5, 10) <= Array<int>(4, 10) );
	}

	SECTION("should return true if the Arrays are equal")
		REQUIRE(oneToFive <= MakeArray(1, 2, 3, 4, 5));
}

TEST_CASE("operator>=", "[Array]") {

	SECTION("should return result of comparing two Arrays lexicographically") {
		REQUIRE( MakeArray(1, 2, 3) >= MakeArray(1, 2) );
		REQUIRE( MakeArray(2) >= MakeArray(1, 2) );
		REQUIRE( MakeArray(1, 3, 2) >= MakeArray(1, 2, 3) );
		REQUIRE( MakeArray(6, 4, 1, 0) >= MakeArray(6, 3, 2, 5) );

		REQUIRE_FALSE( emptyArray > MakeArray(1) );
		REQUIRE_FALSE( Array<int>(4, 10) > Array<int>(5, 10) );
	}

	SECTION("should return true if the Arrays are equal")
		REQUIRE( oneToFive >= MakeArray(1, 2, 3, 4, 5) );
}

TEST_CASE("MakeArray helper", "[Array]") {

	SECTION("should return an Array containing all the given arguments") {
		REQUIRE( MakeArray(1, 2, 3, 4, 5) == oneToFive );
		REQUIRE( MakeArray(10, 10, 10) == Array<int>(3, 10) );
		REQUIRE( MakeArray(3, 5, 10, 2, 8) == (Array<int>{3, 5, 10, 2, 8}) );
	}

	SECTION("the type of the elements of the returned Array, should be the same as the type of the first given argument") {
		REQUIRE( MakeArray(std::string("a"), "b", "c") == (Array<std::string>{"a", "b", "c"}) );
		REQUIRE( MakeArray(1.1, 2.2, 3.3) == (Array<double>{1.1, 2.2, 3.3}) );
	}

	SECTION("should work correctly with variables") {
		int one = 1;
		int two = 2;
		REQUIRE( MakeArray(one, two, 3, 4, 5) == oneToFive );
	}
}

TEST_CASE("ostream operator<<", "[Array]") {

	SECTION("should put all the elements in the stream between square brackets, each two separated by comma and space") {
		std::stringstream stream;
		stream << oneToFive;
		REQUIRE( stream.str() == "[1, 2, 3, 4, 5]" );

		stream.str("");
		stream << emptyArray;
		REQUIRE( stream.str() == "[]" );
	}
}
