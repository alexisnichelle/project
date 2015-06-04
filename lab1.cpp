//cs335 Spring 2015 Lab-1
//Uberdude Alpha Build
//This program executes a game similar in style to megaman
// 
//note: PROJECTILE size for character is currently 24w x 12h
//note: character sprites are currently set to 50x50 pixels
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include <cstring>
#include <cmath>
#include <time.h>
#include <string.h>
#include <X11/Xlib.h>
#include <iostream>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "bryanK.h"
#include "alexisR.h"
#include "tinaT.h"
#include "frankC.h"
#include "mystruct.h"

#include <GL/gl.h>
extern "C" {
#include "fonts.h"
}
#define CHARACTER_WIDTH 25
#define CHARACTER_HEIGHT 25
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define numbox 2000
#define MAX_PROJECTILES 10000
#define MAX_PARTICLES 1
#define MAX_ENEMIES 30
#define GRAVITY 1

#define USE_SOUND

#ifdef USE_SOUND
#include <FMOD/fmod.h>
#include <FMOD/wincompat.h>
#include "fmod.h"
#endif

//X Windows variables
Display *dpy;
Window win;
GLXContext glc;

//global values, dont misuse
//
//time definitions, used across files
struct timespec timeBullet;
struct timespec timeSound;
//returns difference in time from start to end

static int numenemy = 0;
static int lives = 3;
int keys[65536];
//stats as of now are
//[0] health
//[1] movespeed
//[2] fire rate
//[3] damage
int stats[4];

int curbox = 0;
bool liveboss = true;
struct Game {
    Shape *box;
    Character character;
    Boss boss;//to be used for the main boss at end of level
    Boss * enemies;
    Projectile *projectile;
    int n;
    ~Game() {
        delete [] enemies;
        delete [] box;
        delete [] projectile;
    }
    Game() {
        enemies = new Boss[MAX_ENEMIES];
        box = new Shape[numbox];
        projectile = new Projectile[MAX_PROJECTILES];
        //initialize projectiles to prevent nulls
        for (int i = 0; i < MAX_PROJECTILES; i++) {
            Projectile p = projectile[i];
            p=p;//because C++ warnings are stupid when using children only
            p.s.center.x = 0;
            p.s.center.y = 0;
            p.velocity.y = 0;
            p.velocity.x = 0;
            p.s.width = 12;
            p.s.height = 6;
            p.type = 0;
        }

        //basic positioning for ze boss to be changed to end of level
        boss.s.center.x = 3600;
        boss.s.center.y =300;
        boss.velocity.x = 0;
        boss.velocity.y = 0;
        boss.s.width = 50;
        boss.s.height = 50;
        boss.health = 100;
        for(int i = 0; i < MAX_ENEMIES; i++){
            Boss enemy = enemies[i];
            enemy = enemy;//because C++ warnings are stupid about using children only
            enemy.s.center.x = -10;
            enemy.s.center.y = -10;
            enemy.s.width = 1;
            enemy.s.height = 1;
            enemy.velocity.x =0;
            enemy.velocity.y = 0;
            enemy.health = 30;
        }


        //initialize boxes to prevent nulls
        for (int i = 0; i <numbox; i++) {
            box[i].width = 1;
            box[i].height = 1;
            box[i].center.x = 1;
            box[i].center.y = 1;
        }
        buildlevel(box, curbox);
        level(box, curbox);
        level2(box, curbox);
	level2Cont(box,curbox);
        buildEnemyPos(enemies,numenemy);
        n=0;
    }
};

//Function prototypes
void initXWindows(void);
void init_opengl(void);
void cleanupXWindows(void);
void check_mouse(XEvent *e, Game *game);
int check_keys(XEvent *e, Game *game);
void movement(Game *game);
void render(Game *game);
bool start = true;
bool mission = false;
bool dead = false;
bool twolevel = false;

