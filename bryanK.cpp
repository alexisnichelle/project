/*
 * Bryan Kiggens
 * cs335 project v1
 * team4
 * codename "uberdude"
 * cpp implementation
 * collision+physics in main file
 * simultaneous input in main file
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
#include "mystruct.h"
#include "ppm.h"
#include "bryanK.h"
extern "C" {
#include "fonts.h"
}
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
Ppmimage *bgImage = NULL;
Ppmimage *bgImageTwo = NULL;
GLuint bgTextureTwo;
GLuint bgTexture;
GLuint charTexture[17];
GLuint idleTexture[6];
GLuint cprojTexture[9];
GLuint silhouetteTexture;
Ppmimage *charImage[17];
Ppmimage *idleImage[6];
Ppmimage *cprojImage[9];
int bg = 1;
static int bossShotPattern = 0;
struct timespec timeCharacter;
struct timespec timeBossShot;
struct timespec timeEnemyShot;
struct timespec timeCurrent;


struct timespec timeStart;
double oobillion = 1.0/1e9;//for teh nanoseconds
double bossDelay = 0.0;
double enemyDelay = 2.0;

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

void fireProjectile(struct  Projectile *projectile, int &n, float x, float y, float xvel, float yvel){
        Projectile *p = &projectile[n];
        p->r = 100;
        p->g = 100;
        p->b = 100;
        p->type = 1;
        p->s.center.x = x;
        p->s.center.y = y;
        p->s.width = 14;
        p->s.height = 14;
        p->velocity.y = yvel;
        p->velocity.x = xvel;
        n++;
        //clock_gettime(CLOCK_REALTIME, &timeBullet);

}

void enemyFire(struct Projectile * projectile, int &numproj, int x, int y, float xvel, float yvel,int w, int type){
    Projectile *p = &projectile[numproj];
        p->r = 100;
        p->g = 100;
        p->b = 100;
        p->type = type;
        if(xvel > 0){
        p->s.center.x = x+w+p->s.width+1;
        }else{
        p->s.center.x = x-w-p->s.width -1;
        }
        p->s.center.y = y;
        p->velocity.x =xvel;
        p->velocity.y = yvel;
        numproj++;
}
void enemyAllShoot(struct Projectile *projectile, int&numprojectile, struct Boss * enemies, int &numenemy){
    if((timeDiff(&timeEnemyShot, &timeCurrent)) >= enemyDelay){//stats[2] ==firerate
//DO SHIT HERE
        if(numenemy){
            for(int i = 0; i < numenemy; i++){
                Boss *enemy = &enemies[i];
                int x = enemy->s.center.x;
                int y = enemy->s.center.y;
                int w = enemy->s.width;
                enemyFire(projectile,numprojectile,x,y,-5.0,0,w,1);
            }
        }
        clock_gettime(CLOCK_REALTIME, &timeEnemyShot);
           
    }
}



//pattern for boss shots, takes in the array of projectiles, number of active projectiles, character x position (for checking)
//boss x and y position and boss width/height
void bossShot(struct  Projectile *projectile, int &n, float charx, float bossx, float bossy,int bossw, int bossh){
//test creation

    if((timeDiff(&timeBossShot, &timeCurrent)) >= bossDelay){//stats[2] ==firerate
    switch(bossShotPattern) {
        case 0 : 
            fireProjectile(projectile,n,bossx + bossw+15, bossy, 5,5);
            fireProjectile(projectile,n,bossx + bossw+15, bossy, 5,0);
            fireProjectile(projectile,n,bossx + bossw+15, bossy, 5,-4);
            fireProjectile(projectile,n,bossx - bossw-15, bossy, -5,5);
            fireProjectile(projectile,n,bossx - bossw-15, bossy, -5,0);
            fireProjectile(projectile,n,bossx - bossw-15, bossy, -5,-4);
            bossDelay = 1.5;//3.0;
            break;
        case 1 : 
            fireProjectile(projectile,n,bossx + bossw+15, bossy, 5,5);
            fireProjectile(projectile,n,bossx + bossw+15, bossy, 5,-4);
            fireProjectile(projectile,n,bossx - bossw-15, bossy, -5,5);
            fireProjectile(projectile,n,bossx - bossw-15, bossy, -5,-4);
            bossDelay = 4.0;//8.0;
            break;
        case 2 : 
            fireProjectile(projectile,n,bossx + bossw+15, bossy, 5,5);
            fireProjectile(projectile,n,bossx + bossw+15, bossy, 5,0);
            fireProjectile(projectile,n,bossx + bossw+15, bossy, 5,-4);
            fireProjectile(projectile,n,bossx - bossw-15, bossy, -5,5);
            fireProjectile(projectile,n,bossx - bossw-15, bossy, -5,0);
            fireProjectile(projectile,n,bossx - bossw-15, bossy, -5,-4);
            fireProjectile(projectile,n,bossx , bossy-bossh-16, 5,-6);
            fireProjectile(projectile,n,bossx , bossy-bossh-16, 0,-6);
            fireProjectile(projectile,n,bossx , bossy-bossh-16, -5,-6);
            bossDelay = 1.0;//2.0;
            break;
        case 3 : 
            fireProjectile(projectile,n,bossx , bossy-bossh-16, 5,-6);
            fireProjectile(projectile,n,bossx , bossy-bossh-16, 0,-6);
            fireProjectile(projectile,n,bossx , bossy-bossh-16, -5,-6);
            bossDelay = 1.5;//3.0;
            break;





    }
    bossShotPattern +=1;
    if(bossShotPattern >=3){
        bossShotPattern = 0;
    }

    clock_gettime(CLOCK_REALTIME, &timeBossShot);
    }
    /*//testing projectile maker
    */
    //warning clears
    charx = charx;
    bossx = bossx;
    bossy = bossy;
    bossw = bossw;
    bossh = bossh;
}

