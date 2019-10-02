#include <iostream>
#include <fstream>
#include <glm/vec3.hpp>

using namespace std;
using namespace glm;
int main()
{
	int nx = 200;
	int ny = 100;
	ofstream out;
	out.open("img.dat", ios::out | ios::trunc);
	out << "P3\n" << nx << " " << ny << "\n255\n";
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(float(i) / float(nx), float(j) / float(ny), 0.2);
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			out << ir << " " << ig << " " << ib << "\n";
		}
	}
	out.close();
}