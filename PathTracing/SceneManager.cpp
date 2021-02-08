#include"SceneManager.h"
#include<sstream>
#include<fstream>
#include<cmath>

Object* SceneManager::load_obj(const char path[], bool is_calc_vt) {//is_calc_vt：是否自动计算面法向，如果是，那么面数据不得先于点数据
	std::ifstream obj_file(path, std::ios::in);
	std::string buff, tmp;
	int foffset = vtxs.size();
	float mx = 0, my = 0, mz = 0;
	Object* obj_cur = new Object();
	while (std::getline(obj_file, buff)) {
		std::stringstream ss(buff);
		ss >> tmp;
		if (tmp == "v") {
			float x, y, z;
			ss >> x >> y >> z;
			obj_cur->vtxs.push_back(vtxs.size());
			mx += x, my += y, mz += z;
			vtxs.push_back(Vec3(x, y, z));
		}
		else if (tmp == "f") {
			int v1, v2, v3;
			ss >> tmp; v1 = process_face_input(tmp);
			ss >> tmp; v2 = process_face_input(tmp);
			ss >> tmp; v3 = process_face_input(tmp);
			v1 = foffset - 1 + v1; v2 = foffset - 1 + v2; v3 = foffset - 1 + v3;
			obj_cur->faces.push_back(faces.size());
			faces.push_back(Face(v1, v2, v3, objs.size()));
			if (is_calc_vt) fts.push_back((vtxs[v2] - vtxs[v1]).cross(vtxs[v3] - vtxs[v1]).normalize());//顺时针方向的右手法则
		}
		else {
			//TODO
			//printf("Unknown instruction\n");
		}
	}
	int ovtxn = obj_cur->vtxs.size();
	obj_cur->center = Vec3(mx / ovtxn, my / ovtxn, mz / ovtxn);
	objs.push_back(obj_cur);
	return obj_cur;
}

Vec3 SceneManager::light_sample(Vec3& vt, Vec3& itsc) {//向光源采样，以低采样率得到较好的结果，但时间可能与不用的时间差不多(甚至更长)，对于光源较小可用
	int cho = val_emsize(rnd);//这里只随机采样一个发光面
	int chof = emission_idx[cho];
	Face& f = faces[chof];
	float t1 = val01(rnd), t2 = val01(rnd);
	float u = std::min(t1, t2), v = std::abs(t1 - t2), w = 1 - std::max(t1, t2);
	Vec3 aim = vtxs[f.vtx1] * u + vtxs[f.vtx2] * v + vtxs[f.vtx3] * w;
	Vec3 dir = (aim - itsc).normalize();
	float cos1 = vt.dot(dir);
	if (cos1 < 0) return 0;
	int eret; Vec3 eitsc; Ray cray = Ray(itsc, dir);
	eret = ray_surface(cray, eitsc);
	if (eret != chof) return 0;
	float cos2 = dir.dot(fts[eret]);
	if (cos2 > 0) return 0;
	return objs[faces[chof].objidx]->emission*((cos1*-cos2 / itsc.distance2(eitsc))*emission_area[cho] * 1.5f);
}

void SceneManager::build_structure(int max_tri_num) {//布置好场景后务必调用此函数
	for (int i = 0; i < objs.size(); i++)
		if (objs[i]->material == Material::EMISSION) {
			for (int f : objs[i]->faces) {
				Face &ff = faces[f];
				float area = (vtxs[ff.vtx2] - vtxs[ff.vtx1]).cross(vtxs[ff.vtx3] - vtxs[ff.vtx1]).norm() / 2;
				emission_area.push_back(area);
				emission_idx.push_back(f);
			}
		}
	val_emsize = std::uniform_int_distribution<int>(0, emission_idx.size());

	this->max_tri_num = max_tri_num;
	boxes.resize(4 * faces.size());
	fsid.resize(faces.size());
	for (int i = 0; i < fsid.size(); i++)fsid[i] = i;
	_build_structure(0, faces.size(), 1, max_tri_num);
}

int SceneManager::ray_surface(Ray& ray, Vec3& intersection) {//计算射线与最近三角形的交点，并返回面索引
	int ret; Vec3 itsc;
	ret = _ray_surface(ray, 1, 0, faces.size(), itsc);
	if (ret < 0) return ret;
	intersection = vtxs[faces[ret].vtx1] * (1 - itsc.y - itsc.z) + vtxs[faces[ret].vtx2] * itsc.y + vtxs[faces[ret].vtx3] * itsc.z;
	return ret;
}

void SceneManager::move_obj(Object* obj, Vec3 dir) {
	obj->center = obj->center + dir;
	for (int vtx : obj->vtxs) {
		vtxs[vtx] = vtxs[vtx] + dir;
	}
}

