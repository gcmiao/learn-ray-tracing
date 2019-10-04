#pragma once
#include <glm/vec3.hpp>
#include "Hitable.h"

using namespace glm;
class HitableList : public Hitable
{
public:
	Hitable **list;
	int listSize;

	HitableList() {}
	HitableList(Hitable **l, int n) : list(l), listSize(n) {}

	virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const;

};

bool HitableList::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const
{
	HitRecord tempRec;
	bool hitAnything = false;
	double closest = tMax;
	for (int i = 0; i < listSize; i++)
	{
		if (list[i]->hit(r, tMin, closest, tempRec))
		{
			hitAnything = true;
			closest = tempRec.t;
			rec = tempRec;
		}
	}
	return hitAnything;
}