#pragma once

#include "intersectable.h"
#include "material.h"
#include <omp.h>
#include <atomic>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>

class camera {
	public:
		double aspect_ratio = 1.0;
		int image_width = 100;
		int random_samples_per_pixel = 10; // Random samples per pixel
		int threads = 2; // CPU threads
		int max_depth = 10; // Maximum number of ray reflections in scene
		color background; // Background color of the scene
		double vfov = 90; // Vertical view angle
		point3d look_from = point3d(0, 0, 0); // Point camera is looking from
		point3d look_at = point3d(0, 0, -1); // Point camera is looking at
		vec3d vup = point3d(0, 1, 0); // Camera's up direction
		double defocus_angle = 0;
		double focus_dist = 10; // Distance from camera with perfect focus


		void render(const intersectable& world) {
			initialize();

			omp_set_num_threads(threads);

			std::vector<color> framebuffer(image_width*image_height);
			std::atomic<int> rows_done = 0;
			std::atomic<bool> rendering_done = false;

			std::thread progress_thread([&]() {
			while (!rendering_done) {
				double pct = 100.0 * rows_done.load() / image_height;
				std::clog << "\rRendering: " << std::fixed << std::setprecision(2) << pct << "% (" << rows_done << "/" << image_height << ")" << std::flush;
				std::this_thread::sleep_for(std::chrono::milliseconds(150));
			}
		});

			std::cout  << "P3\n" << image_width << ' ' << image_height << "\n255\n" << std::endl;

			#pragma omp parallel for schedule(dynamic)
			for (int j = 0; j < image_height; ++j) {
				for (int i = 0; i < image_width; ++i) {
					color pixel_color(0, 0, 0);
					for (int sample = 0; sample < random_samples_per_pixel; ++sample) {
						double u = (i + random_double()) / (image_width - 1);
						double v = (j + random_double()) / (image_height - 1);
						ray r = get_ray(i, j);
						pixel_color += ray_color(r, max_depth, world);
					}
					framebuffer[j * image_width + i] = pixel_samples_scale * pixel_color;
				}
			rows_done++;
			}

			rendering_done = true;
			progress_thread.join();

			for (int j = 0; j < image_height; ++j) {
				for (int i = 0; i < image_width; ++i) {
					write_color(std::cout, framebuffer[j * image_width + i]);
				}
			}

			std::clog << "\rRendering: 100% (" << image_height << "/" << image_height << ")      \n";

			std::clog << "\rDone.\n";
		}

	private:
		int image_height; // Image height
		point3d camera_center; // Camera center
		point3d first_pixel_loc; // Location of 0,0
		vec3d pixel_delta_u;
		vec3d pixel_delta_v;
		double pixel_samples_scale; // Color scale factor by pixel samples.
		vec3d u, v, w; // Camera frame basis vectors
		vec3d defocus_disk_u;
		vec3d defocus_disk_v;

		void initialize() {
			image_height = int(image_width / aspect_ratio);
			image_height = (image_height < 1) ? 1 : image_height;

			pixel_samples_scale = 1.0 / random_samples_per_pixel;

			camera_center = look_from;

			// Calculate viewport dimensions
			double theta = degrees_to_radians(vfov);
			auto h = std::tan(theta/2);
			double viewport_height = 2 * h * focus_dist;
			double viewport_width = viewport_height * (double(image_width)/image_height);
			// Calculate the basis vectors for the camera coordinate frame.
			w = unit_vector(look_from - look_at);
			u = unit_vector(cross(vup, w));
			v = cross(w, u);

			// Calculate the vectors across the horizontal and down the vertical.

			vec3d viewport_u = viewport_width * u; // Vector across viewport horizontal edge
			vec3d viewport_v = viewport_height * -v; // Vector down viewport vertical edge


			// Calculate the horizontal and vertical delta vectors

			pixel_delta_u = viewport_u / image_width;
			pixel_delta_v = viewport_v / image_height;

			auto viewport_upper_left = camera_center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
			first_pixel_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

			// Calculate the camera defocus disk basis vectors
			auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
			defocus_disk_u = u * defocus_radius;
			defocus_disk_v = v * defocus_radius;
		}

		ray get_ray(int i, int j) const {
			// Camera ray from the defocus disk directed to a random sample around the pixel at i, j.
			auto offset = sample_square();
			auto pixel_sample = first_pixel_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

			auto ray_origin = (defocus_angle <= 0) ? camera_center : defocus_disk_sample();
			auto ray_direction = pixel_sample - ray_origin;
			auto ray_time = random_double();

			return ray(ray_origin, ray_direction, ray_time);
		}

		// Returns a vector to a random point in the .5 unit square.
		vec3d sample_square() const {
			return vec3d(random_double() - 0.5, random_double() - 0.5, 0);
		}

		color ray_color(const ray& r, int depth, const intersectable& world) const {
			// Limit bounces by depth recursion.
			if (depth <= 0) return color(0, 0, 0);

			intersects inte;

			if (!world.intersect(r, interval(0.001, infinity), inte)) return background;

			ray scattered;
			color attenuation;
			color color_from_emission = inte.mat->emitted(inte.u, inte.v, inte.p);

			if (!inte.mat->scatter(r, inte, attenuation, scattered)) return color_from_emission;

			color color_from_scatter = attenuation * ray_color(scattered, depth-1, world);

			return color_from_emission + color_from_scatter;
		}

		// Returns a random point in the camera defocus disk
		point3d defocus_disk_sample() const {
		auto p = random_in_unit_disk();
		return camera_center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
	}
};

