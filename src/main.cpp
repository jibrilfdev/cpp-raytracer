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
#include "include/triangle.h"
#include "include/trianglemesh.h"
#include "include/main.h"

#include <iostream>

void bouncing_spheres(int image_width = 1200, int random_samples_per_pixel = 150, int max_depth = 50, int threads = 2) {
	intersectable_list world;

	auto checker = make_shared<checker_texture>(0.32, color(0.1, 0, 0), color(1.0, 0.6, 0.8));
	world.add(make_shared<sphere>(point3d(0, -1000, 0), 1000, make_shared<lambertian>(checker)));


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

void checkered_spheres(int image_width = 1200, int random_samples_per_pixel = 150, int max_depth = 10, int threads = 2) {
	intersectable_list world;

	auto checker = make_shared<checker_texture>(0.32, color(0.1, 0, 0), color(1.0, 0.6, 0.8));

	world.add(make_shared<sphere>(point3d(0, -10, 0), 10, make_shared<lambertian>(checker)));
	world.add(make_shared<sphere>(point3d(0, 10, 0), 10, make_shared<lambertian>(checker)));

	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = image_width;
	cam.random_samples_per_pixel = random_samples_per_pixel;
	cam.max_depth = max_depth;
	cam.background = color(0.12, 0.12, 0.8);

	cam.vfov = 25;
	cam.look_from = point3d(-12, 3, -4);
	cam.look_at = point3d(0, 0, 0);
	cam.vup = vec3d(0, 1, 0);

	cam.defocus_angle = 0;

	cam.threads = threads;

	cam.render(world);
}

void earth(int image_width = 1200, int random_samples_per_pixel = 100, int max_depth = 5, int threads = 2) {
	auto earth_texture = make_shared<image_texture>("earth.jpg");
	auto earth_surface = make_shared<lambertian>(earth_texture);
	auto globe = make_shared<sphere>(point3d(0, 0, 0), 2, earth_surface);



	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = image_width;
	cam.random_samples_per_pixel = random_samples_per_pixel;
	cam.max_depth = max_depth;
	cam.threads = threads;
	cam.background = color(0.7, 0.8, 1);

	cam.vfov = 25;
	cam.look_from = point3d(0, 0, 12);
	cam.look_at = point3d(0, 0, 0);
	cam.vup = vec3d(0, 1, 0);

	cam.defocus_angle = 0;

	cam.render(intersectable_list(globe));
}

void perlin_spheres(int image_width = 1600, int random_samples_per_pixel = 100, int max_depth = 10, int threads = 2) {
	intersectable_list world;

	auto pertext = make_shared<noise_texture>(5);
	world.add(make_shared<sphere>(point3d(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
	world.add(make_shared<sphere>(point3d(0, 2, 0), 2, make_shared<lambertian>(pertext)));

	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = image_width;
	cam.random_samples_per_pixel = random_samples_per_pixel;
	cam.max_depth = max_depth;
	cam.threads = threads;
	cam.background = color(0.7, 0.8, 1);

	cam.vfov = 25;
	cam.look_from = point3d(13, 2, 3);
	cam.look_at = point3d(0, 0, 0);
	cam.vup = vec3d(0, 1, 0);

	cam.defocus_angle = 0;

	cam.render(world);
}

void quadrilaterals(int image_width = 800, int random_samples_per_pixel = 100, int max_depth = 50, int threads = 2) {
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
	cam.image_width = image_width;
	cam.random_samples_per_pixel = random_samples_per_pixel;
	cam.max_depth = max_depth;
	cam.threads = threads;
	cam.background = color(0.7, 0.8, 1);

	cam.vfov = 90;
	cam.look_from = point3d(0, 0, 9);
	cam.look_at = point3d(0, 0, 0);
	cam.vup = vec3d(0, 1, 0);

	cam.defocus_angle = 0;

	cam.render(world);
}

void basic_light(int image_width = 1600, int random_samples_per_pixel = 100, int max_depth = 25, int threads = 2) {
	intersectable_list world;

	auto pertext = make_shared<noise_texture>(3);
	world.add(make_shared<sphere>(point3d(0, -500, 0), 500, make_shared<lambertian>(pertext)));
	world.add(make_shared<sphere>(point3d(0, 2, 0), 2, make_shared<lambertian>(pertext)));

	auto diffuselight = make_shared<diffuse_light>(color(4, 4, 4));
	world.add(make_shared<quadrilateral>(point3d(3, 1, -2), vec3d(2, 0, 0), vec3d(0, 2, 0), diffuselight));

	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = image_width;
	cam.random_samples_per_pixel = random_samples_per_pixel;
	cam.max_depth = max_depth;
	cam.threads = threads;
	cam.background = color(0, 0, 0);

	cam.vfov = 25;
	cam.look_from = point3d(26, 3, 6);
	cam.look_at = point3d(0, 2, 0);
	cam.vup = vec3d(0, 1, 0);

	cam.defocus_angle = 0;

	cam.render(world);
}

void basic_light_sphere(int image_width = 1600, int random_samples_per_pixel = 100, int max_depth = 25, int threads = 2) {
	intersectable_list world;

	auto pertext = make_shared<noise_texture>(3);
	world.add(make_shared<sphere>(point3d(0, 1000, 0), 1000, make_shared<lambertian>(pertext)));
	world.add(make_shared<sphere>(point3d(0, 2, 0), 2, make_shared<lambertian>(pertext)));

	auto diffuselight = make_shared<diffuse_light>(color(4, 4, 4));
	world.add(make_shared<sphere>(point3d(0, 7, 0), 2, diffuselight));
	world.add(make_shared<quadrilateral>(point3d(3, 1, -2), vec3d(2, 0, 0), vec3d(0, 2, 0), diffuselight));

	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = image_width;
	cam.random_samples_per_pixel = random_samples_per_pixel;
	cam.max_depth = max_depth;
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

void cornell_box(int image_width = 800, int random_samples_per_pixel = 150, int max_depth = 10, int threads = 2) {
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
	cam.image_width = image_width;
	cam.random_samples_per_pixel = random_samples_per_pixel;
	cam.max_depth = max_depth;
	cam.threads = threads;
	cam.background = color(0,0,0);

	cam.vfov = 40;
	cam.look_from = point3d(278, 278, -800);
	cam.look_at = point3d(278, 278, 0);
	cam.vup = vec3d(0,1,0);

	cam.defocus_angle = 0;

	cam.render(world);
}

void cornell_box_smoke(int image_width = 800, int random_samples_per_pixel = 150, int max_depth = 10, int threads = 2) {
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
	cam.image_width = image_width;
	cam.random_samples_per_pixel = random_samples_per_pixel;
	cam.max_depth = max_depth;
	cam.threads = threads;
	cam.background = color(0,0,0);

	cam.vfov = 40;
	cam.look_from = point3d(278, 278, -800);
	cam.look_at = point3d(278, 278, 0);
	cam.vup = vec3d(0,1,0);

	cam.defocus_angle = 0;

	cam.render(world);
}


void everything_so_far_scene(int image_width = 800, int random_samples_per_pixel = 200, int max_depth = 10, int threads = 2) {
	intersectable_list world;

	auto ground = make_shared<lambertian>(color(0.58, 0.89, 0.84));

	intersectable_list boxes1;
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


	world.add(make_shared<bvh_node>(boxes1));

	auto light = make_shared<diffuse_light>(color(7, 7*.6, 7*.8));
	world.add(make_shared<quadrilateral>(point3d(123,594,147), vec3d(300,0,0), vec3d(0,0,265), light));

	auto center1 = point3d(400, 400, 200);
	auto center2 = center1 + vec3d(30,0,0);
	auto sphere_material = make_shared<lambertian>(color(0.70, 0.08, 0.08));
	world.add(make_shared<sphere>(center1, center2, 50, sphere_material));

	world.add(make_shared<sphere>(point3d(360, 150, 45), 50, make_shared<dielectric>(1.5)));
	world.add(make_shared<sphere>(point3d(0, 150, 145), 50, make_shared<metal>(color(0.45, 0.78, 0.92), 1.0)));

	auto boundary = make_shared<sphere>(point3d(360,150,145), 70, make_shared<dielectric>(1.5));
	world.add(boundary);
	world.add(make_shared<constant_medium>(boundary, 0.2, color(0.45, 0.78, 0.92)));
	boundary = make_shared<sphere>(point3d(0,0,0), 5000, make_shared<dielectric>(1.5));
	world.add(make_shared<constant_medium>(boundary, .0001, color(1,1,1)));

	auto emat = make_shared<lambertian>(make_shared<image_texture>("earth.jpg"));
	world.add(make_shared<sphere>(point3d(400,200,400), 100, emat));
	auto pertext = make_shared<noise_texture>(1);
	world.add(make_shared<sphere>(point3d(220,280,300), 80, make_shared<lambertian>(pertext)));

	intersectable_list spheres;
	auto spheres_color = make_shared<lambertian>(color(.98, .89, .69));
	int ns = 1000;
	for (int j = 0; j < ns; j++) {
		spheres.add(make_shared<sphere>(point3d::random(30,195), 10, spheres_color));
	}

	world.add(make_shared<translate>(
		make_shared<rotate_y>(
			make_shared<bvh_node>(spheres), 15),
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

	cam.vfov = 38;
	cam.look_from = point3d(-528, 278, -600);
	cam.look_at = point3d(-15, 278, 0);
	cam.vup = vec3d(0,1,0);

	cam.defocus_angle = 0;

	cam.render(world);
}


void stanford_dragon(int image_width = 400, int random_samples_per_pixel = 200, int max_depth = 40, int threads = 2) {
    intersectable_list world;


	std::cerr << "Loading assets." << std::endl;
    auto ground_material = make_shared<metal>(color(0.8, 0.8, 0.9), 1.0);
    world.add(make_shared<sphere>(point3d(0,-2580,0), 2500, ground_material));

    auto dragon_mat = make_shared<dielectric>(1.5);

    TriangleMesh dragon_mesh("source_images/dragon_recon/dragon_vrip.ply", dragon_mat);


    auto dragon_bvh = make_shared<bvh_node>(dragon_mesh.triangles, 0.0, dragon_mesh.triangles.size());
    world.add(dragon_bvh);


	auto ground_mat = make_shared<lambertian>(color(0.5, 0.5, 0.5));


    point3d mn(+infinity, +infinity, +infinity);
    point3d mx(-infinity, -infinity, -infinity);

    for (auto& tri : dragon_mesh.triangles) {
        aabb b = tri->bounding_box();
        mn = point3d(
            std::min(mn.x(), b.x.min),
            std::min(mn.y(), b.y.min),
            std::min(mn.z(), b.z.min)
        );
        mx = point3d(
            std::max(mx.x(), b.x.max),
            std::max(mx.y(), b.y.max),
            std::max(mx.z(), b.z.max)
        );
    }

    point3d center(
        0.5 * (mn.x() + mx.x()),
        0.5 * (mn.y() + mx.y()),
        0.5 * (mn.z() + mx.z())
    );

    vec3d diag = mx - mn;
    double radius = 0.5 * diag.length();



	world.add(make_shared<sphere>(center + vec3d(-2 * radius, 2 * radius, -2 * radius), .8 * radius, make_shared<diffuse_light>(color(7, 7, 7))));


    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = image_width;
    cam.random_samples_per_pixel = random_samples_per_pixel;
    cam.max_depth = max_depth;
    cam.threads = threads;
	cam.background = color(0.53, 0.81, 0.92);

    cam.vfov = 30;
    cam.look_at = center;
    cam.look_from = center - vec3d(0, -100, -2.5 * radius);

    cam.focus_dist = (cam.look_from - cam.look_at).length();
    cam.vup = vec3d(0, 1, 0);
    cam.defocus_angle = 0;

    cam.render(world);
}

void stanford_bunny(int image_width = 400, int random_samples_per_pixel = 100, int max_depth = 40, int threads = 2) {
    intersectable_list world;

	std::cerr << "Loading assets." << std::endl;
	auto bunnymat = make_shared<diffuse_light>(color(4.0, 4.0, 4.0));
	TriangleMesh bunny_mesh("source_images/dragon_recon/dragon_vrip_res4.ply", bunnymat);


	auto ground_mat = make_shared<lambertian>(color(0.5, 0.5, 0.5));

    auto bunny_bvh = make_shared<bvh_node>(bunny_mesh.triangles, 0, bunny_mesh.triangles.size());
    world.add(bunny_bvh);



    point3d mn(+infinity, +infinity, +infinity);
    point3d mx(-infinity, -infinity, -infinity);

    for (auto& tri : bunny_mesh.triangles) {
        aabb b = tri->bounding_box();
        mn = point3d(
            std::min(mn.x(), b.x.min),
            std::min(mn.y(), b.y.min),
            std::min(mn.z(), b.z.min)
        );
        mx = point3d(
            std::max(mx.x(), b.x.max),
            std::max(mx.y(), b.y.max),
            std::max(mx.z(), b.z.max)
        );
    }

    point3d center(
        0.5 * (mn.x() + mx.x()),
        0.5 * (mn.y() + mx.y()),
        0.5 * (mn.z() + mx.z())
    );

    vec3d diag = mx - mn;
    double radius = 0.5 * diag.length();


    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = image_width;
    cam.random_samples_per_pixel = random_samples_per_pixel;
    cam.max_depth = max_depth;
    cam.threads = threads;
	cam.background = color(0.53, 0.81, 0.92);

    cam.vfov = 30;
    cam.look_at = center;
    cam.look_from = center + vec3d(0, 0.25 * radius, -2.5 * radius);

    cam.focus_dist = (cam.look_from - cam.look_at).length();
    cam.vup = vec3d(0, 1, 0);
    cam.defocus_angle = 0;

    cam.render(world);
}

void cornell_stanford_box(int image_width = 800, int random_samples_per_pixel = 750, int max_depth = 10, int threads = 2) {
    intersectable_list world;

	// Cornell Box structure
    auto red   = make_shared<lambertian>(color(0.65, 0.05, 0.05));
    auto white = make_shared<lambertian>(color(0.73, 0.73, 0.73));
    auto green = make_shared<lambertian>(color(0.12, 0.45, 0.15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    world.add(make_shared<quadrilateral>(point3d(555,0,0), vec3d(0,555,0), vec3d(0,0,555), red));
    world.add(make_shared<quadrilateral>(point3d(0,0,0),   vec3d(0,555,0), vec3d(0,0,555), green));
    world.add(make_shared<quadrilateral>(point3d(343, 554, 332), vec3d(-130,0,0), vec3d(0,0,-105), light));
    world.add(make_shared<quadrilateral>(point3d(0,0,0),   vec3d(555,0,0), vec3d(0,0,555), white));
    world.add(make_shared<quadrilateral>(point3d(555,555,555), vec3d(-555,0,0), vec3d(0,0,-555), white));
    world.add(make_shared<quadrilateral>(point3d(0,0,555), vec3d(555,0,0), vec3d(0,555,0), white));


	std::cerr << "Loading assets." << std::endl;
    auto gold = make_shared<metal>(color(1.0, 0.85, 0.30), 0.02);
    TriangleMesh dragon_mesh("source_images/dragon_recon/dragon_vrip.ply", gold);
    shared_ptr<intersectable> dragon = make_shared<bvh_node>(dragon_mesh.triangles, 0, dragon_mesh.triangles.size());


	auto bunnymat = make_shared<lambertian>(color(1, .6, .8));
	TriangleMesh bunny_mesh("source_images/bunny/reconstruction/bun_zipper.ply", bunnymat);
	shared_ptr<intersectable> bunny = make_shared<bvh_node>(bunny_mesh.triangles, 0, bunny_mesh.triangles.size());


    dragon = make_shared<rotate_y>(dragon, -140);
    dragon = make_shared<translate>(dragon, vec3d(265-125, -60, 295-110));

	bunny = make_shared<rotate_y>(bunny, -80);
	bunny = make_shared<translate>(bunny, vec3d(265+125, -75, 295+70));


    world.add(dragon);
	world.add(bunny);


    camera cam;
    cam.aspect_ratio = 1.0;
    cam.image_width = image_width;
    cam.random_samples_per_pixel = random_samples_per_pixel;
    cam.max_depth = max_depth;
    cam.threads = threads;
    cam.background = color(0,0,0);

    cam.vfov = 40;
    cam.look_from = point3d(278, 278, -800);
    cam.look_at   = point3d(278, 278, 0);
    cam.vup = vec3d(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void guitar(int image_width = 400, int random_samples_per_pixel = 200, int max_depth = 30, int threads = 2) {
    intersectable_list world;

	std::cerr << "Loading assets." << std::endl;
    auto guitarmat = make_shared<lambertian>(color(0.6, 0.0, 0.8));
    TriangleMesh guitar_mesh("source_images/guitar/guitartilted.ply", guitarmat);

    auto guitar_bvh = make_shared<bvh_node>(guitar_mesh.triangles, 0, guitar_mesh.triangles.size());
    world.add(guitar_bvh);


    point3d mn(+infinity, +infinity, +infinity);
    point3d mx(-infinity, -infinity, -infinity);

    for (auto& tri : guitar_mesh.triangles) {
        aabb b = tri->bounding_box();
        mn = point3d(std::min(mn.x(), b.x.min),
                     std::min(mn.y(), b.y.min),
                     std::min(mn.z(), b.z.min));
        mx = point3d(std::max(mx.x(), b.x.max),
                     std::max(mx.y(), b.y.max),
                     std::max(mx.z(), b.z.max));
    }

    point3d center(0.5*(mn.x()+mx.x()), 0.5*(mn.y()+mx.y()), 0.5*(mn.z()+mx.z()));
    double radius = 0.5 * (mx - mn).length();


    auto ground_mat   = make_shared<lambertian>(color(0.0, 0.8, 0.2));

    double ground_R = 200.0 * radius;
    world.add(make_shared<sphere>( point3d(center.x(), mn.y() - ground_R + 0.02*radius, center.z()), ground_R, ground_mat));



	// Light source inside guitar
	auto inside_light_mat = make_shared<diffuse_light>(color(5, 5, 5)); // try 4..20
	double bulb_r = 0.05 * radius;                                      // size of bulb

	point3d bulb_pos = center + vec3d(-385, -0.50 * radius, 0.00 * radius);
	world.add(make_shared<sphere>(bulb_pos, bulb_r, inside_light_mat));



    camera cam;
    cam.aspect_ratio = (1==0) ? 9.0/16.0 : 1.0; // keep your portrait framing
    cam.image_width = image_width;
    cam.random_samples_per_pixel = random_samples_per_pixel;
    cam.max_depth = max_depth;
    cam.threads = threads;

    cam.background = color(0.2, 0.9, 0.9);

    cam.vfov = 30;
    cam.look_at = center;

    cam.look_from = point3d(center.x(), center.y() + 0.5*radius, center.z() - 4.0*radius);

    cam.focus_dist = (cam.look_from - cam.look_at).length();
    cam.vup = vec3d(0, 1, 0);
    cam.defocus_angle = 0;

    cam.render(world);
}


int main(int argc, char* argv[]) {
	if (argc<2) {
		std::cerr << "Didn't pass required argument. Valid arguments are 0 (bouncing spheres), 1 (checkered spheres), 2 (earth), 3 (perlin noise spheres), 4 (quadrilaterals), 5 (basic light), 6 (empty cornell box), 7 (cornell box), 8 (cornell box smoke), 9 (\"everything so far scene\"), 10 (stanford dragon), 11 (stanford bunny), 12 (cornell-stanford box), 13 (guitar)." << std::endl;
		return 0;
	}
	std::string argument = argv[1];
	switch (std::stoi(argument)) {
		case 0:
			bouncing_spheres();
			break;
		case 1:
			checkered_spheres();
			break;
		case 2:
			earth();
			break;
		case 3:
			perlin_spheres();
			break;
		case 4:
			quadrilaterals();
			break;
		case 5:
			basic_light();
			break;
		case 6:
			empty_cornell_box();
			break;
		case 7:
			cornell_box();
			break;
		case 8:
			cornell_box_smoke();
			break;
		case 9:
			everything_so_far_scene();
			break;
		case 10:
			stanford_dragon();
			break;
		case 11:
			stanford_bunny();
			break;
		case 12:
			cornell_stanford_box();
			break;
		case 13:
			guitar();
			break;
		default:
			std::cerr << "Unrecognized argument " + argument + " passed." << std::endl;
			break;
	}
}


