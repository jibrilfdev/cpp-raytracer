#pragma once

#include "aabb.h"
#include "intersectable.h"
#include "intersectable_objects.h"

#include <algorithm>

class bvh_node : public intersectable {
public:
	bvh_node(intersectable_list list) : bvh_node(list.objects, 0, list.objects.size()) {
	}

	bvh_node(std::vector<shared_ptr<intersectable>>& objects, size_t start, size_t end) {
		int axis = random_int(0, 2);

		auto comparator = (axis == 0) ? box_x_compare : (axis == 1) ? box_y_compare : box_z_compare;

		size_t object_span = end - start;

		if (object_span == 1) {
			left = right = objects[start];
		} else if (object_span == 2) {
			left = objects[start];
			right = objects[start+1];
		} else {
			std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

			auto mid = start + object_span/2;
			left = make_shared<bvh_node>(objects, start, mid);
			right = make_shared<bvh_node>(objects, mid, end);

		}
		bbox = aabb(left->bounding_box(), right->bounding_box());
	}

	bool intersect(const ray& r, interval ray_t, intersects& inte) const override {
		if (!bbox.intersect(r, ray_t)) return false;

		bool intersect_left = left->intersect(r, ray_t, inte);
		bool intersect_right = right->intersect(r, interval(ray_t.min, intersect_left ? inte.t : ray_t.max), inte);

		return intersect_left || intersect_right;
	}

	aabb bounding_box() const override { return bbox; }

private:
	shared_ptr<intersectable> left;
	shared_ptr<intersectable> right;
	aabb bbox;

	static bool box_compare(const shared_ptr<intersectable> a, const shared_ptr<intersectable> b, int axis_index) {
		auto a_axis_interval = a->bounding_box().axis_interval(axis_index);
		auto b_axis_interval = b->bounding_box().axis_interval(axis_index);
		return a_axis_interval.min < b_axis_interval.min;
	}

	static bool box_x_compare(const shared_ptr<intersectable> a, const shared_ptr<intersectable> b) {
		return box_compare(a, b, 1);
	}

	static bool box_y_compare(const shared_ptr<intersectable> a, const shared_ptr<intersectable> b) {
		return box_compare(a, b, 1);
	}

	static bool box_z_compare(const shared_ptr<intersectable> a, const shared_ptr<intersectable> b) {
		return box_compare(a, b, 2);
	}
};
