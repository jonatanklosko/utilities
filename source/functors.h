#ifndef FUNCTORS_H
#define FUNCTORS_H

/*math operators functors*/

const auto add = [](const auto& x) {
	return [=](const auto& n) { return n + x; };
};

const auto subtract = [](const auto& x) {
	return [=](const auto& n) { return n - x; };
};

const auto multiplyBy = [](const auto& x) {
	return [=](const auto& n) { return n * x; };
};

const auto divideBy = [](const auto& x) {
	if (x == 0) throw std::logic_error("divide by zero");
	return [=](const auto& n) { return n / x; };
};

/*boolean operators functors*/

const auto lessThan = [](const auto& x) {
	return [=](const auto& n) { return n < x; };
};

const auto greaterThan = [](const auto& x) {
	return [=](const auto& n) { return n > x; };
};

const auto lessOrEqual = [](const auto& x) {
	return [=](const auto& n) { return n <= x; };
};

const auto greaterOrEqual = [](const auto& x) {
	return [=](const auto& n) { return n >= x; };
};

const auto equalTo = [](const auto& x) {
	return [=](const auto& n) { return n == x; };
};

const auto notEqualTo = [](const auto& x) {
	return [=](const auto& n) { return x != n; };
};


/*other functions*/

const auto odd = [](const auto& n)->bool { return n % 2; };

const auto even = [](const auto& n)->bool { return n % 2 == 0; };

#endif // FUNCTORS_H