void buildEnemyPos(struct Boss * enemies, int & numenemy){
    Boss *enemy = &enemies[0];
    enemy->s.center.x = 2030;
    enemy->s.center.y = 180;
    enemy->s.width = 18;
    enemy->s.height = 18;
    enemy->health = 30;
    numenemy++;
    //enemy pos 2
    enemy = &enemies[1];
    enemy->s.center.x = 900;
    enemy->s.center.y = 430;
    enemy->s.width = 18;
    enemy->s.height = 18;
    enemy->health = 30;
    numenemy++;
    //enemy pos 2
    enemy = &enemies[2];
    enemy->s.center.x = 6700;
    enemy->s.center.y = 130;
    enemy->s.width = 18;
    enemy->s.height = 18;
    enemy->health = 30;
    numenemy++;
    //enemy pos 2
    enemy = &enemies[3];
    enemy->s.center.x = 8550;
    enemy->s.center.y = 340;
    enemy->s.width = 18;
    enemy->s.height = 18;
    enemy->health = 30;
    numenemy++;

}





//draws sprite of character running, uses alpha blengind to make background of sprite
//transparent
void drawRunningSprite(float x, float y, int w, int h, bool left)
{
    double curanim;
    int curanimtime;
    curanim = timeDiff(&timeCharacter, &timeCurrent);
    curanim *= 10;
    curanimtime = (int) curanim;
    curanimtime = curanimtime%14;
    glColor3ub(255,255,255);//set color to pure white to avoid blending 
    
    //draw a recangle at character's position using run sprite
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, charTexture[curanimtime]);
    if(left){
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,1.0f); 
    glVertex2i(x+w,y-h);
    glTexCoord2f(0.0f,0.0f); 
    glVertex2i(x+w,y+h);
    glTexCoord2f(1.0f,0.0f); 
    glVertex2i(x-w,y+h);
    glTexCoord2f(1.0f,1.0f); 
    glVertex2i(x-w,y-h);
    glEnd();
    }else{
        glBegin(GL_QUADS);
    glTexCoord2f(0.0f,1.0f); 
    glVertex2i(x-w,y-h);
    glTexCoord2f(0.0f,0.0f); 
    glVertex2i(x-w,y+h);
    glTexCoord2f(1.0f,0.0f); 
    glVertex2i(x+w,y+h);
    glTexCoord2f(1.0f,1.0f); 
    glVertex2i(x+w,y-h);
    glEnd();
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    //disable alpha blending to avoid conflict on other draw functions
    glDisable(GL_ALPHA_TEST);
    //glDisable(GL_TEXTURE_2D);
    glPopMatrix();


}


