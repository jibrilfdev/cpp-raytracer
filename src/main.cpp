#include "include/color.h"
#include "include/3dvec.h"
#include "include/ray.h"
#include "include/sphere.h"
#include "include/intersectable.h"
#include "include/intersectable_objects.h"
#include "include/constants.h"
#include "include/camera.h"
#include "include/material.h"
#include "include/main.h"

#include <iostream>

int main() {
	intersectable_list world;

	auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
	auto material_center = make_shared<lambertian>(color(1.0, 0.5, 0.7));
	auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
	auto material_right = make_shared<metal>(color(0.98, 0.89, 0.69), 1.0);

	world.add(make_shared<sphere>(point3d(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(make_shared<sphere>(point3d(0.0, 0.0, -1.2), 0.5, material_center));
	world.add(make_shared<sphere>(point3d(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(make_shared<sphere>(point3d(1.0, 0.0, -1.0), 0.5, material_right));

	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 600;
	cam.random_samples_per_pixel = 100;
	cam.max_depth = 50;

	cam.render(world);
}
