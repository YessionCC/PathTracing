#pragma once
#include"Vectors.h"

class Transform {
public:
	Vec3 z, y;//look at,  up
	Vec3 position;
	Transform(): z(0, 0, -1, false), y(0, 1, 0, false), position(0, 0, 0, true) {}
};