#pragma once

#include "intersectable.h"

class quadrilateral : public intersectable {
	public:
		quadrilateral(const point3d& Q, const vec3d& u, const vec3d& v, shared_ptr<material> mat) : Q(Q), u(u), v(v), mat(mat) {
			auto n = cross(u, v);
			normal = unit_vector(n);
			D = dot(normal, Q);
			w = n / dot(n, n);
			set_bounding_box();
		}

		virtual void set_bounding_box() {
			auto bbox_diagonal1 = aabb(Q, Q+u+v);
			auto bbox_diagonal2 = aabb(Q+u, Q+v);
			bbox = aabb(bbox_diagonal1, bbox_diagonal2);
		}

		aabb bounding_box() const override { return bbox; }

		bool intersect(const ray& r, interval ray_t, intersects& inte) const override {
			auto denominator = dot(normal, r.direction());

			if (std::fabs(denominator) < 1e-8) return false;

			auto t = (D - dot(normal, r.origin())) / denominator;
			if (!ray_t.contains(t)) return false;

			auto intersection = r.at(t);
			vec3d planar_intersectpt_vector = intersection - Q;
			auto alpha = dot(w, cross(planar_intersectpt_vector, v));
			auto beta = dot(w, cross(u, planar_intersectpt_vector));

			if (!is_interior(alpha, beta, inte)) return false;

			inte.t = t;
			inte.p = intersection;
			inte.mat = mat;
			inte.set_face_normal(r, normal);

			return true;
		}

	virtual bool is_interior(double a, double b, intersects& inte) const {
		interval unit_interval = interval(0, 1);

		if (!unit_interval.contains(a) || !unit_interval.contains(b)) return false;

		inte.u = a;
		inte.v = b;
		return true;
	}

	private:
		point3d Q;
		vec3d u, v, w;
		shared_ptr<material> mat;
		aabb bbox;
		vec3d normal;
		double D;
};
