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

	SECTION("should return copy of self")
		REQUIRE( oneToFive.copy() == oneToFive );

	SECTION("modifying copy should not modify the Array") {
		Array<int> copy = oneToFive.copy();
		copy.push(6);
		REQUIRE(copy != oneToFive);
	}
}
