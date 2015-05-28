//cs335 Spring 2015 Lab-1
//This program executes a game similar in style to megaman
//
// maybe call it UberDude?
//
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <time.h>
#include <X11/Xlib.h>
#include <iostream>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "bryanK.h"
#include "alexisR.h"
#include "tinaT.h"
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
#define GRAVITY 1

//X Windows variables
Display *dpy;
Window win;
GLXContext glc;

//global values, dont misuse
//
//time definitions, used across files
struct timespec timeBullet;
//returns difference in time from start to end

static int lives = 3;
int keys[65536];
//stats as of now are
//[0] health
//[1] movespeed
//[2] fire rate
//[3] damage
int stats[4];

int curbox = 0;

struct Game {
    Shape *box;
    Character character;
    Projectile *projectile;
    int n;
    ~Game() {
        delete [] box;
        delete [] projectile;
    }
    Game() {
        box = new Shape[numbox];
        projectile = new Projectile[MAX_PROJECTILES];
        //initialize projectiles to prevent nulls
        for (int i = 0; i < MAX_PROJECTILES; i++) {
            Projectile p = projectile[i];
            p=p;
            p.s.center.x = 0;
            p.s.center.y = 0;
            p.velocity.y = 0;
            p.velocity.x = 0;
            p.s.width = 0;
            p.s.height = 0;
        }

        //initialize boxes to prevent nulls
        for (int i = 0; i <numbox; i++) {
            box[i].width = 1;
            box[i].height = 1;
            box[i].center.x = 1;
            box[i].center.y = 1;
        }
        buildlevel(box,curbox);
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


int main(void)
{
    int done=0;
    srand(time(NULL));
    initXWindows();
    init_opengl();
    //declare game object
    Game game;
    game.n=0;
    //prep base stats
    stats[0] = 100;//health
    stats[1] = 6;//movespeed
    stats[2] = 0;//firerate
    stats[3] = 0;//damage
    clock_gettime(CLOCK_REALTIME, &timeCharacter);
    clock_gettime(CLOCK_REALTIME, &timeStart);
    clock_gettime(CLOCK_REALTIME, &timeBullet);

    //zero out array of all possible key strokes
    for (int i = 0; i < 65536;i++){
        keys[i] = 0;
    }

    //start animation
    while (!done) {
        while (XPending(dpy)) {
            XEvent e;
            XNextEvent(dpy, &e);
            if (start||dead)
                check_mouse(&e, &game);
            done = check_keys(&e, &game);
        }
        clock_gettime(CLOCK_REALTIME, &timeCurrent);
        movement(&game);
        render(&game);
        glXSwapBuffers(dpy, win);
    }
    cleanupXWindows();
    return 0;
}

void set_title(void)
{
    //Set the window title bar.
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "335 PROJECT");
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

void makeProjectile(Game *game, float x, float y,float xvel, float yvel,int r, int g, int b) 
{

    if (game->n >= MAX_PROJECTILES) {
        std::cout <<"shits borked" <<" Make Projectile" <<x<<" "<<y<<std::endl;
        return;
    }
    if((timeDiff(&timeBullet, &timeCurrent)) >= 2.1){
    //position of particle
    Projectile *p = &game->projectile[game->n];
    p->r = r;
    p->g = g;
    p->b = b;
    p->s.center.x = x;
    p->s.center.y = y;
    p->s.width = 2;
    p->s.height = 2;
    p->velocity.y = yvel;
    p->velocity.x = xvel;
    game->n++;
    p=p;
    clock_gettime(CLOCK_REALTIME, &timeBullet);
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
    /*depreciated keychecks, keep for debugging
      Character *p;
      p = &game->character;

    //Was there input from the keyboard?
    if (e->type == KeyPress) {
    if (key == XK_Escape) {
    return 1;
    }
    if (key == XK_Right) {
    p->velocity.x = 3;
    }

    if (key == XK_Left) {
    p->velocity.x = -3;
    }

    if (key == XK_Up) {
    if(!((p->velocity.y > .1) || (p->velocity.y < -.1) ) ){
    p->velocity.y = 30.0;
    }
    }

    if (key == XK_Down) {
    makeProjectile(game, (p->s.center.x + p->s.width + 0.1),p->s.center.y,10.0,0.0,200,10,10);

    } 
    }

    if (e->type == KeyRelease) {


    }
    return 0;*/
}
void checkMovement(Game *game)
{
    Character *p;
    p = &game->character;

    if ((keys[XK_Shift_L]) || (keys[XK_Shift_R]))
    {
        stats[1] = 3;
    } else {
        stats[1] = 6;
    }
    if (keys[XK_Right]) {
        p->velocity.x = stats[1];
    }
    if (keys[XK_Left]) {
        p->velocity.x = -stats[1];
    }
    if (keys[XK_Up]) {

        if (!((p->velocity.y > .1) || (p->velocity.y < -.1))) {
            p->velocity.y = 30.0;
        }
    }
    if (keys[XK_Down]) {
        makeProjectile(game, (p->s.center.x + p->s.width + 0.1),p->s.center.y,10.0,0.0,200,10,10);
    }
    game = game;
}

void movement(Game *game)
{

    // if(!start){
    checkMovement(game);
    // }
    Character *p;
    Projectile *proj;
    if (game->n <= 0)
        return;

    p = &game->character;

    for (int i = 0; i < game->n;i++) {
        proj = &game->projectile[i];
        proj->s.center.x += proj->velocity.x;
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
        }
    }


    //check for off-screen
    //IF OFF SCREEN = DEAD


    //if off platform reset to start loc 200,190    
    if (p->s.center.y < -200.0) {
        p->s.center.x = 200;
        p->s.center.y = 190;
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
            glTranslatef(s->center.x, s->center.y, s->center.z);
            w = s->width;
            h = s->height;
            glBegin(GL_QUADS);
            glVertex2i(-w,-h);
            glVertex2i(-w, h);
            glVertex2i( w, h);
            glVertex2i( w,-h);
            glEnd();
            glPopMatrix();
        }
        //draw projectiles here
        Projectile *p;
        for (int i=0;i<game->n;i++) {
            p = &game->projectile[i];
            glColor3ub(p->r,p->g,p->b);
            glPushMatrix();
            glTranslatef(p->s.center.x, p->s.center.y, p->s.center.z);
            w = p->s.width;
            h = p->s.height;
            glBegin(GL_QUADS);
            glVertex2i(-w,-h);
            glVertex2i(-w, h);
            glVertex2i( w, h);
            glVertex2i( w,-h);
            glEnd();
            glPopMatrix();
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
            drawRunningSprite(c->x,c->y,w,h);
        } else {
            //IDLE SPRITE CALL HERE FFS
            drawIdleSprite(c->x,c->y,w,h);
        }
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

            r_texture();
            glBindTexture(GL_TEXTURE_2D, 0);
            glEnable(GL_TEXTURE_2D);

        Rect r;
        r.bot = 570;
        r.left = 10;
        r.center = 0;
        if( lives > 0 ) {
            ggprint16(&r, 16, 0x00ffffff, "You died, Click to respawn!");
        }

        if( lives <= 0 ) {
            ggprint16(&r, 16, 0x00ff0000, "Game Over");
        }
    }
    
    if (start) {
        r_texture();
        Rect r;
        glBindTexture(GL_TEXTURE_2D, 0);
        glEnable(GL_TEXTURE_2D);

        r.bot = 570;
        r.left = 10;
        r.center = 0;
        ggprint16(&r, 16, 0x00ffffff, "CLICK TO START!");
    }
}



