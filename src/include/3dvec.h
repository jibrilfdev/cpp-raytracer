#pragma once

#include <cmath>
#include <iostream>
#include "constants.h"

class vec3d {
  public:
    double e[3];

    vec3d() : e{0,0,0} {}
    vec3d(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3d operator-() const { return vec3d(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3d& operator+=(const vec3d& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3d& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3d& operator/=(double t) {
        return *this *= 1/t;
    }

    double length() const {
        return std::sqrt(length_squared());
    }

    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

	// Return true if the vector is close to zero.
	bool near_zero() const {
		auto s = 1e-8;
		return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
	}

	// Return random vector where each element is in the interval [0, 1).
	static vec3d random() {
		return vec3d(random_double(), random_double(), random_double());
	}

	// Return random vector where each element is in the interval [min, max).
	static vec3d random(double min, double max) {
		return vec3d(random_double_range(min,max), random_double_range(min,max), random_double_range(min,max));
	}

};

// Alias
using point3d = vec3d;



inline std::ostream& operator<<(std::ostream& out, const vec3d& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3d operator+(const vec3d& u, const vec3d& v) {
    return vec3d(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3d operator-(const vec3d& u, const vec3d& v) {
    return vec3d(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3d operator*(const vec3d& u, const vec3d& v) {
    return vec3d(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3d operator*(double t, const vec3d& v) {
    return vec3d(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3d operator*(const vec3d& v, double t) {
    return t * v;
}

inline vec3d operator/(const vec3d& v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3d& u, const vec3d& v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline vec3d cross(const vec3d& u, const vec3d& v) {
    return vec3d(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3d unit_vector(const vec3d& v) {
    return v / v.length();
}

inline vec3d random_unit_vector() {
	while (true) {
		auto t = vec3d::random(-1, 1);
		auto lensq = t.length_squared();
		if (1e-160 < lensq && lensq <= 1) return t / sqrt(lensq);
	}
}

inline vec3d random_on_hemisphere(const vec3d& normal) {
	vec3d on_unit_sphere = random_unit_vector();
	// Check for same hemisphere as normal.
	if (dot(on_unit_sphere, normal) > 0.0) return on_unit_sphere;
	return -on_unit_sphere;
}

inline vec3d reflect(const vec3d& v, const vec3d& n) {
	return v - 2*dot(v,n)*n;
}
