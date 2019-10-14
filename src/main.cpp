#include <iostream>
#include <fstream>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include "Ray.h"
#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"
#include <float.h>
#include "drand48.h"

using namespace std;
using namespace glm;

vec3 color(const Ray& r, Hitable *world)
{
	HitRecord rec;
	if (world->hit(r, 0.0, FLT_MAX, rec))
	{
		return 0.5f * vec3(rec.normal.x + 1, rec.normal.y + 1, rec.normal.z + 1);
	}
	else
	{
		vec3 unitDirection = normalize(r.direction());
		float t = 0.5 * (unitDirection.y + 1.0);
		// blended_value = (1 - t) * start_value + t * end_value
		return (1.0f - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

int main()
{
	int nx = 200;
	int ny = 100;
	int ns = 100;

	ofstream out;
	out.open("img.ppm", ios::out | ios::trunc);
	out << "P3\n" << nx << " " << ny << "\n255\n";

	Hitable *list[2];
	list[0] = new Sphere(vec3(0, 0, -1), 0.5);
	list[1] = new Sphere(vec3(0, -100.5, -1), 100);
	Hitable *world = new HitableList(list, 2);

	Camera cam;
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + drand48()) / float(nx);
				float v = float(j + drand48()) / float(ny);
				Ray r = cam.getRay(u, v);
				vec3 p = r.pointAtParameter(2.0);
				col += color(r, world);

			}
			col /= float(ns);
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			out << ir << " " << ig << " " << ib << "\n";
		}
	}
	out.close();
}
