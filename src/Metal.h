#pragma once
#include <glm/glm.hpp>
#include "Material.h"
#include "MathUtils.h"

using namespace glm;

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