//make animated character projectiles
void drawBossIdleSprite(float x, float y, int w, int h)
{
    double curanim;
    int curanimtime;
    curanim = timeDiff(&timeCharacter, &timeCurrent);
    curanimtime = (int) curanim;
    curanimtime = curanimtime%2;
    curanimtime += 14;
    glColor3ub(255,255,255);//set color to pure white to avoid blending 
    
    //draw a recangle at character's position using run sprite
    glPushMatrix();
    //glTranslatef(x,y,0);
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

void drawTurret(float x, float y)
{
    glColor3ub(255,255,255);
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, charTexture[16]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,1.0f); glVertex2i(x-18,y-18);
    glTexCoord2f(0.0f,0.0f); glVertex2i(x-18,y+18);
    glTexCoord2f(1.0f,0.0f); glVertex2i(x+18,y+18);
    glTexCoord2f(1.0f,1.0f); glVertex2i(x+18,y-18);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    //disable alpha blending to avoid conflict on other draw functions
    glDisable(GL_ALPHA_TEST);
    //glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
    
void drawHealth(float x, int health){
    int y = 550;
    x = x+240;
    //draw black box
    glColor3ub(0,0,0);
    glBegin(GL_QUADS);
    glVertex2i(x-105,y-10);
    glVertex2i(x-105,y+10);
    glVertex2i(x+105,y+10);
    glVertex2i(x+105,y-10);
    glEnd();
    glColor3ub(200,0,0);
    glBegin(GL_QUADS);
    glVertex2i(x-100,y-7);
    glVertex2i(x-100,y+7);
    glVertex2i(x+100,y+7);
    glVertex2i(x+100,y-7);
    glEnd();
    glColor3ub(0,200,0);
    glBegin(GL_QUADS);
    glVertex2i(x-100,y-7);
    glVertex2i(x-100,y+7);
    glVertex2i(x-100+(health * 2),y+7);
    glVertex2i(x-100+(health * 2),y-7);
    glEnd();
}

void drawCharProjSprite(float x, float y, int w, int h)
{
    double curanim;
    int curanimtime;
    curanim = timeDiff(&timeCharacter, &timeCurrent);
    curanimtime = (int) curanim;
    curanimtime = curanimtime%4;
    glColor3ub(255,255,255);//set color to pure white to avoid blending 
    
    //draw projectile at projectiles position
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

void drawBossProjSprite(float x, float y, int w, int h)
{
    double curanim;
    int curanimtime;
    curanim = timeDiff(&timeCharacter, &timeCurrent);
    curanimtime = (int) curanim;
    curanimtime = curanimtime%5;
    curanimtime +=4; //skip character sprites
    glColor3ub(255,255,255);//set color to pure white to avoid blending 
    
    //draw projectile at projectiles position
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
    glClearColor(1.0,1.0,1.0,1.0);
    bgImageTwo = ppm6GetImage("./images/leveltwo.ppm");//load a TILEABLE backgroun image
    glGenTextures(1, &bgTextureTwo);
    glBindTexture(GL_TEXTURE_2D, bgTextureTwo);
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,0,3,bgImageTwo->width,bgImageTwo->height,0,
            GL_RGB,GL_UNSIGNED_BYTE,bgImageTwo->data);
}


//function that generates a background using a tileable image
void tileBackground(void) 
{
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0,1.0,1.0);
    if(bg){
        for(int i = -5; i < 5; i++){
            for( int j = -4; j < 17; j++){
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
        for(int zi = -5; zi< 16; zi++){
            for(int zj =0; zj< 3; zj++){
                int x = zi*432 + 6900;
                int y = zj*270;
                glBindTexture(GL_TEXTURE_2D, bgTextureTwo);
                glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f); glVertex2i(x, y);
                glTexCoord2f(0.0f, 0.0f); glVertex2i(x, y+270);
                glTexCoord2f(1.0f, 0.0f); glVertex2i(x+432, y+270);
                glTexCoord2f(1.0f, 1.0f); glVertex2i(x+432, y);
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
    //boss sprite
    charImage[14] = ppm6GetImage("./images/boss1.ppm");
    charImage[15] = ppm6GetImage("./images/boss2.ppm");
    charImage[16] = ppm6GetImage("./images/turret.ppm");
    for(int i = 0; i < 17; i++)
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
    cprojImage[4] = ppm6GetImage("./images/bbullet1.ppm");
    cprojImage[5] = ppm6GetImage("./images/bbullet2.ppm");
    cprojImage[6] = ppm6GetImage("./images/bbullet3.ppm");
    cprojImage[7] = ppm6GetImage("./images/bbullet4.ppm");
    cprojImage[8] = ppm6GetImage("./images/bbullet5.ppm");
    for(int i = 0; i < 9; i++)
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
