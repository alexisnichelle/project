#ifndef _FRANKC_H_
#define _FRANKC_H_

void level(struct Shape *platform, int &num);
void makeEnemy(struct Boss *boss, struct  Projectile *proj);
void level2Cont(struct Shape *platform, int &num);
int moreLives(XEvent *e, bool &up, bool &down, bool &left, bool &right, bool &b, bool &a);

#endif
