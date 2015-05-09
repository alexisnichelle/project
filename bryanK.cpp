/*
 * cs335 project v1
 * team4
 * codename "uberdude"
 * cpp implementation
 * */

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

//prototype char draw, rectangle to be replaced with sprite
void drawCharacter(float x, float y, int w, int h){
    glPushMatrix();
    glColor3ub(150,160,220);
    //draw quad as temp to allow for physics implementation
    glBegin(GL_QUADS);
        glVertex2i(x-w,y-h);
        glVertex2i(x-w,y+h);
        glVertex2i(x+w,y+h);
        glVertex2i(x+w,y-h);
    glEnd();
    glPopMatrix();
}





//builds alpha layer data for ppm file, converts black to transparent on sprite
unsigned char *buildAlphaData(Ppmimage *img)
{
    //modified from rainforrest build
    int a,b,c;
    unsigned char *newdata, *ptr;
    unsigned char *data = (unsigned char *)img->data;
    newdata = new unsigned char[img->width *img->height * 4];
    ptr = newdata;
    for (int i=0; i<img->width * img->height * 3; i+3) {
        a = *(data+0);
        b = *(data+1);
        c = *(data+2);
        *(ptr+0) = a;
        *(ptr+1) = b;
        *(ptr+2) = c;

        //set alpha to 1 (white) unless all three values are 0
        *(ptr+3) = (a|b|c);
        ptr+= 4;
        data+= 3;
    }
    return newdata;
}


