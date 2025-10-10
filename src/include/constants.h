#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

// STD
using std::make_shared;
using std::shared_ptr;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// (Helper) Convert degrees to radians.
inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

// Returns a random real number in the range [0, 1).
inline double random_double() {
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

// Returns a random real in the range [min, max)
inline double random_double_range(double min, double max) {
	return min + (max-min)*random_double();
}

// Generate a random integer in the range [min, max];
inline int random_int(int min, int max) {
	return int(random_double_range(min, max+1));
}
