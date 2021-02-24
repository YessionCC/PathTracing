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
	Material material;//����
	Vec3 color;//��ɫ(����)
	Vec3 emission;//������ɫ
	float rfcn;//���������壬��Ҫ���������ʣ�Խ��Խģ�������ɴ���1
	float glossy_radius;//���ǹ����壬��Ҫ���ù����
	float ceramic_smooth;//���ǹ⻬�մɣ���Ҫ���ã����ƾ��淴�������������
	Vec3 center;//�������ĵ�

	void move(Vec3 dir);//���ܲ���
	void std_scale(int scale);
	void inverse_y_axis();
	void rotate_arround_z(float deg);
	static Object* load_obj(const char path[], bool is_calc_vt = false);
};
