#pragma once
#include <glm/glm.hpp>
#include "Material.h"
#include "drand48.h"

using namespace glm;

extern vec3 randomInUnitSphere();

vec3 reflect(const vec3& v, const vec3& n)
{
	return v - 2 * dot(v, n) * n;
}

class Metal : public Material {
public:
	vec3 albedo;
	float fuzz;

	Metal(const vec3& aa, float f) : albedo(aa), fuzz(f < 1 ? f : 1) {}

	virtual bool scatter(const Ray& rIn, const HitRecord& rec, vec3& attenuation, Ray& scattered) const
	{
		vec3 reflected = reflect(normalize(rIn.direction()), rec.normal);
		scattered = Ray(rec.p, reflected + fuzz * randomInUnitSphere()); // diffuse around reflected direction
		attenuation = albedo;
		return dot(scattered.direction(), rec.normal) > 0;
	}
};