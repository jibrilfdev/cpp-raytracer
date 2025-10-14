#pragma once

#include "3dvec.h"
#include "intersectable.h"

class sphere : public intersectable {
	public:
		sphere(const point3d& static_center, double radius, shared_ptr<material> mat) : center(static_center, vec3d(0,0,0)), radius(std::fmax(0,radius)), mat(mat) {
			auto rvec = vec3d(radius, radius, radius);
			bbox = aabb(static_center - rvec, static_center + rvec);
		}

		// Sphere in motion
		sphere(const point3d& center1, const point3d& center2, double radius, shared_ptr<material> mat) : center(center1, center2 - center1), radius(std::fmax(0,radius)), mat(mat) {
			auto rvec = vec3d(radius, radius, radius);
			aabb box1(center.at(0) - rvec, center.at(0) + rvec);
			aabb box2(center.at(1) - rvec, center.at(1) + rvec);
			bbox = aabb(box1, box2);
		}

		aabb bounding_box() const override { return bbox; }

		bool intersect(const ray& r, interval ray_t, intersects& inte) const override {
		point3d current_center = center.at(r.time());
		vec3d oc = current_center - r.origin();
		double a = r.direction().length_squared();
		double h = dot(r.direction(), oc);
		double c = oc.length_squared() - radius*radius;

		auto discriminant = h*h - a*c;
		if (discriminant < 0) return false;

		auto discriminant_sqrt = std::sqrt(discriminant);

		auto root = (h - discriminant_sqrt) / a;
		if (!ray_t.surrounds(root)) {
			root = (h + discriminant_sqrt) / a;
			if (!ray_t.surrounds(root)) return false;
		}

		inte.t = root;
		inte.p = r.at(inte.t);
		vec3d outward_normal = (inte.p - current_center) / radius;
		inte.set_face_normal(r, outward_normal);
		get_sphere_uv(outward_normal, inte.u, inte.v);
		inte.mat = mat;

		return true;
		}

private:
	ray center;
	double radius;
	shared_ptr<material> mat;
	aabb bbox;

	static void get_sphere_uv(const point3d& p, double& u, double& v) {
		auto theta = std::acos(-p.y());
		auto phi = std::atan2(-p.z(), p.x()) + pi;

		u = phi / (2*pi);
		v = theta / pi;
	}
};

