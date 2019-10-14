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
	float time0, time1;
	float lensRadius;

	Camera(const vec3& lookFrom, const vec3& lookAt, const vec3& vup,
				 float vfov, float aspect, // vfov is top to bottom in degrees
				 float aperture, float focusDist,
				 float t0, float t1)
	{
		time0 = t0;
		time1 = t1;
		lensRadius = aperture / 2;
		float theta = vfov * M_PI / 180;
		float halfHeight = tan(theta / 2); // scalar height at unit focus distance
		float halfWidth = aspect * halfHeight; // scalar width at unit focus distance
		origin = lookFrom;
		w = normalize(lookFrom - lookAt); // looking direction
		u = normalize(cross(vup, w)); // horizontal vector
		v = cross(w, u); // vertical vector
		lowerLeftCorner = origin - focusDist * (halfWidth * u + halfHeight * v + w); // scale unit distance to actual focus distance
		horizontal = 2 * halfWidth * focusDist * u; // width of focus/near plane
		vertical = 2 * halfHeight * focusDist * v; // height of focus/near plane
	}

	Ray getRay(float s, float t)
	{
		vec3 rd = lensRadius * randomInUnitDisk();
		vec3 offset = u * rd.x + v * rd.y;
		float time = time0 + drand48() * (time1 - time0);
		// ray shoots from a random point on the lens to the focus/near plane
		return Ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset, time);
	}
};