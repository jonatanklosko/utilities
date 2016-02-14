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
