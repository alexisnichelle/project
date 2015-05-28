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
#include <string.h>//seriously? memcpy is in string?
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
GLuint charTexture[14];
GLuint idleTexture[6];
GLuint cprojTexture[4];
GLuint silhouetteTexture;
Ppmimage *charImage[14];
Ppmimage *idleImage[6];
Ppmimage *cprojImage[4];
int bg = 1;
struct timespec timeCharacter;
struct timespec timeCurrent;
struct timespec timeStart;
double oobillion = 1.0/1e9;//for teh nanoseconds


//function that returns the difference in time between start and end
double timeDiff(struct timespec *start, struct timespec *end)
{
    return (double) (end->tv_sec - start->tv_sec) +
        (double) (end->tv_nsec - start->tv_nsec) * oobillion;
}


//functionn to copy time structures over
void timeCopy(struct timespec *destination, struct timespec *source)
{
    memcpy(destination, source, sizeof(struct timespec));
}


//draws sprite of character running, uses alpha blengind to make background of sprite
//transparent
void drawRunningSprite(float x, float y, int w, int h)
{
    double curanim;
    int curanimtime;
    curanim = timeDiff(&timeCharacter, &timeCurrent);
    curanimtime = (int) curanim;
    curanimtime = curanimtime%14;
    glColor3ub(255,255,255);//set color to pure white to avoid blending 
    
    //draw a recangle at character's position using run sprite
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, charTexture[curanimtime]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,1.0f); glVertex2i(x-w,y-h);
    glTexCoord2f(0.0f,0.0f); glVertex2i(x-w,y+h);
    glTexCoord2f(1.0f,0.0f); glVertex2i(x+w,y+h);
    glTexCoord2f(1.0f,1.0f); glVertex2i(x+w,y-h);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    //disable alpha blending to avoid conflict on other draw functions
    glDisable(GL_ALPHA_TEST);
    //glDisable(GL_TEXTURE_2D);
    glPopMatrix();


}


//make animated character projectiles
void drawCharProjSprite(float x, float y, int w, int h)
{
    double curanim;
    int curanimtime;
    curanim = timeDiff(&timeCharacter, &timeCurrent);
    curanimtime = (int) curanim;
    curanimtime = curanimtime%4;
    glColor3ub(255,255,255);//set color to pure white to avoid blending 
    
    //draw a recangle at character's position using run sprite
    glPushMatrix();
    //glTranslatef(x,y,0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, cprojTexture[curanimtime]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,1.0f); glVertex2i(x-w,y-h);
    glTexCoord2f(0.0f,0.0f); glVertex2i(x-w,y+h);
    glTexCoord2f(1.0f,0.0f); glVertex2i(x+w,y+h);
    glTexCoord2f(1.0f,1.0f); glVertex2i(x+w,y-h);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    //disable alpha blending to avoid conflict on other draw functions
    glDisable(GL_ALPHA_TEST);
    //glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}


//function that draws an animated idle sprite at characters location, uses
//alpha blending for background transparency
void drawIdleSprite(float x, float y, int w, int h)
{
    double curanim;
    int curanimtime;
    curanim = timeDiff(&timeCharacter, &timeCurrent);
    curanimtime = (int) curanim;
    curanimtime = curanimtime%2;
    glColor3ub(255,255,255);//set color to pure white to avoid blending
    
    //draw a recangle at character's position using idle sprite
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, idleTexture[curanimtime]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,1.0f); glVertex2i(x-w,y-h);
    glTexCoord2f(0.0f,0.0f); glVertex2i(x-w,y+h);
    glTexCoord2f(1.0f,0.0f); glVertex2i(x+w,y+h);
    glTexCoord2f(1.0f,1.0f); glVertex2i(x+w,y-h);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    //disable alpha blending to avoid conflict on other draw functions
    glDisable(GL_ALPHA_TEST);
    //glDisable(GL_TEXTURE_2D);
    glPopMatrix();

}


//testing function, used when prototyping new draw methods to avoid conflict
void drawCharacter(float x, float y, int w, int h){
    //stupid warning removal
    x=x;y=y;w=w;h=h;
}


//function that loads a tileable background image from a file and prepares it to use
//as a background texture
void buildBackgroundImage(void) 
{
    //clear the screen
    glClearColor(1.0,1.0,1.0,1.0);
    bgImage = ppm6GetImage("./images/bgimage.ppm");//load a TILEABLE backgroun image
    glGenTextures(1, &bgTexture);
    glBindTexture(GL_TEXTURE_2D, bgTexture);
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,0,3,bgImage->width,bgImage->height,0,
            GL_RGB,GL_UNSIGNED_BYTE,bgImage->data);
}


//function that generates a background using a tileable image
void tileBackground(void) 
{
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
    //disable texture after finished to prevent conflict with other draw functions
    glDisable(GL_TEXTURE_2D);
}


