#include"Camera.h"
#include"SceneManager.h"
#include<graphics.h>

SceneManager* SceneManager::_instance = new SceneManager();
SceneManager* _ins = SceneManager::instance();

void scene1() {
	Object*obj10 = _ins->load_obj("scenes/scene1/obj1-0.txt", true);
	Object*obj11 = _ins->load_obj("scenes/scene1/obj1-1.txt", true);
	Object*obj12 = _ins->load_obj("scenes/scene1/obj1-2.txt", true);
	obj10->color = Vec3(1, 1, 1);
	obj10->material = Material::SPECULAR;
	obj11->color = Vec3(0.1, 0.1, 1);
	obj11->material = Material::DIFFUSION;
	obj12->color = Vec3(1, 0.6, 0.6);
	obj12->material = Material::DIFFUSION;
	Object* obj2 = _ins->load_obj("scenes/scene2/cube.txt", true);
	obj2->color = Vec3(1, 1, 1);
	_ins->move_obj(obj2, Vec3(0.3, 2.8, 0));
	obj2->material = Material::SPECULAR;
	Object* obj21 = _ins->load_obj("scenes/scene2/cube.txt", true);
	obj21->color = Vec3(1, 1, 1);
	_ins->move_obj(obj21, Vec3(3.4, 1.7, 0));
	obj21->rfcn = 1.5f;
	obj21->material = Material::REFRACTION;
	Object* obj22 = _ins->load_obj("scenes/scene2/cube.txt", true);
	obj22->color = Vec3(0.6, 1, 0.3);
	obj22->emission = Vec3(1.2, 1.2, 1.2);
	_ins->move_obj(obj22, Vec3(2, 3.8, 0));
	obj22->material = Material::EMISSION;
	Object* obj3 = _ins->load_obj("scenes/scene2/cube.txt", true);
	obj3->emission = Vec3(2.5, 2.5, 2.5);
	_ins->move_obj(obj3, Vec3(0.5, 0, 1));
	obj3->material = Material::EMISSION;
	Object* obj4 = _ins->load_obj("scenes/scene2/cube.txt", true);
	obj4->emission = Vec3(2.5, 2.5, 2.5);
	_ins->move_obj(obj4, Vec3(6, 2.1, 0));
	obj4->material = Material::EMISSION;
	Object* obj5 = _ins->load_obj("scenes/scene2/cube.txt", true);
	_ins->move_obj(obj5, Vec3(4, 5.1, 0));
	obj5->color = Vec3(0.2, 1, 0.2);
	obj5->material = Material::DIFFUSION;
}

void scene2() {
	Object*bk = _ins->load_obj("scenes/scene2/bk.txt", true);
	bk->color = Vec3(1, 1, 1);
	bk->emission = Vec3(1, 1, 1);
	bk->material = Material::EMISSION;
	Object*cube = _ins->load_obj("scenes/scene2/cube.txt", true);
	cube->material = Material::REFRACTION;
	cube->rfcn = 1.5f;
	cube->color = Vec3(1, 1, 1);
	_ins->move_obj(cube, Vec3(0.1, 4.5, 4.5));
}

int main() {
	scene1();
	Camera camera(Vec3(12.5, 6, 6), Vec3(-1, -0.3, -0.5));
	//Camera camera(Vec3(4, 5, 5), Vec3(-1, 0, 0));
	_ins->build_structure(4);
	int w = 1024, h = 768;
	Vec3 *cols = camera.render(w, h, 60, 28, 64);
	initgraph(w, h, 1);
	
	printf("Start drawing\n");
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			Vec3 c = cols[j + i * w];
			//printf("%d, %d, %d %d %d\n", i, j, (int)c.x, (int)c.y, (int)c.z);
			putpixel(j, i, RGB((int)c.x, (int)c.y, (int)c.z));
		}
	}
	printf("Drawing complete!\n");
	system("pause");
	return 0;
}