using namespace std;
int main(void)
{
    int done=0;
    srand(time(NULL));
    initXWindows();
    init_opengl();
    createsounds();
    playsound();
    //declare game object
    Game game;
    game.n=0;
    //prep base stats
    stats[0] = 100;//health
    stats[1] = 6;//movespeed
    stats[2] = 7;//firerate
    stats[3] = 0;//damage
    clock_gettime(CLOCK_REALTIME, &timeCharacter);
    clock_gettime(CLOCK_REALTIME, &timeStart);
    clock_gettime(CLOCK_REALTIME, &timeBullet);
    clock_gettime(CLOCK_REALTIME, &timeBossShot);
    clock_gettime(CLOCK_REALTIME, &timeEnemyShot);
    clock_gettime(CLOCK_REALTIME, &timeSound);

    //zero out array of all possible key strokes
    for (int i = 0; i < 65536;i++){
        keys[i] = 0;
    }
    bool up = false, down = false, left = false, right = false, b = false, a = false;
    //start animation
    while (!done) {
        while (XPending(dpy)) {
            XEvent e;
            XNextEvent(dpy, &e);
            if (start) {
                check_mouse(&e, &game);
		lives = moreLives(&e, up, down, left, right, b, a);
	    } else if (dead) {
		check_mouse(&e, &game);
	    }
            done = check_keys(&e, &game);
        }
        clock_gettime(CLOCK_REALTIME, &timeCurrent);
        movement(&game);
        render(&game);
        glXSwapBuffers(dpy, win);
        //slowdown for running on DSM MACHINE
        //usleep(30000);
/*
        int slow = 0;
        for(int slower = 0; slower < 8500000;slower++){
            slow = slow + slower;
        }*/
    }
    cleanupXWindows();
    return 0;
}

void set_title(void)
{
    //Set the window title bar.
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "Uberdude");
}

void cleanupXWindows(void) 
{
    //do not change
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
}

void initXWindows(void) 
{
    //do not change
    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    int w=WINDOW_WIDTH, h=WINDOW_HEIGHT;
    dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
        std::cout << "\n\tcannot connect to X server\n" << std::endl;
        exit(EXIT_FAILURE);
    }
    Window root = DefaultRootWindow(dpy);
    XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
    if (vi == NULL) {
        std::cout << "\n\tno appropriate visual found\n" << std::endl;
        exit(EXIT_FAILURE);
    } 
    Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
        ButtonPress | ButtonReleaseMask |
        PointerMotionMask |
        StructureNotifyMask | SubstructureNotifyMask;
    win = XCreateWindow(dpy, root, 0, 0, w, h, 0, vi->depth,
            InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    set_title();
    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);
}

void init_opengl(void)
{
    //OpenGL initialization
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //Set 2D mode (no perspective)
    glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);
    //Set the screen background color
    glClearColor(0.1, 0.1, 0.1, 1.0);
    //initialize fonts
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();

    texture();
    buildBackgroundImage();
    buildCharImage();
    buildIdleImage();
    buildProjImage();
    platforms();
    game_over();
}

void makeParticle(Game *game, int x, int y) 
{
    if (game->n >= MAX_PARTICLES)
        return;
    //position of particle
    Character *p = &game->character;
    p->s.center.x = x;
    p->s.center.y = y;
    p->s.width = CHARACTER_WIDTH;
    p->s.height = CHARACTER_HEIGHT;
    p->velocity.y = -4.0;
    p->velocity.x = 0;
    game->n++;
}

void makeProjectile(Game *game, float x, float y,float xvel, float yvel,int r, int g, int b, int types) 
{

    if (game->n >= MAX_PROJECTILES) {
        std::cout <<"shits borked" <<" Make Projectile" <<x<<" "<<y<<std::endl;
        return;
    }
    if((timeDiff(&timeBullet, &timeCurrent)) >= (stats[2] * .1)){//stats[2] ==firerate
        //position of particle
        Projectile *p = &game->projectile[game->n];
        p->r = r;
        p->g = g;
        p->b = b;
        p->type = types;
        p->s.center.x = x;
        p->s.center.y = y;
        p->s.width = 12;
        p->s.height = 6;
        p->velocity.y = yvel;
        p->velocity.x = xvel;
        game->n++;
        p=p;
        clock_gettime(CLOCK_REALTIME, &timeBullet);
    }
}