//function that changes the clipping plane of the orthogonal projection to give
//a sense of a camera following the main character
void centerCamera(int left,int right, int bottom, int top)
{
    //clear scrub warnings
    top = top; 
    bottom = bottom;
    //end warning clear
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glViewport(left,bottom,WINDOW_WIDTH,WINDOW_HEIGHT);
    //gluOrtho2D(left, right, bottom, top);
    glOrtho(left, right, 0, WINDOW_HEIGHT,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


//function that adds an alpha layer to loaded ppm images
//turns pure black pixels to transparent alpha values
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


//function that loads in running sprite images for character from files,
//converts them into a texture array for usage during draw calls
void buildCharImage()
{
    charImage[0] = ppm6GetImage("./images/run1.ppm");
    charImage[1] = ppm6GetImage("./images/run2.ppm");
    charImage[2] = ppm6GetImage("./images/run3.ppm");
    charImage[3] = ppm6GetImage("./images/run4.ppm");
    charImage[4] = ppm6GetImage("./images/run5.ppm");
    charImage[5] = ppm6GetImage("./images/run6.ppm");
    charImage[6] = ppm6GetImage("./images/run7.ppm");
    charImage[7] = ppm6GetImage("./images/run8.ppm");
    charImage[8] = ppm6GetImage("./images/run9.ppm");
    charImage[9] = ppm6GetImage("./images/run10.ppm");
    charImage[10] = ppm6GetImage("./images/run11.ppm");
    charImage[11] = ppm6GetImage("./images/run12.ppm");
    charImage[12] = ppm6GetImage("./images/run13.ppm");
    charImage[13] = ppm6GetImage("./images/run14.ppm");
    for(int i = 0; i < 14; i++)
    {
        glGenTextures(1, &charTexture[i]);
        glGenTextures(1, &silhouetteTexture);
        //based on character width and height
        //ensure images are correct size
        int w = charImage[i]->width;
        int h = charImage[i]->height;
        glBindTexture(GL_TEXTURE_2D, charTexture[i]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3 ,w ,h , 0, 
                GL_RGB, GL_UNSIGNED_BYTE, charImage[i]->data);
        unsigned char *silhouetteData = buildAlphaData(charImage[i]);
        glTexImage2D(GL_TEXTURE_2D, 0 ,GL_RGBA, w, h, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
        free(silhouetteData);
    }
}


//function that loads in idle animation sprite images for character from files,
//converts them into a texture array for usage during draw calls
void buildIdleImage()
{
    idleImage[0] = ppm6GetImage("./images/idle1.ppm");
    idleImage[1] = ppm6GetImage("./images/idle2.ppm");
    idleImage[2] = ppm6GetImage("./images/idle3.ppm");
    idleImage[3] = ppm6GetImage("./images/idle4.ppm");
    for(int i = 0; i < 4; i++)
    {
        glGenTextures(1, &idleTexture[i]);
        glGenTextures(1, &silhouetteTexture);
        //based on character width and height
        //make sure images are of correct size
        int w = idleImage[i]->width;
        int h = idleImage[i]->height;
        glBindTexture(GL_TEXTURE_2D, idleTexture[i]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3 ,w ,h , 0, 
                GL_RGB, GL_UNSIGNED_BYTE, idleImage[i]->data);
        unsigned char *silhouetteData = buildAlphaData(idleImage[i]);
        glTexImage2D(GL_TEXTURE_2D, 0 ,GL_RGBA, w, h, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
        free(silhouetteData);
    }
}


//function that loads in images for animated projectilesfrom files
//then converts them into a texture array for usage in draw calls
void buildProjImage()
{
    //load character projectiles here
    cprojImage[0] = ppm6GetImage("./images/cbullet1.ppm");
    cprojImage[1] = ppm6GetImage("./images/cbullet2.ppm");
    cprojImage[2] = ppm6GetImage("./images/cbullet3.ppm");
    cprojImage[3] = ppm6GetImage("./images/cbullet4.ppm");
    for(int i = 0; i < 4; i++)
    {
        glGenTextures(1, &cprojTexture[i]);
        glGenTextures(1, &silhouetteTexture);
        //based on character width and height
        //make sure images are of correct size
        int w = cprojImage[i]->width;
        int h = cprojImage[i]->height;
        glBindTexture(GL_TEXTURE_2D, cprojTexture[i]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3 ,w ,h , 0, 
                GL_RGB, GL_UNSIGNED_BYTE, cprojImage[i]->data);
        unsigned char *silhouetteData = buildAlphaData(cprojImage[i]);
        glTexImage2D(GL_TEXTURE_2D, 0 ,GL_RGBA, w, h, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
        free(silhouetteData);
    }


}
