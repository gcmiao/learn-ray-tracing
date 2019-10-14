#pragma once

#include "drand48.h"

#define M_PI 3.14159265358979323846

vec3 randomInUnitSphere()
{
	vec3 p;
	do {
		// random point in 2x2x2 cube that bounds the unit sphere
		p = 2.f * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
	} while (length(p) >= 1.0); // reject the point that out of the unit shpere
	return p;
}

vec3 randomInUnitDisk()
{
	vec3 p;
	do {
		p = 2.f * vec3(drand48(), drand48(), 0) - vec3(1, 1, 0);
	} while (length(p) >= 1.0);
	return p;
}

vec3 reflect(const vec3& v, const vec3& n)
{
	return v - 2 * dot(v, n) * n;
}