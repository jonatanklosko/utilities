#include "catch.hpp"

#include "functors.h"

#include <string>

TEST_CASE("add", "[functors][math]") {
	
	SECTION("should return function that adds the given argument to its own argument") {
		auto addFiveTo = add(5);
		REQUIRE( addFiveTo(10) == 15 );
		REQUIRE( addFiveTo(-50) == -45 );

		REQUIRE( add(2)(2) == 4 );
		REQUIRE( add(-2)(2) == 0 );
		REQUIRE( add(0)(10) == 10 );
		REQUIRE( add(-11.5)(-8.5) == -20 );
	}

	SECTION("should work with any type that has overloaded plus operator") {
		auto addExclamationMark = add(std::string("!"));
		std::string foo = "foo";
		REQUIRE( addExclamationMark(foo) == "foo!" );
	}
}

TEST_CASE("subtract", "[functors][math]") {

	SECTION("should return function that subtracts the given argument from its own argument") {
		auto subtractFiveFrom = subtract(5);
		REQUIRE( subtractFiveFrom(10) == 5 );
		REQUIRE( subtractFiveFrom(-100.99) == -105.99 );

		REQUIRE( subtract(2)(2) == 0 );
		REQUIRE( subtract(4)(-2) == -6 );
		REQUIRE( subtract(10)(20) == 10 );
		REQUIRE( subtract(0)(1.1) == 1.1 );
	}
}

TEST_CASE("multiplyBy", "[functors][math]") {
	
	SECTION("should return function that multiplies its own argument by the given argument") {
		auto multiplyByTen = multiplyBy(10);
		REQUIRE( multiplyByTen(5) == 50 );
		REQUIRE( multiplyByTen(-9.11) == -91.1 );

		REQUIRE( multiplyBy(8)(5) == 40 );
		REQUIRE( multiplyBy(0)(4) == 0 );
		REQUIRE( multiplyBy(1.1)(2.2) == Approx(2.42) );
		REQUIRE( multiplyBy(-40)(0.25) == -10 );
	}
}

TEST_CASE("divideBy", "[functors][math]") {
	
	SECTION("should return function that divides its own argument by the given argument") {
		auto divideByFive = divideBy(5);
		REQUIRE( divideByFive(10) == 2 );
		REQUIRE( divideByFive(-105) == -21 );

		REQUIRE( divideBy(2.0)(7) == 3.5 );
		REQUIRE( divideBy(1.5)(6) == 4 );
		REQUIRE( divideBy(10)(1000) == 100 );
		REQUIRE( divideBy(3.0)(1) == Approx(0.3333).epsilon(0.0001) );
	}

	SECTION("should throw an exception when 0 is passed as the argument") {
		REQUIRE_THROWS( divideBy(0) );
		REQUIRE_THROWS( divideBy(0)(10) );
		REQUIRE_THROWS( divideBy(0)(1.1) );
	}
}

TEST_CASE("lessThan", "[functors][boolean]") {

	SECTION("should return function that returns if its own argument is less than the given argument") {
		auto lessThanFive = lessThan(5);
		REQUIRE( lessThanFive(3) );
		REQUIRE_FALSE( lessThanFive(7.7) );

		REQUIRE( lessThan(15)(10.5) );
		REQUIRE_FALSE( lessThan(5)(5) );
		REQUIRE( lessThan(1)(-10) );
		REQUIRE_FALSE( lessThan(-1)(0) );
	}
}

TEST_CASE("greaterThan", "[functors][boolean]") {

	SECTION("should return function that returns if its own argument is greater than the given argument") {
		auto greaterThanFive = greaterThan(5);
		REQUIRE( greaterThanFive(7) );
		REQUIRE_FALSE( greaterThanFive(3.3) );

		REQUIRE( greaterThan(10)(15) );
		REQUIRE_FALSE( greaterThan(5)(5) );
		REQUIRE( greaterThan(-10)(-1) );
		REQUIRE_FALSE( greaterThan(0)(-1.1) );
	}
}

TEST_CASE("lessOrEqual", "[functors][boolean]") {

	SECTION("should return function that returns if its own argument is less than or equal to the given argument") {
		auto lessOrEqualFive = lessOrEqual(5);
		REQUIRE( lessOrEqualFive(3) );
		REQUIRE_FALSE( lessOrEqualFive(7.7) );

		REQUIRE( lessOrEqual(15)(10.5) );
		REQUIRE( lessOrEqual(5)(5) );
		REQUIRE_FALSE( lessOrEqual(5)(5.1) );
		REQUIRE( lessOrEqual(-10)(-10.0) );
		REQUIRE( lessOrEqual(1)(-10) );
		REQUIRE_FALSE( lessOrEqual(-1)(0) );
	}
}

TEST_CASE("greaterOrEqual", "[functors][boolean]") {

	SECTION("should return function that returns if its own argument is greater than or equal to the given argument") {
		auto greaterOrEqualFive = greaterOrEqual(5);
		REQUIRE( greaterOrEqualFive(7) );
		REQUIRE_FALSE( greaterOrEqualFive(3.3) );

		REQUIRE( greaterOrEqual(10)(15) );
		REQUIRE( greaterOrEqual(5.0)(5) );
		REQUIRE_FALSE( greaterOrEqual(5.1)(5) );
		REQUIRE( greaterOrEqual(-10)(-1) );
		REQUIRE_FALSE( greaterOrEqual(0)(-1.1) );
	}
}

TEST_CASE("equalTo", "[functors][boolean]") {

	SECTION("should return function that returns if its own argument is equal to the given argument") {
		auto equalToFive = equalTo(5);
		REQUIRE( equalToFive(5.0) );
		REQUIRE_FALSE( equalToFive(7.7) );

		REQUIRE( equalTo(15)(15) );
		REQUIRE_FALSE( equalTo(5)(-5) );
		REQUIRE_FALSE( equalTo(5)(5.1) );
		REQUIRE( equalTo(-10)(-10.0) );
	}
}

TEST_CASE("notEqualTo", "[functors][boolean]") {

	SECTION("should return function that returns if its own argument is not equal to the given argument") {
		auto notEqualToFive = notEqualTo(5);
		REQUIRE( notEqualToFive(7.7) );
		REQUIRE_FALSE( notEqualToFive(5) );

		REQUIRE( notEqualTo(15)(-15) );
		REQUIRE_FALSE( notEqualTo(5)(5) );
		REQUIRE_FALSE( notEqualTo(5)(5.0) );
		REQUIRE( notEqualTo(-10)(-10.12) );
	}
}

TEST_CASE("odd", "[functions][math]") {

	SECTION("should return true if the given argument is odd") {
		REQUIRE( odd(1) );
		REQUIRE( odd(11) );
		REQUIRE( odd(13432423431) );
	}

	SECTION("should return false if the given argument is even") {
		REQUIRE_FALSE( odd(2) );
		REQUIRE_FALSE( odd(104) );
		REQUIRE_FALSE( odd(13432423438) );
	}
}

TEST_CASE("even", "[functions][math]") {

	SECTION("should return true if the given argument is even") {
		REQUIRE( even(2) );
		REQUIRE( even(104) );
		REQUIRE( even(13432423438) );
	}

	SECTION("should return false if the given argument is odd") {
		REQUIRE_FALSE( even(1) );
		REQUIRE_FALSE( even(11) );
		REQUIRE_FALSE( even(13432423431) );
	}
}
