#ifndef _BRYANK_H_
#define _BRYANK_H_

extern void centerCamera(int left, int right, int bottom, int top);
extern void drawCharacter(float x, float y, int w, int h);
extern void drawIdleSprite(float x, float y, int w, int h);
extern void drawRunningSprite(float x, float y, int w, int h);
extern void tileBackground(void);
extern void buildBackgroundImage(void);
#endif