void delete_projectile(int proj, Game *game){
    if(proj < game->n){
        memcpy(&game->projectile[proj], &game->projectile[game->n -1],
                sizeof(struct Projectile));
        game->n--;

    }
}
void delete_enemy(int enemy, Game *game){
    if(enemy < game->n){
        memcpy(&game->enemies[enemy], &game->enemies[numenemy -1],
                sizeof(struct Boss));
        numenemy--;

    }
}


void check_mouse(XEvent *e, Game *game)
{
    //clear stupid warning
    game = game;

    static int savex = 0;
    static int savey = 0;
    static int n = 0;

    if (e->type == ButtonRelease) {
        return;
    }
    if (e->type == ButtonPress) {
        if (e->xbutton.button==1) {
            //Left button was pressed
            if(start){
                makeParticle(game,200,190); //e->xbutton.x,y );
                start = false;
                mission = true;
            }
            if(dead){
                if(lives > 0) {
                    dead = false;
                    mission = true;
                }
            }
            return;
        }
        if (e->xbutton.button==3) {
            //Right button was pressed
            return;
        }
    }
    //Did the mouse move?
    if (savex != e->xbutton.x || savey != e->xbutton.y) {
        savex = e->xbutton.x;
        savey = e->xbutton.y;
        if (++n < 10)
            return;
    }
}

//gather key inputs, set array
int check_keys(XEvent *e, Game *game)
{
    //clear stupid warning
    game = game;

    int key = XLookupKeysym(&e->xkey, 0);
    if (e->type == KeyPress) {
        if (key == XK_Escape) {
            return 1;
        }
        keys[key] = 1;
        return 0;
    }

    if (e->type == KeyRelease) {
        keys[key] = 0;
        return 0;
    } else {
        return 0;
    }
}

void checkMovement(Game *game)
{
    Character *p;
    p = &game->character;

    if ((keys[XK_Shift_L]) || (keys[XK_Shift_R]))
    {
        stats[1] = 16;
    } else {
        stats[1] = 8;
    }
    if (keys[XK_Right]) {
        p->velocity.x = stats[1];
    }
    if (keys[XK_Left]) {
        p->velocity.x = -stats[1];
    }
    if (keys[XK_Up]) {

        if (!((p->velocity.y > .1) || (p->velocity.y < -.1))) {
            if(keys[XK_Control_L]){
                p->velocity.y = 18;
            }else{

                p->velocity.y = 33.0;
            }
        }
    }
    if (keys[XK_space]) {
        //game,x,y,xvel,yvel
        makeProjectile(game, (p->s.center.x + p->s.width + 12.1),p->s.center.y,10.0,0.0,200,10,10,0);
    }
    if (keys[XK_Tab]){
	//jetpack cheat
	jetpack(p);
    }
    game = game;
}

