#pragma once

#include "intersectable.h"
#include "material.h"
#include "texture.h"

class constant_medium : public intersectable {
	public:
		constant_medium(shared_ptr<intersectable> boundary, double density, shared_ptr<texture> tex)
		  : boundary(boundary), neg_inv_density(-1/density),
			phase_function(make_shared<isotropic>(tex))
		{}

		constant_medium(shared_ptr<intersectable> boundary, double density, const color& albedo)
		  : boundary(boundary), neg_inv_density(-1/density),
			phase_function(make_shared<isotropic>(albedo))
		{}

		bool intersect(const ray& r, interval ray_t, intersects& inte) const override {
			intersects inte1, inte2;

			if (!boundary->intersect(r, interval::universe, inte1)) return false;

			if (!boundary->intersect(r, interval(inte1.t+0.0001, infinity), inte2)) return false;

			if (inte1.t < ray_t.min) inte1.t = ray_t.min;
			if (inte2.t > ray_t.max) inte2.t = ray_t.max;

			if (inte1.t >= inte2.t) return false;

			if (inte1.t < 0) inte1.t = 0;

			auto ray_length = r.direction().length();
			auto distance_inside_boundary = (inte2.t - inte1.t) * ray_length;
			auto intersect_distance = neg_inv_density * std::log(random_double());

			if (intersect_distance > distance_inside_boundary) return false;

			inte.t = inte1.t + intersect_distance / ray_length;
			inte.p = r.at(inte.t);

			inte.normal = vec3d(1,0,0);
			inte.front_face = true;
			inte.mat = phase_function;

			return true;
		}

		aabb bounding_box() const override { return boundary->bounding_box(); }

	  private:
		shared_ptr<intersectable> boundary;
		double neg_inv_density;
		shared_ptr<material> phase_function;
};
