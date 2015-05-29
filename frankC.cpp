#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <GL/gl.h>
#include "ppm.h"
extern "C" {
#include "fonts.h"
}
#include "mystruct.h"


void level(Shape *platform, int &num) {

	platform[27].width = 100;
	platform[27].height = 10;
        platform[27].center.x = 3000;
	platform[27].center.y = 200;
	num++;

	platform[28].width = 100;
	platform[28].height = 10;
        platform[28].center.x = 3200;
	platform[28].center.y = 200;
	num++;
}

/*void makeEnemy(Boss *boss, Projectile *proj) {
	Boss enemy;
	*boss = enemy;
//	Projectile p = new Projectile[10];


}*/
