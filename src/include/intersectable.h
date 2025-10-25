#pragma once

#include "ray.h"
#include "interval.h"
#include "aabb.h"

class material;

class intersects {
public:
	point3d p;
	vec3d normal;
	shared_ptr<material> mat;
	double t;
	double u;
	double v;
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

	virtual aabb bounding_box() const = 0;
};

class translate : public intersectable {
	public:
		translate(shared_ptr<intersectable> object, const vec3d& offset) : object(object), offset(offset) {
			bbox = object->bounding_box() + offset;
		}

		bool intersect(const ray& r, interval ray_t, intersects& inte) const override {
			ray offset_r(r.origin() - offset, r.direction(), r.time());

			if (!object->intersect(offset_r, ray_t, inte)) return false;

			inte.p += offset;

			return true;
		}

	aabb bounding_box() const override {
		return bbox;
	}

	private:
		shared_ptr<intersectable> object;
		vec3d offset;
		aabb bbox;
};

class rotate_y : public intersectable {
	public:

		rotate_y(shared_ptr<intersectable> object, double angle) : object(object) {
			auto radians = degrees_to_radians(angle);
			sin_theta = std::sin(radians);
			cos_theta = std::cos(radians);
			bbox = object->bounding_box();

			point3d min( infinity,  infinity,  infinity);
			point3d max(-infinity, -infinity, -infinity);

			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					for (int k = 0; k < 2; k++) {
						auto x = i*bbox.x.max + (1-i)*bbox.x.min;
						auto y = j*bbox.y.max + (1-j)*bbox.y.min;
						auto z = k*bbox.z.max + (1-k)*bbox.z.min;

						auto newx =  cos_theta*x + sin_theta*z;
						auto newz = -sin_theta*x + cos_theta*z;

						vec3d tester(newx, y, newz);

						for (int c = 0; c < 3; c++) {
							min[c] = std::fmin(min[c], tester[c]);
							max[c] = std::fmax(max[c], tester[c]);
						}
					}
				}
			}

			bbox = aabb(min, max);
		}

		bool intersect(const ray& r, interval ray_t, intersects& inte) const override {
			auto origin = point3d((cos_theta * r.origin().x()) - (sin_theta * r.origin().z()), r.origin().y(), (sin_theta * r.origin().x()) + (cos_theta * r.origin().z()));

			auto direction = vec3d((cos_theta * r.direction().x()) - (sin_theta * r.direction().z()), r.direction().y(), (sin_theta * r.direction().x()) + (cos_theta * r.direction().z()));

			ray rotated_r(origin, direction, r.time());

			if (!object->intersect(rotated_r, ray_t, inte)) return false;


			inte.p = point3d((cos_theta * inte.p.x()) + (sin_theta * inte.p.z()), inte.p.y(), (-sin_theta * inte.p.x()) + (cos_theta * inte.p.z()));

			inte.normal = vec3d((cos_theta * inte.normal.x()) + (sin_theta * inte.normal.z()), inte.normal.y(),(-sin_theta * inte.normal.x()) + (cos_theta * inte.normal.z()));

			return true;
		}

		aabb bounding_box() const override {
			return bbox;
		}

	private:
		shared_ptr<intersectable> object;
		double sin_theta;
		double cos_theta;
		aabb bbox;
};
