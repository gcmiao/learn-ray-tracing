#pragma once
#include <glm/glm.hpp>
#include "Material.h"
#include "MathUtils.h"

using namespace glm;

class Lambertian : public Material
{
public:
	vec3 albedo;

	Lambertian(const vec3& aa) : albedo(aa) {}

	virtual bool scatter(const Ray& rIn, const HitRecord& rec, vec3& attenuation, Ray& scattered) const
	{
		// get a random point based on the hit point
		vec3 target = rec.p + rec.normal + randomInUnitSphere();
		scattered = Ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}
};