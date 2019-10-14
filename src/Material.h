#pragma once
#include <glm/vec3.hpp>
#include "Ray.h"
#include "Hitable.h"

using namespace glm;

class Material
{
public:
	virtual bool scatter(const Ray& rIn, const HitRecord& rec, vec3& attenuation, Ray& scattered) const = 0;
};