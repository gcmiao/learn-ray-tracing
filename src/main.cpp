#include <iostream>
#include <fstream>
#include <filesystem>
#include <glm/glm.hpp>
#include "Ray.h"
#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"
#include <float.h>
#include "Lambertian.h"
#include "Metal.h"
#include "Dielectric.h"

using namespace std;
using namespace glm;

const static char OUTPUT_TEMP_NAME[] = "img.ppm";
const static int BOUNCE_LIMIT_COUNT = 50;
const static float REFLECT_RATE = 0.5;
const static float HIT_ESP = 0.001;
const static int NS = 100;
const static int OBJ_COUNT = 5;

vec3 color(const Ray& r, Hitable *world, int depth = 0)
{
	HitRecord rec;
	if (world->hit(r, HIT_ESP, FLT_MAX, rec))
	{
		Ray scattered;
		vec3 attenuation;
		if (depth < BOUNCE_LIMIT_COUNT && rec.matPtr->scatter(r, rec, attenuation, scattered))
		{
			// the reflected ray maybe hit another sphere
			// so the the adjacent region will be darker because
			// the shpere absorbs energy on each bounce
			// the color will be scaled smaller and smaller
			return attenuation * color(scattered, world, depth + 1);
		}
		else
		{
			return vec3(0, 0, 0);
		}
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

	ofstream out;
	out.open(OUTPUT_TEMP_NAME, ios::out | ios::trunc);
	out << "P3\n" << nx << " " << ny << "\n255\n";

	Hitable *list[OBJ_COUNT];
	float R = cos(M_PI / 4);
	list[0] = new Sphere(vec3(0, 0, -1), 0.5, new Lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new Sphere(vec3(0, -100.5, -1), 100, new Lambertian(vec3(0.8, 0.8, 0.0)));
	// list[0] = new Sphere(vec3(-R, 0, -1), R, new Lambertian(vec3(0, 0, 1)));
	// list[1] = new Sphere(vec3(R, 0, -1), R, new Lambertian(vec3(1, 0, 0)));
	list[2] = new Sphere(vec3(1, 0, -1), 0.5, new Metal(vec3(0.8, 0.6, 0.2), 1.0));
	list[3] = new Sphere(vec3(-1, 0, -1), 0.5, new Dielectric(1.5));
	// if you use a negative radius, the geometry is unaffected
	// but the surface normal points inward
	// so it can be used as a bubble to make a hollow glass sphere
	list[4] = new Sphere(vec3(-1, 0, -1), -0.45, new Dielectric(1.5));
	Hitable *world = new HitableList(list, OBJ_COUNT);
	// world = new HitableList(list, 2);

	Camera cam(vec3(-2, 2, 1), vec3(0, 0, -1), vec3(0, 1, 0), 20, float(nx) / float(ny));
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < NS; s++)
			{
				float u = float(i + drand48()) / float(nx);
				float v = float(j + drand48()) / float(ny);
				Ray r = cam.getRay(u, v);
				vec3 p = r.pointAtParameter(2.0);
				col += color(r, world);

			}
			col /= float(NS);
			// raise the color to the power 1/gamma
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			out << ir << " " << ig << " " << ib << "\n";
		}
	}
	out.close();
	char fileName[100];
	snprintf(fileName, 100, "img_ns_%d_ref_%.2f_l_%d_esp_%.4f.ppm", NS, REFLECT_RATE, BOUNCE_LIMIT_COUNT, HIT_ESP);
	// rename(OUTPUT_TEMP_NAME, fileName);
	return 0;
}
