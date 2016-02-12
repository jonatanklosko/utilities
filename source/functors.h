#ifndef FUNCTORS_H
#define FUNCTORS_H

#include <stdexcept>

/*math operators functors*/

auto add = [](const auto& x) {
	return [&](const auto& n) { return n + x; };
};

auto subtract = [](const auto& x) {
	return [&](const auto& n) { return n - x; };
};

auto multiplyBy = [](const auto& x) {
	return [&](const auto& n) { return n * x; };
};

auto divideBy = [](const auto& x) {
	if (x == 0) throw std::logic_error("divide by zero");
	return [&](const auto& n) { return n / x; };
};

/*boolean operators functors*/

auto lessThan = [](const auto& x) {
	return [&](const auto& n) { return n < x; };
};

auto greaterThan = [](const auto& x) {
	return [&](const auto& n) { return n > x; };
};

auto lessOrEqual = [](const auto& x) {
	return [&](const auto& n) { return n <= x; };
};

auto greaterOrEqual = [](const auto& x) {
	return [&](const auto& n) { return n >= x; };
};

auto equalTo = [](const auto& x) {
	return [&](const auto& n) { return n == x; };
};

auto notEqualTo = [](const auto& x) {
	return [&](const auto& n) { return x != n; };
};


/*other functions*/

auto odd = [](const auto& n)->bool { return n % 2; };

auto even = [](const auto& n)->bool { return n % 2 == 0; };

#endif // FUNCTORS_H