void movement(Game *game)
{
    if(mission){
        // if(!start){
        checkMovement(game);
        // }
        Character *p;
        Projectile *proj;
      //  if (game->n <= 0)
        //    return;
        p = &game->character;
        /*    bossShot(game->projectile,game->n,p->s.center.x,game->boss.s.center.x,game->boss.s.center.y,game->boss.s.width
                    ,game->boss.s.height);
        }
  */      
        enemyAllShoot(game->projectile,game->n, game->enemies, numenemy);

        //projectile physics
        //seperate loop for enemy projectile collisions for ease of reading
        if(numenemy > 0){
            if(game->n > 0){
                for(int i = 0; i < game->n;i++){
                    proj = &game->projectile[i];
                    for(int ndx =0; ndx < numenemy;ndx++){
                        Boss *enemy = &game->enemies[ndx];
                            if(((proj->s.center.x + proj->s.width) > (enemy->s.center.x - enemy->s.width))
                                &&((proj->s.center.x - proj->s.width) < (enemy->s.center.x +  enemy->s.width))
                                &&((proj->s.center.y + proj->s.height) > (enemy->s.center.y - enemy->s.height))
                                &&((proj->s.center.y - proj->s.height) < (enemy->s.center.y + enemy->s.height))){
                                    delete_projectile(i,game);
                                    enemy->health -=10;
                                    std::cout << "enemy " <<ndx<<"hit health:"<<enemy->health<< std::endl;
                                    if(enemy->health <= 0){
                                        delete_enemy(ndx,game);
                                    }
                            }
                    }   
            }
        }
        }
        if(game->n >0){
        for (int i = 0; i < game->n;i++) {
            proj = &game->projectile[i];
            proj->s.center.x += proj->velocity.x;
            proj->s.center.y += proj->velocity.y;
            Shape *boss = &game->boss.s;
            //check for boss collision

            if((stats[0] > 0)&&//xcheck
                    (((proj->s.center.x + proj->s.width) > (p->s.center.x - p->s.width) &&
                      (proj->s.center.x - proj->s.width) < (p->s.center.x + p->s.width)) 
                     &&//ycheck
                     (((proj->s.center.y + proj->s.height) > (p->s.center.y - p->s.height)) &&
                      ((proj->s.center.y - proj->s.height) < (p->s.center.y + p->s.height)))))
            {
                stats[0] -= 10;//DAMAGE
                if(stats[0] <= 0){
                    //kill yourself
                    //do death anim
                    if(twolevel){
                        p->s.center.x = 5200;
                        p->s.center.y = 150;
                    }else{
                        p->s.center.x = 200;
                        p->s.center.y = 190;
                    }
                    
                    std::cout <<"you died"<<std::endl;
                    dead = true;
                    mission = false;
                    lives--;
                    stats[0] = 100;


                }
                delete_projectile(i,game);
            }
                else if(liveboss&&//xcheck
                    (((proj->s.center.x + proj->s.width) > (boss->center.x - boss->width) &&
                      (proj->s.center.x - proj->s.width) < (boss->center.x + boss->width)) 
                     &&//ycheck
                     (((proj->s.center.y + proj->s.height) > (boss->center.y - boss->height)) &&
                      ((proj->s.center.y - proj->s.height) < (boss->center.y + boss->height))))){
                game->boss.health -= 10;//DAMAGE
                if(game->boss.health <=0){
                    //kill it
                    liveboss = false;
                    //do death anim

                    //level 2
                    twolevel = true;
                    p->s.center.x = 5200;
                    p->s.center.y = 150;

                    //if off platform reposition to 5200, 100
                    /*
                       if (p->s.center.y < -200.0) {
                       p->s.center.x = 5200;
                       p->s.center.y = 100;
                       std::cout <<"you died"<<std::endl;
                       dead = true;
                       mission = false;
                       lives--;
                       }*/

                    //loop through box collisions
                    Shape *testbox;
                    for( int boxxy = 0; boxxy < curbox; boxxy++){
                        testbox = &game->box[boxxy];
                        if((((proj->s.center.x + proj->s.width) > (testbox->center.x - testbox->width)) &&
                                    ((proj->s.center.x - proj->s.width) < (testbox->center.x + testbox->width)) &&
                                    ((proj->s.center.y + proj->s.height) > (testbox->center.y - testbox->height)) &&
                                    ((proj->s.center.y - proj->s.height) < (testbox->center.y + testbox->height))))
                        {//iff projectile collides with plabtorm
                            delete_projectile(i,game);
                        }
                    }

                }
                delete_projectile(i,game);
            }//delete if projectile went too far
            else if((proj->s.center.x > (p->s.center.x + 600))||(proj->s.center.x < (p->s.center.x - 600))){
                delete_projectile(i,game);
            }else {
                //loop through box collisions
                Shape *testbox;
                for( int boxxy = 0; boxxy < curbox; boxxy++){
                    testbox = &game->box[boxxy];
                    if((((proj->s.center.x + proj->s.width) > (testbox->center.x - testbox->width)) &&
                                ((proj->s.center.x - proj->s.width) < (testbox->center.x + testbox->width)) &&
                                ((proj->s.center.y + proj->s.height) > (testbox->center.y - testbox->height)) &&
                                ((proj->s.center.y - proj->s.height) < (testbox->center.y + testbox->height))))
                    {//iff projectile collides with plabtorm
                        delete_projectile(i,game);
                    }
                }
            }
}


        }
        //apply gravity
        p->velocity.y -= 2.1;
        //apply velocity  
        p->s.center.x += p->velocity.x;
        p->s.center.y += p->velocity.y;

        //check for collision with shapes...
        Shape *b;//for brevity
        //checks for downwards collision
        for (int i = 0; i < curbox; i++) {
            b = &game->box[i];
	    shapecollision(p, b);
            }


            /*//old collision detect
              if ((p->s.center.x + p->s.width) >= b->center.x - b->width &&
              (p->s.center.x - p->s.width) <= b->center.x + b->width &&
              (p->s.center.y - p->s.height) < b->center.y + b->height &&
              (p->s.center.y + p->s.height) > b->center.y - b->height){ 
              if((p->s.center.y < b->center.y) && (p->velocity.y > 0)){//collision upwards??
              p->velocity.y = -.11;
              p->s.center.y = b->center.y - b->height -p->s.height - 0.1;
              } else {
              p->s.center.y = b->center.y + b->height + p->s.height + 0.1;
              p->velocity.y = 0;
              }
              }*/
        if(liveboss){
            //extern void bossShot(struct  Projectile * projectile, int &n, float charx, float bossx, float bossy,int bossw, int bossh);

            bossShot(game->projectile,game->n,p->s.center.x,game->boss.s.center.x,game->boss.s.center.y,game->boss.s.width
                    ,game->boss.s.height);
        }
        //if off platform reset to start loc 200,190    
        if (p->s.center.y < -200.0) {
            if(twolevel){
                p->s.center.x = 5200;
                p->s.center.y = 150;
            }else{

                p->s.center.x = 200;
                p->s.center.y = 190;
            }
            stats[0] =100;
            std::cout <<"you died"<<std::endl;
            dead = true;
            mission = false;
            lives--;
        }

        //movement is called post checkkeys
        //set X velocity to zero unless jumping or a key is depressed
        if (!((p->velocity.y > 0.1) || (p->velocity.y < -.1))) {
            p->velocity.x /= 1.5;
            if ((p->velocity.x > -.5) && (p->velocity.x < .5)) {
                p->velocity.x = 0;
            }
        }
    }
}

