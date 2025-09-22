#pragma once

#include "constants.h"
#include "ray.h"
#include "color.h"

// Detect sphere intersections.
double sphere_intersect(const point3d&, double radius, const ray& r);

// Determine ray color.
color ray_color(const ray&);

