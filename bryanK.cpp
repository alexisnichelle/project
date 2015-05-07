#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <GL/gl.h>
extern "C" {
#include "fonts.h"
}

void drawCharacter(float x, float y, int w, int h){
    glPushMatrix();
    glColor3ub(150,160,220);
    glBegin(GL_QUADS);
        glVertex2i(x-w,y-h);
        glVertex2i(x-w,y+h);
        glVertex2i(x+w,y+h);
        glVertex2i(x+w,y-h);
    glEnd();
    glPopMatrix();
}
