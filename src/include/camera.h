#pragma once

#include "intersectable.h"
#include "material.h"

class camera {
	public:
		double aspect_ratio = 1.0;
		int image_width = 100;
		int random_samples_per_pixel = 10; // Random samples per pixel
		int max_depth = 10; // Maximum number of ray reflections in scene

		void render(const intersectable& world) {
			initialize();

			std::cout  << "P3\n" << image_width << ' ' << image_height << "\n255\n" << std::endl;

			for (int j = 0; j < image_height; ++j) {
				std::clog << "Line generation progress: " << j << '/' << image_height << '\n' << std::flush;
				for (int i = 0; i < image_width; ++i) {
					color pixel_color(0, 0, 0);
					for (int sample = 0; sample < random_samples_per_pixel; ++sample) {
						double u = (i + random_double()) / (image_width - 1);
						double v = (j + random_double()) / (image_height - 1);
						ray r = get_ray(i, j);
						pixel_color += ray_color(r, max_depth, world);
					}
					write_color(std::cout, pixel_samples_scale * pixel_color);
				}
			}

			std::clog << "\rDone.\n";
		}

	private:
		int image_height; // Image height
		point3d camera_center; // Camera center
		point3d first_pixel_loc; // Location of 0,0
		vec3d pixel_delta_u;
		vec3d pixel_delta_v;
		double pixel_samples_scale; // Color scale factor by pixel samples.

		void initialize() {
			image_height = int(image_width / aspect_ratio);
			image_height = (image_height < 1) ? 1 : image_height;

			pixel_samples_scale = 1.0 / random_samples_per_pixel;

			camera_center = point3d(0, 0, 0);

			// Calculate viewport dimensions
			double focal_length = 1.0;
			double viewport_height = 2.0;
			double viewport_width = viewport_height * (double(image_width)/image_height);

			// Calculate the vectors across the horizontal and down the vertical.

			auto viewport_u = vec3d(viewport_width, 0, 0);
			auto viewport_v = vec3d(0, -viewport_height, 0);

			// Calculate the horizontal and vertical delta vectors

			pixel_delta_u = viewport_u / image_width;
			pixel_delta_v = viewport_v / image_height;

			auto viewport_upper_left = camera_center - vec3d(0,0,focal_length) - viewport_u/2 - viewport_v/2;
			first_pixel_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
		}

		ray get_ray(int i, int j) const {
			// Camera ray from the origin directed to a random sample around the pixel at i, j.
			auto offset = sample_square();
			auto pixel_sample = first_pixel_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

			auto ray_origin = camera_center;
			auto ray_direction = pixel_sample - ray_origin;

			return ray(ray_origin, ray_direction);
		}

		// Returns a vector to a random point in the .5 unit square.
		vec3d sample_square() const {
			return vec3d(random_double() - 0.5, random_double() - 0.5, 0);
		}

		color ray_color(const ray& r, int depth, const intersectable& world) const {
			// Limit bounces by depth recursion.
			if (depth <= 0) return color(0, 0, 0);

			intersects inte;

			if (world.intersect(r, interval(0.001, infinity), inte)) {
				ray scattered;
				color attenuation;
				if (inte.mat->scatter(r, inte, attenuation, scattered)) return attenuation * ray_color(scattered, depth-1, world);
				return color(0, 0, 0);
			}

			vec3d unit_direction = unit_vector(r.direction());
			auto a = 0.5*(unit_direction.y() + 1.0);
			return (1.0-a)*color(1, 0.5, 0.7) + a*color(1, 1, 1);
		}
};