void render(Game *game)
{
    float w, h;
    glClear(GL_COLOR_BUFFER_BIT);
    Vec *c = &game->character.s.center;
    Vec *vel = &game->character.velocity;
    int left = c->x -(WINDOW_WIDTH/2);
    int right = left + (WINDOW_WIDTH);
    int top = c->y + (WINDOW_HEIGHT/2);
    int bottom = top - WINDOW_HEIGHT;

    if (mission) {

        centerCamera(left,right,bottom,top);
        tileBackground();
        Rect r;
        r.bot = 570;
        r.left = c->x - 300;
        r.center = c->x;
        glBindTexture(GL_TEXTURE_2D, 0);
        glEnable(GL_TEXTURE_2D);
        ggprint16(&r, 16, 0x0000ffff, "Lives: %i",lives);
        glDisable(GL_TEXTURE_2D);

        //temp disable texture to allow for basic color shapes
        //draw platforms
        Shape *s;
        glColor3ub(0,0,0);
        for (int i=0;i<curbox;i++) {
            s = &game->box[i];
            glPushMatrix();
            glTranslatef(s->center.x, s->center.y, 0);
            w = s->width;
            h = s->height;

            glBindTexture(GL_TEXTURE_2D, 0);
            glEnable(GL_TEXTURE_2D);
            drawPlatforms(w, h);
            glDisable(GL_TEXTURE_2D);
            glPopMatrix();
        }
        //temp draw calls for BOSS

        if(liveboss){
            s= &game->boss.s;
            w = s->width;
            h = s->height;

            drawBossIdleSprite(s->center.x,s->center.y, w, h);/*
                                                                 glPushMatrix();
                                                                 glTranslatef(s->center.x, s->center.y, 0);
                                                                 glBegin(GL_QUADS);
                                                                 glVertex2i(-w,-h);
                                                                 glVertex2i(-w,h);
                                                                 glVertex2i(w,h);
                                                                 glVertex2i(w,-h);
                                                                 glEnd();
                                                                 glPopMatrix();*/
        }
        if(numenemy){//if there are enemies
            for(int i = 0; i<numenemy;i++){
                Boss *enemy = &game->enemies[i];
                int xx = enemy->s.center.x;
                int yy = enemy->s.center.y;
                drawTurret(xx,yy);
                /*
                int w = enemy->s.width;
                int h = enemy->s.height;
                glBegin(GL_QUADS);
                glVertex2i(xx-w,yy-h);
                glVertex2i(xx-w,yy+h);
                glVertex2i(xx+w,yy+h);
                glVertex2i(xx+w,yy-h);
                glEnd();
                */
            }
            }
            


        //draw projectiles here
        if(game->n > 0){
        Projectile *p;
        for (int i=0;i<game->n;i++) {
            p = &game->projectile[i];
            //glColor3ub(p->r,p->g,p->b);

            if(p->type == 0){
                drawCharProjSprite(p->s.center.x,p->s.center.y, p->s.width, p->s.height);
            }
            if(p->type ==1){

                drawBossProjSprite(p->s.center.x,p->s.center.y, p->s.width, p->s.height);
            }
        }
}

        //draw all particles here(character)
        w = CHARACTER_WIDTH;
        h = CHARACTER_HEIGHT;
        //remove the following call after simple sprites are added ffs
        drawCharacter(c->x,c->y,w,h);
        //temp if for 
        if ((keys[XK_Right])||(keys[XK_Left])) {
            //moving, use moving sprites
            //MOVEMENT SPRITE CALL HERE DOES NOTHING FFS
            if((keys[XK_Right])){
                drawRunningSprite(c->x,c->y,w,h,false);
            }
            else{
                drawRunningSprite(c->x,c->y,w,h,true);
            }

        } else {
            //IDLE SPRITE CALL HERE FFS
            drawIdleSprite(c->x,c->y,w,h);
        }
        drawHealth(c->x, stats[0]);
        //re-enable textures after basic shapes are done
        glEnable(GL_TEXTURE_2D);



    }
    //click to start

    if (dead) {
        c->y = 250;
        vel->y = 0;
        vel->x = 0;
        //refocus camera on start screen area
        centerCamera(0,WINDOW_WIDTH,0,WINDOW_HEIGHT);

        Rect r;
        r.bot = 260;
        r.left = 285;
        r.center = 0;
        if( lives > 0 ) {
            r_texture();
            glBindTexture(GL_TEXTURE_2D, 0);
            glEnable(GL_TEXTURE_2D);
            ggprint16(&r, 16, 0x00ffffff, "You died, Click to respawn!");
        }

        if( lives <= 0 ) {
            //fmod_stopsound();
            //play_gameOver();
            r_gameOver();
            glBindTexture(GL_TEXTURE_2D, 0);
            glEnable(GL_TEXTURE_2D);
        }
    }

    if (start) {
        r_texture();
        text();
    }
}



