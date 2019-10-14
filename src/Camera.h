#pragma once
#include <glm/vec3.hpp>
#include "MathUtils.h"
#include "Ray.h"

using namespace glm;

class Camera
{
public:
	vec3 origin;
	vec3 lowerLeftCorner;
	vec3 horizontal;
	vec3 vertical;

	Camera(const vec3& lookFrom, const vec3& lookAt, const vec3& vup,
				 float vfov, float aspect) // vfov is top to bottom in degrees
	{
		vec3 u, v, w;
		float theta = vfov * M_PI / 180;
		float halfHeight = tan(theta / 2);
		float halfWidth = aspect * halfHeight;
		origin = lookFrom;
		w = normalize(lookFrom - lookAt);
		u = normalize(cross(vup, w));
		v = cross(w, u);
		lowerLeftCorner = origin - halfWidth * u - halfHeight * v - w;
		horizontal = 2 * halfWidth * u;
		vertical = 2 * halfHeight * v;
	}

	Ray getRay(float u, float v)
	{
		return Ray(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
	}
};