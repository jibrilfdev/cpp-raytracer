#pragma once

#include <memory>
#include <vector>

#include "aabb.h"
#include "intersectable.h"

using std::make_shared;
using std::shared_ptr;

class intersectable_list : public intersectable {
	public:
		std::vector<shared_ptr<intersectable>> objects;

		intersectable_list() {}
		intersectable_list(shared_ptr<intersectable> object) { add(object); }

		void clear() { objects.clear(); }

		void add(shared_ptr<intersectable> object) {
			objects.push_back(object);
			bbox = aabb(bbox, object->bounding_box());
		}

		bool intersect(const ray& r, interval ray_t, intersects& inte) const override {
			intersects temp_inte;
			bool object_intersected = false;
			auto closest = ray_t.max;

			for (const auto& object : objects) {
				if (object->intersect(r, interval(ray_t.min, closest), temp_inte)) {
					object_intersected = true;
					closest = temp_inte.t;
					inte = temp_inte;
				}
			}

			return object_intersected;
		}

		aabb bounding_box() const override { return bbox; }
	private:
		aabb bbox;
};

