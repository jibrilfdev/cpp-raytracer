#ifndef RAY_H
#define RAY_H

#include "./3dvec.h"

class ray {
  public:
    ray() {}

    ray(const point3d& origin, const vec3d& direction) : orig(origin), dir(direction) {}

    const point3d& origin() const  { return orig; }
    const vec3d& direction() const { return dir; }

    point3d at(double t) const {
        return orig + t*dir;
    }

  private:
    point3d orig;
    vec3d dir;
};

#endif
