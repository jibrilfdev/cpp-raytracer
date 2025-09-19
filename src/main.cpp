#include <iostream>

int main() {
	// Image variables

	int image_width = 256;
	int image_height = 256;

	// Image Render

	std::cout  << "P3\n" << image_width << ' ' << image_height << "\n255" << std::endl;

	for (int j = 0; j < image_height; ++j) {
		for (int i = 0; i < image_width; ++i) {
			auto r = double(i) / (image_width);
			auto g = double(j) / (image_height);
			auto b = 0.0;

			int scaled_r = int(255.999 * r);
			int scaled_g = int(255.999 * g);
			int scaled_b = int(255.999 * b);

			std::cout << scaled_r << ' ' << scaled_g << ' ' << scaled_b << '\n';
		}
	}
}
