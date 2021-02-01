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
	Material material;//����
	Vec3 color;//��ɫ(����)
	Vec3 emission;//������ɫ
	float rfcn;//���������壬��Ҫ����������
	Vec3 center;//�������ĵ�
};
