/* 
 * Alexis Ragus
 * cs335 project: Uberdude
 * sound
 * beginning of level 2
 * textures for background, platforms, title & game over screen
 */

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
#include "mystruct.h"
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define numbox 3

#define USE_SOUND
#ifdef USE_SOUND
#include <FMOD/fmod.h>
#include <FMOD/wincompat.h>
#include "fmod.h"
#endif

Ppmimage *titleImage=NULL;
Ppmimage *overImage=NULL;
Ppmimage *levelImage=NULL;
Ppmimage *winnerImage=NULL;
GLuint winnerTexture;
GLuint titleTexture;
GLuint levelTexture;
GLuint overTexture;
int title = 1;
int gameOver = 1;
int level = 1;
//int winner = 1;


void text(void)
{ 
    Rect r;
    r.bot = 260;
    r.left = 325;
    r.center = 0;
    ggprint13(&r,36,0x00cdc2c2,"C l i c k   t o   S t a r t !");

}

void platforms(void)
{
    //Clear the screen
    glClearColor(1.0, 1.0, 1.0, 1.0);
    //load the images file into a ppm structure.
    levelImage = ppm6GetImage("./images/level.ppm");
    //create opengl texture elements
    glGenTextures(1, &levelTexture);

    //level
    glBindTexture(GL_TEXTURE_2D, levelTexture);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
            levelImage->width, levelImage->height,
            0, GL_RGB, GL_UNSIGNED_BYTE, levelImage->data);

}

void drawPlatforms(float w, float h)
{

    glColor3f(1.0, 1.0, 1.0);
    if (level) {
        glBindTexture(GL_TEXTURE_2D, levelTexture);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-w, -h);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, h);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(w, h);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(w, -h);
        glEnd();
    }

}


void texture(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    //glClear(GL_COLOR_BUFFER_BIT);
    titleImage = ppm6GetImage("./images/uberdude.ppm");
    //create opengl texture elements
    glGenTextures(1, &titleTexture);

    //maverick
    glBindTexture(GL_TEXTURE_2D, titleTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
            titleImage->width, titleImage->height,
            0, GL_RGB, GL_UNSIGNED_BYTE, titleImage->data);


    //initialize fonts
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();
}

void r_texture()
{

    //texture background
    glColor3f(1.0, 1.0, 1.0);
    if (title) {
        glBindTexture(GL_TEXTURE_2D, titleTexture);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(0, WINDOW_HEIGHT);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(WINDOW_WIDTH, WINDOW_HEIGHT);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(WINDOW_WIDTH, 0);
        glEnd();
    }

}

//this was our original sound function but was changed during the last few days of the quarter
//this is no longer being called
void createsounds(void)
{
#ifdef USE_SOUND
    //FMOD_RESULT result;
    if (fmod_init()) {
        printf("ERROR - createsounds\n\n");
        return;
    }
    if (fmod_createsound((const char *)"./sounds/HeatMan.mp3", 0)) {
        printf("ERROR - createsounds\n\n");
        return;
    }
    /*if (fmod_createsound((const char *)"./sounds/game.mp3", 1)) {
        printf("ERROR - fmod_createsound()\n\n");
        return;
    }
    if (fmod_createsound((const char *)"./sounds/game_over.mp3", 2)) {
        printf("ERROR - fmod_createsound()\n\n");
        return;
    } */
    fmod_setmode(0,FMOD_LOOP_NORMAL);
    //fmod_systemupdate();
#endif //USE_SOUND
}

void playsound(void)
{
#ifdef USE_SOUND
    fmod_playsound(0);
#endif
}

void play_gameOver(void)
{
#ifdef USE_SOUND
    fmod_playsound(2);
#endif
}

void game_over()
{
    //Clear the screen
    //glClearColor(1.0, 1.0, 1.0, 1.0);
    //glClear(GL_COLOR_BUFFER_BIT);
    //load the images file into a ppm structure.
    overImage = ppm6GetImage("./images/game_over.ppm");
    //create opengl texture elements
    glGenTextures(1, &overTexture);

    //game over
    glBindTexture(GL_TEXTURE_2D, overTexture);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
            overImage->width, overImage->height,
            0, GL_RGB, GL_UNSIGNED_BYTE, overImage->data);


}

void r_gameOver()
{
    //texture background
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, overTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(0, WINDOW_HEIGHT);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(WINDOW_WIDTH, WINDOW_HEIGHT);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(WINDOW_WIDTH, 0);
    glEnd();

}

void winner_screen()
{
    //Clear the screen
    //glClearColor(1.0, 1.0, 1.0, 1.0);
    winnerImage = ppm6GetImage("./images/winner.ppm");
    //create opengl texture elements
    glGenTextures(1, &winnerTexture);

    //game over
    glBindTexture(GL_TEXTURE_2D, winnerTexture);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
            winnerImage->width, winnerImage->height,
            0, GL_RGB, GL_UNSIGNED_BYTE, winnerImage->data);


}

void r_winner()
{
    //texture background
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, winnerTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(0, WINDOW_HEIGHT);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(WINDOW_WIDTH, WINDOW_HEIGHT);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(WINDOW_WIDTH, 0);
    glEnd();

}

