#pragma once
#include"Vec3.h"
#include<algorithm>

class CollisionBox {
public:
	float x0, y0, z0, x1, y1, z1;
	CollisionBox() :x0(1e20), y0(1e20), z0(1e20), x1(-1e20), y1(-1e20), z1(-1e20) {}
	inline void update(Vec3 vtx) {
		x0 = std::min(x0, vtx.x); x1 = std::max(x1, vtx.x);
		y0 = std::min(y0, vtx.y); y1 = std::max(y1, vtx.y);
		z0 = std::min(z0, vtx.z); z1 = std::max(z1, vtx.z);
	}
};