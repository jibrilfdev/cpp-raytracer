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

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3d(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3d center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3d(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.7) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.9) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double_range(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3d(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.2, 0.7, 0.7));
    world.add(make_shared<sphere>(point3d(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.7, 0.2), 0.0);
    world.add(make_shared<sphere>(point3d(4, 1, 0), 1.0, material3));


	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 1200;
	cam.random_samples_per_pixel = 500;
	cam.max_depth = 50;

	cam.vfov = 25;
	cam.look_from = point3d(-12, 3, -4);
	cam.look_at = point3d(0, 0, 0);
	cam.vup = vec3d(0, 1, 0);

	cam.defocus_angle = 0.6;
	cam.focus_dist = 10.0;

	cam.render(world);
}
