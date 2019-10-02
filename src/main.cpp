#include <iostream>
#include <fstream>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include "Ray.h"

using namespace std;
using namespace glm;

bool hitSphere(const vec3& center, float radius, const Ray& r)
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0 * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	return (discriminant > 0);
}

vec3 color(const Ray& r)
{
	if (hitSphere(vec3(0, 0, -1), 0.5, r))
		return vec3(1, 0, 0);

	vec3 unitDirection = normalize(r.direction());
	float t = 0.5 * (unitDirection.y + 1.0);
	// blended_value = (1 - t) * start_value + t * end_value
	return (1.0f - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main()
{
	int nx = 200;
	int ny = 100;
	ofstream out;
	out.open("img.ppm", ios::out | ios::trunc);
	out << "P3\n" << nx << " " << ny << "\n255\n";

	vec3 lowerLeftCorner(-2.0, -1.0, - 1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			Ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);
			vec3 col = color(r);
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			out << ir << " " << ig << " " << ib << "\n";
		}
	}
	out.close();
}
