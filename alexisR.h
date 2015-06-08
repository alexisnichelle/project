#ifndef _ALEXISR_H_
#define _ALEXISR_H_

extern void text(void);
extern void texture(void); 
extern void r_texture(void);
extern void platforms(void);
extern void drawPlatforms(float w, float h);
extern void createsounds(void);
extern void playsound(void);
extern void play_gameOver(void);
extern void sound(void);
extern void game_over(void);
extern void r_gameOver(void);
extern void winner_screen(void);
extern void r_winner(void);
extern void level2(struct Shape *platform, int &num);

#endif
