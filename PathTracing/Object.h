#pragma once
#include<vector>
#include"Vec3.h"

enum Material {
	DIFFUSION,
	REFRACTION,
	SPECULAR,
	GLOSSY,
	CERAMIC,
	EMISSION
};

class Object {
public:
	std::vector<int> vtxs;
	std::vector<int> faces;
	Material material;//材质
	Vec3 color;//颜色(纹理)
	Vec3 emission;//发光颜色
	float rfcn;//若是折射体，需要设置折射率，越大越模糊，不可大于1
	float glossy_radius;//若是光泽体，需要设置光泽度
	float ceramic_smooth;//若是光滑陶瓷，需要设置，控制镜面反射与漫反射比率
	Vec3 center;//物体中心点

	void move(Vec3 dir);//傀儡操作
	void std_scale(int scale);
	void inverse_y_axis();
	void rotate_arround_z(float deg);
	static Object* load_obj(const char path[], bool is_calc_vt = false);
};
