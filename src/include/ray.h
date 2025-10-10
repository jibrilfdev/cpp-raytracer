#pragma once

#include "3dvec.h"

class ray {
  public:
    ray() {}

	ray(const point3d& origin, const vec3d direction, double time) : orig(origin), dir(direction), tm(time) {}

	ray(const point3d& origin, const vec3d& direction) : ray(origin, direction, 0) {}

    const point3d& origin() const  { return orig; }
    const vec3d& direction() const { return dir; }

	double time() const { return tm; }

    point3d at(double t) const {
        return orig + t*dir;
    }

  private:
    point3d orig;
    vec3d dir;
	double tm;
};

