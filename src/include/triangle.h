#pragma once

#include "3dvec.h"
#include "intersectable.h"

class triangle : public intersectable {
public:
	triangle(const point3d& a, const point3d& b, const point3d& c, shared_ptr<material> mat) : v0(a), v1(b), v2(c), mat(mat) {
		// Compute bounding box
		point3d min_point( std::fmin(v0.x(), std::fmin(v1.x(), v2.x())), std::fmin(v0.y(), std::fmin(v1.y(), v2.y())), std::fmin(v0.z(), std::fmin(v1.z(), v2.z())) );

		point3d max_point( std::fmax(v0.x(), std::fmax(v1.x(), v2.x())), std::fmax(v0.y(), std::fmax(v1.y(), v2.y())), std::fmax(v0.z(), std::fmax(v1.z(), v2.z())) );

		bbox = aabb(min_point, max_point);


		edge1 = v1 - v0;
		edge2 = v2 - v0;

		// Compute normal
		normal = unit_vector(cross(edge1, edge2));
	}

	aabb bounding_box() const override { return bbox; }

	bool intersect(const ray& r, interval ray_t, intersects& inte) const override {
		vec3d h = cross(r.direction(), edge2);
		double a = dot(edge1, h);
		if (fabs(a) < 1e-8) return false; // Ray parallel to triangle check

		double f = 1.0 / a;
		vec3d s = r.origin() - v0;
		double u = f * dot(s, h);
		if (u < 0.0 || u > 1.0) return false;

		vec3d q = cross(s, edge1);
		double v = f * dot(r.direction(), q);
		if (v < 0.0 || u + v > 1.0) return false;

		double t = f * dot(edge2, q);
		//if (!ray_t.surrounds(t)) return false;
		if (!ray_t.contains(t)) return false;

		inte.t = t;
		inte.p = r.at(t);
		inte.set_face_normal(r, normal);
		inte.mat = mat;
		inte.u = u;
		inte.v = v;
		return true;
	}

private:
	point3d v0, v1, v2;
	vec3d edge1, edge2;
	vec3d normal;
	shared_ptr<material> mat;
	aabb bbox;
};
