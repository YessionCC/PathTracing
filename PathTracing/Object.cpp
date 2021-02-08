#include"Object.h"
#include"SceneManager.h"

void Object::move(Vec3 dir) {
	SceneManager::instance()->move_obj(this, dir);
}

void Object::std_scale(int scale) {
	SceneManager::instance()->standizing_obj(this, scale);
}

Object * Object::load_obj(const char path[], bool is_calc_vt) {
	return SceneManager::instance()->load_obj(path, is_calc_vt);
}
