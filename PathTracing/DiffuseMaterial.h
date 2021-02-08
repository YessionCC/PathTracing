#pragma once
#include"Material.h"

class DiffuseMaterial : public Material {
	Vec3 bsdf(Ray& ray, Vec3& itsc, Vec3& vn) {
		float dftd = ray.dir.dot(vn);//����cos��ֵ
		Vec3 vb = (ray.dir - vn * dftd).normalize();
		Vec3 vc = vn.cross(vb);
		float u, v, w; do { u = val11(rnd), v = val11(rnd); } while (u*u + v * v > 1);//���������
		w = std::sqrt(1 - u * u - v * v);//wҲ�൱��cos��(�����)
		return obj_col.castmult(
			/*scene->light_sample(vn, itsc) + */
			radiance(Ray(itsc, vb*u + vc * v + vn * w), depth + 1/*, true*/)*w
		);//����w�Ǳ�׼��brdf������ɫ��ƫ������Ҫ����������Ч��Ҳ����
		//���������ɫ����Ƭ���׵������
	}
};