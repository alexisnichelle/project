#ifndef MYSTRUCT_H_
#define MYSTRUCT_H_

//global structures for usage accross .cpp files
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

struct Boss {
    Shape s;
    Vec velocity;
    int health;
};

struct Projectile {
    Shape s;
    Vec velocity;
    int r;
    int b;
    int g;
};

#endif
