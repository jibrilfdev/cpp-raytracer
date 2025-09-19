#ifndef MAIN_H
#define MAIN_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

#include "color.h"
#include "ray.h"
#include "3dvec.h"

// STD

using std::make_shared;
using std::shared_ptr;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;



// Detect sphere intersections.
double sphere_intersect(const point3d&, double radius, const ray& r);

// Determine ray color.
color ray_color(const ray&);

// (Helper) Convert degrees to radians.
inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

#endif
