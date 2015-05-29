#ifndef _BRYANK_H_
#define _BRYANK_H_

//global variables for time
extern struct timespec timeCharacter;//used for frame check on character sprites
extern struct timespec timeCurrent;//current time via clock
extern struct timespec timeStart;
//functions for time, functionality notes in bryanK.cpp
extern double timeDiff(struct timespec *start, struct timespec *end); 
extern void timeCopy(struct timespec *destination, struct timespec *source);

//camera functions
extern void centerCamera(int left, int right, int bottom, int top);

//draw functions
extern void drawBossIdleSprite(float x, float y, int w, int h);
extern void drawCharacter(float x, float y, int w, int h);
extern void drawCharProjSprite(float x, float y, int w, int h);
extern void drawIdleSprite(float x, float y, int w, int h);
extern void drawRunningSprite(float x, float y, int w, int h);
extern void tileBackground(void);

//image and alpha building functions
extern void buildBackgroundImage(void);
extern void buildCharImage();
extern void buildIdleImage();
extern void buildProjImage();

#endif
