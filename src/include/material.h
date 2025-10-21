#pragma once

#include "intersectable.h"
#include "texture.h"

class material {
	public:
		virtual ~material() = default;

		virtual bool scatter(const ray& r_in, const intersects& inte, color& attenuation, ray& scattered) const {
			return false;
		}

		virtual color emitted(double u, double v, const point3d& p) const {
			return color(0, 0, 0);
		}
};

class lambertian : public material {
	public:
		lambertian(const color& albedo) : tex(make_shared<solid_color>(albedo)) {}
		lambertian(shared_ptr<texture> tex) : tex(tex) {}

		bool scatter(const ray& r_in, const intersects& inte, color& attenuation, ray& scattered) const override {
			auto scatter_direction = inte.normal + random_unit_vector();

			if (scatter_direction.near_zero()) scatter_direction = inte.normal;

			scattered = ray(inte.p, scatter_direction, r_in.time());
			attenuation = tex->value(inte.u, inte.v, inte.p);
			return true;
		}

	private:
		shared_ptr<texture> tex;
		color albedo;
};

class metal : public material {
	public:
		metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

		bool scatter(const ray& r_in, const intersects& inte, color& attenuation, ray& scattered) const override {
			vec3d reflected = reflect(r_in.direction(), inte.normal);
			reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
			scattered = ray(inte.p, reflected, r_in.time());
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
			double cos_theta = std::fmin(dot(-unit_direction, inte.normal), 1.0);
			double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

			bool must_reflect = (ri * sin_theta) > 1.0;

			vec3d direction;

			if (must_reflect || reflectance(cos_theta, ri) > random_double()) direction = reflect(unit_direction, inte.normal);
			else direction = refract(unit_direction, inte.normal, ri);

			scattered = ray(inte.p, direction, r_in.time());
			return true;
		}

	private:
		// Refractive index
		double refraction_index;

		static double reflectance(double cosine, double refraction_index) {
		// Schlick's approximation.
		auto r0 = (1 - refraction_index) / (1 + refraction_index);
		r0 = r0*r0;
		return r0 + (1-r0)*std::pow((1 - cosine),5);
	}
};

class diffuse_light : public material {
	public:
		diffuse_light(shared_ptr<texture> tex) : tex(tex) {}
		diffuse_light(const color& emit) : tex(make_shared<solid_color>(emit)) {}

		color emitted(double u, double v, const point3d& p) const override {
			return tex->value(u, v, p);
		}

	private:
		shared_ptr<texture> tex;
};
