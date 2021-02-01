#pragma once
#include<vector>
#include"Vec3.h"

enum Material {
	DIFFUSION,
	REFRACTION,
	SPECULAR,
	EMISSION
};

class Object {
public:
	std::vector<int> vtxs;
	std::vector<int> faces;
	Material material;//材质
	Vec3 color;//颜色(纹理)
	Vec3 emission;//发光颜色
	float rfcn;//若是折射体，需要设置折射率
	Vec3 center;//物体中心点
};
