//Program: CS335 Lab 6 / Homework 3
//Author: Alexis Ragus
//Purpose: Refractoring of CS335 project
//

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/gl.h>
#include "ppm.h"
extern "C" {
#include "fonts.h"
}
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define numbox 3

Ppmimage *maverickImage=NULL;
GLuint maverickTexture;
int maverick = 1;

void texture(void)
{
    //Clear the screen
    glClearColor(1.0, 1.0, 1.0, 1.0);
    //glClear(GL_COLOR_BUFFER_BIT);
    //load the images file into a ppm structure.
    maverickImage = ppm6GetImage("./images/maverick.ppm");
    //create opengl texture elements
    glGenTextures(1, &maverickTexture);

    //---------------------------------------------------------------------
    //maverick
    glBindTexture(GL_TEXTURE_2D, maverickTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
            maverickImage->width, maverickImage->height,
            0, GL_RGB, GL_UNSIGNED_BYTE, maverickImage->data);


    //initialize fonts
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();
}

void r_texture()
{

    //texture background
    glColor3f(1.0, 1.0, 1.0);
    if (maverick) {
        glBindTexture(GL_TEXTURE_2D, maverickTexture);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(0, WINDOW_HEIGHT);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(WINDOW_WIDTH, WINDOW_HEIGHT);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(WINDOW_WIDTH, 0);
        glEnd();
    }

}

