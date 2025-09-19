#ifndef SPHERE_H
#define SPHERE_H

#include "3dvec.h"
#include "intersectable.h"

class sphere : public intersectable {
	public:
		sphere(const point3d& center, double radius) : center(center), radius(std::fmax(0,radius)) {}

		bool intersect(const ray& r, double ray_tmin, double ray_tmax, intersects& inte) const override {
		vec3d oc = center - r.origin();
		double a = r.direction().length_squared();
		double h = dot(r.direction(), oc);
		double c = oc.length_squared() - radius*radius;

		auto discriminant = h*h - a*c;
		if (discriminant < 0) return false;

		auto discriminant_sqrt = std::sqrt(discriminant);

		auto root = (h - discriminant_sqrt) / a;
		if (root <= ray_tmin || ray_tmax <= root) {
			root = (h + discriminant_sqrt) / a;
			if (root <= ray_tmin || ray_tmax <= root) return false;
		}

		inte.t = root;
		inte.p = r.at(inte.t);
		vec3d outward_normal = (inte.p - center) / radius;
		inte.set_face_normal(r, outward_normal);

		return true;
		}

private:
	point3d center;
	double radius;
};

#endif
