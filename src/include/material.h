#pragma once

#include "intersectable.h"

class material {
	public:
		virtual ~material() = default;

		virtual bool scatter(const ray& r_in, const intersects& inte, color& attenuation, ray& scattered) const {
			return false;
		}
};

class lambertian : public material {
	public:
		lambertian(const color& albedo) : albedo(albedo) {}

		bool scatter(const ray& r_in, const intersects& inte, color& attenuation, ray& scattered) const override {
			auto scatter_direction = inte.normal + random_unit_vector();

			if (scatter_direction.near_zero()) scatter_direction = inte.normal;

			scattered = ray(inte.p, scatter_direction);
			attenuation = albedo;
			return true;
		}

	private:
		color albedo;
};

class metal : public material {
	public:
		metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

		bool scatter(const ray& r_in, const intersects& inte, color& attenuation, ray& scattered) const override {
			vec3d reflected = reflect(r_in.direction(), inte.normal);
			reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
			scattered = ray(inte.p, reflected);
			attenuation = albedo;
			return (dot(scattered.direction(), inte.normal) > 0);
		}

	private:
		color albedo;
		double fuzz;
};

class dielectric : public material {
	public:
		dielectric(double refraction_index) : refraction_index(refraction_index) {}

		bool scatter(const ray& r_in, const intersects& inte, color& attenuation, ray& scattered) const override {
			attenuation = color(1.0, 1.0, 1.0);
			double ri = inte.front_face ? (1.0 / refraction_index) : refraction_index;

			vec3d unit_direction = unit_vector(r_in.direction());
			vec3d refracted = refract(unit_direction, inte.normal, ri);

			scattered = ray(inte.p, refracted);
			return true;
		}

	private:
		// Refractive index
		double refraction_index;
};
