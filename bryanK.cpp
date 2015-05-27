/*
 * Bryan Kiggens
 * cs335 project v1
 * team4
 * codename "uberdude"
 * cpp implementation
 * */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <GL/gl.h>
#include "ppm.h"
#include "bryanK.h"
extern "C" {
#include "fonts.h"
}
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
Ppmimage *bgImage = NULL;
GLuint bgTexture;
int bg = 1;
struct timespec timeCharacter;
struct timespec timeCurrent;
struct timespec timeStart;
double oobillion = 1.0/1e9;

double timeDiff(struct timespec *start, struct timespec *end) {
     return (double) (end->tv_sec - start->tv_sec) +
         (double) (end->tv_nsec - start->tv_nsec) * oobillion;
}


void timeCopy(struct timespec *destination, struct timespec *source){
    memcpy(destination, source, sizeof(struct timespec));
}
//prototype char draw, rectangle to be replaced with sprite
void drawCharacter(float x, float y, int w, int h){
    glPushMatrix();
    double curanim;
    int curanimtime;
    curanim = timeDiff(&timeCharacter, &timeCurrent);
    curanimtime = (int) curanim;
    curanimtime = curanimtime%3;
    glColor3ub(75 * curanimtime,160,220);
    //draw quad as temp to allow for physics implementation
    glBegin(GL_QUADS);
    glVertex2i(x-w,y-h);
    glVertex2i(x-w,y+h);
    glVertex2i(x+w,y+h);
    glVertex2i(x+w,y-h);
    glEnd();
    glPopMatrix();
}

void drawIdleSprite(float x, float y, int w, int h){
    //add shit here
    //
    //stupid warning removal
    x=x;y=y;w=w;h=h;
}

void drawRunningSprite(float x, float y, int w, int h){
    //add shit here too
    //
    //stupid warning removal
    x=x;y=y;w=w;h=h;
}

void buildBackgroundImage(void) {
    //clear the screen
    glClearColor(1.0,1.0,1.0,1.0);
    bgImage = ppm6GetImage("./images/bgimage.ppm");
    glGenTextures(1, &bgTexture);

    glBindTexture(GL_TEXTURE_2D, bgTexture);


    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,0,3,bgImage->width,bgImage->height,0,
            GL_RGB,GL_UNSIGNED_BYTE,bgImage->data);
}

void tileBackground(void) {
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0,1.0,1.0);
    if(bg){
        for(int i = -5; i < 5; i++){
            for( int j = -4; j < 21; j++){
                int y = i * 300;
                int x = j*300;
                glBindTexture(GL_TEXTURE_2D, bgTexture);
                glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f); glVertex2i(x, y);
                glTexCoord2f(0.0f, 0.0f); glVertex2i(x, y+300);
                glTexCoord2f(1.0f, 0.0f); glVertex2i(x+300, y+300);
                glTexCoord2f(1.0f, 1.0f); glVertex2i(x+300, y);
                glEnd();
            }
        }
    }
    glDisable(GL_TEXTURE_2D);
}

void centerCamera(int left,int right, int bottom, int top){
    //clear scrub warnings
    top = top; bottom = bottom;
    //end warning clear
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glViewport(left,bottom,WINDOW_WIDTH,WINDOW_HEIGHT);
    //gluOrtho2D(left, right, bottom, top);
    glOrtho(left, right, 0, WINDOW_HEIGHT,-1,1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}










//builds alpha layer data for ppm file, converts black to transparent on sprite
unsigned char *buildAlphaData(Ppmimage *img)
{
    int a,b,c;
    unsigned char *newdata, *ptr;
    unsigned char *data = (unsigned char *)img->data;
    newdata = new unsigned char[img->width *img->height * 4];
    ptr = newdata;
    for (int i=0; i<img->width * img->height * 3; i+=3) {
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


