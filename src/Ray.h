#pragma once
#include <glm/vec3.hpp>

using namespace glm;
class Ray
{
	vec3 A, B;
	float time;
public:
	Ray() {}
	Ray(const vec3& a, const vec3& b, float ti = 0.0) : A(a), B(b), time(ti) {}

	vec3 origin() const { return A; }
	vec3 direction() const { return B; }
	float getTime() const { return time; }
	vec3 pointAtParameter(float t) const { return A + t * B; }
};