#pragma once
#include <glm/glm.hpp>
#include "Material.h"
#include "drand48.h"

using namespace glm;

vec3 randomInUnitSphere()
{
	vec3 p;
	do {
		// random point in 2x2x2 cube that bounds the unit sphere
		p = 2.f * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
	} while (length(p) >= 1.0); // reject the point that out of the unit shpere
	return p;
}

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