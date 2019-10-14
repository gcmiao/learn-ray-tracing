#pragma once
#include <glm/vec3.hpp>
#include "Hitable.h"

using namespace glm;
class Material;
class Sphere : public Hitable
{
public:
	vec3 center;
	float radius;
	Material* material;

	Sphere() {}
	Sphere(vec3 cen, float r, Material* m) : center(cen), radius(r), material(m) {};

	virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const;
};

bool Sphere::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - a * c;
	if (discriminant > 0)
	{
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < tMax && temp > tMin)
		{
			rec.t = temp;
			rec.p = r.pointAtParameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.matPtr = material;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < tMax && temp > tMin)
		{
			rec.t = temp;
			rec.p = r.pointAtParameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.matPtr = material;
			return true;
		}
	}
	return false;
}