#pragma once

#include "ray.h"
#include "interval.h"

class material;

class intersects {
public:
	point3d p;
	vec3d normal;
	shared_ptr<material> mat;
	double t;
	bool front_face;

	void set_face_normal(const ray& r, const vec3d& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class intersectable {
public:
	virtual ~intersectable() = default;

	virtual bool intersect(const ray& r, interval ray_t, intersects& inte) const = 0;
};

