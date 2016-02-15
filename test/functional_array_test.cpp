#include "catch.hpp"

#include "functional_array.h"

#include "functors.h"
#include <vector>

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
		
		oneToThree.eachAssign(add(2));
		REQUIRE( oneToThree == threeToFive );
	}
}

TEST_CASE("endsWith method", "[Array]") {

	SECTION("should return true if the given Array is an ending of an Array") {
		Array<int> threeToFive{ 3, 4, 5 };
		REQUIRE(oneToFive.endsWith(threeToFive));
	}

	SECTION("should return true if the given Array is empty")
		REQUIRE( oneToFive.endsWith(emptyArray) );

	SECTION("should return false if the given Array is not an ending of an Array") {
		Array<int> fourToSix{ 4, 5, 6 };
		REQUIRE_FALSE( oneToFive.endsWith(fourToSix) );
	}
}

TEST_CASE("filter method", "[Array]") {

	Array<int> oneToSix{ 1, 2, 3, 4, 5, 6 };

	SECTION("should keep in an Array only these elements that satisfies the given condition") {
		REQUIRE( oneToSix.filter(even) == MakeArray(2, 4, 6) );
		REQUIRE( oneToSix.size() == 3 );
	}

	SECTION("if every element satisfies the given condition, an Array should not change")
		REQUIRE( oneToSix.filter(greaterThan(0)) == oneToSix );

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
