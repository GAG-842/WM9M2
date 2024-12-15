#include "window.h"
#include "MdxCore.h"
#include "mesh.h"
#include "shader.h"
#include "GamesEngineeringBase.h"
#include "Animation.h"
#include "camera.h"
string loadfile(string filename) {
	stringstream buffer;
	ifstream file(filename);
	buffer << file.rdbuf();
	file.close();
	return buffer.str();
}
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	Window canvas;
	DxCore dx;
	shader sdStatic;
	shader sdAnimation;
	GamesEngineeringBase::Timer tim;
	canvas.Init("MyWindow", 1024, 768);
	dx.Init(1024, 768, canvas.hwnd);
	float time = 0.0f;


	//triangle
	// 	Triangle t;
	// 	t.init(&dx);
	//sd.load(&dx, "Resources/vertexshader.txt", "Resources/pixelshader.txt");


	//plane
	plane p;
	p.init(&dx);
	sdStatic.loadStatic(&dx, "Resources/worldvertex.txt", "Resources/TexturePS.txt");

	//cube
	//cube cu;
	//cu.init(&dx);
	//sd.loadStatic(&dx, "Resources/worldvertex.txt", "Resources/worldpixel.txt");
	//

	//sky
	sphere s;
	s.init(&dx, 100, 100, 10000.f);
	sdStatic.loadStatic(&dx, "Resources/worldvertex.txt", "Resources/TexturePS.txt");

	//tree
	tree pine;
	pine.init(&dx, "Resources/pine.gem");
	sdStatic.loadStatic(&dx, "Resources/worldvertex.txt", "Resources/TexturePS.txt");
	tree tr;
	tr.init(&dx, "Resources/acacia_003.gem");
	sdStatic.loadStatic(&dx, "Resources/worldvertex.txt", "Resources/TexturePS.txt");

	//dinosaur
	AnimationDinosaur la;
	la.init(&dx, "Resources/TRex.gem");
	sdAnimation.loadAnimation(&dx, "Resources/AnimationVS.txt", "Resources/TexturePS.txt");

	//soldier
	AnimationPlayer player;
	player.init(&dx, "Resources/Soldier.gem");
	sdAnimation.loadAnimation(&dx, "Resources/AnimationVS.txt", "Resources/TexturePS.txt");

	// Cannot run, even use class AnimationPlayer
	//weapon
	//tree carbine;
	//carbine.init(&dx, "Resources/Uzi.gem");
	//sdStatic.loadStatic(&dx, "Resources/worldvertex.txt", "Resources/TexturePS.txt");


	camera c;
	c.init(canvas.hwnd);
	bool running = true;
	while (running) {
		canvas.processMessages();
		dx.clear();
		float dt = tim.dt();
		time += dt;
		//t.draw(&sd, &dx, time);
		p.draw(&sdStatic, &dx,time);
		//cu.draw(&sd, &dx, time);
		s.draw(&sdStatic, &dx, time);
		for (int i = 1; i < 20; i++) {
			for (int j = 1; j < 20; j++) {
				pine.draw(&sdStatic, &dx, Vec3(i * 20 - 200, 0, j * 20 - 200));
				tr.draw(&sdStatic, &dx, Vec3(i * 20 - 190, 0, j * 20 - 190));
			}
			
		}
		/*carbine.draw(&sdStatic, &dx, Vec3(5, 1.8, 5));*/
		player.handleMovement(c);
		player.draw(&sdAnimation, &dx, dt, &c, &canvas);
		la.draw(&sdAnimation, &dx, dt, c.position);
		c.update(&canvas, &sdAnimation);
		c.update(&canvas, &sdStatic);
		if (canvas.keyPressed(VK_ESCAPE)) { break; }
		dx.present();
	}
}