void SceneManager::standizing_obj(Object* obj, float lim) {
	float scale = 0;
	for (int vtx : obj->vtxs) {
		vtxs[vtx] = vtxs[vtx] - obj->center;
		scale = std::max({ std::abs(vtxs[vtx].x), std::abs(vtxs[vtx].y), std::abs(vtxs[vtx].z) , scale });
	}
	scale = lim / scale;
	for (int vtx : obj->vtxs) {
		vtxs[vtx] = vtxs[vtx] * scale;
	}
	obj->center = Vec3();
}

int SceneManager::process_face_input(std::string& str) {
	int ret = 0;//面顶点索引
	for (int i = 0; i < str.size(); i++) {
		if (str[i] == '/') break;
		ret = ret * 10 + str[i] - '0';
	}
	return ret;
}

void SceneManager::_build_structure(int L, int R, int o, int max_tri_num) {
	for (int i = L; i < R; i++) {
		Face& f = faces[fsid[i]];
		boxes[o].update(vtxs[f.vtx1]);
		boxes[o].update(vtxs[f.vtx2]);
		boxes[o].update(vtxs[f.vtx3]);
	}
	if (R - L <= max_tri_num) return;
	float dx = boxes[o].x1 - boxes[o].x0;
	float dy = boxes[o].y1 - boxes[o].y0;
	float dz = boxes[o].z1 - boxes[o].z0;//选差值最大的轴
	if (dx > dy && dx > dz) std::sort(fsid.begin() + L, fsid.begin() + R, [=](int f1, int f2) {
		return vtxs[faces[f1].vtx1].x + vtxs[faces[f1].vtx2].x + vtxs[faces[f1].vtx3].x < vtxs[faces[f2].vtx1].x + vtxs[faces[f2].vtx2].x + vtxs[faces[f2].vtx3].x;
	});
	else if (dy > dz) std::sort(fsid.begin() + L, fsid.begin() + R, [=](int f1, int f2) {
		return vtxs[faces[f1].vtx1].y + vtxs[faces[f1].vtx2].y + vtxs[faces[f1].vtx3].y < vtxs[faces[f2].vtx1].y + vtxs[faces[f2].vtx2].y + vtxs[faces[f2].vtx3].y;
	});
	else std::sort(fsid.begin() + L, fsid.begin() + R, [=](int f1, int f2) {
		return vtxs[faces[f1].vtx1].z + vtxs[faces[f1].vtx2].z + vtxs[faces[f1].vtx3].z < vtxs[faces[f2].vtx1].z + vtxs[faces[f2].vtx2].z + vtxs[faces[f2].vtx3].z;
	});
	int mid = (L + R) >> 1;
	_build_structure(L, mid, o * 2, max_tri_num);
	_build_structure(mid, R, o * 2 + 1, max_tri_num);
}

int SceneManager::_ray_surface(Ray& ray, int o, int L, int R, Vec3& intersection) {//intersection(t, u, v)
	if (R - L <= max_tri_num) {
		//printf("%d %d\n", L, R);
		float mint = FLT_MAX; int idx = -1;
		for (int i = L; i < R; i++) {//计算交点
			Face& f = faces[fsid[i]];
			Vec3 e1 = vtxs[f.vtx2] - vtxs[f.vtx1];
			Vec3 e2 = vtxs[f.vtx3] - vtxs[f.vtx1];
			Vec3 t = ray.o - vtxs[f.vtx1];
			Vec3 p = ray.dir.cross(e2);
			Vec3 q = t.cross(e1);
			float tt = p.dot(e1);
			if (std::abs(tt) < 1e-6) continue;
			Vec3 itsc = Vec3(q.dot(e2), p.dot(t), q.dot(ray.dir))*(1.0f / tt);
			if (itsc.x < 1e-4f || itsc.x>mint || itsc.y < 0 || itsc.z < 0 || itsc.y + itsc.z>1) continue;//tt < 1e-8f防止反射时碰到自己
			mint = itsc.x; idx = fsid[i];
			intersection = itsc;
		}
		return idx;
	}
	else {
		float tmin, tmax;
		ray.check_collide_box(boxes[o], tmin, tmax);
		//printf("%f, %f\n", tmin, tmax);
		if (tmax < tmin || tmax < 0)
			return -1;
		int mid = (L + R) >> 1;
		Vec3 itsc1, itsc2;
		int ret1 = _ray_surface(ray, o * 2, L, mid, itsc1);
		int ret2 = _ray_surface(ray, o * 2 + 1, mid, R, itsc2);
		if (ret1 >= 0 && ret2 >= 0) {
			if (itsc1.x < itsc2.x) {
				intersection = itsc1;
				return ret1;
			}
			else {
				intersection = itsc2;
				return ret2;
			}
		}
		else if (ret1 >= 0) { intersection = itsc1; return ret1; }
		else if (ret2 >= 0) { intersection = itsc2; return ret2; }
		else return -1;
	}
}