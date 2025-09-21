#ifndef INTERSECTABLE_H
#define INTERSECTABLE_H

#include "ray.h"
#include "interval.h"

class intersects {
public:
	point3d p;
	vec3d normal;
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

#endif
