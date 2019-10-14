#pragma once
#include <glm/glm.hpp>
#include "Material.h"
#include "drand48.h"

using namespace glm;

extern vec3 randomInUnitSphere();
extern vec3 reflect(const vec3& v, const vec3& n);

bool refract(const vec3& v, const vec3& n, float niOverNt, vec3& refracted)
{
	vec3 uv = normalize(v);
	float dt = dot(uv, n);
	float discriminant = 1.0f - niOverNt * niOverNt * (1 - dt * dt);
	if (discriminant > 0)
	{
		refracted = niOverNt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else
	{
		return false;
	}
}

float schlick(float cosine, float refIdx)
{
	float r0 = (1 - refIdx) / (1 + refIdx);
	r0 *= r0;
	return r0 + (1 - r0) * pow(1 - cosine, 5);
}

class Dielectric : public Material
{
public:
	float refIdx;

	Dielectric(float ri) : refIdx(ri) {}

	virtual bool scatter(const Ray& rIn, const HitRecord& rec, vec3& attenuation, Ray& scattered) const
	{
		vec3 outwardNormal;
		vec3 reflected = reflect(rIn.direction(), rec.normal);
		float niOverNt;
		attenuation = vec3(1.0, 1.0, 1.0); // transparency object does not obsorb energy
		vec3 refracted;
		float reflectProb;
		float cosine;
		if (dot(rIn.direction(), rec.normal) > 0)
		{
			outwardNormal = -rec.normal;
			niOverNt = refIdx;
			cosine = refIdx * dot(rIn.direction(), rec.normal) / length(rIn.direction());
		}
		else
		{
			outwardNormal = rec.normal;
			niOverNt = 1.0 / refIdx;
			cosine = -dot(rIn.direction(), rec.normal) / length(rIn.direction());
		}
		if (refract(rIn.direction(), outwardNormal, niOverNt, refracted))
		{
			reflectProb = schlick(cosine, refIdx);
		}
		else
		{
			reflectProb = 1;
		}
		if (drand48() < reflectProb)
		{
			scattered = Ray(rec.p, reflected);
		}
		else
		{
			scattered = Ray(rec.p, refracted);
		}
		return true;
	}
};