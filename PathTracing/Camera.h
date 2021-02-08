#pragma once
#include"Ray.h"
#include"SceneManager.h"
#include<random>

extern std::mt19937 rnd;

class Camera {
private:
	Vec3 position;
	Vec3 lookz, looky, lookx;
	float rrp = 0.8f;
	SceneManager* scene;

	std::uniform_real_distribution<float> val01;
	std::uniform_real_distribution<float> val11;

public:
	Camera(Vec3 position, Vec3 lookz, Vec3 looky);
	Camera(Vec3 position, Vec3 lookz);

	Vec3* render(int w, int h, float fov, int samps, int antiliasing = 4);

private:
	int gamma_correct(float col);
	Vec3 radiance(Ray ray, int depth, bool not_calc_em = false);
};
