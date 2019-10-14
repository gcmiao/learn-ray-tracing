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
#include "MovingSphere.h"
#include <omp.h>

using namespace std;
using namespace glm;

const static char OUTPUT_TEMP_NAME[] = "img.ppm";
const static int BOUNCE_LIMIT_COUNT = 50;
const static float REFLECT_RATE = 0.5;
const static float HIT_ESP = 0.001;
const static int NS = 100;

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

Hitable* randomScene()
{
	int n = 50000;
	Hitable** list = new Hitable*[n + 1];
	list[0] = list[1] = new Sphere(vec3(0, -1000, 0), 1000, new Lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int aa = -10; aa < 10; aa++)
	{
		for (int bb = -10; bb < 10; bb++)
		{
			float chooseMat = drand48();
			vec3 center(aa + 0.9 * drand48(), 0.2, bb + 0.9 * drand48());
			if (length(center - vec3(4, 0.2, 0)) > 0.9)
			{
				if (chooseMat < 0.8) // diffuse
				{
					list[i++] = new MovingSphere(center, center + vec3(0, 0.5 * drand48(), 0), 0.0, 1.0, 0.2, new Lambertian(vec3(drand48() * drand48(), drand48() * drand48(), drand48() * drand48())));
				}
				else if (chooseMat < 0.95) // metal
				{
					list[i++] = new Sphere(center, 0.2, new Metal(vec3(0.5 * (1 + drand48()), 0.5 * (1 + drand48()), 0.5 * (1 + drand48())), 0.5 * drand48()));
				}
				else // glass
				{
					list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
				}
			}
		}
	}
	list[i++] = new Sphere(vec3(0, 1, 0), 1.0, new Dielectric(1.5));
	list[i++] = new Sphere(vec3(-4, 1, 0), 1.0, new Lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new Sphere(vec3(4, 1, 0), 1.0, new Metal(vec3(0.7, 0.6, 0.5), 0.0));
	
	return new HitableList(list, i);
}

int main()
{
	int nx = 600;
	int ny = 400;
	int lw = 13;

	ofstream out;
	out.open(OUTPUT_TEMP_NAME, ios::out | ios::trunc);
	out << "P3\n" << nx << " " << ny << "\n255\n";

	char* outCache = new char[nx * ny * lw + 10]{ 0 };

	Hitable* world = randomScene();

	vec3 lookFrom(13, 2, 3);
	vec3 lookAt(0, 0, 0);
	float distToFocus = 10;
	float aperture = 0.0;
	Camera cam(lookFrom, lookAt, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, distToFocus, 0.0, 1.0);

	int procNum = omp_get_num_procs();
	#pragma omp parallel num_threads(procNum)
	#pragma omp for
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

			// out << ir << " " << ig << " " << ib << "\n";
			snprintf(outCache + (j * nx + i) * lw, lw, "%d %d %d\n", ir, ig, ib);
		}
		int tid = omp_get_thread_num();
		printf("Complete: %d / %d on thread %d\n", ny - j, ny, tid);
	}

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			out << outCache + (j * nx + i) * lw;
		}
	}
	out.close();
	char fileName[100];
	snprintf(fileName, 100, "img_ns_%d_ref_%.2f_l_%d_esp_%.4f.ppm", NS, REFLECT_RATE, BOUNCE_LIMIT_COUNT, HIT_ESP);
	// rename(OUTPUT_TEMP_NAME, fileName);
	return 0;
}
