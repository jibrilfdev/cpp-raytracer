#include "color.h"
#include "ray.h"
#include "3dvec.h"

// Detect sphere intersections.
double sphere_intersect(const point3d&, double radius, const ray& r);

// Determine ray color.
color ray_color(const ray&);
