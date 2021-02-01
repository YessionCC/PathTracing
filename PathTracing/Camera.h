#pragma once
#include"Ray.h"
#include"Object.h"
#include"SceneManager.h"
#include<random>
#include<omp.h>

std::mt19937 rnd(100);
std::uniform_real_distribution<float> val11(-1, 1);
std::uniform_real_distribution<float> val01(0, 1);

class Camera {
private:
	Vec3 position;
	Vec3 lookz, looky, lookx;
	float rrp = 0.8f;
	SceneManager* scene;
public:
	Camera(Vec3 position, Vec3 lookz, Vec3 looky) : position(position) {
		lookz.normalize();
		looky.normalize();
		this->lookz = lookz;
		this->looky = looky;
		this->lookx = lookz.cross(looky);
		scene = SceneManager::instance();
	}
	Camera(Vec3 position, Vec3 lookz): position(position) {//自适应正向
		lookz.normalize();
		Vec3 up(0, 0, 1);
		this->lookx = up.cross(lookz).normalize();
		this->looky = lookz.cross(lookx);
		this->lookz = lookz;
		scene = SceneManager::instance();
	}
	Vec3* render(int w, int h, float fov, int samps, int antiliasing = 4) {
		Vec3* cols = new Vec3[w*h];
		float scale = std::tanf(fov*3.14f/180 / 2) / w;
		int antinum = (int)std::sqrtf(antiliasing);
		float aoffset = 0.5f / antinum;

		printf("CPU cores number: %d all prepared\n", omp_get_num_procs());
		for (int i = 0; i < h; i++) {
			printf("\rrender processing: %.3f%%", 100.0f*i/(h-1));//\r光标回到行首
#pragma omp parallel for
			for (int j = 0; j < w; j++) {
				Vec3& curcol = cols[j + w * i];
				/*if (i == 308 && j == 476) {
					system("pause");
				}*/
				for (int ax = 0; ax < antinum; ax++) for (int ay = 0; ay < antinum; ay++) {//抗锯齿
					for (int s = 0; s < samps; s++) {
						Vec3 dir = lookx * (scale*(w / 2 - j + aoffset * (1 + 2 * ax))) + looky * (scale*(h / 2 - i + aoffset * (1 + 2 * ay))) + lookz;
						curcol = curcol + radiance(Ray(position, dir.normalize()), 0)*(1.0f / samps / antiliasing);
					}
				}
				curcol = curcol * (1.4);//提升亮度
				curcol = Vec3(gamma_correct(curcol.x), gamma_correct(curcol.y), gamma_correct(curcol.z));
				/*if (i==308 && j==476) {
					printf("%d %d, ", i, j);
					curcol.print();
					system("pause");
				}*/
			}
		}
		printf("\nRendering complete!\n");
		return cols;
	}

private:
	int gamma_correct(float col) {//gamma矫正，提高因正片叠底造成的亮度偏低的问题
		if (col > 1) col = 1;
		return (int)(std::powf(col, 1 / 2.2f) * 255 + 0.5);
	}
	Vec3 radiance(Ray ray, int depth, bool not_calc_em = false) {
		int ret; Vec3 itsc;
		//if (depth == 50) system("pause");
		//printf("%d\n", depth);
		ret = scene->ray_surface(ray, itsc);
		if (ret == -1) return Vec3();
		Object* obj = scene->get_obj_by_faceid(ret);
		Vec3 vt = scene->get_vt_by_faceid(ret);
		Vec3 obj_col = obj->color;
		if (depth > 5) {
			float p = val01(rnd);
			if (p < rrp) obj_col*(1.0f/rrp);//固定概率的RR，注意保持期望一致
			else return obj->emission;//RR fail则不再计算反射光
		}
		float dftd = ray.dir.dot(vt);//入射cos角值
		if (obj->material == Material::EMISSION) {//发光体
			return not_calc_em ? Vec3() : obj->emission;
		}
		else if (obj->material == Material::DIFFUSION) {//漫反射
			Vec3 vb = (ray.dir - vt * dftd).normalize();
			Vec3 vc = vt.cross(vb);
			float u, v, w; do { u = val11(rnd), v = val11(rnd); } while (u*u + v * v > 1);//半球面采样
			w = std::sqrt(1 - u * u - v * v);//w也相当于cosθ(反射角)
			return obj_col.castmult(
				//scene->light_sample(vt, itsc) + 
				radiance(Ray(itsc, vb*u + vc * v + vt * w), depth+1/*, true*/)*w
			);//乘上w是标准的brdf，但颜色会偏暗，需要矫正，不乘效果也可以
			//以物体的颜色，正片叠底到反射光
		}
		else if (obj->material == Material::SPECULAR) {//镜面反射
			return obj_col.castmult(radiance(Ray(itsc, ray.dir - vt * (2 * dftd)), depth + 1));
		}
		else if (obj->material == Material::REFRACTION) {//折射
			Ray ref(itsc, ray.dir - vt * (2 * dftd));
			float nnt = dftd > 0 ? obj->rfcn : 1.0f / obj->rfcn;//折射率比
			float sint = nnt * nnt*(1 - dftd * dftd), cost = 1 - sint;//出射角的平方
			if (cost < 0) return obj_col.castmult(radiance(ref, depth + 1));//构成全反射条件
			sint = std::sqrtf(sint), cost = std::sqrtf(cost);
			Ray refr; Vec3 vb = (ray.dir - vt * dftd).normalize();
			if (dftd < 0) refr = Ray(itsc, vt*-cost + vb * sint); //射入介质
			else refr = Ray(itsc, vt*cost + vb * sint); //射出介质
			float na = obj->rfcn - 1, nb = obj->rfcn + 1, r = na * na / (nb*nb), c = 1 - (dftd < 0 ? -dftd : cost);
			float re = r + (1 - r)*c*c*c*c*c;//五阶近似菲涅尔项
			re = 0.5f;//不使用菲涅尔项
			if (depth > 3) {//由于折射光将使得光线数量加倍，必须在折射光处使用RR，否则当rrp>0.5后，光线数量增而不停
				float p = val01(rnd);
				if (p < rrp) return obj_col.castmult(radiance(ref, depth + 1)*(re / rrp));
				else return obj_col.castmult(radiance(refr, depth + 1)*((1 - re) / (1 - rrp)));//保证符合菲涅尔公式的条件下期望不变
			}
			else return obj_col.castmult(radiance(ref, depth + 1)*re + radiance(refr, depth + 1)*(1 - re));
		}
		return Vec3();//
	}
};
