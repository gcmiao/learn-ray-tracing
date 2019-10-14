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
	vec3 u, v, w;
	float lensRadius;

	Camera(const vec3& lookFrom, const vec3& lookAt, const vec3& vup,
				 float vfov, float aspect,
				 float aperture, float focusDist) // vfov is top to bottom in degrees
	{
		lensRadius = aperture / 2;
		float theta = vfov * M_PI / 180;
		float halfHeight = tan(theta / 2);
		float halfWidth = aspect * halfHeight;
		origin = lookFrom;
		w = normalize(lookFrom - lookAt);
		u = normalize(cross(vup, w));
		v = cross(w, u);
		lowerLeftCorner = origin - focusDist * (halfWidth * u + halfHeight * v + w);
		horizontal = 2 * halfWidth * focusDist * u;
		vertical = 2 * halfHeight * focusDist * v;
	}

	Ray getRay(float s, float t)
	{
		vec3 rd = lensRadius * randomInUnitDisk();
		vec3 offset = u * rd.x + v * rd.y;
		return Ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset);
	}
};