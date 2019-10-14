#pragma once
#include <glm/vec3.hpp>
#include "Hitable.h"

using namespace glm;
class Material;
class MovingSphere : public Hitable
{
public:
	vec3 center0, center1;
	float time0, time1;
	float radius;
	Material* material;

	MovingSphere() {}
	MovingSphere(vec3 cen0, vec3 cen1, float t0, float t1,
				float r, Material* mm) :
				center0(cen0), center1(cen1), time0(t0),
				time1(t1), radius(r), material(mm) {};

	vec3 center(float time) const;
	virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const;
};

vec3 MovingSphere::center(float time) const
{
	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool MovingSphere::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const
{
	vec3 oc = r.origin() - center(r.getTime());
	float aa = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float cc = dot(oc, oc) - radius * radius;
	float discriminant = b * b - aa * cc;
	if (discriminant > 0)
	{
		float temp = (-b - sqrt(discriminant)) / aa;
		if (temp < tMax && temp > tMin)
		{
			rec.t = temp;
			rec.p = r.pointAtParameter(rec.t);
			rec.normal = (rec.p - center(r.getTime())) / radius;
			rec.matPtr = material;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / aa;
		if (temp < tMax && temp > tMin)
		{
			rec.t = temp;
			rec.p = r.pointAtParameter(rec.t);
			rec.normal = (rec.p - center(r.getTime())) / radius;
			rec.matPtr = material;
			return true;
		}
	}
	return false;
}
