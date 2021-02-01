#pragma once
#include"Vec3.h"
#include"CollisionBox.h"
#include<algorithm>

class Ray {
public:
	Vec3 o, dir;
	Ray() {}
	Ray(Vec3 o, Vec3 dir) :o(o), dir(dir) {}
	void check_collide_box(CollisionBox& box, float& tmin, float& tmax) {
		tmin = 1e20, tmax = -1e20;
		float t1, t2;
		if (dir.x != 0) {
			t1 = (box.x0 - o.x) / dir.x;
			t2 = (box.x1 - o.x) / dir.x;
			if (t1 > t2)std::swap(t1, t2);
			tmin = std::min(tmin, t1);
			tmax = std::max(tmax, t2);
		}
		if (dir.y != 0) {
			t1 = (box.y0 - o.y) / dir.y;
			t2 = (box.y1 - o.y) / dir.y;
			if (t1 > t2)std::swap(t1, t2);
			tmin = std::min(tmin, t1);
			tmax = std::max(tmax, t2);
		}
		if (dir.z != 0) {
			t1 = (box.z0 - o.z) / dir.z;
			t2 = (box.z1 - o.z) / dir.z;
			if (t1 > t2)std::swap(t1, t2);
			tmin = std::min(tmin, t1);
			tmax = std::max(tmax, t2);
		}
	}
};
