#include "include/color.h"
#include "include/3dvec.h"
#include "include/ray.h"
#include "include/sphere.h"
#include "include/intersectable.h"
#include "include/intersectable_objects.h"
#include "include/constants.h"
#include "include/camera.h"
#include "include/main.h"

#include <iostream>

int main() {
	intersectable_list world;

	world.add(make_shared<sphere>(point3d(0,0,-1), 0.5));
	world.add(make_shared<sphere>(point3d(0,-100.5,-1), 100));

	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 600;
	cam.random_samples_per_pixel = 100;

	cam.render(world);
}
