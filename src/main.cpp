#include "include/color.h"
#include "include/3dvec.h"
#include "include/ray.h"
#include "include/sphere.h"
#include "include/intersectable.h"
#include "include/intersectable_objects.h"
#include "include/constants.h"
#include "include/bvh.h"
#include "include/camera.h"
#include "include/constant_medium.h"
#include "include/material.h"
#include "include/quadrilateral.h"
#include "include/main.h"

#include <iostream>

void bouncing_spheres(int threads = 2) {
	intersectable_list world;

	auto checker = make_shared<checker_texture>(0.32, color(0.1, 0, 0), color(1.0, 0.6, 0.8));
	world.add(make_shared<sphere>(point3d(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

//    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
//    world.add(make_shared<sphere>(point3d(0,-1000,0), 1000, ground_material));

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
					auto center2 = center + vec3d(0, random_double_range(0,.5), 0);
                    world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
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

	world = intersectable_list(make_shared<bvh_node>(world));


	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 1200;
	cam.random_samples_per_pixel = 100;
	cam.max_depth = 50;
	cam.background = color(0.12, 0.12, 0.8);

	cam.vfov = 25;
	cam.look_from = point3d(-12, 3, -4);
	cam.look_at = point3d(0, 0, 0);
	cam.vup = vec3d(0, 1, 0);

	cam.defocus_angle = 0.6;
	cam.focus_dist = 10.0;

	cam.threads = threads;

	cam.render(world);
}

void checkered_spheres(int threads = 2) {
	intersectable_list world;

	auto checker = make_shared<checker_texture>(0.32, color(0.1, 0, 0), color(1.0, 0.6, 0.8));

	world.add(make_shared<sphere>(point3d(0, -10, 0), 10, make_shared<lambertian>(checker)));
	world.add(make_shared<sphere>(point3d(0, 10, 0), 10, make_shared<lambertian>(checker)));

	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 1200;
	cam.random_samples_per_pixel = 100;
	cam.max_depth = 50;
	cam.background = color(0.12, 0.12, 0.8);

	cam.vfov = 25;
	cam.look_from = point3d(-12, 3, -4);
	cam.look_at = point3d(0, 0, 0);
	cam.vup = vec3d(0, 1, 0);

	cam.defocus_angle = 0;

	cam.threads = threads;

	cam.render(world);
}

void earth(int threads = 2) {
	auto earth_texture = make_shared<image_texture>("earth.jpg");
	auto earth_surface = make_shared<lambertian>(earth_texture);
	auto globe = make_shared<sphere>(point3d(0, 0, 0), 2, earth_surface);



	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 1200;
	cam.random_samples_per_pixel = 100;
	cam.max_depth = 50;
	cam.threads = threads;
	cam.background = color(0.7, 0.8, 1);

	cam.vfov = 25;
	cam.look_from = point3d(0, 0, 12);
	cam.look_at = point3d(0, 0, 0);
	cam.vup = vec3d(0, 1, 0);

	cam.defocus_angle = 0;

	cam.render(intersectable_list(globe));
}

void perlin_spheres(int threads = 2) {
	intersectable_list world;

	auto pertext = make_shared<noise_texture>(5);
	world.add(make_shared<sphere>(point3d(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
	world.add(make_shared<sphere>(point3d(0, 2, 0), 2, make_shared<lambertian>(pertext)));

	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 1600;
	cam.random_samples_per_pixel = 100;
	cam.max_depth = 50;
	cam.threads = threads;
	cam.background = color(0.7, 0.8, 1);

	cam.vfov = 25;
	cam.look_from = point3d(13, 2, 3);
	cam.look_at = point3d(0, 0, 0);
	cam.vup = vec3d(0, 1, 0);

	cam.defocus_angle = 0;

	cam.render(world);
}

void quadrilaterals(int threads = 2) {
	intersectable_list world;

	// Materials
	auto left_blue = make_shared<lambertian>(color(0.2, 0.2, 1.0));
	auto right_red = make_shared<lambertian>(color(1.0, 0.2, 0.2));
	auto back_green = make_shared<lambertian>(color(0.2, 1.0, 0.2));
	auto upper_pink = make_shared<lambertian>(color(1.0, 0.6, 0.8));
	auto lower_teal = make_shared<lambertian>(color(0.2, 0.8, 0.8));

	world.add(make_shared<quadrilateral>(point3d(-3, -2, 5), vec3d(0, 0, -4), vec3d(0, 4, 0), left_blue));
	world.add(make_shared<quadrilateral>(point3d(3, -2, 1), vec3d(0, 0, 4), vec3d(0, 4, 0), right_red));
	world.add(make_shared<quadrilateral>(point3d(-2, -2, 0), vec3d(4, 0, 0), vec3d(0, 4, 0), back_green));
	world.add(make_shared<quadrilateral>(point3d(-2, 3, 1), vec3d(4, 0, 0), vec3d(0, 0, 4), upper_pink));
	world.add(make_shared<quadrilateral>(point3d(-2, -3, 5), vec3d(4, 0, 0), vec3d(0, 0, -4), lower_teal));

	camera cam;

	cam.aspect_ratio = 1.0;
	cam.image_width = 800;
	cam.random_samples_per_pixel = 100;
	cam.max_depth = 50;
	cam.threads = threads;
	cam.background = color(0.7, 0.8, 1);

	cam.vfov = 90;
	cam.look_from = point3d(0, 0, 9);
	cam.look_at = point3d(0, 0, 0);
	cam.vup = vec3d(0, 1, 0);

	cam.defocus_angle = 0;

	cam.render(world);
}

void basic_light(int threads = 2) {
	intersectable_list world;

	auto pertext = make_shared<noise_texture>(3);
	world.add(make_shared<sphere>(point3d(0, -500, 0), 500, make_shared<lambertian>(pertext)));
	world.add(make_shared<sphere>(point3d(0, 2, 0), 2, make_shared<lambertian>(pertext)));

	auto diffuselight = make_shared<diffuse_light>(color(4, 4, 4));
	world.add(make_shared<quadrilateral>(point3d(3, 1, -2), vec3d(2, 0, 0), vec3d(0, 2, 0), diffuselight));

	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 1600;
	cam.random_samples_per_pixel = 100;
	cam.max_depth = 50;
	cam.threads = threads;
	cam.background = color(0, 0, 0);

	cam.vfov = 25;
	cam.look_from = point3d(26, 3, 6);
	cam.look_at = point3d(0, 2, 0);
	cam.vup = vec3d(0, 1, 0);

	cam.defocus_angle = 0;

	cam.render(world);
}

void basic_light_sphere(int threads = 2) {
	intersectable_list world;

	auto pertext = make_shared<noise_texture>(3);
	world.add(make_shared<sphere>(point3d(0, 1000, 0), 1000, make_shared<lambertian>(pertext)));
	world.add(make_shared<sphere>(point3d(0, 2, 0), 2, make_shared<lambertian>(pertext)));

	auto diffuselight = make_shared<diffuse_light>(color(4, 4, 4));
	world.add(make_shared<sphere>(point3d(0, 7, 0), 2, diffuselight));
	world.add(make_shared<quadrilateral>(point3d(3, 1, -2), vec3d(2, 0, 0), vec3d(0, 2, 0), diffuselight));

	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 1600;
	cam.random_samples_per_pixel = 100;
	cam.max_depth = 50;
	cam.threads = threads;
	cam.background = color(0, 0, 0);

	cam.vfov = 25;
	cam.look_from = point3d(26, 3, 6);
	cam.look_at = point3d(0, 2, 0);
	cam.vup = vec3d(0, 1, 0);

	cam.defocus_angle = 0;

	cam.render(world);
}

void empty_cornell_box(int threads = 2) {
	intersectable_list world;

	auto red = make_shared<lambertian>(color(0.65, 0.05, 0.05));
	auto white = make_shared<lambertian>(color(0.73, 0.73, 0.73));
	auto green = make_shared<lambertian>(color(0.12, 0.45, 0.15));
	auto light = make_shared<diffuse_light>(color(15, 15, 15));

	world.add(make_shared<quadrilateral>(point3d(555,0,0), vec3d(0,555,0), vec3d(0,0,555), green));
	world.add(make_shared<quadrilateral>(point3d(0,0,0), vec3d(0,555,0), vec3d(0,0,555), red));
	world.add(make_shared<quadrilateral>(point3d(343, 554, 332), vec3d(-130,0,0), vec3d(0,0,-105), light));
	world.add(make_shared<quadrilateral>(point3d(0,0,0), vec3d(555,0,0), vec3d(0,0,555), white));
	world.add(make_shared<quadrilateral>(point3d(555,555,555), vec3d(-555,0,0), vec3d(0,0,-555), white));
	world.add(make_shared<quadrilateral>(point3d(0,0,555), vec3d(555,0,0), vec3d(0,555,0), white));

	camera cam;

	cam.aspect_ratio = 1.0;
	cam.image_width = 600;
	cam.random_samples_per_pixel = 200;
	cam.max_depth = 50;
	cam.threads = threads;
	cam.background = color(0,0,0);

	cam.vfov = 40;
	cam.look_from = point3d(278, 278, -800);
	cam.look_at = point3d(278, 278, 0);
	cam.vup = vec3d(0,1,0);

	cam.defocus_angle = 0;

	cam.render(world);
}

void cornell_box(int threads = 2) {
	intersectable_list world;

	auto red = make_shared<lambertian>(color(0.65, 0.05, 0.05));
	auto white = make_shared<lambertian>(color(0.73, 0.73, 0.73));
	auto green = make_shared<lambertian>(color(0.12, 0.45, 0.15));
	auto light = make_shared<diffuse_light>(color(15, 15, 15));

	world.add(make_shared<quadrilateral>(point3d(555,0,0), vec3d(0,555,0), vec3d(0,0,555), green));
	world.add(make_shared<quadrilateral>(point3d(0,0,0), vec3d(0,555,0), vec3d(0,0,555), red));
	world.add(make_shared<quadrilateral>(point3d(343, 554, 332), vec3d(-130,0,0), vec3d(0,0,-105), light));
	world.add(make_shared<quadrilateral>(point3d(0,0,0), vec3d(555,0,0), vec3d(0,0,555), white));
	world.add(make_shared<quadrilateral>(point3d(555,555,555), vec3d(-555,0,0), vec3d(0,0,-555), white));
	world.add(make_shared<quadrilateral>(point3d(0,0,555), vec3d(555,0,0), vec3d(0,555,0), white));

	shared_ptr<intersectable> box1 = box(point3d(0, 0, 0), point3d(165, 330, 165), white);
	box1 = make_shared<rotate_y>(box1, 15);
	box1 = make_shared<translate>(box1, vec3d(265, 0, 295));
	world.add(box1);

	shared_ptr<intersectable> box2 = box(point3d(0, 0, 0), point3d(165, 165, 165), white);
	box2 = make_shared<rotate_y>(box2, -18);
	box2 = make_shared<translate>(box2, vec3d(130, 0, 65));
	world.add(box2);

	camera cam;

	cam.aspect_ratio = 1.0;
	cam.image_width = 800;
	cam.random_samples_per_pixel = 200;
	cam.max_depth = 50;
	cam.threads = threads;
	cam.background = color(0,0,0);

	cam.vfov = 40;
	cam.look_from = point3d(278, 278, -800);
	cam.look_at = point3d(278, 278, 0);
	cam.vup = vec3d(0,1,0);

	cam.defocus_angle = 0;

	cam.render(world);
}

void cornell_box_smoke(int threads = 2) {
	intersectable_list world;

	auto red   = make_shared<lambertian>(color(.65, .05, .05));
	auto white = make_shared<lambertian>(color(.73, .73, .73));
	auto green = make_shared<lambertian>(color(.12, .45, .15));
	auto light = make_shared<diffuse_light>(color(7, 7, 7));

	world.add(make_shared<quadrilateral>(point3d(555,0,0), vec3d(0,555,0), vec3d(0,0,555), green));
	world.add(make_shared<quadrilateral>(point3d(0,0,0), vec3d(0,555,0), vec3d(0,0,555), red));
	world.add(make_shared<quadrilateral>(point3d(113,554,127), vec3d(330,0,0), vec3d(0,0,305), light));
	world.add(make_shared<quadrilateral>(point3d(0,555,0), vec3d(555,0,0), vec3d(0,0,555), white));
	world.add(make_shared<quadrilateral>(point3d(0,0,0), vec3d(555,0,0), vec3d(0,0,555), white));
	world.add(make_shared<quadrilateral>(point3d(0,0,555), vec3d(555,0,0), vec3d(0,555,0), white));

	shared_ptr<intersectable> box1 = box(point3d(0,0,0), point3d(165,330,165), white);
	box1 = make_shared<rotate_y>(box1, 15);
	box1 = make_shared<translate>(box1, vec3d(265,0,295));

	shared_ptr<intersectable> box2 = box(point3d(0,0,0), point3d(165,165,165), white);
	box2 = make_shared<rotate_y>(box2, -18);
	box2 = make_shared<translate>(box2, vec3d(130,0,65));

	world.add(make_shared<constant_medium>(box1, 0.01, color(0,0,0)));
	world.add(make_shared<constant_medium>(box2, 0.01, color(1,1,1)));

	camera cam;

	cam.aspect_ratio = 1.0;
	cam.image_width = 800;
	cam.random_samples_per_pixel = 200;
	cam.max_depth = 50;
	cam.threads = threads;
	cam.background = color(0,0,0);

	cam.vfov = 40;
	cam.look_from = point3d(278, 278, -800);
	cam.look_at = point3d(278, 278, 0);
	cam.vup = vec3d(0,1,0);

	cam.defocus_angle = 0;

	cam.render(world);
}


void final_scene(int image_width, int random_samples_per_pixel, int max_depth, int threads = 2) {
	intersectable_list boxes1;
	auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

	int boxes_per_side = 20;
	for (int i = 0; i < boxes_per_side; i++) {
		for (int j = 0; j < boxes_per_side; j++) {
			auto w = 100.0;
			auto x0 = -1000.0 + i*w;
			auto z0 = -1000.0 + j*w;
			auto y0 = 0.0;
			auto x1 = x0 + w;
			auto y1 = random_double_range(1,101);
			auto z1 = z0 + w;

			boxes1.add(box(point3d(x0,y0,z0), point3d(x1,y1,z1), ground));
		}
	}

	intersectable_list world;

	world.add(make_shared<bvh_node>(boxes1));

	auto light = make_shared<diffuse_light>(color(7, 7*.6, 7*.8));
	world.add(make_shared<quadrilateral>(point3d(123,554,147), vec3d(300,0,0), vec3d(0,0,265), light));

	auto center1 = point3d(400, 400, 200);
	auto center2 = center1 + vec3d(30,0,0);
	auto sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
	world.add(make_shared<sphere>(center1, center2, 50, sphere_material));

	world.add(make_shared<sphere>(point3d(260, 150, 45), 50, make_shared<dielectric>(1.5)));
	world.add(make_shared<sphere>(point3d(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)));

	auto boundary = make_shared<sphere>(point3d(360,150,145), 70, make_shared<dielectric>(1.5));
	world.add(boundary);
	world.add(make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
	boundary = make_shared<sphere>(point3d(0,0,0), 5000, make_shared<dielectric>(1.5));
	world.add(make_shared<constant_medium>(boundary, .0001, color(1,1,1)));

	auto emat = make_shared<lambertian>(make_shared<image_texture>("earth.jpg"));
	world.add(make_shared<sphere>(point3d(400,200,400), 100, emat));
	auto pertext = make_shared<noise_texture>(0.2);
	world.add(make_shared<sphere>(point3d(220,280,300), 80, make_shared<lambertian>(pertext)));

	intersectable_list boxes2;
	auto white = make_shared<lambertian>(color(.73, .73, .73));
	int ns = 1000;
	for (int j = 0; j < ns; j++) {
		boxes2.add(make_shared<sphere>(point3d::random(0,165), 10, white));
	}

	world.add(make_shared<translate>(
		make_shared<rotate_y>(
			make_shared<bvh_node>(boxes2), 15),
			vec3d(-100,270,395)
		)
	);

	camera cam;

	cam.aspect_ratio = 1.0;
	cam.image_width = image_width;
	cam.random_samples_per_pixel = random_samples_per_pixel;
	cam.max_depth = max_depth;
	cam.threads = threads;
	cam.background = color(0,0,0);

	cam.vfov = 35;
	cam.look_from = point3d(-478, 278, -600);
	cam.look_at = point3d(0, 278, 0);
	cam.vup = vec3d(0,1,0);

	cam.defocus_angle = 0;

	cam.render(world);
}

void final_scene_main(int image_width, int random_samples_per_pixel, int max_depth, int threads = 2) {
	intersectable_list boxes1;
	auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

	int boxes_per_side = 20;
	for (int i = 0; i < boxes_per_side; i++) {
		for (int j = 0; j < boxes_per_side; j++) {
			auto w = 100.0;
			auto x0 = -1000.0 + i*w;
			auto z0 = -1000.0 + j*w;
			auto y0 = 0.0;
			auto x1 = x0 + w;
			auto y1 = random_double_range(1,101);
			auto z1 = z0 + w;

			boxes1.add(box(point3d(x0,y0,z0), point3d(x1,y1,z1), ground));
		}
	}

	intersectable_list world;

	world.add(make_shared<bvh_node>(boxes1));

	auto light = make_shared<diffuse_light>(color(7, 7, 7));
	world.add(make_shared<quadrilateral>(point3d(123,554,147), vec3d(300,0,0), vec3d(0,0,265), light));

	auto center1 = point3d(400, 400, 200);
	auto center2 = center1 + vec3d(30,0,0);
	auto sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
	world.add(make_shared<sphere>(center1, center2, 50, sphere_material));

	world.add(make_shared<sphere>(point3d(260, 150, 45), 50, make_shared<dielectric>(1.5)));
	world.add(make_shared<sphere>(point3d(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)));

	auto boundary = make_shared<sphere>(point3d(360,150,145), 70, make_shared<dielectric>(1.5));
	world.add(boundary);
	world.add(make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
	boundary = make_shared<sphere>(point3d(0,0,0), 5000, make_shared<dielectric>(1.5));
	world.add(make_shared<constant_medium>(boundary, .0001, color(1,1,1)));

	auto emat = make_shared<lambertian>(make_shared<image_texture>("earth.jpg"));
	world.add(make_shared<sphere>(point3d(400,200,400), 100, emat));
	auto pertext = make_shared<noise_texture>(0.2);
	world.add(make_shared<sphere>(point3d(220,280,300), 80, make_shared<lambertian>(pertext)));

	intersectable_list boxes2;
	auto white = make_shared<lambertian>(color(.73, .73, .73));
	int ns = 1000;
	for (int j = 0; j < ns; j++) {
		boxes2.add(make_shared<sphere>(point3d::random(0,165), 10, white));
	}

	world.add(make_shared<translate>(
		make_shared<rotate_y>(
			make_shared<bvh_node>(boxes2), 15),
			vec3d(-100,270,395)
		)
	);

	camera cam;

	cam.aspect_ratio = 1.0;
	cam.image_width = image_width;
	cam.random_samples_per_pixel = random_samples_per_pixel;
	cam.max_depth = max_depth;
	cam.threads = threads;
	cam.background = color(0,0,0);

	cam.vfov = 40;
	cam.look_from = point3d(478, 278, -600);
	cam.look_at = point3d(278, 278, 0);
	cam.vup = vec3d(0,1,0);

	cam.defocus_angle = 0;

	cam.render(world);
}


int main() {
	final_scene(800, 10, 40);
}
