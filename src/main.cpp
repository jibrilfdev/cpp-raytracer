#include "include/color.h"
#include "include/3dvec.h"

#include <iostream>

int main() {
	// Image variables

	int image_width = 256;
	int image_height = 256;

	// Image Render

	std::cout  << "P3\n" << image_width << ' ' << image_height << "\n255" << std::endl;

	for (int j = 0; j < image_height; ++j) {
		std::clog << "Line generation progress: " << j << '/' << image_height << '\n' << std::flush;
		for (int i = 0; i < image_width; ++i) {
			auto pixel_color = color(double(i)/(image_width), double(j)/(image_height), 0);
			write_color(std::cout, pixel_color);
		}
	}

	std::clog << "\rDone.\n";
}