void level2(Shape *platform, int &num) 
{
	//start level 2 platform
    platform[136].width = 100;
	platform[136].height = 20;
    platform[136].center.x = 5000;
	platform[136].center.y = 100;
	num++;

	platform[137].width = 100;
	platform[137].height = 20;
    platform[137].center.x = 5200;
	platform[137].center.y = 100;
	num++;

	platform[138].width = 100;
	platform[138].height = 20;
    platform[138].center.x = 5400;
	platform[138].center.y = 100;
	num++;

	platform[139].width = 100;
	platform[139].height = 20;
    platform[139].center.x = 5600;
	platform[139].center.y = 100;
	num++;
	//
    platform[140].width = 100;
	platform[140].height = 10;
    platform[140].center.x = 5800;
	platform[140].center.y = 300;
	num++;
    
    platform[141].width = 100;
	platform[141].height = 10;
    platform[141].center.x = 5850;
	platform[141].center.y = 280;
	num++;
    //column
    platform[142].width = 50;
	platform[142].height = 10;
    platform[142].center.x = 6200;
	platform[142].center.y = 200;
	num++;

    platform[143].width = 50;
	platform[143].height = 10;
    platform[143].center.x = 6200;
	platform[143].center.y = 215;
	num++;

    platform[144].width = 50;
	platform[144].height = 10;
    platform[144].center.x = 6200;
	platform[144].center.y = 230;
	num++;

    platform[145].width = 50;
	platform[145].height = 10;
    platform[145].center.x = 6200;
	platform[145].center.y = 250;
	num++;
    //
    platform[146].width = 100;
	platform[146].height = 10;
    platform[146].center.x = 6500;
	platform[146].center.y = 50;
	num++;

    platform[147].width = 100;
	platform[147].height = 10;
    platform[147].center.x = 6700;
	platform[147].center.y = 100;
	num++;
    
    platform[148].width = 100;
	platform[148].height = 10;
    platform[148].center.x = 6800;
	platform[148].center.y = 300;
	num++;
    //
    platform[149].width = 50;
	platform[149].height = 10;
    platform[149].center.x = 6800;
	platform[149].center.y = 315;
	num++;

    platform[150].width = 50;
	platform[150].height = 10;
    platform[150].center.x = 6800;
	platform[150].center.y = 325;
	num++;
    
    platform[151].width = 50;
	platform[151].height = 10;
    platform[151].center.x = 6800;
	platform[151].center.y = 335;
	num++;
    
    platform[152].width = 100;
	platform[152].height = 10;
    platform[152].center.x = 6900;
	platform[152].center.y = 300;
	num++;
    //
    platform[153].width = 100;
	platform[153].height = 25;
    platform[153].center.x = 7200;
	platform[153].center.y = 10;
	num++;

    platform[154].width = 100;
	platform[154].height = 25;
    platform[154].center.x = 7400;
	platform[154].center.y = 10;
	num++;

    platform[155].width = 50;
	platform[155].height = 25;
    platform[155].center.x = 7450;
	platform[155].center.y = 50;
	num++;

    platform[156].width = 50;
	platform[156].height = 25;
    platform[156].center.x = 7450;
	platform[156].center.y = 100;
	num++;

    platform[157].width = 50;
	platform[157].height = 25;
    platform[157].center.x = 7450;
	platform[157].center.y = 150;
	num++;

    platform[158].width = 25;
	platform[158].height = 25;
    platform[158].center.x = 7475;
	platform[158].center.y = 175;
	num++;

    platform[159].width = 25;
	platform[159].height = 25;
    platform[159].center.x = 7475;
	platform[159].center.y = 200;
	num++;

    platform[160].width = 25;
	platform[160].height = 25;
    platform[160].center.x = 7475;
	platform[160].center.y = 250;
	num++;

    platform[161].width = 25;
	platform[161].height = 25;
    platform[161].center.x = 7475;
	platform[161].center.y = 300;
	num++;

    platform[162].width = 25;
	platform[162].height = 25;
    platform[162].center.x = 7475;
	platform[162].center.y = 350;
	num++;

    platform[163].width = 25;
	platform[163].height = 15;
    platform[163].center.x = 7475;
	platform[163].center.y = 375;
	num++;
    //
    platform[164].width = 100;
	platform[164].height = 10;
    platform[164].center.x = 7700;
	platform[164].center.y = 500;
	num++;

    platform[165].width = 100;
	platform[165].height = 10;
    platform[165].center.x = 7900;
	platform[165].center.y = 400;
	num++;

    platform[166].width = 100;
	platform[166].height = 10;
    platform[166].center.x = 8200;
	platform[166].center.y = 200;
	num++;

    platform[167].width = 100;
	platform[167].height = 15;
    platform[167].center.x = 8000;
	platform[167].center.y = 10;
	num++;

    platform[168].width = 100;
	platform[168].height = 15;
    platform[168].center.x = 8200;
	platform[168].center.y = 10;
	num++;

    platform[169].width = 100;
	platform[169].height = 15;
    platform[169].center.x = 8400;
	platform[169].center.y = 10;
	num++;

}
