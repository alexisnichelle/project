#ifndef MYSTRUCT_H_
#define MYSTRUCT_H_


struct Vec {
    float x, y, z;
};

struct Shape {
    float width, height;
    float radius;
    Vec center;
};

struct Character {
    Shape s;
    Vec velocity;
};
struct Projectile {
    Shape s;
    Vec velocity;
    int r;
    int b;
    int g;
};

#endif
