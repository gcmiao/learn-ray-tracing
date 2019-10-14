#pragma once
#include <glm/vec3.hpp>
#include "Ray.h"

using namespace glm;

class Material;
struct HitRecord
{
	float t;
	vec3 p;
	vec3 normal;
	Material* matPtr;
};

class Hitable
{
public:
	virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const = 0;
};