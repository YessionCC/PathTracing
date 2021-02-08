#pragma once
#include"Material.h"

class DiffuseMaterial : public Material {
	Vec3 bsdf(Ray& ray, Vec3& itsc, Vec3& vn) {
		float dftd = ray.dir.dot(vn);//入射cos角值
		Vec3 vb = (ray.dir - vn * dftd).normalize();
		Vec3 vc = vn.cross(vb);
		float u, v, w; do { u = val11(rnd), v = val11(rnd); } while (u*u + v * v > 1);//半球面采样
		w = std::sqrt(1 - u * u - v * v);//w也相当于cosθ(反射角)
		return obj_col.castmult(
			/*scene->light_sample(vn, itsc) + */
			radiance(Ray(itsc, vb*u + vc * v + vn * w), depth + 1/*, true*/)*w
		);//乘上w是标准的brdf，但颜色会偏暗，需要矫正，不乘效果也可以
		//以物体的颜色，正片叠底到反射光
	}
};