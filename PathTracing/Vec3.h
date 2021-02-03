#pragma once
#include<cstdio>
#include<cmath>

class Vec3 {
public:
	float x, y, z;
	Vec3(float x=0, float y=0, float z=0):x(x), y(y), z(z) {}
	Vec3& normalize() {
		float m = std::sqrt(x*x + y * y + z * z);
		if (m < 1e-8) return *this;
		x /= m, y /= m, z /= m;
		return *this;
	}
	Vec3 cross(Vec3 v) {
		return Vec3(y*v.z - z * v.y, z*v.x - x * v.z, x*v.y - y * v.x);
	}
	float dot(Vec3 v) {
		return v.x*x + v.y*y + v.z*z;
	}
	float norm() {
		return std::sqrt(x*x + y * y + z * z);
	}
	Vec3 castmult(Vec3 v) {
		return Vec3(x*v.x, y*v.y, z*v.z);
	}
	Vec3 operator+(Vec3 rhs) {
		return Vec3(rhs.x + x, rhs.y + y, rhs.z + z);
	}
	Vec3 operator-(Vec3 rhs) {
		return Vec3(-rhs.x + x, -rhs.y + y, -rhs.z + z);
	}
	Vec3 operator*(float scale) {
		return Vec3(x*scale, y*scale, z*scale);
	}
	float distance2(Vec3 v) {
		return (x - v.x)*(x - v.x) + (y - v.y)*(y - v.y) + (z - v.z)*(z - v.z);
	}
	Vec3& clamp() {
		if (x > 1) x = 1;
		if (y > 1) y = 1;
		if (z > 1) z = 1;
		return *this;
	}
	void print() {
		printf("%f, %f, %f\n", x, y, z);
	}
};
