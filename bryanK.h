#ifndef _BRYANK_H_
#define _BRYANK_H_

extern struct timespec timeCharacter;
extern struct timespec timeCurrent;
extern struct timespec timeStart;
extern double timeDiff(struct timespec *start, struct timespec *end); 
extern void timeCopy(struct timespec *destination, struct timespec *source);
extern void centerCamera(int left, int right, int bottom, int top);
extern void drawCharacter(float x, float y, int w, int h);
extern void drawIdleSprite(float x, float y, int w, int h);
extern void drawRunningSprite(float x, float y, int w, int h);
extern void tileBackground(void);

extern void buildBackgroundImage(void);
extern void buildCharImage();
extern void buildIdleImage();

#endif
