#pragma once
#include <glm/vec3.hpp>

using namespace glm;
class Ray
{
	vec3 A, B;
public:
	Ray() {}
	Ray(const vec3& a, const vec3& b) { A = a; B = b; }

	vec3 origin() const { return A; }
	vec3 direction() const { return B; }
	vec3 point_at_parameter(float t) const { return A + t * B; }
};