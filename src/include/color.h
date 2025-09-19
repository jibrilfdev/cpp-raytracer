#ifndef COLOR_H
#define COLOR_H

#include "./3dvec.h"
#include <iostream>

// Alias
using color = vec3d;

void write_color(std::ostream& output, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    int scaled_r = int(255.999 * r);
    int scaled_g = int(255.999 * g);
    int scaled_b = int(255.999 * b);

    // Write out the pixel color components.
    output << scaled_r << ' ' << scaled_g << ' ' << scaled_b << '\n';
}

#endif
