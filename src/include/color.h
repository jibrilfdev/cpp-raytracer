#pragma once

#include "interval.h"
#include "3dvec.h"
#include <iostream>

// Alias
using color = vec3d;

inline double linear_to_gamma(double linear_component) {
	if (linear_component > 0) return std::sqrt(linear_component);
	return 0;
}

void write_color(std::ostream& output, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

	// Apply a linear to gamma transform.
	r = linear_to_gamma(r);
	g = linear_to_gamma(g);
	b = linear_to_gamma(b);

	// Scaled values to the range [0, 255].
	static const interval intensity(0.000, 0.999);
    int scaled_r = int(256 * intensity.clamp(r));
    int scaled_g = int(256 * intensity.clamp(g));
    int scaled_b = int(256 * intensity.clamp(b));

    // Write out the pixel color components.
    output << scaled_r << ' ' << scaled_g << ' ' << scaled_b << '\n';
}

