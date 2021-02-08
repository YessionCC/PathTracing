#pragma once
#include<vector>
#include<string>
#include<random>
#include"Vec3.h"
#include"Face.h"
#include"Ray.h"
#include"Object.h"
#include"CollisionBox.h"

extern std::mt19937 rnd;

class SceneManager {
private:
	static SceneManager* _instance;

	std::vector<Vec3> vtxs;
	std::vector<Face> faces;
	std::vector<Vec3> fts;//面法向

	int max_tri_num;
	std::vector<CollisionBox> boxes;
	std::vector<int> fsid;//形成BVH结构后的面顺序

	std::uniform_real_distribution<float> val01;
	std::uniform_int_distribution<int> val_emsize;

	SceneManager(): val01(0, 1) {}
public:
	std::vector<Object*> objs;//场景中的所有物体(包括发光体)
	std::vector<int> emission_idx;//发光面的索引，用于随机抽样
	std::vector<float> emission_area;//发光面的面积

	static SceneManager* instance() {
		return _instance;
	}

	Object* load_obj(const char path[], bool is_calc_vt = false);

	inline Object* get_obj_by_faceid(int fid) {
		return objs[faces[fid].objidx];
	}

	inline Vec3 get_vt_by_faceid(int fid) {
		return fts[fid];
	}

	Vec3 light_sample(Vec3& vt, Vec3& itsc);

	void build_structure(int max_tri_num = 4);

	int ray_surface(Ray& ray, Vec3& intersection);

	void move_obj(Object* obj, Vec3 dir);

	void standizing_obj(Object* obj, float lim);

private:
	int process_face_input(std::string& str);

	void _build_structure(int L, int R, int o, int max_tri_num);

	int _ray_surface(Ray& ray, int o, int L, int R, Vec3& intersection);
};
