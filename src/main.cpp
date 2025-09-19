#include "include/color.h"
#include "include/ray.h"
#include "include/3dvec.h"
#include "include/main.h"

#include <iostream>


color ray_color(const ray& r) {
	auto sh = sphere_intersect(point3d(0, 0, -1), 0.5, r);
	if (sh > 0.0) {
		vec3d v = unit_vector(r.at(sh) - vec3d(0, 0, -1));
		return 0.5*color(v.x()+1, v.y()+1, v.z()+1);
	}

	vec3d unit_direction = unit_vector(r.direction());
	auto a = 0.5 * (unit_direction.y() + 1.0);
	return (1.0-a) * color(1.0,1.0,1.0) + a * color(0.5, 0.9, 1.0);
}

double sphere_intersect(const point3d& center, double radius, const ray& r) {
	vec3d oc = center - r.origin();
	double a = r.direction().length_squared();
	double h = dot(r.direction(), oc);
	double c = oc.length_squared() - radius*radius;
	auto discriminant = h*h - a*c;

	return discriminant < 0 ? -1.0 : ((h - std::sqrt(discriminant)) / (a));
}

int main() {
	// Image variables

	double aspect_ratio = 16.0 / 9.0;
	int image_width = 600;

	// Calculate image height

	int image_height = int(image_width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;

	// Camera

	double focal_length = 1.0;
	double viewport_height = 2.0;
	double viewport_width = viewport_height * (double(image_width)/image_height);
	auto camera_center = point3d(0,0,0);

	// Calculate the vectors across the horizontal and down the vertical.

	auto viewport_u = vec3d(viewport_width, 0, 0);
	auto viewport_v = vec3d(0, -viewport_height, 0);

	// Calculate the horizontal and vertical delta vectors

	auto pixel_delta_u = viewport_u / image_width;
	auto pixel_delta_v = viewport_v / image_height;

	auto viewport_upper_left = camera_center - vec3d(0,0,focal_length) - viewport_u/2 - viewport_v/2;
	auto first_pixel_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

	// Image Render

	std::cout  << "P3\n" << image_width << ' ' << image_height << "\n255" << std::endl;

	for (int j = 0; j < image_height; ++j) {
		std::clog << "Line generation progress: " << j << '/' << image_height << '\n' << std::flush;
		for (int i = 0; i < image_width; ++i) {
			auto pixel_center = first_pixel_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
			auto ray_direction = pixel_center - camera_center;
			ray r(camera_center, ray_direction);

			color pixel_color = ray_color(r);
			write_color(std::cout, pixel_color);
		}
	}

	std::clog << "\rDone.\n";
}
